#include "pch.h"
#include "CPlayerWalk.h"

#include "CCore.h"
#include "CTimer.h"

#include "CAnimator.h"
#include "CKeyManager.h"
#include "CMovement.h"
#include "CPlayer.h"

CPlayerWalk::CPlayerWalk()
	:
	CState{L"WALK"}
{}

CPlayerWalk::~CPlayerWalk() = default;

void CPlayerWalk::Update()
{
	Vec2       pos = GetOwner().GetComponent<CTransform>()->GetPos();
	const Vec2 dir = GetOwner().GetComponent<CMovement>()->GetDir();
	GetOwner().GetComponent<CMovement>()->SetSpeed(150.f);
	pos += dir * GetOwner().GetComponent<CMovement>()->GetSpeed() * DS;
	GetOwner().GetComponent<CTransform>()->SetPos(pos);

	if (KEY_AWAY(KEY::UP) || KEY_AWAY(KEY::LEFT) || KEY_AWAY(KEY::RIGHT) || KEY_AWAY(KEY::DOWN))
	{
		ChangeState(L"IDLE");
	}
	if (KEY_TAP(KEY::L_CTRL))
	{
		ChangeState(L"RUN");
	}
	if (KEY_TAP(KEY::A))
	{
		ChangeState(L"SELECT");
	}
	if (KEY_TAP(KEY::S))
	{
		ChangeState(L"WHISTLE");
	}
	if (KEY_TAP(KEY::SPACE))
	{
		ChangeState(L"IDLE");
	}
	if (KEY_TAP(KEY::Z))
	{
		ChangeState(L"USE");
	}
}
void CPlayerWalk::Enter()
{
	const Vec2 dir = GetOwner().GetComponent<CMovement>()->GetDir();
	GetOwner().GetComponent<CAnimator>()->PlayGroup(dir, L"WALK_0");
}

void CPlayerWalk::Exit()
{
	GetOwner().GetComponent<CAnimator>()->GetCurrent().Reset();
}
