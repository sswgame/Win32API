#include "pch.h"
#include "CPlayerEat.h"

#include "CAnimator.h"
#include "CCollider.h"
#include "CCore.h"
#include "CEventManager.h"
#include "CPlayer.h"
#include "CSpriteComponent.h"
#include "CStage.h"
#include "CStageManager.h"
#include "CTile.h"
#include "CTimer.h"
#include "Name.h"

CPlayerEat::CPlayerEat()
	:
	CState{L"EAT"},
	m_delay{0},
	m_pItem{nullptr} {}

CPlayerEat::~CPlayerEat() = default;

void CPlayerEat::Update()
{
	m_delay += DS;
	if (m_delay >= 0.5f)
	{
		m_pItem->GetComponent<CSpriteComponent>()->SetEnabled(false);
	}
	if (GetOwner().GetComponent<CAnimator>()->GetCurrent().IsFinished())
	{
		CPlayer* pPlayer = static_cast<CPlayer*>(&GetOwner());
		CCore::GetInstance().GetEventManager().Send<DeletionEvent>(DeletionEvent{m_pItem});
		pPlayer->SetCarryingObject(nullptr);
		m_pItem = nullptr;
		m_delay = 0;
		GetOwner().GetComponent<CAnimator>()->GetCurrent().Reset();
		ChangeState(L"IDLE");
	}
}

void CPlayerEat::Enter()
{
	CPlayer* pPlayer = static_cast<CPlayer*>(&GetOwner());
	m_pItem          = static_cast<CItem*>(pPlayer->GetCarryingObject());
	pPlayer->SetHp(pPlayer->GetHP() + m_pItem->GetInfo().hp);

	const Vec2 pos   = CCore::GetInstance().GetStageManager().GetCurrent().GetCurrentTile(pPlayer)->GetComponent<
		CTransform>()->GetPos();

	Vec2 scale = m_pItem->GetComponent<CCollider>()->GetScale();
	m_pItem->GetComponent<CTransform>()->SetPos(pos - scale);
	GetOwner().GetComponent<CAnimator>()->Play(PLAYER::ANIMATIONS[PLAYER::EAT_FRONT_0], false);
}
void CPlayerEat::Exit() {}
