#include "pch.h"
#include "CStage_Shop.h"

#include "CCamera.h"
#include "CCollider.h"
#include "CFSM.h"
#include "CNpc.h"
#include "CPlayer.h"
#include "CSpriteComponent.h"
#include "CTeleport.h"
#include "CTile.h"

#include "CTileManager.h"
#include "Name.h"

namespace
{
	std::map<std::wstring, ItemID> mapMedicine =
	{
		{L"MEDICINE", {TOOL::MEDICINE, ITEM_CATEGORY::TOOL}},
		{L"SPECIAL_MEDICINE", {TOOL::SPECIAL_MEDICINE, ITEM_CATEGORY::TOOL}}
	};
}

CStage_Shop::CStage_Shop()  = default;
CStage_Shop::~CStage_Shop() = default;

void CStage_Shop::Init()
{
	LoadStage(STAGE_TYPE::SHOP, LR"(map\AnimalShop.xml)");
	const MapInfo& mapInfo = CCore::GetInstance().GetTileManager().GetMapInfos(STAGE_TYPE::SHOP);
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

			if (data.first == L"SEED")
			{
				id = {TOOL::SEED, ITEM_CATEGORY::TOOL};
			}
			ItemInfo info = BASIC_INFO::MAP_DATA[id];
			info.location = Vec2{data.second.location.x, data.second.location.y};
			CItem* pItem  = new CItem{};
			pItem->SetName(data.first);
			pItem->SetInfo(info);
			pItem->GetComponent<CTransform>()->SetPos(Vec2{info.location} - data.second.size * .5f);

			CSpriteComponent* pSprite = pItem->GetComponent<CSpriteComponent>();
			if (data.first == L"SEED")
			{
				pSprite->SetTexture(PLOT::TYPES[PLOT::SECRETED_FLOWERED], LR"(texture\crops.bmp)");
			}
			else
			{
				pSprite->SetTexture(TOOL::TYPES[info.id.kind], LR"(texture\tools.bmp)");
			}

			CCollider* pCollider = pItem->GetComponent<CCollider>();
			pCollider->SetScale(data.second.size);
			pCollider->SetOffsetPos(Vec2{data.second.size.x * .5f, data.second.size.y * .5f});
			AddGameObject(pItem, OBJECT_TYPE::ITEM);
		}
	}
}

void CStage_Shop::Enter()
{
	CCore::GetInstance().GetCamera().FadeIn(2.f);
	if (GetObjects(OBJECT_TYPE::NPC).size() <= 0)
	{
		const MapInfo& mapInfo = CCore::GetInstance().GetTileManager().GetMapInfos(STAGE_TYPE::SHOP);
		for (auto& data : mapInfo.vecObjects)
		{
			if (data.second.type == OBJECT_TYPE::NPC)
			{
				CNpc* pNPC = new CNpc{};
				pNPC->SetName(data.first);
				pNPC->GetComponent<CTransform>()->SetPos(data.second.location);
				CCollider* pCollider = pNPC->GetComponent<CCollider>();
				pCollider->SetOffsetPos(Vec2{0.f, -40.f} * .5f);
				CTile* pTile = GetCurrentTile(pNPC);
				pTile->SetLyingObject(pNPC);
				pNPC->GetComponent<CTransform>()->SetPos(data.second.location + Vec2{20.f, 40.f});
				AddGameObject(pNPC, OBJECT_TYPE::NPC);
			}
		}
	}
	GetObjects(OBJECT_TYPE::PLAYER)[0]->GetComponent<CTransform>()->SetPos(GetPlayerLocation());
	DrawObject(OBJECT_TYPE::ITEM, true);
	DrawObject(OBJECT_TYPE::TILE, true);
	RemoveItem();
}

void CStage_Shop::Exit()
{
	CPlayer* pPlayer = static_cast<CPlayer*>(GetObjects(OBJECT_TYPE::PLAYER)[0]);
	DrawObject(OBJECT_TYPE::ITEM, false);
	DrawObject(OBJECT_TYPE::TILE, false);

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
}

void CStage_Shop::RemoveItem()
{
	CPlayer* pPlayer = static_cast<CPlayer*>(GetObjects(OBJECT_TYPE::PLAYER)[0]);

	CItem* pInventoryItem = pPlayer->GetItemFromInventory(mapMedicine[L"MEDICINE"]);
	if (nullptr != pInventoryItem && pInventoryItem->GetInfo().currentCount > 0)
	{
		std::vector<CGameObject*>& vecItems = GetObjects(OBJECT_TYPE::ITEM);
		for (int i = 0; i < vecItems.size(); ++i)
		{
			CItem* pItem = static_cast<CItem*>(vecItems[i]);
			if (pItem->GetInfo().id == pInventoryItem->GetInfo().id)
			{
				pItem->GetComponent<CCollider>()->Enable(false);
				pItem->GetComponent<CSpriteComponent>()->SetEnabled(false);
				break;
			}
		}
	}

	pInventoryItem = pPlayer->GetItemFromInventory(mapMedicine[L"SPECIAL_MEDICINE"]);
	if (nullptr != pInventoryItem && pInventoryItem->GetInfo().currentCount > 0)
	{
		std::vector<CGameObject*>& vecItems = GetObjects(OBJECT_TYPE::ITEM);
		for (int i = 0; i < vecItems.size(); ++i)
		{
			CItem* pItem = static_cast<CItem*>(vecItems[i]);
			if (pItem->GetInfo().id == pInventoryItem->GetInfo().id)
			{
				pItem->GetComponent<CCollider>()->Enable(false);
				pItem->GetComponent<CSpriteComponent>()->SetEnabled(false);
				break;
			}
		}
	}
}
