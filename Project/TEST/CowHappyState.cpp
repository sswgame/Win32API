#include "pch.h"
#include "CowHappyState.h"

#include "CAnimal.h"
#include "CAnimator.h"
#include "CMovement.h"
#include "Name.h"

CowHappyState::CowHappyState()
	:
	CState{L"HAPPY"} {}

CowHappyState::~CowHappyState() = default;

void CowHappyState::Update()
{
	if (GetOwner().GetComponent<CAnimator>()->GetCurrent().IsFinished())
	{
		ChangeState(L"COW");
	}
}

void CowHappyState::Enter()
{
	const Vec2     dir     = GetOwner().GetComponent<CMovement>()->GetDir();
	const CAnimal* pAnimal = static_cast<CAnimal*>(&GetOwner());

	if (pAnimal->GetInfo().currentLevel == GROW_LEVEL::CHILD)
	{
		GetOwner().GetComponent<CAnimator>()->PlayGroup(
			dir, COW::GROUPS::ANIMATIONS[COW::GROUPS::IDLE_CHILD_1], false);
	}
	else if (pAnimal->GetInfo().currentLevel == GROW_LEVEL::TEEN)
	{
		GetOwner().GetComponent<CAnimator>()->PlayGroup(
			dir, COW::GROUPS::ANIMATIONS[COW::GROUPS::IDLE_TEEN_HAPPY_0], false);
	}
	else if (pAnimal->GetInfo().currentLevel == GROW_LEVEL::ADULT)
	{
		GetOwner().GetComponent<CAnimator>()->PlayGroup(
			dir, COW::GROUPS::ANIMATIONS[COW::GROUPS::IDLE_HAPPY_0], false);
	}
	else if (pAnimal->GetInfo().currentLevel == GROW_LEVEL::PREGNANT)
	{
		GetOwner().GetComponent<CAnimator>()->PlayGroup(
			dir, COW::GROUPS::ANIMATIONS[COW::GROUPS::IDLE_PREGNANT_1], false);
	}
}

void CowHappyState::Exit()
{
	GetOwner().GetComponent<CAnimator>()->GetCurrent().Reset();
}
