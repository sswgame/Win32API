#include "pch.h"
#include "CCarryRunState.h"

#include "CAnimator.h"
#include "CCollider.h"
#include "CKeyManager.h"
#include "CMovement.h"
#include "CTimer.h"
#include "CCore.h"
#include "CPlayer.h"
#include "CStage.h"
#include "CStageManager.h"
#include "Name.h"
#include "CTile.h"

CCarryRunState::CCarryRunState()
	:
	CState{L"CARRY_RUN"} {}

CCarryRunState::~CCarryRunState() = default;

void CCarryRunState::Update()
{
	Vec2       pos = GetOwner().GetComponent<CTransform>()->GetPos();
	const Vec2 dir = GetOwner().GetComponent<CMovement>()->GetDir();
	GetOwner().GetComponent<CMovement>()->SetSpeed(300.f);
	pos += dir * GetOwner().GetComponent<CMovement>()->GetSpeed() * DS;
	GetOwner().GetComponent<CTransform>()->SetPos(pos);

	const Vec2 playerScale = GetOwner().GetComponent<CTransform>()->GetScale();
	pos.x -= playerScale.x * .5f;
	pos.y -= playerScale.y * 3.f;
	CGameObject* pObject = static_cast<CPlayer*>(&GetOwner())->GetCarryingObject();
	if (pObject)
	{
		if (pObject->GetType() == OBJECT_TYPE::ANIMAL)
		{
			pos.y += playerScale.y + 1.5f;
			pos.x += playerScale.x * .5f;
		}
		pObject->GetComponent<CTransform>()->SetPos(pos);
	}

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
			ChangeState(L"CARRY_JUMP");
		}
	}
	else if (KEY_AWAY(KEY::L_CTRL))
	{
		ChangeState(L"CARRY_IDLE");
	}
	else if (KEY_TAP(KEY::UP))
	{
		GetOwner().GetComponent<CMovement>()->SetDir({0, -1});
		GetOwner().GetComponent<CAnimator>()->Play(PLAYER::ANIMATIONS[PLAYER::CARRYING_RUN_BACK_0]);
	}
	else if (KEY_TAP(KEY::LEFT))
	{
		GetOwner().GetComponent<CMovement>()->SetDir({-1, 0});
		GetOwner().GetComponent<CAnimator>()->Play(PLAYER::ANIMATIONS[PLAYER::CARRYING_RUN_LEFT_0]);
	}
	else if (KEY_TAP(KEY::RIGHT))
	{
		GetOwner().GetComponent<CMovement>()->SetDir({1, 0});
		GetOwner().GetComponent<CAnimator>()->Play(PLAYER::ANIMATIONS[PLAYER::CARRYING_RUN_RIGHT_0]);
	}
	else if (KEY_TAP(KEY::DOWN))
	{
		GetOwner().GetComponent<CMovement>()->SetDir({0, 1});
		GetOwner().GetComponent<CAnimator>()->Play(PLAYER::ANIMATIONS[PLAYER::CARRYING_RUN_FRONT_0]);
	}
}

void CCarryRunState::Enter()
{
	const Vec2 dir = GetOwner().GetComponent<CMovement>()->GetDir();
	GetOwner().GetComponent<CAnimator>()->PlayGroup(dir, PLAYER::GROUPS::ANIMATIONS[PLAYER::GROUPS::CARRYING_RUN_0]);
}

void CCarryRunState::Exit()
{
	GetOwner().GetComponent<CAnimator>()->GetCurrent().Reset();
}
