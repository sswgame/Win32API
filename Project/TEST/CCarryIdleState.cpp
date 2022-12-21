#include "pch.h"
#include "CCarryIdleState.h"

#include "CAnimator.h"
#include "CKeyManager.h"
#include "CMovement.h"
#include "CPlayer.h"
#include "Name.h"
#include "CCore.h"

CCarryIdleState::CCarryIdleState()
	:
	CState{L"CARRY_IDLE"} {}

CCarryIdleState::~CCarryIdleState() = default;

void CCarryIdleState::Update()
{
	Vec2       pos         = GetOwner().GetComponent<CTransform>()->GetPos();
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

	if (GetOwner().GetComponent<CAnimator>()->GetCurrent().IsFinished())
	{
		const Vec2 dir = GetOwner().GetComponent<CMovement>()->GetDir();
		GetOwner().GetComponent<CAnimator>()->
		           PlayGroup(dir, PLAYER::GROUPS::ANIMATIONS[PLAYER::GROUPS::CARRY_IDLE_0], false);
	}

	if (KEY_PRESSED(KEY::UP))
	{
		Walk({0, -1});
	}
	else if (KEY_PRESSED(KEY::LEFT))
	{
		Walk({-1, 0});
	}
	else if (KEY_PRESSED(KEY::RIGHT))
	{
		Walk({1, 0});
	}
	else if (KEY_PRESSED(KEY::DOWN))
	{
		Walk({0, 1});
	}

	if (KEY_PRESSED(KEY::L_CTRL))
	{
		Run();
	}

	if (KEY_TAP(KEY::SPACE))
	{
		Throw();
	}
}

void CCarryIdleState::Enter()
{
	const Vec2 dir = GetOwner().GetComponent<CMovement>()->GetDir();
	GetOwner().GetComponent<CAnimator>()->PlayGroup(dir, PLAYER::GROUPS::ANIMATIONS[PLAYER::GROUPS::CARRY_IDLE_0],
	                                                false);
	
}

void CCarryIdleState::Exit()
{
	GetOwner().GetComponent<CAnimator>()->GetCurrent().Reset();
}

void CCarryIdleState::Walk(const Vec2& _dir)
{
	GetOwner().GetComponent<CMovement>()->SetDir(_dir);
	ChangeState(L"CARRY_WALK");
}

void CCarryIdleState::Run()
{
	ChangeState(L"CARRY_RUN");
}

void CCarryIdleState::Throw()
{
	ChangeState(L"CARRY_THROW");
}
