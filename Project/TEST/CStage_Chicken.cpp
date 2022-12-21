#include "pch.h"
#include "CStage_Chicken.h"

#include "CCamera.h"
#include "CCollider.h"
#include "CFSM.h"
#include "CPlayer.h"
#include "CSpriteComponent.h"
#include "CTeleport.h"
#include "CTileManager.h"
#include "Name.h"
CStage_Chicken::CStage_Chicken()  = default;
CStage_Chicken::~CStage_Chicken() = default;

void CStage_Chicken::Init()
{
	LoadStage(STAGE_TYPE::CHICKEN, LR"(map\ChickenCoop.xml)");
	const MapInfo& mapInfo = CCore::GetInstance().GetTileManager().GetMapInfos(STAGE_TYPE::CHICKEN);
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
		else if (data.second.type == OBJECT_TYPE::ITEM)
		{
			ItemID id{};
			id.type = data.second.extra.id.type;
			id.kind = data.second.extra.id.kind;

			ItemInfo info                     = BASIC_INFO::MAP_DATA[id];
			info.location                     = Vec2{data.second.location.x, data.second.location.y};
			BASIC_INFO::MAP_DATA[id].location = info.location;
			CItem* pItem                      = new CItem{};
			pItem->SetName(data.first);
			pItem->SetInfo(info);
			pItem->GetComponent<CTransform>()->SetPos(Vec2{info.location});

			CSpriteComponent* pSprite = pItem->GetComponent<CSpriteComponent>();
			if (info.id.kind >= 16)
			{
				pSprite->SetTexture(TOOL::TYPES[info.id.kind], LR"(texture\tools.bmp)");
			}
			else
			{
				pSprite->SetTexture(TOOL::TYPES[info.id.kind], LR"(texture\extra.bmp)");
			}

			CCollider* pCollider = pItem->GetComponent<CCollider>();
			pCollider->SetScale(data.second.size);
			pCollider->SetOffsetPos(Vec2{data.second.size.x * .5f, data.second.size.y * .5f});
			AddGameObject(pItem, OBJECT_TYPE::ITEM);

			CPlayer* pPlayer = s_pPlayer;
			if (false == pPlayer->HasItem(pItem->GetInfo().id))
			{
				pPlayer->InsertIntoInventory(pItem);
			}
		}
	}
}

void CStage_Chicken::Enter()
{
	CCore::GetInstance().GetCamera().FadeIn(2.f);

	GetObjects(OBJECT_TYPE::PLAYER)[0]->GetComponent<CTransform>()->SetPos(GetPlayerLocation());
	DrawObject(OBJECT_TYPE::TILE, true);
	DrawObject(OBJECT_TYPE::ITEM, true);
}

void CStage_Chicken::Exit()
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
