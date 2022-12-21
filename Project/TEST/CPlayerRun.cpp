#include "pch.h"
#include "CPlayerRun.h"

#include "CAnimator.h"
#include "CCollider.h"
#include "Name.h"
#include "CCore.h"
#include "CKeyManager.h"
#include "CMovement.h"
#include "CTimer.h"

#include "CPlayer.h"
#include "CStage.h"
#include "CStageManager.h"
#include "CTile.h"

CPlayerRun::CPlayerRun()
	:
	CState{L"RUN"}
{ }

CPlayerRun::~CPlayerRun() = default;

void CPlayerRun::Update()
{
	
	Vec2       pos = GetOwner().GetComponent<CTransform>()->GetPos();
	const Vec2 dir = GetOwner().GetComponent<CMovement>()->GetDir();
	GetOwner().GetComponent<CMovement>()->SetSpeed(300.f);
	pos += dir * GetOwner().GetComponent<CMovement>()->GetSpeed() * DS;
	GetOwner().GetComponent<CTransform>()->SetPos(pos);


	if (GetOwner().GetComponent<CMovement>()->CheckCanJump())
	{
		CTile*     pCurrent       = CCore::GetInstance().GetStageManager().GetCurrent().GetCurrentTile(&GetOwner());
		const Vec2 tilePos        = pCurrent->GetComponent<CTransform>()->GetPos();
		const Vec2 tileSize       = pCurrent->GetComponent<CTransform>()->GetScale();
		const Vec2 collisionPos   = GetOwner().GetComponent<CCollider>()->GetFinalPos();
		const Vec2 collisionScale = GetOwner().GetComponent<CCollider>()->GetScale();

		if ((dir == DIR::LEFT && tilePos.x > collisionPos.x - collisionScale.x * .5f)
			|| (dir == DIR::RIGHT && tilePos.x + tileSize.x < collisionPos.x + collisionScale.x * .5f)
			|| (dir == DIR::UP && tilePos.y > collisionPos.y - collisionScale.y * .5f)
			|| (dir == DIR::DOWN && tilePos.y + tileSize.y < collisionPos.y + collisionScale.y * .5f))
		{
			ChangeState(L"JUMP");
		}
	}
	else if (KEY_AWAY(KEY::L_CTRL))
	{
		ChangeState(L"IDLE");
	}
	else if (KEY_TAP(KEY::UP))
	{
		GetOwner().GetComponent<CMovement>()->SetDir({0, -1});
		GetOwner().GetComponent<CAnimator>()->Play(PLAYER::ANIMATIONS[PLAYER::RUN_BACK_0]);
	}
	else if (KEY_TAP(KEY::LEFT))
	{
		GetOwner().GetComponent<CMovement>()->SetDir({-1, 0});
		GetOwner().GetComponent<CAnimator>()->Play(PLAYER::ANIMATIONS[PLAYER::RUN_LEFT_0]);
	}
	else if (KEY_TAP(KEY::RIGHT))
	{
		GetOwner().GetComponent<CMovement>()->SetDir({1, 0});
		GetOwner().GetComponent<CAnimator>()->Play(PLAYER::ANIMATIONS[PLAYER::RUN_RIGHT_0]);
	}
	else if (KEY_TAP(KEY::DOWN))
	{
		GetOwner().GetComponent<CMovement>()->SetDir({0, 1});
		GetOwner().GetComponent<CAnimator>()->Play(PLAYER::ANIMATIONS[PLAYER::RUN_FRONT_0]);
	}
}
void CPlayerRun::Enter()
{
	const Vec2 dir = GetOwner().GetComponent<CMovement>()->GetDir();
	GetOwner().GetComponent<CAnimator>()->PlayGroup(dir, PLAYER::GROUPS::ANIMATIONS[PLAYER::GROUPS::RUN_0]);
}

void CPlayerRun::Exit()
{}
