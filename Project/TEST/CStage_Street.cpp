#include "pch.h"
#include "CStage_Street.h"

#include "CCamera.h"
#include "CCollider.h"
#include "CFSM.h"
#include "CPlayer.h"
#include "CTeleport.h"
#include "CTileManager.h"
CStage_Street::CStage_Street()  = default;
CStage_Street::~CStage_Street() = default;

void CStage_Street::Init()
{
	LoadStage(STAGE_TYPE::STREET, LR"(map\StreetSpring.xml)");
	const MapInfo& mapInfo = CCore::GetInstance().GetTileManager().GetMapInfos(STAGE_TYPE::STREET);
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
}

void CStage_Street::Enter()
{
	CCore::GetInstance().GetCamera().FadeIn(2.f);
	GetObjects(OBJECT_TYPE::PLAYER)[0]->GetComponent<CTransform>()->SetPos(GetPlayerLocation());
	DrawObject(OBJECT_TYPE::ITEM, true);
	DrawObject(OBJECT_TYPE::TILE, true);
}

void CStage_Street::Exit()
{
	CPlayer* pPlayer = static_cast<CPlayer*>(GetObjects(OBJECT_TYPE::PLAYER)[0]);
	DrawObject(OBJECT_TYPE::ITEM, false);
	DrawObject(OBJECT_TYPE::TILE, false);

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
