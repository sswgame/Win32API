#include "pch.h"
#include "CPlayerWhistle.h"

#include "CAnimator.h"
#include "CMovement.h"
#include "CPlayer.h"
#include"Name.h"

CPlayerWhistle::CPlayerWhistle()
	:
	CState{L"WHISTLE"} {}

CPlayerWhistle::~CPlayerWhistle() = default;

void CPlayerWhistle::Update()
{
	if (GetOwner().GetComponent<CAnimator>()->GetCurrent().IsFinished())
	{
		ChangeState(L"IDLE");
	}
}

void CPlayerWhistle::Enter()
{
	const Vec2 dir = GetOwner().GetComponent<CMovement>()->GetDir();
	GetOwner().GetComponent<CAnimator>()->PlayGroup(dir, PLAYER::GROUPS::ANIMATIONS[PLAYER::GROUPS::WHISTLE_0], false);
}

void CPlayerWhistle::Exit()
{
	GetOwner().GetComponent<CAnimator>()->GetCurrent().Reset();
}
