#include "pch.h"
#include "CStage_Farm.h"

#include "CCamera.h"
#include "CCollider.h"
#include "CCore.h"
#include "CFSM.h"
#include "CInvoker.h"
#include "CObstacle.h"
#include "CPlayer.h"
#include "CSpriteComponent.h"
#include "CTeleport.h"
#include "CTile.h"

#include "CTileManager.h"
#include "Name.h"

CStage_Farm::CStage_Farm()  = default;
CStage_Farm::~CStage_Farm() = default;

void CStage_Farm::Init()
{
	LoadStage(STAGE_TYPE::FARM, LR"(map\FarmSpring.xml)");
	const MapInfo& mapInfo = CCore::GetInstance().GetTileManager().GetMapInfos(STAGE_TYPE::FARM);
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
	}
	const std::vector<CGameObject*>& vecMapTiles = GetObjects(OBJECT_TYPE::TILE);
	for (auto& data : vecMapTiles)
	{
		CTile* pTile = static_cast<CTile*>(data);

		if (pTile->GetTileInfo().id == 208)
		{
			CObstacle* pObstacle = new CObstacle{};
			pObstacle->SetName(L"WALL");
			pObstacle->GetComponent<CTransform>()->SetPos(pTile->GetComponent<CTransform>()->GetPos());
			pObstacle->SetHP(2);
			pObstacle->SetObstacleType(OBSTACLE_TYPE::WALL);
			CSpriteComponent* pSpriteComponent = pObstacle->GetComponent<CSpriteComponent>();
			SpriteInfo*       pSpriteInfo      = CCore::GetInstance().GetResourceManager().
			                                                          GetSpriteInfo(EXTRA::TYPES[EXTRA::WOOD]);
			pSpriteComponent->SetTexture(EXTRA::TYPES[EXTRA::WOOD], LR"(texture\extra.bmp)");
			pSpriteComponent->SetInfo(pSpriteInfo);

			pSpriteInfo = CCore::GetInstance().GetResourceManager().GetSpriteInfo(L"251");
			pTile->SetLyingObject(pObstacle);
			pTile->GetTileInfo().canMove = false;
			pTile->GetComponent<CSpriteComponent>()->SetInfo(pSpriteInfo);
			pTile->GetComponent<CSpriteComponent>()->SetTexture(L"251", LR"(texture\newTileSet.bmp)");
			AddGameObject(pObstacle, OBJECT_TYPE::OBSTACLE);
		}
	}
	CInvoker* pInvoker = new CInvoker{};
	pInvoker->SetName(L"WOOD");
	pInvoker->GetComponent<CTransform>()->SetPos({540, 640});
	CCollider* pCollider = pInvoker->AddComponent<CCollider>();
	pCollider->SetOffsetPos({20, 10});
	pCollider->SetScale({40, 40});

	AddGameObject(pInvoker, OBJECT_TYPE::INVOKER);
}

void CStage_Farm::Enter()
{
	CCore::GetInstance().GetCamera().FadeIn(2.f);

	GetObjects(OBJECT_TYPE::PLAYER)[0]->GetComponent<CTransform>()->SetPos(GetPlayerLocation());
	DrawObject(OBJECT_TYPE::TILE, true);
	DrawObject(OBJECT_TYPE::PLOT, true);
	DrawObject(OBJECT_TYPE::ITEM, true);
}

void CStage_Farm::Exit()
{
	CPlayer* pPlayer = static_cast<CPlayer*>(GetObjects(OBJECT_TYPE::PLAYER)[0]);
	DrawObject(OBJECT_TYPE::TILE, false);
	DrawObject(OBJECT_TYPE::PLOT, false);

	MoveObjectsToNext(OBJECT_TYPE::PLAYER);
	MoveObjectsTo(STAGE_TYPE::END, OBJECT_TYPE::ITEM, pPlayer->GetItems().first);
	MoveObjectsTo(STAGE_TYPE::END, OBJECT_TYPE::ITEM, pPlayer->GetItems().second);
	if (pPlayer->GetCarryingObject())
	{
		MoveObjectsTo(STAGE_TYPE::END, pPlayer->GetCarryingObject()->GetType(), pPlayer->GetCarryingObject());
	}
	else
	{
		pPlayer->GetComponent<CFSM>()->ChangeState(L"IDLE");
	}

	MoveObjectsToNext(OBJECT_TYPE::RAYCAST);
}

