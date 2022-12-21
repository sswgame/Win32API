#include "pch.h"
#include "CStage_Mountain.h"

#include "CCamera.h"
#include "CCollider.h"
#include "CFSM.h"
#include "CObstacle.h"
#include "CPlayer.h"
#include "CSpriteComponent.h"
#include "CTeleport.h"
#include "CTile.h"

#include "CTileManager.h"
#include "Name.h"

namespace
{
	std::wstring FARM     = L"164";
	std::wstring MOUNTAIN = L"944";
}

CStage_Mountain::CStage_Mountain()  = default;
CStage_Mountain::~CStage_Mountain() = default;

void CStage_Mountain::Init()
{
	/*LoadStage(STAGE_TYPE::MOUNTAIN, LR"(map\MountainSpring.xml)");*/
}

void CStage_Mountain::Enter()
{
	CCore::GetInstance().GetCamera().FadeIn(2.f);
	if (GetObjects(OBJECT_TYPE::TILE).empty())
	{
		LoadStage(STAGE_TYPE::MOUNTAIN, LR"(map\MountainSpring.xml)");
		const MapInfo& mapInfo = CCore::GetInstance().GetTileManager().GetMapInfos(STAGE_TYPE::MOUNTAIN);
		for (auto& data : mapInfo.vecObjects)
		{
			if (data.second.type == OBJECT_TYPE::TELEPORTER)
			{
				CTeleport* pTeleport = new CTeleport{};
				pTeleport->SetName(data.first);
				pTeleport->GetComponent<CTransform>()->SetPos(data.second.location);
				pTeleport->SetTarget(Vec2{data.second.extra.x, data.second.extra.y});
				CCollider* pCollider = pTeleport->GetComponent<CCollider>();
				pCollider->SetScale(data.second.size);
				pCollider->SetOffsetPos(Vec2{data.second.size.x * .5f, data.second.size.y * .55f});
				AddGameObject(pTeleport, OBJECT_TYPE::TELEPORTER);
			}
			else if (data.second.type == OBJECT_TYPE::OBSTACLE)
			{
				CObstacle* pObstacle = new CObstacle{};
				pObstacle->SetName(data.first);
				pObstacle->GetComponent<CTransform>()->SetPos(data.second.location - data.second.size * .5f);
				CSpriteComponent* pSpriteComponent = pObstacle->GetComponent<CSpriteComponent>();
				SpriteInfo*       pSpriteInfo{};
				if (data.first.find(L"GRASS") != std::wstring::npos)
				{
					pObstacle->SetObstacleType(OBSTACLE_TYPE::GRASS);
					pObstacle->SetHP(1);
					pSpriteInfo = CCore::GetInstance().GetResourceManager().GetSpriteInfo(
						EXTRA::TYPES[EXTRA::OBSTACLE_GRASS]);
					pSpriteComponent->SetTexture(EXTRA::TYPES[EXTRA::OBSTACLE_GRASS], LR"(texture\extra.bmp)");
				}
				else if (data.first.find(L"BIG") != std::wstring::npos)
				{
					pObstacle->SetObstacleType(OBSTACLE_TYPE::BIG_ROCK);
					pObstacle->SetHP(6);
					pSpriteInfo = CCore::GetInstance().GetResourceManager().GetSpriteInfo(
						EXTRA::TYPES[EXTRA::OBSTACLE_BIG_ROCK]);
					pSpriteComponent->SetTexture(EXTRA::TYPES[EXTRA::OBSTACLE_BIG_ROCK], LR"(texture\extra.bmp)");
				}
				else if (data.first.find(L"SMALL") != std::wstring::npos)
				{
					pObstacle->SetObstacleType(OBSTACLE_TYPE::SMALL_ROCK);
					pObstacle->SetHP(1);
					pSpriteInfo = CCore::GetInstance().GetResourceManager().GetSpriteInfo(
						EXTRA::TYPES[EXTRA::OBSTACLE_SMALL_ROCK]);
					pSpriteComponent->SetTexture(EXTRA::TYPES[EXTRA::OBSTACLE_SMALL_ROCK], LR"(texture\extra.bmp)");
				}
				else if (data.first.find(L"TREE") != std::wstring::npos)
				{
					pObstacle->SetObstacleType(OBSTACLE_TYPE::WOOD);
					pObstacle->SetHP(6);
					pSpriteInfo = CCore::GetInstance().GetResourceManager().GetSpriteInfo(
						EXTRA::TYPES[EXTRA::OBSTACLE_WOOD]);
					pSpriteComponent->SetTexture(EXTRA::TYPES[EXTRA::OBSTACLE_WOOD], LR"(texture\extra.bmp)");
				}
				pSpriteComponent->SetInfo(pSpriteInfo);

				CTile* pTile = GetCurrentTile(pObstacle);
				pSpriteInfo  = CCore::GetInstance().GetResourceManager().GetSpriteInfo(MOUNTAIN);

				if (pObstacle->GetObstacleType() == OBSTACLE_TYPE::BIG_ROCK || pObstacle->GetObstacleType() ==
					OBSTACLE_TYPE::WOOD)
				{
					CTile* pAdjacentTiles[3] = {pTile->GetRight(), pTile->GetDown(), pTile->GetRight()->GetDown()};
					for (int i = 0; i < _countof(pAdjacentTiles); ++i)
					{
						pAdjacentTiles[i]->GetComponent<CSpriteComponent>()->SetInfo(pSpriteInfo);
						pAdjacentTiles[i]->GetComponent<CSpriteComponent>()->SetTexture(
							MOUNTAIN, LR"(texture\newTileSet.bmp)");
						pAdjacentTiles[i]->SetLyingObject(pObstacle);
						pAdjacentTiles[i]->GetTileInfo().canMove = false;
					}
				}
				pTile->SetLyingObject(pObstacle);
				pTile->GetTileInfo().canMove = false;
				pTile->GetComponent<CSpriteComponent>()->SetInfo(pSpriteInfo);
				pTile->GetComponent<CSpriteComponent>()->SetTexture(MOUNTAIN, LR"(texture\newTileSet.bmp)");
				AddGameObject(pObstacle, OBJECT_TYPE::OBSTACLE);
			}
			else if (data.second.type == OBJECT_TYPE::ITEM)
			{
				CItem* pItem = new CItem{};
				ItemID id    = {EXTRA::FRUIT, ITEM_CATEGORY::DEFAULT};

				CSpriteComponent* pSpriteComponent = pItem->GetComponent<CSpriteComponent>();
				SpriteInfo*       pSpriteInfo      = CCore::GetInstance().GetResourceManager().GetSpriteInfo(
					EXTRA::TYPES[EXTRA::FRUIT]);
				pSpriteComponent->SetInfo(pSpriteInfo);
				pSpriteComponent->SetTexture(EXTRA::TYPES[EXTRA::FRUIT], LR"(texture\extra.bmp)");

				pItem->SetInfo(BASIC_INFO::MAP_DATA[id]);
				pItem->SetName(data.first);
				pItem->GetComponent<CTransform>()->SetPos(data.second.location - Vec2{40.f, 40.f} * .5f);
				pItem->GetComponent<CCollider>()->SetScale(pSpriteInfo->scale);
				pItem->GetComponent<CCollider>()->SetOffsetPos(pSpriteInfo->scale * .5f);
				pItem->GetComponent<CCollider>()->Enable(true);

				CTile* pTile = GetCurrentTile(pItem);
				pSpriteInfo  = CCore::GetInstance().GetResourceManager().GetSpriteInfo(MOUNTAIN);
				pTile->GetComponent<CSpriteComponent>()->SetInfo(pSpriteInfo);
				pTile->GetComponent<CSpriteComponent>()->SetTexture(MOUNTAIN, LR"(texture\newTileSet.bmp)");
				AddGameObject(pItem, OBJECT_TYPE::ITEM);
			}
		}
	}

	GetObjects(OBJECT_TYPE::PLAYER)[0]->GetComponent<CTransform>()->SetPos(GetPlayerLocation());
}

void CStage_Mountain::Exit()
{
	CPlayer* pPlayer = static_cast<CPlayer*>(GetObjects(OBJECT_TYPE::PLAYER)[0]);
	MoveObjectsToNext(OBJECT_TYPE::PLAYER);
	MoveObjectsTo(STAGE_TYPE::STREET, OBJECT_TYPE::ITEM, pPlayer->GetItems().first);
	MoveObjectsTo(STAGE_TYPE::STREET, OBJECT_TYPE::ITEM, pPlayer->GetItems().second);
	if (pPlayer->GetCarryingObject())
	{
		MoveObjectsTo(STAGE_TYPE::STREET, pPlayer->GetCarryingObject()->GetType(), pPlayer->GetCarryingObject());
	}
	else
	{
		pPlayer->GetComponent<CFSM>()->ChangeState(L"IDLE");
	}
	MoveObjectsToNext(OBJECT_TYPE::RAYCAST);

	Clear();
}
