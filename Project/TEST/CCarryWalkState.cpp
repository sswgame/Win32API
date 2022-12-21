#include "pch.h"
#include "CCarryWalkState.h"

#include "CAnimator.h"
#include "CKeyManager.h"
#include "CMovement.h"
#include "CCore.h"
#include "CPlayer.h"
#include "CTimer.h"
#include "Name.h"

CCarryWalkState::CCarryWalkState()
	:
	CState{L"CARRY_WALK"} {}

CCarryWalkState::~CCarryWalkState() = default;

void CCarryWalkState::Update()
{
	Vec2       pos = GetOwner().GetComponent<CTransform>()->GetPos();
	const Vec2 dir = GetOwner().GetComponent<CMovement>()->GetDir();
	GetOwner().GetComponent<CMovement>()->SetSpeed(150.f);
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
	if (KEY_AWAY(KEY::UP) || KEY_AWAY(KEY::LEFT) || KEY_AWAY(KEY::RIGHT) || KEY_AWAY(KEY::DOWN))
	{
		ChangeState(L"CARRY_IDLE");
	}
	if (KEY_TAP(KEY::L_CTRL))
	{
		ChangeState(L"CARRY_RUN");
	}
	if (KEY_TAP(KEY::A))
	{
		ChangeState(L"SELECT");
	}

	if (KEY_TAP(KEY::SPACE))
	{
		ChangeState(L"CARRY_THROW");
	}
}

void CCarryWalkState::Enter()
{
	const Vec2 dir = GetOwner().GetComponent<CMovement>()->GetDir();
	GetOwner().GetComponent<CAnimator>()->PlayGroup(dir, PLAYER::GROUPS::ANIMATIONS[PLAYER::GROUPS::CARRYING_WALK_0],
	                                                false);
}

void CCarryWalkState::Exit()
{
	GetOwner().GetComponent<CAnimator>()->GetCurrent().Reset();
}
