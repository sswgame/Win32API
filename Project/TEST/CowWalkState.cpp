#include "pch.h"
#include "CowWalkState.h"

#include "CAnimal.h"
#include "CAnimator.h"
#include "CMovement.h"
#include "Name.h"
#include "CCore.h"
#include "CTimer.h"

CowWalkState::CowWalkState()
	:
	CState{L"COW_WALK"},
	m_elapsedTime{0},
	m_maxTime{2.f} {}

CowWalkState::~CowWalkState() = default;

void CowWalkState::Update()
{
	Vec2       pos = GetOwner().GetComponent<CTransform>()->GetPos();
	const Vec2 dir = GetOwner().GetComponent<CMovement>()->GetDir();
	GetOwner().GetComponent<CMovement>()->SetSpeed(100.f);
	pos += dir * GetOwner().GetComponent<CMovement>()->GetSpeed() * DS;
	GetOwner().GetComponent<CTransform>()->SetPos(pos);

	m_elapsedTime += DS;

	if (GetOwner().GetComponent<CAnimator>()->GetCurrent().IsFinished())
	{
		GetOwner().GetComponent<CAnimator>()->GetCurrent().Reset();
		if (m_elapsedTime > m_maxTime)
		{
			ChangeState(L"COW");
		}
	}
}

void CowWalkState::Enter()
{
	const Vec2     dir     = GetOwner().GetComponent<CMovement>()->GetDir();
	const CAnimal* pAnimal = static_cast<CAnimal*>(&GetOwner());

	if (pAnimal->GetInfo().currentLevel == GROW_LEVEL::CHILD)
	{
		GetOwner().GetComponent<CAnimator>()->PlayGroup(
			dir, COW::GROUPS::ANIMATIONS[COW::GROUPS::WALK_CHILD_0], false);
	}
	else if (pAnimal->GetInfo().currentLevel == GROW_LEVEL::TEEN)
	{
		if (pAnimal->GetInfo().isSick)
		{
			GetOwner().GetComponent<CAnimator>()->PlayGroup(
				dir, COW::GROUPS::ANIMATIONS[COW::GROUPS::WALK_TEEN_FRUSTRATED_0], false);
		}
		else
		{
			GetOwner().GetComponent<CAnimator>()->PlayGroup(
				dir, COW::GROUPS::ANIMATIONS[COW::GROUPS::WALK_TEEN_0], false);
		}
	}
	else if (pAnimal->GetInfo().currentLevel == GROW_LEVEL::ADULT)
	{
		if (pAnimal->GetInfo().isSick)
		{
			GetOwner().GetComponent<CAnimator>()->PlayGroup(
				dir, COW::GROUPS::ANIMATIONS[COW::GROUPS::WALK_SICK_0], false);
		}
		else
		{
			GetOwner().GetComponent<CAnimator>()->PlayGroup(
				dir, COW::GROUPS::ANIMATIONS[COW::GROUPS::WALK_0], false);
		}
	}
	else if (pAnimal->GetInfo().currentLevel == GROW_LEVEL::PREGNANT)
	{
		GetOwner().GetComponent<CAnimator>()->PlayGroup(
			dir, COW::GROUPS::ANIMATIONS[COW::GROUPS::WALK_PREGNANT_0], false);
	}
}

void CowWalkState::Exit()
{
	GetOwner().GetComponent<CAnimator>()->GetCurrent().Reset();
	m_elapsedTime = 0;
}
