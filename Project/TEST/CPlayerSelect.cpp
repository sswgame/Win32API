#include "pch.h"
#include "CPlayerSelect.h"

#include "CAnimator.h"
#include "Name.h"
#include "CItem.h"
#include "CPlayer.h"
#include "CSpriteComponent.h"

CPlayerSelect::CPlayerSelect()
	:
	CState{L"SELECT"} {}

CPlayerSelect::~CPlayerSelect() = default;

void CPlayerSelect::Update()
{
	if (GetOwner().GetComponent<CAnimator>()->GetCurrent().IsFinished())
	{
		GetOwner().GetComponent<CAnimator>()->GetCurrent().Reset();
		ChangeState(L"IDLE");
	}
}

void CPlayerSelect::Enter()
{
	GetOwner().GetComponent<CAnimator>()->Play(PLAYER::ANIMATIONS[PLAYER::SELECT_FRONT_0], false);

	CPlayer& owner = static_cast<CPlayer&>(GetOwner());
	owner.SelectItem();
	CItem* pItem = owner.GetCurrentItem();
	if (nullptr != pItem)
	{
		Vec2 itemScale = pItem->GetComponent<CSpriteComponent>()->GetScale();

		Vec2       pos         = GetOwner().GetComponent<CTransform>()->GetPos();
		const Vec2 playerScale = GetOwner().GetComponent<CTransform>()->GetScale();
		pos.x                  = pos.x - playerScale.x * 1.5f;
		pos.y                  = pos.y - playerScale.y * 3.25f;

		pItem->GetComponent<CTransform>()->SetPos(pos);
		pItem->GetComponent<CSpriteComponent>()->SetEnabled(true);
	}
}

void CPlayerSelect::Exit()
{
	CItem* pItem = static_cast<CPlayer&>(GetOwner()).GetCurrentItem();
	if (nullptr != pItem)
	{
		pItem->GetComponent<CSpriteComponent>()->SetEnabled(false);
	}
}
