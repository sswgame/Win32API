#include "pch.h"
#include "CowState.h"

#include "CAnimal.h"
#include "CAnimator.h"
#include "CCore.h"
#include "CMovement.h"
#include "Name.h"
#include "CTimer.h"

CowState::CowState()
	:
	CState{L"COW"},
	m_elapsedTime{0.f},
	m_maxTime{3.f},
	m_changeDirTime{0},
	m_changeDirection{false},
	m_toWalk{false},
	m_dir{}
{
	srand(static_cast<UINT>(time(nullptr)));
}

CowState::~CowState() = default;

void CowState::Update()
{
	if (GetOwner().IsDead())
	{
		return;
	}
	m_elapsedTime += DS;

	if (m_elapsedTime >= m_maxTime)
	{
		m_toWalk      = rand() % 2;
		m_elapsedTime = 0.0f;
	}

	m_changeDirTime += DS;
	if (m_changeDirTime >= 1.f)
	{
		m_changeDirTime   = 0;
		m_changeDirection = rand() % 2;
	}

	if (m_toWalk && m_changeDirection)
	{
		m_changeDirection = false;
		m_dir             = static_cast<DIR>(rand() % 4);
		switch (m_dir)
		{
		case DIR::UP:
			GetOwner().GetComponent<CMovement>()->SetDir({0, -1});
			break;
		case DIR::RIGHT:
			GetOwner().GetComponent<CMovement>()->SetDir({1, 0});
			break;
		case DIR::LEFT:
			GetOwner().GetComponent<CMovement>()->SetDir({-1, 0});
			break;
		case DIR::DOWN:
			GetOwner().GetComponent<CMovement>()->SetDir({0, 1});
			break;
		default: ;
		}
	}
	if (GetOwner().GetComponent<CAnimator>()->GetCurrent().IsFinished())
	{
		if (m_toWalk)
		{
			ChangeState(L"COW_WALK");
		}
		else
		{
			ChangeIdleAnimation();
		}
	}
}

void CowState::Enter()
{
	const Vec2     dir     = GetOwner().GetComponent<CMovement>()->GetDir();
	const CAnimal* pAnimal = static_cast<CAnimal*>(&GetOwner());

	if (pAnimal->GetInfo().currentLevel == GROW_LEVEL::CHILD)
	{
		GetOwner().GetComponent<CAnimator>()->PlayGroup(
			dir, COW::GROUPS::ANIMATIONS[COW::GROUPS::IDLE_CHILD_0], false);
	}
	else if (pAnimal->GetInfo().currentLevel == GROW_LEVEL::TEEN)
	{
		if (pAnimal->GetInfo().isSick)
		{
			GetOwner().GetComponent<CAnimator>()->PlayGroup(
				dir, COW::GROUPS::ANIMATIONS[COW::GROUPS::IDLE_TEEN_SICK_0], false);
		}
		else
		{
			GetOwner().GetComponent<CAnimator>()->PlayGroup(
				dir, COW::GROUPS::ANIMATIONS[COW::GROUPS::IDLE_TEEN_0], false);
		}
	}
	else if (pAnimal->GetInfo().currentLevel == GROW_LEVEL::ADULT)
	{
		if (pAnimal->GetInfo().isSick)
		{
			GetOwner().GetComponent<CAnimator>()->PlayGroup(
				dir, COW::GROUPS::ANIMATIONS[COW::GROUPS::IDLE_SICK_0], false);
		}
		else
		{
			GetOwner().GetComponent<CAnimator>()->PlayGroup(
				dir, COW::GROUPS::ANIMATIONS[COW::GROUPS::IDLE_0], false);
		}
	}
	else if (pAnimal->GetInfo().currentLevel == GROW_LEVEL::PREGNANT)
	{
		GetOwner().GetComponent<CAnimator>()->PlayGroup(
			dir, COW::GROUPS::ANIMATIONS[COW::GROUPS::IDLE_PREGNANT_0], false);
	}
}

void CowState::Exit()
{
	m_elapsedTime   = 0;
	m_changeDirTime = 0;
	m_toWalk        = false;

	GetOwner().GetComponent<CAnimator>()->GetCurrent().Reset();
}

void CowState::ChangeIdleAnimation()
{
	CAnimator* pAnimator = GetOwner().GetComponent<CAnimator>();
	const Vec2 dir       = GetOwner().GetComponent<CMovement>()->GetDir();

	GetOwner().GetComponent<CAnimator>()->GetCurrent().Reset();

	const CAnimal* pAnimal = static_cast<CAnimal*>(&GetOwner());
	if (pAnimal->GetInfo().currentLevel == GROW_LEVEL::CHILD)
	{
		const int animationIndex = rand() % 3 + COW::GROUPS::IDLE_CHILD_0;
		GetOwner().GetComponent<CAnimator>()->PlayGroup(
			dir, COW::GROUPS::ANIMATIONS[animationIndex], false);
	}
	else if (pAnimal->GetInfo().currentLevel == GROW_LEVEL::TEEN)
	{
		if (pAnimal->GetInfo().isSick)
		{
			GetOwner().GetComponent<CAnimator>()->PlayGroup(
				dir, COW::GROUPS::ANIMATIONS[COW::GROUPS::IDLE_TEEN_SICK_0], false);
		}
		else
		{
			const int animationIndex = rand() % 4 + COW::GROUPS::IDLE_TEEN_0;
			GetOwner().GetComponent<CAnimator>()->PlayGroup(
			dir, COW::GROUPS::ANIMATIONS[COW::GROUPS::IDLE_TEEN_0], false);
		}
	}
	else if (pAnimal->GetInfo().currentLevel == GROW_LEVEL::ADULT)
	{
		if (pAnimal->GetInfo().isSick)
		{
			const int animationIndex = rand() % 4 + COW::GROUPS::IDLE_SICK_0;
			GetOwner().GetComponent<CAnimator>()->PlayGroup(
				dir, COW::GROUPS::ANIMATIONS[COW::GROUPS::IDLE_SICK_0], false);
		}
		else
		{
			const int animationIndex = rand() % 5 + COW::GROUPS::IDLE_0;
			GetOwner().GetComponent<CAnimator>()->PlayGroup(
			dir, COW::GROUPS::ANIMATIONS[COW::GROUPS::IDLE_0], false);
		}
	}
	else if (pAnimal->GetInfo().currentLevel == GROW_LEVEL::PREGNANT)
	{
		const int animationIndex = rand() % 3 + COW::GROUPS::IDLE_PREGNANT_0;
		GetOwner().GetComponent<CAnimator>()->PlayGroup(
			dir, COW::GROUPS::ANIMATIONS[COW::GROUPS::IDLE_PREGNANT_0], false);
	}
}
