#include "pch.h"
#include "CPlayerIdle.h"

#include <mutex>

#include "CAnimator.h"
#include "CCamera.h"
#include "CCollider.h"
#include "CPlayer.h"
#include "CCore.h"
#include "CEventManager.h"
#include "CKeyManager.h"
#include "CMovement.h"
#include "CObstacle.h"
#include "CPlot.h"
#include "CRayCast.h"
#include "CResourceManager.h"
#include "CSpriteComponent.h"
#include "CStage.h"
#include "CStageManager.h"
#include "CTile.h"

#include "CTimer.h"
#include "CUIManager.h"
#include "Name.h"


CPlayerIdle::CPlayerIdle()
	:
	CState{L"IDLE"},
	m_elapsedTime{0.f},
	m_maxTime{5.f},
	m_changeAnimation{false}
{
	srand(static_cast<UINT>(time(nullptr)));
}

CPlayerIdle::~CPlayerIdle() = default;

void CPlayerIdle::Update()
{
	if (GetOwner().IsDead())
	{
		return;
	}
	m_elapsedTime += DS;

	if (m_elapsedTime >= m_maxTime)
	{
		ChangeIdleAnimation();
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
	if (KEY_TAP(KEY::A))
	{
		Select();
	}
	if (KEY_TAP(KEY::S))
	{
		Whistle();
	}
	if (KEY_TAP(KEY::Z))
	{
		Use();
	}
	if (KEY_TAP(KEY::ENTER))
	{
		Info();
	}
	if (KEY_TAP(KEY::SPACE))
	{
		Confirm();
	}
}

void CPlayerIdle::Enter()
{
	const Vec2 dir = GetOwner().GetComponent<CMovement>()->GetDir();
	GetOwner().GetComponent<CAnimator>()->PlayGroup(dir, L"IDLE_0", false);
	CPlayer* pPlayer = static_cast<CPlayer*>(&GetOwner());
	if (pPlayer->IsFarmUpdated())
	{
		CCore::GetInstance().GetCamera().FadeIn(2.f);
		pPlayer->SetFarmUpdated(false);
		pPlayer->SetSleep(false);
	}

	CRayCast* pRay = pPlayer->GetRay();
	pRay->GetComponent<CCollider>()->Enable(false);
}

void CPlayerIdle::Exit()
{
	m_elapsedTime     = 0.0f;
	m_changeAnimation = false;

	CPlayer*  pPlayer = static_cast<CPlayer*>(&GetOwner());
	CRayCast* pRay    = pPlayer->GetRay();
	pRay->GetComponent<CCollider>()->Enable(false);
}

void CPlayerIdle::ChangeIdleAnimation()
{
	CAnimator* pAnimator = GetOwner().GetComponent<CAnimator>();
	const Vec2 dir       = GetOwner().GetComponent<CMovement>()->GetDir();

	const bool selectAnimation = rand() % 2;

	if (nullptr == pAnimator)
	{
		return;
	}

	if (false == m_changeAnimation)
	{
		if (true == selectAnimation)
		{
			pAnimator->PlayGroup(dir, L"IDLE_1", false);
		}
		else
		{
			pAnimator->PlayGroup(dir, L"IDLE_2", false);
		}

		m_changeAnimation = true;
	}
	if (pAnimator->GetCurrent().IsFinished())
	{
		pAnimator->GetCurrent().Reset();
		pAnimator->PlayGroup(dir, L"IDLE_0", false);
		m_changeAnimation = false;
		m_elapsedTime     = 0;
	}
}


void CPlayerIdle::Walk(const Vec2& _dir)
{
	GetOwner().GetComponent<CMovement>()->SetDir(_dir);
	ChangeState(L"WALK");
}

void CPlayerIdle::Run()
{
	ChangeState(L"RUN");
}

void CPlayerIdle::Select()
{
	GetOwner().GetComponent<CMovement>()->SetDir({0, 1});
	ChangeState(L"SELECT");
}

void CPlayerIdle::Use()
{
	ChangeState(L"USE");
}

void CPlayerIdle::Confirm()
{
	CPlayer* pPlayer = static_cast<CPlayer*>(&GetOwner());
	if (CheckWithTile())
	{
		pPlayer->SetCheckTile(true);
		ChangeState(L"CONFIRM");
	}
	else if (CheckWitheTileAfter())
	{
		pPlayer->SetCheckTileAfter(true);
		ChangeState(L"CONFIRM");
	}
	else
	{
		CRayCast* pRay = pPlayer->GetRay();
		pRay->GetComponent<CCollider>()->Enable(true);
	}
}

void CPlayerIdle::Info()
{
	CPlayer* pPlayer = static_cast<CPlayer*>(&GetOwner());
	pPlayer->SetInfoEnabled(true);
	ChangeState(L"UI");
}

void CPlayerIdle::Whistle()
{
	ChangeState(L"WHISTLE");
}

bool CPlayerIdle::CheckWithTile()
{
	static std::set<int> s_setGrassTile = {621, 617, 550, 548};

	CTile*     pCurrentTile = CCore::GetInstance().GetStageManager().GetCurrent().GetCurrentTile(&GetOwner());
	const Vec2 dir          = GetOwner().GetComponent<CMovement>()->GetDir();
	CTile*     pTile        = pCurrentTile->GetNextTile(dir);

	CGameObject* pObject = pTile->GetLyingObject();
	if (nullptr != pObject && pObject->GetType() == OBJECT_TYPE::PLOT)
	{
		CPlot* pPlot = static_cast<CPlot*>(pObject);

		if (pPlot->IsFullyGrowed() && pPlot->GetSeedItemId().kind != TOOL::SEED_GRASS)
		{
			CPlayer* pPlayer = static_cast<CPlayer*>(&GetOwner());

			CItem* pItem = new CItem{};
			pItem->SetInfo(pPlot->GetGrowedItemInfo());

			SpriteInfo* info = CCore::GetInstance().GetResourceManager().GetSpriteInfo(
				PLOT::TYPES[pPlot->GetGrowedItemInfo().id.kind]);
			pItem->GetComponent<CSpriteComponent>()->SetInfo(info);
			pItem->GetComponent<CSpriteComponent>()->SetTexture(
				pPlot->GetCurrentState(), LR"(texture\crops.bmp)");
			pItem->GetComponent<CSpriteComponent>()->SetEnabled(true);

			const Vec2 plotPos = pPlot->GetComponent<CTransform>()->GetPos();
			pItem->GetComponent<CTransform>()->SetPos(plotPos);
			pItem->GetComponent<CTransform>()->SetScale(pPlot->GetComponent<CTransform>()->GetScale());

			pItem->GetComponent<CCollider>()->SetOffsetPos(pItem->GetComponent<CTransform>()->GetScale() * .5f);
			pItem->GetComponent<CCollider>()->SetScale(pItem->GetComponent<CTransform>()->GetScale());
			pItem->GetComponent<CCollider>()->Enable(false);

			pPlot->Reset();
			const CreationEvent event{pItem, OBJECT_TYPE::ITEM};
			CCore::GetInstance().GetEventManager().Send<CreationEvent>(event);

			pPlayer->SetCarryingObject(pItem);

			return true;
		}
	}
	if (nullptr != pTile && s_setGrassTile.count(pTile->GetTileInfo().id) != 0)
	{
		CPlayer* pPlayer = static_cast<CPlayer*>(&GetOwner());
		if (pPlayer->GetGrassCount() <= 0)
		{
			return false;
		}
		pPlayer->SetGrass(pPlayer->GetGrassCount() - 1);

		CItem* pItem = new CItem{};
		pItem->SetName(L"FEED");
		SpriteInfo* info = CCore::GetInstance().GetResourceManager().GetSpriteInfo(EXTRA::TYPES[EXTRA::FEED]);
		pItem->GetComponent<CSpriteComponent>()->SetInfo(info);
		pItem->GetComponent<CSpriteComponent>()->SetTexture(EXTRA::TYPES[EXTRA::FEED],
		                                                    LR"(texture\extra.bmp)");
		pItem->GetComponent<CTransform>()->SetPos(pTile->GetComponent<CTransform>()->GetPos());

		const CreationEvent event{pItem, OBJECT_TYPE::ITEM};
		CCore::GetInstance().GetEventManager().Send<CreationEvent>(event);
		pPlayer->SetCarryingObject(pItem);
		return true;
	}
	if (nullptr != pTile && pTile->GetLyingObject() && pTile->GetLyingObject()->GetType() == OBJECT_TYPE::OBSTACLE)
	{
		CObstacle* pObstacle = static_cast<CObstacle*>(pTile->GetLyingObject());
		if (pObstacle->GetObstacleType() == OBSTACLE_TYPE::WALL)
		{
			CPlayer* pPlayer = static_cast<CPlayer*>(&GetOwner());
			if (pObstacle->GetHP() == 2)
			{
				CSpriteComponent* pSpriteComponent = pObstacle->GetComponent<CSpriteComponent>();
				SpriteInfo*       pSpriteInfo      = CCore::GetInstance().GetResourceManager().
				                                                          GetSpriteInfo(
					                                                          EXTRA::TYPES[EXTRA::WOOD_CARRYING]);
				pSpriteComponent->SetTexture(EXTRA::TYPES[EXTRA::WOOD_CARRYING], LR"(texture\extra.bmp)");
				pSpriteComponent->SetInfo(pSpriteInfo);

				pPlayer->SetCarryingObject(pObstacle);
				pTile->GetTileInfo().canJump = false;
				pTile->GetTileInfo().canMove = true;
				pTile->SetLyingObject(nullptr);
				return true;
			}
		}
	}
	return false;
}

bool CPlayerIdle::CheckWitheTileAfter()
{
	CTile*     pCurrentTile = CCore::GetInstance().GetStageManager().GetCurrent().GetCurrentTile(&GetOwner());
	const Vec2 dir          = GetOwner().GetComponent<CMovement>()->GetDir();
	CTile*     pTile        = pCurrentTile->GetNextTile(dir);
	CTile*     pTileAfter{};
	if (pTile)
	{
		pTileAfter = pTile->GetNextTile(dir);
	}

	CGameObject* pObject{};
	if (pTileAfter)
	{
		pObject = pTileAfter->GetLyingObject();
	}
	if (nullptr != pObject && pObject->GetType() == OBJECT_TYPE::NPC)
	{
		return true;
	}
	return false;
}
