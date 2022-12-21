#include "pch.h"
#include "ChickenState.h"

#include "CCore.h"
#include "CTimer.h"

#include "CMovement.h"
#include "CAnimator.h"

#include "CAnimal.h"
#include "Name.h"

ChickenState::ChickenState()
	:
	CState{L"CHICKEN"},
	m_elapsedTime{0.f},
	m_maxTime{3.f},
	m_changeDirTime{0},
	m_changeDirection{false},
	m_toWalk{false},
	m_dir{}
{
	srand(static_cast<UINT>(time(nullptr)));
}

ChickenState::~ChickenState() = default;

void ChickenState::Update()
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
	if (m_toWalk)
	{
		Vec2       pos = GetOwner().GetComponent<CTransform>()->GetPos();
		const Vec2 dir = GetOwner().GetComponent<CMovement>()->GetDir();
		GetOwner().GetComponent<CMovement>()->SetSpeed(100.f);
		pos += dir * GetOwner().GetComponent<CMovement>()->GetSpeed() * DS;
		GetOwner().GetComponent<CTransform>()->SetPos(pos);
	}

	if (GetOwner().GetComponent<CAnimator>()->GetCurrent().IsFinished())
	{
		
		const Vec2     dir     = GetOwner().GetComponent<CMovement>()->GetDir();
		const CAnimal* pAnimal = static_cast<CAnimal*>(&GetOwner());
		if (false == pAnimal->IsCarried())
		{
			GetOwner().GetComponent<CAnimator>()->GetCurrent().Reset();
		}
		if (pAnimal->GetInfo().type == ANIMAL_TYPE::CHICKEN && false == pAnimal->IsCarried())
		{
			if (pAnimal->GetInfo().currentLevel == GROW_LEVEL::CHILD)
			{
				GetOwner().GetComponent<CAnimator>()->PlayGroup(
					dir, CHICKEN::GROUPS::ANIMATIONS[CHICKEN::GROUPS::CHICK_WALK_0], false);
			}
			else
			{
				GetOwner().GetComponent<CAnimator>()->PlayGroup(
					dir, CHICKEN::GROUPS::ANIMATIONS[CHICKEN::GROUPS::WALK_0], false);
			}
		}
	}
}

void ChickenState::Enter() { }

void ChickenState::Exit() {}

