#include "pch.h"
#include "CStage_House.h"

#include "CCamera.h"
#include "CCollider.h"
#include "CCore.h"
#include "CFSM.h"
#include "CGameObject.h"
#include "CInvoker.h"
#include "CObstacle.h"
#include "CPlayer.h"
#include "CSpriteComponent.h"
#include "CTeleport.h"
#include "CTile.h"
#include "CTileManager.h"
#include "Name.h"

CStage_House::CStage_House() = default;

CStage_House::~CStage_House() = default;

void CStage_House::Init()
{
	LoadStage(STAGE_TYPE::HOUSE, LR"(map\JacksHouseSmall.xml)");
	const MapInfo& mapInfo  = CCore::GetInstance().GetTileManager().GetMapInfos(STAGE_TYPE::HOUSE);
	const Vec2     startPos = mapInfo.vecObjects.find(L"START_SPAWN")->second.location + GetMapInfo().tileWidth * .5f;
	SetPlayerLocation(startPos);

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
			pCollider->SetOffsetPos(Vec2{data.second.size.x * .5f, data.second.size.y * .5f});
			AddGameObject(pTeleport, OBJECT_TYPE::TELEPORTER);
		}
	}
	
	CInvoker* pInvoker = new CInvoker{};
	pInvoker->SetName(L"SLEEP");
	pInvoker->GetComponent<CTransform>()->SetPos({160, 160});
	CCollider* pCollider = pInvoker->AddComponent<CCollider>();
	pCollider->SetOffsetPos({20, 10});
	pCollider->SetScale({40, 40});

	AddGameObject(pInvoker, OBJECT_TYPE::INVOKER);
}

void CStage_House::Enter()
{
	CCore::GetInstance().GetCamera().FadeIn(2.f);

	GetObjects(OBJECT_TYPE::PLAYER)[0]->GetComponent<CTransform>()->SetPos(GetPlayerLocation());
	std::vector<CGameObject*>& vecTiles = GetObjects(OBJECT_TYPE::TILE);
	DrawObject(OBJECT_TYPE::TILE, true);
}

void CStage_House::Exit()
{
	CPlayer* pPlayer = static_cast<CPlayer*>(GetObjects(OBJECT_TYPE::PLAYER)[0]);

	DrawObject(OBJECT_TYPE::ITEM, false);
	DrawObject(OBJECT_TYPE::TILE, false);

	MoveObjectsToNext(OBJECT_TYPE::PLAYER);
	MoveObjectsTo(STAGE_TYPE::FARM, OBJECT_TYPE::ITEM, pPlayer->GetItems().first);
	MoveObjectsTo(STAGE_TYPE::FARM, OBJECT_TYPE::ITEM, pPlayer->GetItems().second);
	if (pPlayer->GetCarryingObject())
	{
		MoveObjectsTo(STAGE_TYPE::FARM, pPlayer->GetCarryingObject()->GetType(), pPlayer->GetCarryingObject());
	}
	else
	{
		pPlayer->GetComponent<CFSM>()->ChangeState(L"IDLE");
	}

	MoveObjectsToNext(OBJECT_TYPE::RAYCAST);
}
