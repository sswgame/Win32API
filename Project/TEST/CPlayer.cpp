#include "pch.h"
#include "CPlayer.h"

#include "CAnimator.h"
#include "CCollider.h"
#include "CFSM.h"

#include "CAnimal.h"
#include "CCarryIdleState.h"
#include "CCarryJump.h"
#include "CCarryRunState.h"
#include "CCarryThrowState.h"
#include "CCarryWalkState.h"

#include "CCore.h"
#include "CEventManager.h"
#include "Name.h"
#include "CPlayerIdle.h"
#include "CPlayerRun.h"
#include "CPlayerSelect.h"
#include "CPlayerWalk.h"
#include "CPlayerWhistle.h"
#include "CItem.h"
#include "CKeyManager.h"
#include "CMovement.h"
#include "CObstacle.h"
#include "CPlayerConfirm.h"
#include "CPlayerEat.h"
#include "CPlayerJump.h"
#include "CPlayerUIState.h"
#include "CPlayerUse.h"
#include "CPlot.h"
#include "CRayCast.h"
#include "CResourceManager.h"
#include "CSound.h"
#include "CSoundManager.h"
#include "CSpriteComponent.h"
#include "CStage.h"
#include "CStageManager.h"
#include "CTile.h"
#include "CTileManager.h"

namespace
{
	float VOLUME = 100.f;

	std::map<ItemID, ItemID> mapChangeTool =
	{
		{{TOOL::SPECIAL_AXE, ITEM_CATEGORY::TOOL}, {TOOL::NORMAL_AXE, ITEM_CATEGORY::TOOL}},
		{{TOOL::SPECIAL_HAMMER, ITEM_CATEGORY::TOOL}, {TOOL::NORMAL_HAMMER, ITEM_CATEGORY::TOOL}},
		{{TOOL::SPECIAL_HOE, ITEM_CATEGORY::TOOL}, {TOOL::NORMAL_HOE, ITEM_CATEGORY::TOOL}},
		{{TOOL::SPECIAL_WATER, ITEM_CATEGORY::TOOL}, {TOOL::NORMAL_WATER, ITEM_CATEGORY::TOOL}},
		{{TOOL::SPECIAL_SICKLE, ITEM_CATEGORY::TOOL}, {TOOL::NORMAL_SICKLE, ITEM_CATEGORY::TOOL}},
	};
}

CPlayer::CPlayer()
	:
	m_maxHp{100},
	m_woodCount{0},
	m_grassCount{0},
	m_money{0},
	m_arrSelected{nullptr},
	m_currentIndex{0},
	m_pCarrying{nullptr},
	m_pRayCast{nullptr},
	m_show{false},
	m_isBuyingAnimal{false},
	m_isToSleep{false},
	m_isUpdatingFarm{false},
	m_withRay{false},
	m_withTile{false},
	m_isBuyingItem{false},
	m_withTileAfter{false},
	m_cowFeedCount{0},
	m_chickenFeedCount{0}
{
	m_Hp = m_maxHp;
	SetName(L"Player");

	CMovement* pMovement = AddComponent<CMovement>();
	pMovement->SetDir({0, 1});

	CAnimator* pAnimator = AddComponent<CAnimator>();
	pAnimator->LoadAnimation(LR"(texture\player.bmp)", LR"(animation\player.xml)");

	SetAnimations();
	pAnimator->Play(PLAYER::ANIMATIONS[PLAYER::IDLE_FRONT_0]);

	constexpr int tileSize = 40;

	GetComponent<CTransform>()->SetScale(Vec2{tileSize, tileSize} * .8f);

	CCollider* pCollider = AddComponent<CCollider>();
	pCollider->SetOffsetPos(Vec2{0.f, -GetComponent<CTransform>()->GetScale().y * .5f});
	pCollider->SetScale(GetComponent<CTransform>()->GetScale());


	CFSM* pFSM = AddComponent<CFSM>();
	pFSM->AddState(new CPlayerIdle{});
	pFSM->AddState(new CPlayerWalk{});
	pFSM->AddState(new CPlayerRun{});
	pFSM->AddState(new CPlayerSelect{});
	pFSM->AddState(new CPlayerWhistle{});
	pFSM->AddState(new CPlayerUse{});
	pFSM->AddState(new CPlayerConfirm{});
	pFSM->AddState(new CPlayerJump{});
	pFSM->AddState(new CCarryIdleState{});
	pFSM->AddState(new CCarryRunState{});
	pFSM->AddState(new CCarryWalkState{});
	pFSM->AddState(new CCarryThrowState{});
	pFSM->AddState(new CCarryJump{});
	pFSM->AddState(new CPlayerUIState{});
	pFSM->AddState(new CPlayerEat{});
	pFSM->SetState(L"IDLE");

	srand(static_cast<UINT>(time(nullptr)));
}

CPlayer::~CPlayer() = default;

void CPlayer::UpdateSpecific()
{
	if (KEY_TAP(KEY::N1))
	{
		m_Hp -= static_cast<int>(m_maxHp * 0.125f);
		if (m_Hp < 0)
		{
			m_Hp = 0;
		}
	}
	if (KEY_TAP(KEY::N2))
	{
		m_Hp += static_cast<int>(m_maxHp * 0.125f);
		if (m_Hp > m_maxHp)
		{
			m_Hp = m_maxHp;
		}
	}
	if (KEY_TAP(KEY::N3))
	{
		CAnimal* pAnimal = new CAnimal{ANIMAL_TYPE::COW};
		pAnimal->GetComponent<CTransform>()->SetPos(GetComponent<CTransform>()->GetPos() + Vec2{50.f, 0.f});
		CCore::GetInstance().GetEventManager().Send<CreationEvent>(CreationEvent{pAnimal, OBJECT_TYPE::ANIMAL});
	}
	if (KEY_TAP(KEY::N4))
	{
		CAnimal* pAnimal = new CAnimal{ANIMAL_TYPE::CHICKEN};
		pAnimal->GetComponent<CTransform>()->SetPos(GetComponent<CTransform>()->GetPos() + Vec2{50.f, 0.f});
		CCore::GetInstance().GetEventManager().Send<CreationEvent>(CreationEvent{pAnimal, OBJECT_TYPE::ANIMAL});
	}
	if (KEY_TAP(KEY::N5))
	{
		m_money += 1000;
		m_grassCount += 10;
		m_woodCount += 10;
	}

	if (KEY_TAP(KEY::NUMPAD_0))
	{
		CSound* pSound = CCore::GetInstance().GetSoundManager().GetCurrentBGM();
		pSound->SetVolume(VOLUME - 10.f);
		VOLUME = (VOLUME < 0) ? 0 : VOLUME - 10.f;
	}
	if (KEY_TAP(KEY::NUMPAD_2))
	{
		CSound* pSound = CCore::GetInstance().GetSoundManager().GetCurrentBGM();
		pSound->SetVolume(VOLUME + 10.f);
		VOLUME = (VOLUME > 100.f) ? 100.f : VOLUME + 10.f;
	}
}

CItem* CPlayer::GetCurrentItem()
{
	return m_arrSelected[m_currentIndex];
}

void CPlayer::SelectItem()
{
	++m_currentIndex;
	m_currentIndex = m_currentIndex % 2;
}

void CPlayer::OnCollisionEnter(CGameObject* _pOther)
{
	const OBJECT_TYPE type = _pOther->GetType();
	switch (type)
	{
	case OBJECT_TYPE::TELEPORTER:
		{
			if (_pOther->GetName() == L"FARM")
		{
			CCore::GetInstance().GetEventManager().Send<ChangeStageEvent>(STAGE_TYPE::FARM);
		}
			else if (_pOther->GetName() == L"HOME")
			{
				CCore::GetInstance().GetEventManager().Send<ChangeStageEvent>(STAGE_TYPE::HOUSE);
		}
			else if (_pOther->GetName() == L"GARAGE")
			{
				CCore::GetInstance().GetEventManager().Send<ChangeStageEvent>(STAGE_TYPE::GARAGE);
		}
			else if (_pOther->GetName() == L"COW")
			{
				CCore::GetInstance().GetEventManager().Send<ChangeStageEvent>(STAGE_TYPE::COW);
			}
			else if (_pOther->GetName() == L"CHICKEN")
			{
				CCore::GetInstance().GetEventManager().Send<ChangeStageEvent>(STAGE_TYPE::CHICKEN);
			}
			else if (_pOther->GetName() == L"STREET")
			{
				CCore::GetInstance().GetEventManager().Send<ChangeStageEvent>(STAGE_TYPE::STREET);
			}
			else if (_pOther->GetName() == L"SHOP")
			{
				CCore::GetInstance().GetEventManager().Send<ChangeStageEvent>(STAGE_TYPE::SHOP);
			}
			else if (_pOther->GetName() == L"MOUNTAIN")
			{
				CCore::GetInstance().GetEventManager().Send<ChangeStageEvent>(STAGE_TYPE::MOUNTAIN);
			}
		}
		break;
	case OBJECT_TYPE::ITEM: {}
	case OBJECT_TYPE::ANIMAL: { }
	case OBJECT_TYPE::NPC: {}
		break;
	default:
		break;
	}
}

void CPlayer::OnCollisionStay(CGameObject* _pOther)
{
	OBJECT_TYPE type = _pOther->GetType();
	if (type != OBJECT_TYPE::TELEPORTER)
	{
		GetComponent<CMovement>()->CheckCanMove(_pOther);
	}
}

void CPlayer::Use()
{
	CItem* pItem = GetCurrentItem();
	assert(pItem);
	const ItemID id = pItem->GetInfo().id;
	switch (id.type)
	{
	case ITEM_CATEGORY::TOOL:
		{
			switch (id.kind)
		{
		case TOOL::NORMAL_WATER:
		case TOOL::SPECIAL_WATER:
			DoWater(id);
			break;
		case TOOL::NORMAL_HOE:
		case TOOL::SPECIAL_HOE:
			DoHoe(id);
			break;
		case TOOL::NORMAL_AXE:
		case TOOL::SPECIAL_AXE:
			DoAxe(id);
			break;
		case TOOL::NORMAL_SICKLE:
		case TOOL::SPECIAL_SICKLE:
			DoSickle(id);
			break;
		case TOOL::NORMAL_HAMMER:
		case TOOL::SPECIAL_HAMMER:
			DoHammer(id);
			break;
		case TOOL::CHICKEN_FOOD:
		case TOOL::COW_FOOD:
			DoFeed(id);
			break;
		case TOOL::BELL:
			DoBell();
			break;
		case TOOL::BRUSH:
			DoBrush();
			break;
		case TOOL::MEDICINE:
		case TOOL::SPECIAL_MEDICINE:
			DoMedicine(id);
			break;
		case TOOL::SEED_POTATO:
		case TOOL::SEED_TOMATO:
		case TOOL::SEED_GRASS:
		case TOOL::SEED_TURNIP:
		case TOOL::SEED_CORN:
			DoSeed(id);
			break;
		case TOOL::PAINT:
			DoPainting();
			break;
		case TOOL::MILKING_TOOL:
			DoMilking();
			break;
		default: ;
		}
		}
		break;
	case ITEM_CATEGORY::SELL: break;
	case ITEM_CATEGORY::DEFAULT:

		break;
	default: ;
	}
	m_Hp += BASIC_INFO::MAP_DATA[id].hp;
	UseItem();
}

void CPlayer::DoSeed(ItemID _id)
{
	CSound* pSound = CCore::GetInstance().GetResourceManager().FindSound(L"seeding");
	pSound->SetPosition(20.f);
	pSound->Play(false);
	const CTile*              pCurrentTile = CCore::GetInstance().GetStageManager().GetCurrent().GetCurrentTile(this);
	const std::vector<CTile*> vecResult    = CCore::GetInstance().GetStageManager().GetCurrent().GetTilesAround(
		pCurrentTile);

	for (int i = 0; i < vecResult.size(); ++i)
	{
		CGameObject* pObject = vecResult[i]->GetLyingObject();
		if (pObject && pObject->GetType() == OBJECT_TYPE::PLOT)
		{
			CPlot* pPlot = static_cast<CPlot*>(pObject);
			if (true == pPlot->IsSeeded())
			{
				continue;
			}
			pPlot->SetSeed(_id);
			pPlot->SetIsSeeded(true);
			SpriteInfo* info = CCore::GetInstance().GetResourceManager().GetSpriteInfo(
				PLOT::TYPES[PLOT::SEEDED_PLOT]);
			vecResult[i]->GetComponent<CSpriteComponent>()->SetInfo(info);

			//TODO::NEED TO DO MAKE PLOT CLASS
			vecResult[i]->GetComponent<CSpriteComponent>()->SetTexture(
				PLOT::TYPES[PLOT::SEEDED_PLOT], LR"(texture\crops.bmp)");
		}
	}
}

void CPlayer::DoHoe(ItemID _id)
{
	CSound* pSound = CCore::GetInstance().GetResourceManager().FindSound(L"plot");
	pSound->SetPosition(20.f);
	pSound->Play(false);


	static std::set<int> setAvailableTiles =
	{
		179, 250, 251, 252, 184, 187, 188, 192, 193, 195, 196, 231, 197, 198, 199, 200,
		164, 165, 166, 167, 168, 169, 170, 171, 172, 207, 208,
	};

	CTile*     pCurrentTile = CCore::GetInstance().GetStageManager().GetCurrent().GetCurrentTile(this);
	const Vec2 dir          = GetComponent<CMovement>()->GetDir();
	int        range        = 1;
	if (_id.kind == TOOL::SPECIAL_HOE)
	{
		range = 6;
	}
	CTile* pTile = pCurrentTile->GetNextTile(dir);

	for (int i = 0; i < range; ++i)
	{
		if (nullptr != pTile && pTile->GetTileInfo().canMove)
		{
			if (nullptr == pTile->GetLyingObject() && setAvailableTiles.count(pTile->GetTileInfo().id) != 0)
			{
				CPlot* pPlot = new CPlot{};
				pPlot->SetTile(pTile);
				pPlot->GetComponent<CTransform>()->SetPos(pTile->GetComponent<CTransform>()->GetPos());
				pPlot->GetComponent<CTransform>()->SetScale(pTile->GetComponent<CTransform>()->GetScale());
				SpriteInfo* info = CCore::GetInstance().GetResourceManager().
				                                        GetSpriteInfo(PLOT::TYPES[PLOT::EMPTY_PLOT]);

				pTile->GetComponent<CSpriteComponent>()->SetInfo(info);
				pTile->GetComponent<CSpriteComponent>()->SetTexture(
					PLOT::TYPES[PLOT::EMPTY_PLOT], LR"(texture\crops.bmp)");
				pTile->SetLyingObject(pPlot);

				CreationEvent event{pPlot, OBJECT_TYPE::PLOT};
				CCore::GetInstance().GetEventManager().Send<CreationEvent>(event);
			}
			pTile = pTile->GetNextTile(dir);
		}
		else
		{
			break;
		}
	}
}

void CPlayer::DoWater(ItemID _id)
{
	CSound* pSound = CCore::GetInstance().GetResourceManager().FindSound(L"water");
	pSound->SetPosition(50.f);
	pSound->Play(false);

	static std::vector<std::pair<std::wstring, std::wstring>> vecAvailableTiles =
	{
		{PLOT::TYPES[PLOT::EMPTY_PLOT], PLOT::TYPES[PLOT::EMPTY_WATERED_PLOT]},
		{PLOT::TYPES[PLOT::SEEDED_PLOT], PLOT::TYPES[PLOT::SEEDED_WATERED_PLOT]},
		{PLOT::TYPES[PLOT::TURNIP_0], PLOT::TYPES[PLOT::TURNIP_WATERED_0]},
		{PLOT::TYPES[PLOT::TURNIP_1], PLOT::TYPES[PLOT::TURNIP_WATERED_1]},
		{PLOT::TYPES[PLOT::POTATO_0], PLOT::TYPES[PLOT::POTATO_WATERED_0]},
		{PLOT::TYPES[PLOT::POTATO_1], PLOT::TYPES[PLOT::POTATO_WATERED_1]},
		{PLOT::TYPES[PLOT::TOMATO_0], PLOT::TYPES[PLOT::TOMATO_WATERED_0]},
		{PLOT::TYPES[PLOT::TOMATO_1], PLOT::TYPES[PLOT::TOMATO_WATERED_1]},
		{PLOT::TYPES[PLOT::TOMATO_2], PLOT::TYPES[PLOT::TOMATO_WATERED_2]},
		{PLOT::TYPES[PLOT::TOMATO_3], PLOT::TYPES[PLOT::TOMATO_WATERED_3]},
		{PLOT::TYPES[PLOT::CORN_0], PLOT::TYPES[PLOT::CORN_WATERED_0]},
		{PLOT::TYPES[PLOT::CORN_1], PLOT::TYPES[PLOT::CORN_WATERED_1]},
		{PLOT::TYPES[PLOT::CORN_2], PLOT::TYPES[PLOT::CORN_WATERED_2]},
		{PLOT::TYPES[PLOT::CORN_3], PLOT::TYPES[PLOT::CORN_WATERED_3]},
	};
	CTile*     pCurrentTile = CCore::GetInstance().GetStageManager().GetCurrent().GetCurrentTile(this);
	const Vec2 dir          = GetComponent<CMovement>()->GetDir();
	if (_id.kind == TOOL::NORMAL_WATER)
	{
		CTile* pTile = pCurrentTile->GetNextTile(dir);
		if (nullptr == pTile)
		{
			return;
		}
		for (int i = 0; i < vecAvailableTiles.size(); ++i)
		{
			CGameObject* pObject = pTile->GetLyingObject();
			if (pObject && pObject->GetType() == OBJECT_TYPE::PLOT)
			{
				CPlot* pPlot = static_cast<CPlot*>(pObject);

				if (pPlot->GetCurrentState() == vecAvailableTiles[i].first)
				{
					pPlot->SetWatered(true);

					SpriteInfo* info = CCore::GetInstance().GetResourceManager().GetSpriteInfo(
						vecAvailableTiles[i].second);

					pTile->GetComponent<CSpriteComponent>()->SetInfo(info);
					pTile->SetName(vecAvailableTiles[i].second);
					pTile->GetComponent<CSpriteComponent>()->SetTexture(
						vecAvailableTiles[i].second, LR"(texture\crops.bmp)");
					break;
				}
			}
		}
	}
	else
	{
		const std::vector<CTile*> vecResult = CCore::GetInstance().GetStageManager().GetCurrent().GetTilesAround(
			pCurrentTile);

		for (int i = 0; i < vecResult.size(); ++i)
		{
			for (int j = 0; j < vecAvailableTiles.size(); ++j)
			{
				CGameObject* pObject = vecResult[i]->GetLyingObject();
				if (pObject && pObject->GetType() == OBJECT_TYPE::PLOT)
				{
					CPlot* pPlot = static_cast<CPlot*>(pObject);
					if (pPlot->GetCurrentState() == vecAvailableTiles[j].first)
					{
						pPlot->SetWatered(true);
						SpriteInfo* info = CCore::GetInstance().GetResourceManager().GetSpriteInfo(
							vecAvailableTiles[j].second);

						vecResult[i]->GetComponent<CSpriteComponent>()->SetInfo(info);
						vecResult[i]->GetComponent<CSpriteComponent>()->SetTexture(
							vecAvailableTiles[j].second, LR"(texture\crops.bmp)");
						break;
					}
				}
			}
		}
	}
}

void CPlayer::DoSickle(ItemID _id)
{
	CSound* pSound = CCore::GetInstance().GetResourceManager().FindSound(L"sickle");
	pSound->SetPosition(20.f);
	pSound->Play(false);

	static std::pair<std::wstring, std::wstring> target = {EXTRA::TYPES[EXTRA::GRASS_3], EXTRA::TYPES[EXTRA::GRASS_0]};

	const Vec2 dir          = GetComponent<CMovement>()->GetDir();
	CTile*     pCurrentTile = CCore::GetInstance().GetStageManager().GetCurrent().GetCurrentTile(this);

	if (_id.kind == TOOL::NORMAL_SICKLE)
	{
		CTile* pTile = pCurrentTile->GetNextTile(dir);

		CGameObject* pObject = pTile->GetLyingObject();
		if (pObject && pObject->GetType() == OBJECT_TYPE::PLOT)
		{
			CPlot* pPlot = static_cast<CPlot*>(pObject);

			if (pPlot->GetCurrentState() == target.first)
			{
				SpriteInfo* info = CCore::GetInstance().GetResourceManager().GetSpriteInfo(
					target.second);

				pTile->GetComponent<CSpriteComponent>()->SetInfo(info);
				pTile->GetComponent<CSpriteComponent>()->SetTexture(
					target.second, LR"(texture\extra.bmp)");
				pPlot->Reset();

				CItem* pItem = new CItem{};
				pItem->GetComponent<CTransform>()->SetPos(
					pPlot->GetComponent<CTransform>()->GetPos() + pTile->GetComponent<CTransform>()->GetScale() *
					.5f);
				info = CCore::GetInstance().GetResourceManager().GetSpriteInfo(EXTRA::TYPES[EXTRA::GRASS_CRASHED]);
				pItem->GetComponent<CSpriteComponent>()->SetInfo(info);
				pItem->GetComponent<CSpriteComponent>()->SetTexture(EXTRA::TYPES[EXTRA::GRASS_CRASHED],
				                                                    LR"(texture\extra.bmp)");
				pItem->GetComponent<CTransform>()->SetScale(pTile->GetComponent<CTransform>()->GetScale());
				pItem->SetThrow(true);
				CCore::GetInstance().GetEventManager().Send<CreationEvent>(CreationEvent{pItem, OBJECT_TYPE::ITEM});

				SetGrass(m_grassCount + 1);
			}
		}
		else if (pObject && pObject->GetType() == OBJECT_TYPE::OBSTACLE)
		{
			CObstacle* pObstacle = static_cast<CObstacle*>(pObject);
			if (pObstacle->GetObstacleType() == OBSTACLE_TYPE::GRASS)
			{
				pObstacle->SetHP(0);
			}
		}
	}
	else
	{
		const std::vector<CTile*> vecResult = CCore::GetInstance().GetStageManager().GetCurrent().GetTilesAround(
			pCurrentTile);

		for (int i = 0; i < vecResult.size(); ++i)
		{
			CGameObject* pObject = vecResult[i]->GetLyingObject();
			if (pObject && pObject->GetType() == OBJECT_TYPE::PLOT)
			{
				CPlot* pPlot = static_cast<CPlot*>(pObject);

				if (pPlot->GetCurrentState() == target.first)
				{
					SpriteInfo* info = CCore::GetInstance().GetResourceManager().GetSpriteInfo(
						target.second);

					vecResult[i]->GetComponent<CSpriteComponent>()->SetInfo(info);
					vecResult[i]->GetComponent<CSpriteComponent>()->SetTexture(
						target.second, LR"(texture\extra.bmp)");
					pPlot->Reset();

					CItem* pItem = new CItem{};
					Vec2   size  = vecResult[i]->GetComponent<CTransform>()->GetScale();
					pItem->GetComponent<CTransform>()->SetPos(pPlot->GetComponent<CTransform>()->GetPos() + size * .5f);
					info = CCore::GetInstance().GetResourceManager().GetSpriteInfo(EXTRA::TYPES[EXTRA::GRASS_CRASHED]);
					pItem->GetComponent<CSpriteComponent>()->SetInfo(info);
					pItem->GetComponent<CSpriteComponent>()->SetTexture(EXTRA::TYPES[EXTRA::GRASS_CRASHED],
					                                                    LR"(texture\extra.bmp)");
					pItem->GetComponent<CTransform>()->SetScale(size);
					pItem->SetThrow(true);

					CCore::GetInstance().GetEventManager().Send<CreationEvent>(CreationEvent{pItem, OBJECT_TYPE::ITEM});
					SetGrass(m_grassCount + 1);
				}
			}
			else if (pObject && pObject->GetType() == OBJECT_TYPE::OBSTACLE)
			{
				CObstacle* pObstacle = static_cast<CObstacle*>(pObject);
				if (pObstacle->GetObstacleType() == OBSTACLE_TYPE::GRASS)
				{
					pObstacle->SetHP(0);
				}
			}
		}
	}
}

void CPlayer::DoHammer(ItemID _id)
{
	CSound* pSound = CCore::GetInstance().GetResourceManager().FindSound(L"hammer");
	pSound->SetPosition(0);
	pSound->Play(false);
	CTile* pCurrentTile = CCore::GetInstance().GetStageManager().GetCurrent().GetCurrentTile(this);

	const Vec2 dir    = GetComponent<CMovement>()->GetDir();
	int        damage = 1;
	if (_id.kind != TOOL::NORMAL_HAMMER)
	{
		damage = 6;
	}
	CTile*       pTile       = pCurrentTile->GetNextTile(dir);
	CGameObject* pGameObject = pTile->GetLyingObject();
	if (pGameObject && pGameObject->GetType() == OBJECT_TYPE::OBSTACLE)
	{
		CObstacle* pObstacle = static_cast<CObstacle*>(pGameObject);
		if (pObstacle->GetObstacleType() == OBSTACLE_TYPE::BIG_ROCK ||
			pObstacle->GetObstacleType() == OBSTACLE_TYPE::SMALL_ROCK)
		{
			int hp = pObstacle->GetHP();
			hp -= damage;
			pObstacle->SetHP(hp);
		}
		else if (pObstacle->GetObstacleType() == OBSTACLE_TYPE::WALL)
		{
			if (pObstacle->GetHP() == 1)
			{
				pObstacle->SetHP(0);
			}
		}
	}
}

void CPlayer::DoBrush()
{
	CTile*     pCurrentTile = CCore::GetInstance().GetStageManager().GetCurrent().GetCurrentTile(this);
	const Vec2 dir          = GetComponent<CMovement>()->GetDir();
	CTile*     pTile        = pCurrentTile->GetNextTile(dir);

	CItem* pItem = GetCurrentItem();
	pItem->GetComponent<CCollider>()->Enable(true);
	pItem->GetComponent<CTransform>()->SetPos(pTile->GetComponent<CTransform>()->GetPos());
	pItem->GetComponent<CCollider>()->SetScale(Vec2{20.f, 20.f});
}

void CPlayer::DoMilking()
{
	CTile*     pCurrentTile = CCore::GetInstance().GetStageManager().GetCurrent().GetCurrentTile(this);
	const Vec2 dir          = GetComponent<CMovement>()->GetDir();
	CTile*     pTile        = pCurrentTile->GetNextTile(dir);

	CItem* pItem = GetCurrentItem();
	pItem->GetComponent<CCollider>()->Enable(true);
	pItem->GetComponent<CTransform>()->SetPos(pTile->GetComponent<CTransform>()->GetPos());
	pItem->GetComponent<CCollider>()->SetScale(Vec2{20.f, 20.f});
}

void CPlayer::DoPainting() {}

void CPlayer::DoMedicine(ItemID _id)
{
	CTile*     pCurrentTile = CCore::GetInstance().GetStageManager().GetCurrent().GetCurrentTile(this);
	const Vec2 dir          = GetComponent<CMovement>()->GetDir();
	CTile*     pTile        = pCurrentTile->GetNextTile(dir);

	const Vec2 pos   = pTile->GetComponent<CTransform>()->GetPos();
	const Vec2 scale = pTile->GetComponent<CTransform>()->GetScale();
	RECT       tileRect{};
	tileRect.left   = static_cast<int>(pos.x);
	tileRect.right  = static_cast<int>(pos.x + scale.x);
	tileRect.top    = static_cast<int>(pos.y);
	tileRect.bottom = static_cast<int>(pos.y + scale.y);

	const std::vector<CGameObject*>& vecAnimal = CCore::GetInstance().GetStageManager().GetCurrent().GetObjects(
		OBJECT_TYPE::ANIMAL);

	for (int i = 0; i < vecAnimal.size(); ++i)
	{
		CAnimal* pAnimal = static_cast<CAnimal*>(vecAnimal[i]);

		if (pAnimal->GetInfo().type == ANIMAL_TYPE::COW)
		{
			const Vec2 animalPos   = pAnimal->GetComponent<CCollider>()->GetFinalPos();
			const Vec2 animalScale = pAnimal->GetComponent<CCollider>()->GetScale();
			RECT       animalRect{};
			animalRect.left   = static_cast<int>(animalPos.x - animalScale.x * .5f);
			animalRect.right  = static_cast<int>(animalPos.x + animalScale.x * .5f);
			animalRect.top    = static_cast<int>(animalPos.y - animalScale.y * .5f);
			animalRect.bottom = static_cast<int>(animalPos.y + animalScale.y * .5f);

			RECT rect{};

			IntersectRect(&rect, &tileRect, &animalRect);
			const int width  = rect.right - rect.left;
			const int height = rect.bottom - rect.top;

			const bool isCollided = width || height;

			if (isCollided && pAnimal->GetInfo().isSick)
			{
				AnimalInfo info = pAnimal->GetInfo();
				info.isSick     = false;
				info.hp         = info.maxHp;
				pAnimal->SetInfo(info);
				break;
			}
			if (isCollided
				&& false == pAnimal->GetInfo().isSick
				&& false == pAnimal->GetInfo().isPregnant
				&& _id == ItemID{TOOL::SPECIAL_MEDICINE, ITEM_CATEGORY::TOOL}
				&& pAnimal->GetInfo().currentLevel == GROW_LEVEL::ADULT)
			{
				AnimalInfo info   = pAnimal->GetInfo();
				info.isPregnant   = true;
				info.currentLevel = GROW_LEVEL::PREGNANT;
				pAnimal->SetInfo(info);
				break;
			}
		}
	}


}

void CPlayer::DoBell()
{
	std::vector<CGameObject*>& vecAnimals = CCore::GetInstance().GetStageManager().GetCurrent().GetObjects(
		OBJECT_TYPE::ANIMAL);
	Vec2 playerPos = GetComponent<CTransform>()->GetPos();
	Vec2 playerDir = GetComponent<CMovement>()->GetDir();
	for (int i = 0; i < vecAnimals.size(); ++i)
	{
		CAnimal* pAnimal = static_cast<CAnimal*>(vecAnimals[i]);
		if (pAnimal->GetInfo().type == ANIMAL_TYPE::COW)
		{
			Vec2 dir = playerDir * -1;
			pAnimal->GetComponent<CMovement>()->SetDir(dir);
			pAnimal->GetComponent<CFSM>()->ChangeState(L"COW_WALK");
		}
	}
}

void CPlayer::DoFeed(ItemID _id)
{
	const Vec2 pos          = GetComponent<CCollider>()->GetFinalPos();
	const Vec2 dir          = GetComponent<CMovement>()->GetDir();
	CTile*     pCurrentTile = CCore::GetInstance().GetStageManager().GetCurrent().GetCurrentTile(this);

	if (_id.kind == TOOL::CHICKEN_FOOD)
	{
		++m_chickenFeedCount;
		constexpr int availableTile = 522;
		CItem*        pItem         = new CItem{};
		pItem->SetName(L"FEED");
		SpriteInfo* info = CCore::GetInstance().GetResourceManager().GetSpriteInfo(EXTRA::TYPES[EXTRA::FEED]);
		pItem->GetComponent<CSpriteComponent>()->SetInfo(info);
		pItem->GetComponent<CSpriteComponent>()->SetTexture(EXTRA::TYPES[EXTRA::FEED],
		                                                    LR"(texture\extra.bmp)");

		CTile* pTile      = pCurrentTile->GetNextTile(dir);
		CTile* pAfterTile = pTile->GetUp();

		if (nullptr == pTile
			|| nullptr == pAfterTile
			|| nullptr != pAfterTile->GetLyingObject()
			|| pAfterTile->GetTileInfo().id != availableTile)
		{
			pItem->GetComponent<CTransform>()->SetPos(pos + dir * 40.f);
			pItem->SetThrow(true);
		}
		else
		{
			pItem->GetComponent<CTransform>()->SetPos(pAfterTile->GetComponent<CTransform>()->GetPos());
			pAfterTile->SetLyingObject(pItem);
		}
		CCore::GetInstance().GetEventManager().Send<CreationEvent>(CreationEvent{pItem, OBJECT_TYPE::ITEM});
	}
	else
	{
		++m_cowFeedCount;
		constexpr int availableTile[2] = {593, 605};
		CItem*        pItem = new CItem{};
		SpriteInfo*   info = CCore::GetInstance().GetResourceManager().GetSpriteInfo(EXTRA::TYPES[EXTRA::FEED]);
		pItem->GetComponent<CSpriteComponent>()->SetInfo(info);
		pItem->GetComponent<CSpriteComponent>()->SetTexture(EXTRA::TYPES[EXTRA::FEED],
		                                                    LR"(texture\extra.bmp)");

		CTile* pTile       = pCurrentTile->GetNextTile(dir);
		bool   isAvailable = false;
		if (pTile)
		{
			isAvailable = (pTile->GetTileInfo().id == availableTile[0]) || (pTile->GetTileInfo().id == availableTile[
				1]);
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
		}
		CCore::GetInstance().GetEventManager().Send<CreationEvent>(CreationEvent{pItem, OBJECT_TYPE::ITEM});

		/*CItem* pCurrent = GetCurrentItem();
		--pCurrent->GetInfo().currentCount;
		if (pCurrent->GetInfo().currentCount <= 0)
		{
			if (m_arrSelected[0] == pCurrent)
			{
				m_arrSelected[0] = nullptr;
			}
			else
			{
				m_arrSelected[1] = nullptr;
			}
			CCore::GetInstance().GetEventManager().Send<DeletionEvent>(DeletionEvent{ pCurrent });
		}*/
	}
}

void CPlayer::SetHp(int _hp)
{
	m_Hp = _hp;
	if (m_Hp > 100)
	{
		m_Hp = 100;
	}
	else if (m_Hp < 0)
	{
		m_Hp = 0;
	}
}

void CPlayer::DoAxe(ItemID _id)
{
	CTile*     pCurrentTile = CCore::GetInstance().GetStageManager().GetCurrent().GetCurrentTile(this);
	const Vec2 dir          = GetComponent<CMovement>()->GetDir();
	int        damage       = 1;

	if (_id.kind != TOOL::NORMAL_AXE)
	{
		damage = 6;
	}

	const CTile* pTile       = pCurrentTile->GetNextTile(dir);
	CGameObject* pGameObject = pTile->GetLyingObject();
	if (pGameObject && pGameObject->GetType() == OBJECT_TYPE::OBSTACLE)
	{
		CObstacle* pObstacle = static_cast<CObstacle*>(pGameObject);
		if (pObstacle->GetObstacleType() == OBSTACLE_TYPE::WOOD)
		{
			int hp = pObstacle->GetHP();
			hp -= damage;
			pObstacle->SetHP(hp);

			SetWood(m_woodCount + 6);

			CSound* pSound = CCore::GetInstance().GetResourceManager().FindSound(L"axe");
			pSound->SetPosition(20.f);
			pSound->Play(false);
		}
		else
		{
			CSound* pSound = CCore::GetInstance().GetResourceManager().FindSound(L"wrong");
			pSound->SetPosition(20.f);
			pSound->Play(false);
		}
	}
	else
	{
		CSound* pSound = CCore::GetInstance().GetResourceManager().FindSound(L"wrong");
		pSound->SetPosition(20.f);
		pSound->Play(false);
	}
}

inline void CPlayer::SetGrass(int _count)
{
	m_grassCount = _count;
	if (m_grassCount > 999)
	{
		m_grassCount = 999;
	}
	if (m_grassCount < 0)
	{
		m_grassCount = 0;
	}
}

inline void CPlayer::SetWood(int _count)
{
	m_woodCount = _count;
	if (m_woodCount > 999)
	{
		m_woodCount = 999;
	}
}

bool CPlayer::HasItem(ItemID _id)
{
	for (int i = 0; i < m_vecInventory.size(); ++i)
	{
		if (_id == m_vecInventory[i]->GetInfo().id)
		{
			return true;
		}
	}
	return false;
}

void CPlayer::InsertIntoInventory(CItem* _pItem)
{
	m_vecInventory.push_back(_pItem);
}

void CPlayer::ChangeTool(CItem* _pItem)
{
	ItemID changeToolId = mapChangeTool[_pItem->GetInfo().id];
	for (int i = 0; i < m_arrSelected.size(); ++i)
	{
		if (m_arrSelected[i] && m_arrSelected[i]->GetInfo().id == changeToolId)
		{
			CStage& stage = CCore::GetInstance().GetStageManager().GetCurrent();
			stage.MoveObjectsTo(STAGE_TYPE::GARAGE, OBJECT_TYPE::ITEM, m_arrSelected[i]);
			m_arrSelected[i] = nullptr;
		}
	}
	for (int i = 0; i < m_vecInventory.size(); ++i)
	{
		if (m_vecInventory[i]->GetInfo().id == changeToolId)
		{
			SpriteInfo* pSpriteInfo = _pItem->GetComponent<CSpriteComponent>()->GetInfo();
			m_vecInventory[i]->GetComponent<CSpriteComponent>()->SetInfo(pSpriteInfo);
			m_vecInventory[i]->GetComponent<CSpriteComponent>()->SetTexture(
				pSpriteInfo->textureName, LR"(texture/tools.bmp)");
			ItemInfo itemInfo = _pItem->GetInfo();
			itemInfo.location = m_vecInventory[i]->GetInfo().location;
			m_vecInventory[i]->SetInfo(itemInfo);
			m_vecInventory[i]->GetComponent<CTransform>()->SetPos(itemInfo.location);
			_pItem->GetComponent<CSpriteComponent>()->SetEnabled(false);
			_pItem->GetComponent<CCollider>()->Enable(false);
			CCore::GetInstance().GetEventManager().Send<DeletionEvent>(DeletionEvent{_pItem});
			break;
		}
	}
}

CItem* CPlayer::GetItemFromInventory(ItemID _id)
{
	for (int i = 0; i < m_vecInventory.size(); ++i)
	{
		if (m_vecInventory[i]->GetInfo().id == _id)
		{
			return m_vecInventory[i];
		}
	}
	return nullptr;
}

void CPlayer::DeleteFromInventory(CItem* _pItem)
{
	m_vecInventory.erase(std::remove_if(m_vecInventory.begin(), m_vecInventory.end(), [_pItem](CItem* pItem)
	{
		if (pItem->GetInfo().id == _pItem->GetInfo().id)
		{
			return true;
		}
		return false;
	}));
}

void CPlayer::UpdateFarm()
{
	GetComponent<CTransform>()->SetPos({340, 300});
	GetComponent<CMovement>()->SetDir({0, 1});
	m_Hp = m_maxHp;


#pragma region UPDATE_FARM

	CStage&                    farm    = CCore::GetInstance().GetStageManager().GetStage(STAGE_TYPE::FARM);
	std::vector<CGameObject*>& vecPlot = farm.GetObjects(OBJECT_TYPE::PLOT);
	for (int i = 0; i < vecPlot.size(); ++i)
	{
		CPlot* pPlot = static_cast<CPlot*>(vecPlot[i]);
		pPlot->FinishDay();
	}
	std::vector<CGameObject*>& vecObstacles = farm.GetObjects(OBJECT_TYPE::OBSTACLE);

	int                        crashedCount = 0;

	for (int i = 0; i < vecObstacles.size(); ++i)
	{
		CObstacle* pObstacle = static_cast<CObstacle*>(vecObstacles[i]);

		const int random = rand() % 5;
		const int hp     = random == 1 ? 1 : 0;

		if (pObstacle->GetHP() == 1)
		{
			++crashedCount;
		}
		pObstacle->SetHP(pObstacle->GetHP() - hp);
		if (pObstacle->GetHP() == 0)
		{
			pObstacle->SetHP(1);
		}
		if (pObstacle->GetHP() == 1)
		{
			CTile* pTile                       = farm.GetCurrentTile(pObstacle);
			pTile->GetTileInfo().canJump       = false;
			CSpriteComponent* pSpriteComponent = pObstacle->GetComponent<CSpriteComponent>();
			SpriteInfo*       pInfo            = pInfo = CCore::GetInstance().GetResourceManager().GetSpriteInfo(
				EXTRA::TYPES[EXTRA::WOOD_CRASHED]);
			pSpriteComponent->SetTexture(EXTRA::TYPES[EXTRA::WOOD_CRASHED], LR"(texture\extra.bmp)");
			pSpriteComponent->SetInfo(pInfo);
		}
	}
#pragma endregion

#pragma region UPDATE_COWBARN
	CStage&                    cow    = CCore::GetInstance().GetStageManager().GetStage(STAGE_TYPE::COW);
	std::vector<CGameObject*>& vecCow = cow.GetObjects(OBJECT_TYPE::ANIMAL);

	bool isEnough = m_cowFeedCount > 0 ? true : false;

	for (int i = 0; i < vecCow.size(); ++i)
	{
		CAnimal* pAnimal = static_cast<CAnimal*>(vecCow[i]);
		if (false == isEnough
			&& (pAnimal->GetInfo().currentLevel == GROW_LEVEL::TEEN ||
				pAnimal->GetInfo().currentLevel == GROW_LEVEL::ADULT))
		{
			AnimalInfo info = pAnimal->GetInfo();
			--info.hp;
			pAnimal->SetInfo(info);
		}
		pAnimal->FinishDay();
	}
	vecCow.erase(std::remove_if(vecCow.begin(), vecCow.end(), [](CGameObject* pGameObject)
	{
		const CAnimal* pAnimal = static_cast<CAnimal*>(pGameObject);
		if (pAnimal->GetInfo().hp == 0)
		{
			delete pAnimal;
			return true;
		}
		return false;
	}), vecCow.end());

	const std::pair<CItem*, CItem*> holdings = GetItems();
	CItem*                          pItem    = GetItemFromInventory(ItemID{{TOOL::COW_FOOD, ITEM_CATEGORY::TOOL}});
	if (pItem && holdings.first != pItem && holdings.second != pItem)
	{
		cow.Clear(OBJECT_TYPE::ITEM, pItem);
	}
	else
	{
		cow.Clear(OBJECT_TYPE::ITEM);
	}
	cow.Clear(OBJECT_TYPE::TILE);
	cow.LoadStage(STAGE_TYPE::COW, LR"(map\CowBarn.xml)");

#pragma endregion

#pragma region UPDATE_CHICKEN_COOP

	CStage&                          chicken    = CCore::GetInstance().GetStageManager().GetStage(STAGE_TYPE::CHICKEN);
	const std::vector<CGameObject*>& vecChicken = chicken.GetObjects(OBJECT_TYPE::ANIMAL);

	pItem = GetItemFromInventory(ItemID{{TOOL::CHICKEN_FOOD, ITEM_CATEGORY::TOOL}});
	if (pItem && holdings.first != pItem && holdings.second != pItem)
	{
		chicken.Clear(OBJECT_TYPE::ITEM, pItem);
	}
	else
	{
		chicken.Clear(OBJECT_TYPE::ITEM);
	}
	chicken.Clear(OBJECT_TYPE::TILE);
	chicken.LoadStage(STAGE_TYPE::CHICKEN, LR"(map\ChickenCoop.xml)");

	int                        chickenCount = 0;
	for (int i = 0; i < vecChicken.size(); ++i)
	{
		CAnimal* pAnimal = static_cast<CAnimal*>(vecChicken[i]);
		if (pAnimal->IsFullyGrown())
		{
			++chickenCount;
		}
		pAnimal->FinishDay();
	}
	isEnough = m_chickenFeedCount >= chickenCount ? true : false;
	if (isEnough)
	{
		const int count = rand() % (vecChicken.size() + 1);
		for (int i = 0; i < count; ++i)
		{
			pItem = new CItem{};
			pItem->GetComponent<CTransform>()->SetPos({100 + i * 40, 220});
			ItemID id = ItemID{EXTRA::EGG, ITEM_CATEGORY::SELL};

			pItem->SetInfo(BASIC_INFO::MAP_DATA[id]);
			SpriteInfo* info = CCore::GetInstance().GetResourceManager().GetSpriteInfo(
				EXTRA::TYPES[id.kind]);
			pItem->GetComponent<CSpriteComponent>()->SetInfo(info);
			pItem->GetComponent<CSpriteComponent>()->SetTexture(
				EXTRA::TYPES[id.kind], LR"(texture\extra.bmp)");
			pItem->GetComponent<CTransform>()->SetScale(info->scale);
			pItem->GetComponent<CCollider>()->SetOffsetPos(info->scale * .5f);
			pItem->GetComponent<CCollider>()->SetScale(info->scale);

			chicken.AddGameObject(pItem, OBJECT_TYPE::ITEM);
		}
	}

	if (chickenCount > 0 && crashedCount > 0)
	{
		for (int i = 0; i < vecChicken.size(); ++i)
		{
			CAnimal* pAnimal = static_cast<CAnimal*>(vecChicken[i]);
			if (pAnimal->GetInfo().currentLevel == pAnimal->GetInfo().maxGrowLevel)
			{
				chicken.RemoveObject(pAnimal, OBJECT_TYPE::ANIMAL);
				break;
			}
		}
	}
#pragma endregion

	m_isUpdatingFarm = true;
}

void CPlayer::SetAnimations()
{
	CAnimator* pAnimator = GetComponent<CAnimator>();

	pAnimator->SetAnimGroup(PLAYER::GROUPS::ANIMATIONS[PLAYER::GROUPS::IDLE_0],
	                        {
		                        PLAYER::ANIMATIONS[PLAYER::IDLE_LEFT_0],
		                        PLAYER::ANIMATIONS[PLAYER::IDLE_RIGHT_0],
		                        PLAYER::ANIMATIONS[PLAYER::IDLE_BACK_0],
		                        PLAYER::ANIMATIONS[PLAYER::IDLE_FRONT_0]
	                        });
	pAnimator->SetAnimGroup(PLAYER::GROUPS::ANIMATIONS[PLAYER::GROUPS::IDLE_1],
	                        {
		                        PLAYER::ANIMATIONS[PLAYER::IDLE_LEFT_1],
		                        PLAYER::ANIMATIONS[PLAYER::IDLE_RIGHT_1],
		                        PLAYER::ANIMATIONS[PLAYER::IDLE_BACK_1],
		                        PLAYER::ANIMATIONS[PLAYER::IDLE_FRONT_1]
	                        });
	pAnimator->SetAnimGroup(PLAYER::GROUPS::ANIMATIONS[PLAYER::GROUPS::IDLE_2],
	                        {
		                        PLAYER::ANIMATIONS[PLAYER::IDLE_LEFT_2],
		                        PLAYER::ANIMATIONS[PLAYER::IDLE_RIGHT_2],
		                        PLAYER::ANIMATIONS[PLAYER::IDLE_BACK_2],
		                        PLAYER::ANIMATIONS[PLAYER::IDLE_FRONT_2]
	                        });
	pAnimator->SetAnimGroup(PLAYER::GROUPS::ANIMATIONS[PLAYER::GROUPS::WALK_0],
	                        {
		                        PLAYER::ANIMATIONS[PLAYER::WALK_LEFT_0],
		                        PLAYER::ANIMATIONS[PLAYER::WALK_RIGHT_0],
		                        PLAYER::ANIMATIONS[PLAYER::WALK_BACK_0],
		                        PLAYER::ANIMATIONS[PLAYER::WALK_FRONT_0]
	                        });
	pAnimator->SetAnimGroup(PLAYER::GROUPS::ANIMATIONS[PLAYER::GROUPS::RUN_0],
	                        {
		                        PLAYER::ANIMATIONS[PLAYER::RUN_LEFT_0],
		                        PLAYER::ANIMATIONS[PLAYER::RUN_RIGHT_0],
		                        PLAYER::ANIMATIONS[PLAYER::RUN_BACK_0],
		                        PLAYER::ANIMATIONS[PLAYER::RUN_FRONT_0]
	                        });
	pAnimator->SetAnimGroup(PLAYER::GROUPS::ANIMATIONS[PLAYER::GROUPS::JUMP_0],
	                        {
		                        PLAYER::ANIMATIONS[PLAYER::JUMP_LEFT_0],
		                        PLAYER::ANIMATIONS[PLAYER::JUMP_RIGHT_0],
		                        PLAYER::ANIMATIONS[PLAYER::JUMP_BACK_0],
		                        PLAYER::ANIMATIONS[PLAYER::JUMP_FRONT_0]
	                        });
	pAnimator->SetAnimGroup(PLAYER::GROUPS::ANIMATIONS[PLAYER::GROUPS::SELECT_0],
	                        {
		                        PLAYER::ANIMATIONS[PLAYER::SELECT_LEFT_0],
		                        PLAYER::ANIMATIONS[PLAYER::SELECT_RIGHT_0],
		                        PLAYER::ANIMATIONS[PLAYER::SELECT_BACK_0],
		                        PLAYER::ANIMATIONS[PLAYER::SELECT_FRONT_0]
	                        });
	pAnimator->SetAnimGroup(PLAYER::GROUPS::ANIMATIONS[PLAYER::GROUPS::CARRYING_WALK_0],
	                        {
		                        PLAYER::ANIMATIONS[PLAYER::CARRYING_WALK_LEFT_0],
		                        PLAYER::ANIMATIONS[PLAYER::CARRYING_WALK_RIGHT_0],
		                        PLAYER::ANIMATIONS[PLAYER::CARRYING_WALK_BACK_0],
		                        PLAYER::ANIMATIONS[PLAYER::CARRYING_WALK_FRONT_0]
	                        });
	pAnimator->SetAnimGroup(PLAYER::GROUPS::ANIMATIONS[PLAYER::GROUPS::CARRYING_RUN_0],
	                        {
		                        PLAYER::ANIMATIONS[PLAYER::CARRYING_RUN_LEFT_0],
		                        PLAYER::ANIMATIONS[PLAYER::CARRYING_RUN_RIGHT_0],
		                        PLAYER::ANIMATIONS[PLAYER::CARRYING_RUN_BACK_0],
		                        PLAYER::ANIMATIONS[PLAYER::CARRYING_RUN_FRONT_0]
	                        });
	pAnimator->SetAnimGroup(PLAYER::GROUPS::ANIMATIONS[PLAYER::GROUPS::CARRYING_JUMP_0],
	                        {
		                        PLAYER::ANIMATIONS[PLAYER::CARRYING_JUMP_LEFT_0],
		                        PLAYER::ANIMATIONS[PLAYER::CARRYING_JUMP_RIGHT_0],
		                        PLAYER::ANIMATIONS[PLAYER::CARRYING_JUMP_BACK_0],
		                        PLAYER::ANIMATIONS[PLAYER::CARRYING_JUMP_FRONT_0]
	                        });
	pAnimator->SetAnimGroup(PLAYER::GROUPS::ANIMATIONS[PLAYER::GROUPS::FEED_0],
	                        {
		                        PLAYER::ANIMATIONS[PLAYER::FEED_LEFT_0],
		                        PLAYER::ANIMATIONS[PLAYER::FEED_RIGHT_0],
		                        PLAYER::ANIMATIONS[PLAYER::FEED_BACK_0],
		                        PLAYER::ANIMATIONS[PLAYER::FEED_FRONT_0]
	                        });
	pAnimator->SetAnimGroup(PLAYER::GROUPS::ANIMATIONS[PLAYER::GROUPS::BELL_0],
	                        {
		                        PLAYER::ANIMATIONS[PLAYER::BELL_LEFT_0],
		                        PLAYER::ANIMATIONS[PLAYER::BELL_RIGHT_0],
		                        PLAYER::ANIMATIONS[PLAYER::BELL_FRONT_0],
		                        PLAYER::ANIMATIONS[PLAYER::BELL_BACK_0]
	                        });
	pAnimator->SetAnimGroup(PLAYER::GROUPS::ANIMATIONS[PLAYER::GROUPS::NORMAL_SICKLE_0],
	                        {
		                        PLAYER::ANIMATIONS[PLAYER::NORMAL_SICKLE_LEFT_0],
		                        PLAYER::ANIMATIONS[PLAYER::NORMAL_SICKLE_RIGHT_0],
		                        PLAYER::ANIMATIONS[PLAYER::NORMAL_SICKLE_BACK_0],
		                        PLAYER::ANIMATIONS[PLAYER::NORMAL_SICKLE_FRONT_0]
	                        });
	pAnimator->SetAnimGroup(PLAYER::GROUPS::ANIMATIONS[PLAYER::GROUPS::NORMAL_HAMMER_0],
	                        {
		                        PLAYER::ANIMATIONS[PLAYER::NORMAL_HAMMER_LEFT_0],
		                        PLAYER::ANIMATIONS[PLAYER::NORMAL_HAMMER_RIGHT_0],
		                        PLAYER::ANIMATIONS[PLAYER::NORMAL_HAMMER_BACK_0],
		                        PLAYER::ANIMATIONS[PLAYER::NORMAL_HAMMER_FRONT_0]
	                        });
	pAnimator->SetAnimGroup(PLAYER::GROUPS::ANIMATIONS[PLAYER::GROUPS::SPECIAL_HAMMER_0],
	                        {
		                        PLAYER::ANIMATIONS[PLAYER::SPECIAL_HAMMER_LEFT_0],
		                        PLAYER::ANIMATIONS[PLAYER::SPECIAL_HAMMER_RIGHT_0],
		                        PLAYER::ANIMATIONS[PLAYER::SPECIAL_HAMMER_BACK_0],
		                        PLAYER::ANIMATIONS[PLAYER::SPECIAL_HAMMER_FRONT_0]
	                        });
	pAnimator->SetAnimGroup(PLAYER::GROUPS::ANIMATIONS[PLAYER::GROUPS::NORMAL_WATER_0],
	                        {
		                        PLAYER::ANIMATIONS[PLAYER::NORMAL_WATER_LEFT_0],
		                        PLAYER::ANIMATIONS[PLAYER::NORMAL_WATER_RIGHT_0],
		                        PLAYER::ANIMATIONS[PLAYER::NORMAL_WATER_BACK_0],
		                        PLAYER::ANIMATIONS[PLAYER::NORMAL_WATER_FRONT_0]
	                        });
	pAnimator->SetAnimGroup(PLAYER::GROUPS::ANIMATIONS[PLAYER::GROUPS::NORMAL_AXE_0],
	                        {
		                        PLAYER::ANIMATIONS[PLAYER::NORMAL_AXE_LEFT_0],
		                        PLAYER::ANIMATIONS[PLAYER::NORMAL_AXE_RIGHT_0],
		                        PLAYER::ANIMATIONS[PLAYER::NORMAL_AXE_BACK_0],
		                        PLAYER::ANIMATIONS[PLAYER::NORMAL_AXE_FRONT_0]
	                        });
	pAnimator->SetAnimGroup(PLAYER::GROUPS::ANIMATIONS[PLAYER::GROUPS::SPECIAL_AXE_0],
	                        {
		                        PLAYER::ANIMATIONS[PLAYER::SPECIAL_AXE_LEFT_0],
		                        PLAYER::ANIMATIONS[PLAYER::SPECIAL_AXE_RIGHT_0],
		                        PLAYER::ANIMATIONS[PLAYER::SPECIAL_AXE_BACK_0],
		                        PLAYER::ANIMATIONS[PLAYER::SPECIAL_AXE_FRONT_0]
	                        });
	pAnimator->SetAnimGroup(PLAYER::GROUPS::ANIMATIONS[PLAYER::GROUPS::NORMAL_HOE_0],
	                        {
		                        PLAYER::ANIMATIONS[PLAYER::NORMAL_HOE_LEFT_0],
		                        PLAYER::ANIMATIONS[PLAYER::NORMAL_HOE_RIGHT_0],
		                        PLAYER::ANIMATIONS[PLAYER::NORMAL_HOE_BACK_0],
		                        PLAYER::ANIMATIONS[PLAYER::NORMAL_HOE_FRONT_0]
	                        });
	pAnimator->SetAnimGroup(PLAYER::GROUPS::ANIMATIONS[PLAYER::GROUPS::SPECIAL_HOE_0],
	                        {
		                        PLAYER::ANIMATIONS[PLAYER::SPECIAL_HOE_LEFT_0],
		                        PLAYER::ANIMATIONS[PLAYER::SPECIAL_HOE_RIGHT_0],
		                        PLAYER::ANIMATIONS[PLAYER::SPECIAL_HOE_BACK_0],
		                        PLAYER::ANIMATIONS[PLAYER::SPECIAL_HOE_FRONT_0]
	                        });
	pAnimator->SetAnimGroup(PLAYER::GROUPS::ANIMATIONS[PLAYER::GROUPS::BRUSH_0],
	                        {
		                        PLAYER::ANIMATIONS[PLAYER::BRUSH_LEFT_0],
		                        PLAYER::ANIMATIONS[PLAYER::BRUSH_RIGHT_0],
		                        PLAYER::ANIMATIONS[PLAYER::BRUSH_BACK_0],
		                        PLAYER::ANIMATIONS[PLAYER::BRUSH_FRONT_0]
	                        });
	pAnimator->SetAnimGroup(PLAYER::GROUPS::ANIMATIONS[PLAYER::GROUPS::MILKING_0],
	                        {
		                        PLAYER::ANIMATIONS[PLAYER::MILKING_LEFT_0],
		                        PLAYER::ANIMATIONS[PLAYER::MILKING_RIGHT_0],
		                        PLAYER::ANIMATIONS[PLAYER::MILKING_BACK_0],
		                        PLAYER::ANIMATIONS[PLAYER::MILKING_FRONT_0]
	                        });

	pAnimator->SetAnimGroup(PLAYER::GROUPS::ANIMATIONS[PLAYER::GROUPS::WHISTLE_0],
	                        {
		                        PLAYER::ANIMATIONS[PLAYER::WHISTLE_LEFT_0],
		                        PLAYER::ANIMATIONS[PLAYER::WHISTLE_RIGHT_0],
		                        PLAYER::ANIMATIONS[PLAYER::WHISTLE_FRONT_0],
		                        PLAYER::ANIMATIONS[PLAYER::WHISTLE_FRONT_0]
	                        });

	pAnimator->SetAnimGroup(PLAYER::GROUPS::ANIMATIONS[PLAYER::GROUPS::CARRY_0],
	                        {
		                        PLAYER::ANIMATIONS[PLAYER::CARRY_LEFT_0],
		                        PLAYER::ANIMATIONS[PLAYER::CARRY_RIGHT_0],
		                        PLAYER::ANIMATIONS[PLAYER::CARRY_BACK_0],
		                        PLAYER::ANIMATIONS[PLAYER::CARRY_FRONT_0],
	                        });
	pAnimator->SetAnimGroup(PLAYER::GROUPS::ANIMATIONS[PLAYER::GROUPS::CARRY_IDLE_0],
	                        {
		                        PLAYER::ANIMATIONS[PLAYER::CARRY_IDLE_LEFT_0],
		                        PLAYER::ANIMATIONS[PLAYER::CARRY_IDLE_RIGHT_0],
		                        PLAYER::ANIMATIONS[PLAYER::CARRY_IDLE_BACK_0],
		                        PLAYER::ANIMATIONS[PLAYER::CARRY_IDLE_FRONT_0],
	                        });

	pAnimator->SetAnimGroup(PLAYER::GROUPS::ANIMATIONS[PLAYER::GROUPS::CARRYING_THROW_0],
	                        {
		                        PLAYER::ANIMATIONS[PLAYER::CARRYING_THROW_LEFT_0],
		                        PLAYER::ANIMATIONS[PLAYER::CARRYING_THROW_RIGHT_0],
		                        PLAYER::ANIMATIONS[PLAYER::CARRYING_THROW_BACK_0],
		                        PLAYER::ANIMATIONS[PLAYER::CARRYING_THROW_FRONT_0],
	                        });
}

void CPlayer::UseItem()
{
	CItem* pItem = GetCurrentItem();
	assert(pItem);

	const ItemInfo& info = pItem->GetInfo();
	if (true == info.consumable)
	{
		--pItem->GetInfo().currentCount;
	}

	if (pItem->GetInfo().currentCount <= 0)
	{
		if (m_arrSelected[0] == pItem)
		{
			m_arrSelected[0] = nullptr;
		}
		else
		{
			m_arrSelected[1] = nullptr;
		}
		DeleteFromInventory(pItem);

		CCore::GetInstance().GetEventManager().Send<DeletionEvent>(DeletionEvent{pItem});
	}
}
