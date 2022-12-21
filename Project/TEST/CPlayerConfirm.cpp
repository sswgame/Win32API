#include "pch.h"
#include "CPlayerConfirm.h"

#include "CAnimal.h"
#include "CAnimator.h"
#include "CCollider.h"
#include "CMovement.h"
#include "CPlayer.h"
#include "CRayCast.h"
#include "Name.h"
#include "CCore.h"

#include "CSpriteComponent.h"
#include "CITem.h"

#include "CSound.h"
#include "CStage.h"
#include "CStageManager.h"

namespace
{
	std::vector<ItemID> vecItemID =
	{
		{TOOL::COW_FOOD, ITEM_CATEGORY::TOOL},
		{TOOL::CHICKEN_FOOD, ITEM_CATEGORY::TOOL},
	};
}

CPlayerConfirm::CPlayerConfirm()
	:
	CState{L"CONFIRM"},
	m_pItem{nullptr} {}


CPlayerConfirm::~CPlayerConfirm() = default;

void CPlayerConfirm::Update()
{
	CPlayer* pPlayer = static_cast<CPlayer*>(&GetOwner());
	if (pPlayer->IsCheckedWithRay())
	{
		CRayCast* pRay         = pPlayer->GetRay();
		const int currentIndex = pPlayer->GetComponent<CAnimator>()->GetCurrent().GetCurrentIndex();
		if (pRay->GetTarget() && false == pPlayer->IsBuyingItem())
		{
			pRay->GetTarget()->GetComponent<CTransform>()->SetPos(
				m_startPos + m_step * static_cast<float>(currentIndex));
		}
		if (GetOwner().GetComponent<CAnimator>()->GetCurrent().IsFinished())
		{
			if (pRay->GetTarget() && pRay->GetTarget()->GetType() == OBJECT_TYPE::ITEM)
			{
				CItem* pItem = static_cast<CItem*>(pRay->GetTarget());
				if (false == pItem->GetInfo().canSell && false == pPlayer->IsBuyingItem())
				{
					if (false == pPlayer->IsFull())
					{
						if (nullptr != pPlayer->GetCurrentItem())
						{
							pPlayer->SelectItem();
						}
						pPlayer->SetItem(pItem);
						pPlayer->SelectItem();
						pItem->GetComponent<CSpriteComponent>()->SetEnabled(false);
					}
					else
					{
						CItem*          pCurrent = pPlayer->GetCurrentItem();
						const ItemInfo& info     = pCurrent->GetInfo();

						pCurrent->GetComponent<CTransform>()->SetPos(info.location);
						pCurrent->GetComponent<CCollider>()->Enable(true);
						pCurrent->GetComponent<CCollider>()->SetScale(Vec2{40.f, 40.f});
						pCurrent->GetComponent<CSpriteComponent>()->SetEnabled(true);
						pCurrent->SetOwner(nullptr);
						if (info.id == vecItemID[0])
						{
							CCore::GetInstance().GetStageManager().GetCurrent().MoveObjectsTo(
								STAGE_TYPE::COW, OBJECT_TYPE::ITEM, pCurrent);
						}
						else if (info.id == vecItemID[1])
						{
							CCore::GetInstance().GetStageManager().GetCurrent().MoveObjectsTo(
								STAGE_TYPE::CHICKEN, OBJECT_TYPE::ITEM, pCurrent);
						}
						else
						{
							CCore::GetInstance().GetStageManager().GetCurrent().MoveObjectsTo(
								STAGE_TYPE::GARAGE, OBJECT_TYPE::ITEM, pCurrent);
						}
						pPlayer->SetItem(pItem);
						pItem->GetComponent<CSpriteComponent>()->SetEnabled(false);
					}

					pItem->SetOwner(pPlayer);
					pRay->GetComponent<CCollider>()->Enable(false);
					ChangeState(L"IDLE");
				}
				else
				{
					pPlayer->SetCarryingObject(pItem);
					if (pItem->GetInfo().canEat || true == pPlayer->IsBuyingItem())
					{
						ChangeState(L"UI");
					}
					else
					{
						CSound* pSound = CCore::GetInstance().GetResourceManager().FindSound(L"take");
						pSound->SetPosition(0);
						pSound->Play(false);

						ChangeState(L"CARRY_IDLE");
					}
				}
			}
			else
			{
				pPlayer->SetCarryingObject(pRay->GetTarget());
				pRay->SetTarget(nullptr);
				ChangeState(L"CARRY_IDLE");
			}
		}
	}
	else if (pPlayer->IsCheckedWithTile())
	{
		const int currentIndex = pPlayer->GetComponent<CAnimator>()->GetCurrent().GetCurrentIndex();
		if (m_pItem)
		{
			m_pItem->GetComponent<CTransform>()->SetPos(m_startPos + m_step * static_cast<float>(currentIndex));
		}
		if (GetOwner().GetComponent<CAnimator>()->GetCurrent().IsFinished())
		{
			pPlayer->SetCarryingObject(m_pItem);
			m_pItem = nullptr;
			pPlayer->SetCheckTile(false);
			ChangeState(L"CARRY_IDLE");
		}
	}
	else if (pPlayer->IsCheckedWithTileAfter())
	{
		CPlayer* pPlayer = static_cast<CPlayer*>(&GetOwner());
		pPlayer->SetBuyingAnimal(true);
		ChangeState(L"UI");
	}
}

void CPlayerConfirm::Enter()
{
	CPlayer*   pPlayer = static_cast<CPlayer*>(&GetOwner());
	const Vec2 dir     = GetOwner().GetComponent<CMovement>()->GetDir();

	if (pPlayer->IsCheckedWithTile())
	{
		CSound* pSound = CCore::GetInstance().GetResourceManager().FindSound(L"take");
		pSound->SetPosition(0);
		pSound->Play(false);

		GetOwner().GetComponent<CAnimator>()->
		           PlayGroup(dir, PLAYER::GROUPS::ANIMATIONS[PLAYER::GROUPS::CARRY_0], false);
		CItem* pItem = static_cast<CItem*>(pPlayer->GetCarryingObject());

		Vec2       lastPos     = pPlayer->GetComponent<CTransform>()->GetPos();
		const Vec2 playerScale = pPlayer->GetComponent<CTransform>()->GetScale();
		lastPos.y -= playerScale.y * 3.5f;
		lastPos.x -= playerScale.x * .5f;
		m_startPos = pItem->GetComponent<CTransform>()->GetPos();
		m_step = (lastPos - m_startPos);
		m_step /= static_cast<float>(GetOwner().GetComponent<CAnimator>()->GetCurrent().FrameCount());

		m_pItem = pItem;
	}
	else if (pPlayer->IsCheckedWithTileAfter())
	{
		GetOwner().GetComponent<CAnimator>()->
		           PlayGroup(dir, PLAYER::GROUPS::ANIMATIONS[PLAYER::GROUPS::IDLE_0], false);
	}
	else if (pPlayer->IsCheckedWithRay())
	{
		const CRayCast* pRay = pPlayer->GetRay();
		if (pRay->GetTarget() && pRay->GetTarget()->GetType() == OBJECT_TYPE::ITEM)
		{
			CItem* pItem = static_cast<CItem*>(pRay->GetTarget());
			pItem->GetComponent<CCollider>()->Enable(false);

			if (pItem->GetInfo().id.type == ITEM_CATEGORY::TOOL && false == pPlayer->HasItem(pItem->GetInfo().id)
				|| (pItem->GetName() == L"SEED"))
			{
				pPlayer->SetBuyingItem(true);
				pItem->GetComponent<CCollider>()->Enable(true);
			}
		}
		else if (pRay->GetTarget() && pRay->GetTarget()->GetType() == OBJECT_TYPE::ANIMAL)
		{
			CAnimal* pAnimal = static_cast<CAnimal*>(pRay->GetTarget());
			if (true == pAnimal->GetInfo().canCarry && false == pAnimal->IsCarried())
			{
				pAnimal->SetCarry(true);
				pAnimal->GetComponent<CCollider>()->Enable(false);
			}
			else
			{
				ChangeState(L"IDLE");
			}
		}
		if (pRay->GetTarget() && pRay->GetTarget()->GetType() == OBJECT_TYPE::OBSTACLE)
		{
			CSound* pSound = CCore::GetInstance().GetResourceManager().FindSound(L"take");
			pSound->SetPosition(0);
			pSound->Play(false);
		}
		if (pPlayer->IsBuyingItem())
		{
			GetOwner().GetComponent<CAnimator>()->
			           PlayGroup(dir, PLAYER::GROUPS::ANIMATIONS[PLAYER::GROUPS::IDLE_0], false);
		}
		else
		{
			GetOwner().GetComponent<CAnimator>()->
			           PlayGroup(dir, PLAYER::GROUPS::ANIMATIONS[PLAYER::GROUPS::CARRY_0], false);
		}

		Vec2       lastPos     = pPlayer->GetComponent<CTransform>()->GetPos();
		const Vec2 playerScale = pPlayer->GetComponent<CTransform>()->GetScale();
		lastPos.y -= playerScale.y * 3.5f;
		lastPos.x -= playerScale.x * .5f;
		m_startPos = pRay->GetTarget()->GetComponent<CTransform>()->GetPos();
		m_step     = (lastPos - m_startPos);
		m_step /= static_cast<float>(GetOwner().GetComponent<CAnimator>()->GetCurrent().FrameCount());
	}
}

void CPlayerConfirm::Exit()
{
	m_pItem          = nullptr;
	CPlayer* pPlayer = static_cast<CPlayer*>(&GetOwner());

	pPlayer->SetCheckTileAfter(false);
	pPlayer->SetCheckRay(false);
	pPlayer->SetCheckTileAfter(false);

	CRayCast* pRay = pPlayer->GetRay();
	pRay->SetTarget(nullptr);

	GetOwner().GetComponent<CAnimator>()->GetCurrent().Reset();
}
