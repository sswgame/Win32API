#include "pch.h"
#include "CCarryThrowState.h"

#include "CAnimal.h"
#include "CAnimator.h"
#include "CCollider.h"
#include "CCore.h"
#include "CEventManager.h"
#include "CMovement.h"
#include "CObstacle.h"
#include "CPlayer.h"
#include "CResourceManager.h"
#include "CSound.h"
#include "CSpriteComponent.h"
#include "CStage.h"
#include "CStageManager.h"
#include "CTile.h"
#include "Name.h"

namespace
{
	std::set<int> setMoneyTile = {
		397, 398,
		403, 404,
		557, 558,
		563, 564,
	};
	std::set<int> setFeedTile = {522, 593, 605};
	std::set<int> setFarmTile =
	{
		179, 250, 251, 252, 184, 187, 188, 192, 193, 195, 196, 231, 197, 198, 199, 200,
		164, 165, 166, 167, 168, 169, 170, 171, 172, 207
	};
}

CCarryThrowState::CCarryThrowState()
	:
	CState{L"CARRY_THROW"},
	m_pObject{nullptr},
	m_pTile{nullptr} {}

CCarryThrowState::~CCarryThrowState() = default;

void CCarryThrowState::Update()
{
	if (GetOwner().GetComponent<CAnimator>()->GetCurrent().IsFinished())
	{
		CPlayer*   pPlayer      = static_cast<CPlayer*>(&GetOwner());
		CTile*     pCurrentTile = CCore::GetInstance().GetStageManager().GetCurrent().GetCurrentTile(pPlayer);
		const Vec2 dir          = GetOwner().GetComponent<CMovement>()->GetDir();
		CTile*     pNext        = pCurrentTile->GetNextTile(dir);

		m_pObject->GetComponent<CTransform>()->SetPos(pNext->GetComponent<CTransform>()->GetPos());
		if (m_pObject->GetType() == OBJECT_TYPE::ITEM && m_pObject->GetName() != L"FEED")
		{
			const bool result = CheckMoneyTile();
			if (true == result)
			{
				CItem* pItem = static_cast<CItem*>(m_pObject);
				pPlayer->SetMoney(pPlayer->GetMoney() + pItem->GetInfo().price);

				CCore::GetInstance().GetEventManager().Send<DeletionEvent>(DeletionEvent{pItem});
			}
			else
			{
				static_cast<CItem*>(m_pObject)->SetThrow(true);
			}
		}
		else if (nullptr != m_pObject && m_pObject->GetName() == L"FEED")
		{
			const Vec2       pos       = GetOwner().GetComponent<CCollider>()->GetFinalPos();
			const STAGE_TYPE stageType = CCore::GetInstance().GetStageManager().GetStageType();
			CTile*           pTile     = pCurrentTile->GetNextTile(dir);
			CItem*           pItem     = static_cast<CItem*>(m_pObject);
			if (stageType == STAGE_TYPE::COW)
			{
				bool isAvailable = false;
				if (pTile)
				{
					isAvailable = setFeedTile.count(pTile->GetTileInfo().id) != 0;
				}
				if (nullptr == pTile
					|| nullptr != pTile->GetLyingObject()
					|| false == isAvailable)
				{
					pItem->GetComponent<CTransform>()->SetPos(pos + dir * 40.f);
					pItem->SetThrow(true);
				}
				else if (isAvailable)
				{
					pItem->GetComponent<CTransform>()->SetPos(pTile->GetComponent<CTransform>()->GetPos());
					pTile->SetLyingObject(pItem);
					pPlayer->SetCowFeedCount(pPlayer->GetCowFeedCount() + 1);
				}
			}
			else if (stageType == STAGE_TYPE::CHICKEN)
			{
				CTile* pAfterTile = pTile->GetUp();

				if (nullptr == pTile
					|| nullptr == pAfterTile
					|| nullptr != pAfterTile->GetLyingObject()
					|| setFeedTile.count(pAfterTile->GetTileInfo().id) == 0)
				{
					pItem->GetComponent<CTransform>()->SetPos(pos + dir * 40.f);
					pItem->SetThrow(true);
				}
				else
				{
					pItem->GetComponent<CTransform>()->SetPos(pAfterTile->GetComponent<CTransform>()->GetPos());
					pAfterTile->SetLyingObject(pItem);
					pPlayer->SetChickenFeedCount(pPlayer->GetChickenFeedCount() + 1);
				}
			}
			else
			{
				pItem->SetThrow(true);
			}
		}
		if (m_pObject->GetType() == OBJECT_TYPE::ANIMAL)
		{
			static_cast<CAnimal*>(m_pObject)->SetCarry(false);
			m_pObject->GetComponent<CCollider>()->Enable(true);
		}
		if (m_pObject->GetType() == OBJECT_TYPE::OBSTACLE)
		{
			CObstacle* pObstacle           = static_cast<CObstacle*>(m_pObject);
			m_pTile->GetTileInfo().canJump = true;
			m_pTile->GetTileInfo().canMove = false;
			m_pTile->SetLyingObject(pObstacle);
			pObstacle->GetComponent<CTransform>()->SetPos(m_pTile->GetComponent<CTransform>()->GetPos());
			CSpriteComponent* pSpriteComponent = pObstacle->GetComponent<CSpriteComponent>();
			SpriteInfo*       pSpriteInfo      = CCore::GetInstance().GetResourceManager().
			                                                          GetSpriteInfo(EXTRA::TYPES[EXTRA::WOOD]);
			pSpriteComponent->SetTexture(EXTRA::TYPES[EXTRA::WOOD], LR"(texture\extra.bmp)");
			pSpriteComponent->SetInfo(pSpriteInfo);
		}
		pPlayer->SetCarryingObject(nullptr);
		m_pObject = nullptr;
		ChangeState(L"IDLE");


	}
}

void CCarryThrowState::Enter()
{
	const Vec2   dir         = GetOwner().GetComponent<CMovement>()->GetDir();
	CGameObject* pGameObject = static_cast<CPlayer*>(&GetOwner())->GetCarryingObject();
	if (nullptr == pGameObject)
	{
		ChangeState(L"IDLE");
		return;
	}

	if (pGameObject->GetType() == OBJECT_TYPE::OBSTACLE)
	{
		CTile* pCurrentTile = CCore::GetInstance().GetStageManager().GetCurrent().GetCurrentTile(&GetOwner());
		CTile* pTile        = pCurrentTile->GetNextTile(dir);
		if (!pTile && setFarmTile.count(pTile->GetTileInfo().id) == 0)
		{
			ChangeState(L"CARRY_IDLE");
			return;
		}
		m_pTile = pTile;
	}
	GetOwner().GetComponent<CAnimator>()->PlayGroup(
		dir, PLAYER::GROUPS::ANIMATIONS[PLAYER::GROUPS::CARRYING_THROW_0], false);

	m_pObject = pGameObject;

	CSound* pSound = CCore::GetInstance().GetResourceManager().FindSound(L"putdown");
	pSound->SetPosition(0);
	pSound->Play(false);

}

void CCarryThrowState::Exit()
{
	GetOwner().GetComponent<CAnimator>()->GetCurrent().Reset();
}

bool CCarryThrowState::CheckMoneyTile()
{
	const Vec2 dir          = GetOwner().GetComponent<CMovement>()->GetDir();
	CTile*     pCurrentTile = CCore::GetInstance().GetStageManager().GetCurrent().GetCurrentTile(&GetOwner());

	CTile* pTile = pCurrentTile->GetNextTile(dir);
	CTile* pAfterTile{};
	if (pTile)
	{
		pAfterTile = pTile->GetNextTile(dir);
	}

	if (nullptr == pTile || nullptr == pAfterTile)
	{
		return false;
	}

	if (setMoneyTile.count(pTile->GetTileInfo().id) != 0
		|| setMoneyTile.count(pAfterTile->GetTileInfo().id) != 0)
	{
		return true;
	}
	return false;
}
