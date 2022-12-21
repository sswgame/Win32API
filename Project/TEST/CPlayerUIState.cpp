#include "pch.h"
#include "CPlayerUIState.h"

#include "CAnimal.h"
#include "CAnimator.h"
#include "CCollider.h"
#include "CCore.h"
#include "CEventManager.h"
#include "CItem.h"
#include "CKeyManager.h"
#include "CPlayer.h"
#include "CSpriteComponent.h"
#include "CStage.h"
#include "CStageManager.h"
#include "CUI.h"
#include "CUIManager.h"
#include "Name.h"

namespace
{
	std::map<ANSWER_TYPE, ItemID> g_mapSeed =
	{
		{ANSWER_TYPE::GRASS, {TOOL::SEED_GRASS, ITEM_CATEGORY::TOOL}},
		{ANSWER_TYPE::POTATO, {TOOL::SEED_POTATO, ITEM_CATEGORY::TOOL}},
		{ANSWER_TYPE::CORN, {TOOL::SEED_CORN, ITEM_CATEGORY::TOOL}},
		{ANSWER_TYPE::TOMATO, {TOOL::SEED_TOMATO, ITEM_CATEGORY::TOOL}},
		{ANSWER_TYPE::TURNIP, {TOOL::SEED_TURNIP, ITEM_CATEGORY::TOOL}},

	};
	std::map<ItemID, std::wstring> g_mapPurchasedName =
	{
		{{TOOL::SEED_GRASS, ITEM_CATEGORY::TOOL}, L"GRASS"},
		{{TOOL::SEED_POTATO, ITEM_CATEGORY::TOOL}, L"POTATO"},
		{{TOOL::SEED_CORN, ITEM_CATEGORY::TOOL}, L"CORN"},
		{{TOOL::SEED_TOMATO, ITEM_CATEGORY::TOOL}, L"TOMATO"},
		{{TOOL::SEED_TURNIP, ITEM_CATEGORY::TOOL}, L"TURNIP"},
		{{TOOL::SPECIAL_MEDICINE, ITEM_CATEGORY::TOOL}, L"SPECIAL MEDICINE"},
		{{TOOL::MEDICINE, ITEM_CATEGORY::TOOL}, L"MEDICINE"},
	};
}

CPlayerUIState::CPlayerUIState()
	:
	CState{L"UI"},
	m_pUI{nullptr},
	m_pObject{nullptr},
	m_selected{ANSWER_TYPE::NONE},
	m_madeDecision{false} {}

CPlayerUIState::~CPlayerUIState() = default;

void CPlayerUIState::Update()
{
	if (KEY_TAP(KEY::LEFT))
	{
		if (m_pUI && m_pUI->IsSetShowSelection())
		{
			m_pUI->MoveTo(MOVE_TYPE::LEFT);
		}
	}
	if (KEY_TAP(KEY::RIGHT))
	{
		if (m_pUI && m_pUI->IsSetShowSelection())
		{
			m_pUI->MoveTo(MOVE_TYPE::RIGHT);
		}
	}
	if (KEY_TAP(KEY::SPACE))
	{
		if (false == CCore::GetInstance().GetUIManager().IsShowedAll())
		{
			return;
		}


		CPlayer*    pPlayer = static_cast<CPlayer*>(&GetOwner());
		ANSWER_TYPE answer{};
		if (m_pUI)
		{
			answer = m_pUI->GetMark();
			CCore::GetInstance().GetUIManager().SetUI(nullptr);
			CCore::GetInstance().GetEventManager().Send<DeletionEvent>(DeletionEvent{m_pUI});
			m_pUI = nullptr;
		}

		if (true == m_madeDecision)
		{
			ChangeState(L"IDLE");
			return;
		}

		if (m_pObject && m_pObject->GetType() == OBJECT_TYPE::ITEM)
		{
			CItem* pSelected = static_cast<CItem*>(m_pObject);
			if (ChooseToEat(pSelected, answer))
			{
				ChangeState(L"EAT");
			}
			else if (ChooseToBuyTool(pPlayer, pSelected, answer))
			{
				if (pPlayer->GetMoney() < pSelected->GetInfo().price)
				{
					ShowCannotBuy(pPlayer, pSelected);
					pPlayer->SetCarryingObject(nullptr);
					m_madeDecision = true;
				}
				else
				{
					pPlayer->SetMoney(pPlayer->GetMoney() - pSelected->GetInfo().price);

					if (pSelected->GetInfo().id == ItemID{TOOL::MEDICINE, ITEM_CATEGORY::TOOL}
						|| pSelected->GetInfo().id == ItemID{TOOL::SPECIAL_MEDICINE, ITEM_CATEGORY::TOOL})
					{
						BuyCowItems(pPlayer, pSelected);
						pPlayer->SetCarryingObject(nullptr);
						ChangeState(L"IDLE");
					}
					else
					{
						pPlayer->ChangeTool(pSelected);
						pPlayer->SetCarryingObject(nullptr);
						ChangeState(L"IDLE");
					}
				}
			}
			else if (ChooseToBuySeed(pPlayer, pSelected))
			{
				BuySeedItem(pPlayer, pSelected, answer);
			}
			else
			{
				if (pPlayer->GetCarryingObject() && pSelected->GetInfo().id.type != ITEM_CATEGORY::TOOL)
				{
					ChangeState(L"CARRY_IDLE");
				}
				else
				{
					pPlayer->SetCarryingObject(nullptr);
					ChangeState(L"IDLE");
				}
			}
		}
		else if (pPlayer->IsBuyingAnimal())
		{
			BuyingAnimal(pPlayer, answer);
		}
		else if (pPlayer->IsInfo())
		{
			pPlayer->SetInfoEnabled(false);
			if (pPlayer->GetCarryingObject())
			{
				ChangeState(L"CARRY_IDLE");
			}
			else
			{
				ChangeState(L"IDLE");
			}
		}
		else if (pPlayer->IsGoingToSleep())
		{
			if (answer == ANSWER_TYPE::OKAY)
			{
				pPlayer->UpdateFarm();
			}
			else
			{
				pPlayer->SetSleep(false);
			}
			ChangeState(L"IDLE");
		}
	}
}

void CPlayerUIState::Enter()
{
	CPlayer* pPlayer = static_cast<CPlayer*>(&GetOwner());

	if (pPlayer->GetCarryingObject() && pPlayer->GetCarryingObject()->GetType() == OBJECT_TYPE::ITEM)
	{
		CItem* pItem = static_cast<CItem*>(pPlayer->GetCarryingObject());
		if (pItem->GetInfo().canEat)
		{
			pPlayer->GetComponent<CAnimator>()->Play(PLAYER::ANIMATIONS[PLAYER::CARRY_IDLE_FRONT_0], false);
			m_pUI = new CUI{};
			m_pUI->SetText(L"Do you want to eat?");
			m_pUI->SetShowSelection(true);
			m_pObject = pItem;
			CCore::GetInstance().GetEventManager().Send<CreationEvent>(CreationEvent{m_pUI, OBJECT_TYPE::UI});
			CCore::GetInstance().GetUIManager().SetUI(m_pUI);
		}
		else if (false == pPlayer->HasItem(pItem->GetInfo().id) && pItem->GetName() != L"SEED")
		{
			m_pUI = new CUI{};
			m_pUI->SetText(L"Do you want to buy?");
			m_pUI->SetShowSelection(true);
			m_pObject = pItem;
			CCore::GetInstance().GetEventManager().Send<CreationEvent>(CreationEvent{m_pUI, OBJECT_TYPE::UI});
			CCore::GetInstance().GetUIManager().SetUI(m_pUI);
		}
		else if (false == pPlayer->HasItem(pItem->GetInfo().id) && pItem->GetName() == L"SEED")
		{
			CUI*         pUI  = new CUI{};
			std::wstring text = L"what seed do you want to buy?";
			pUI->SetSelectionType(SELECTION_TYPE::SEED);
			pUI->SetShowSelection(true);
			m_pObject = pItem;
			pUI->SetText(text);
			m_queueUI.push(pUI);
			CCore::GetInstance().GetEventManager().Send<CreationEvent>(CreationEvent{pUI, OBJECT_TYPE::UI});

			pUI  = new CUI{};
			text = L"Do you really want to buy it?";
			pUI->SetSelectionType(SELECTION_TYPE::YES_NO);
			pUI->SetShowSelection(true);
			pUI->SetText(text);
			m_queueUI.push(pUI);

			CCore::GetInstance().GetEventManager().Send<CreationEvent>(CreationEvent{pUI, OBJECT_TYPE::UI});
			m_pUI = m_queueUI.front();
			m_queueUI.pop();

			CCore::GetInstance().GetUIManager().SetUI(m_pUI);
		}
	}
	else if (pPlayer->IsInfo())
	{
		m_pUI                   = new CUI{};
		const std::wstring text = L"Current Money : " + std::to_wstring(pPlayer->GetMoney()) + L"\n" +
				L"Current Grass : " + std::to_wstring(pPlayer->GetGrassCount()) + L"\n" +
				L"Current Wood :" + std::to_wstring(pPlayer->GetWoodCount()) + L".";
		m_pUI->SetText(text);
		m_pUI->SetShowSelection(false);
		CCore::GetInstance().GetEventManager().Send<CreationEvent>(CreationEvent{m_pUI, OBJECT_TYPE::UI});
		CCore::GetInstance().GetUIManager().SetUI(m_pUI);
	}
	else if (pPlayer->IsBuyingAnimal())
	{
		CUI*         pUI  = new CUI{};
		std::wstring text = L"What do you want to buy?";
		pUI->SetSelectionType(SELECTION_TYPE::COW_CHICKEN);
		pUI->SetShowSelection(true);
		pUI->SetText(text);
		m_queueUI.push(pUI);

		pUI  = new CUI{};
		text = L"Do you really want to buy it?";
		pUI->SetSelectionType(SELECTION_TYPE::YES_NO);
		pUI->SetShowSelection(true);
		pUI->SetText(text);
		m_queueUI.push(pUI);

		m_pUI = m_queueUI.front();
		m_queueUI.pop();
		CCore::GetInstance().GetEventManager().Send<CreationEvent>(CreationEvent{m_pUI, OBJECT_TYPE::UI});
		CCore::GetInstance().GetUIManager().SetUI(m_pUI);
	}
	else if (false == pPlayer->IsGoingToSleep())
	{
		pPlayer->SetSleep(true);
		m_pUI                   = new CUI{};
		const std::wstring text = L"Are you going to sleep?";
		m_pUI->SetSelectionType(SELECTION_TYPE::YES_NO);
		m_pUI->SetShowSelection(true);
		m_pUI->SetText(text);
		CCore::GetInstance().GetEventManager().Send<CreationEvent>(CreationEvent{m_pUI, OBJECT_TYPE::UI});
		CCore::GetInstance().GetUIManager().SetUI(m_pUI);
	}
}

void CPlayerUIState::Exit()
{
	m_madeDecision   = false;
	m_selected       = ANSWER_TYPE::NONE;
	m_pObject        = nullptr;
	m_pUI            = nullptr;
	CPlayer* pPlayer = static_cast<CPlayer*>(&GetOwner());
	pPlayer->SetBuyingItem(false);
	pPlayer->SetBuyingAnimal(false);
}

void CPlayerUIState::BuyingAnimal(CPlayer* pPlayer, ANSWER_TYPE answer)
{
	if (false == m_queueUI.empty())
	{
		if (m_selected == ANSWER_TYPE::NONE)
		{
			m_selected = answer;
		}
		m_pUI = m_queueUI.front();
		m_queueUI.pop();
		CCore::GetInstance().GetEventManager().Send<CreationEvent>(
			CreationEvent{m_pUI, OBJECT_TYPE::UI});
		CCore::GetInstance().GetUIManager().SetUI(m_pUI);
	}
	else
	{
		if (answer == ANSWER_TYPE::OKAY && pPlayer->IsBuyingAnimal())
		{
			if (pPlayer->GetMoney() < 2000 && m_selected == ANSWER_TYPE::OKAY)
			{
				m_pUI             = new CUI{};
				std::wstring text = L"you have only " + std::to_wstring(pPlayer->GetMoney()) + L". The price is " +
						std::to_wstring(2000);
				m_pUI->SetShowSelection(false);
				m_pUI->SetText(text);
				CCore::GetInstance().GetEventManager().Send<CreationEvent>(CreationEvent{m_pUI, OBJECT_TYPE::UI});
				CCore::GetInstance().GetUIManager().SetUI(m_pUI);
			}
			else if (pPlayer->GetMoney() < 1000 && m_selected == ANSWER_TYPE::CANCEL)
			{
				m_pUI             = new CUI{};
				std::wstring text = L"you have only " + std::to_wstring(pPlayer->GetMoney()) + L". The price is " +
						std::to_wstring(1000);
				m_pUI->SetShowSelection(false);
				m_pUI->SetText(text);
				CCore::GetInstance().GetEventManager().Send<CreationEvent>(CreationEvent{m_pUI, OBJECT_TYPE::UI});
				CCore::GetInstance().GetUIManager().SetUI(m_pUI);
			}
			else
			{
				if (m_selected == ANSWER_TYPE::OKAY) //cow 
				{
					pPlayer->SetMoney(pPlayer->GetMoney() - 2000);
					CAnimal*   pAnimal = new CAnimal{ANIMAL_TYPE::COW};
					AnimalInfo info    = pAnimal->GetInfo();
					pAnimal->SetOwner(pPlayer);
					pAnimal->GetComponent<CTransform>()->SetPos({500, 420});
					CCore::GetInstance().GetStageManager().GetCurrent().MoveObjectsTo(
						STAGE_TYPE::COW, OBJECT_TYPE::ANIMAL, pAnimal);
				}
				else //chicken
				{
					pPlayer->SetMoney(pPlayer->GetMoney() - 1000);
					CAnimal* pAnimal = new CAnimal{ANIMAL_TYPE::CHICKEN};
					pAnimal->SetOwner(pPlayer);
					pAnimal->GetComponent<CTransform>()->SetPos({260, 260});
					CCore::GetInstance().GetStageManager().GetCurrent().MoveObjectsTo(
						STAGE_TYPE::CHICKEN, OBJECT_TYPE::ANIMAL, pAnimal);
				}

				m_pUI             = new CUI{};
				std::wstring text = L"Animal will be in the farm";
				m_pUI->SetShowSelection(false);
				m_pUI->SetText(text);
				CCore::GetInstance().GetEventManager().Send<CreationEvent>(
					CreationEvent{m_pUI, OBJECT_TYPE::UI});
				CCore::GetInstance().GetUIManager().SetUI(m_pUI);
			}
		}
		else
		{
			pPlayer->SetBuyingAnimal(false);
			ChangeState(L"IDLE");
		}
	}
}

void CPlayerUIState::ShowCannotBuy(CPlayer* pPlayer, CItem* pSelected)
{
	int price{};
	if (pSelected->GetName() == L"SEED")
	{
		const ItemID itemId = g_mapSeed[m_selected];
		price               = BASIC_INFO::MAP_DATA[itemId].price;
	}
	else
	{
		price = pSelected->GetInfo().price;
	}
	m_pUI             = new CUI{};
	std::wstring text = L"you have only " + std::to_wstring(pPlayer->GetMoney()) + L". The price is " +
			std::to_wstring(price);
	m_pUI->SetShowSelection(false);
	m_pUI->SetText(text);
	CCore::GetInstance().GetEventManager().Send<CreationEvent>(CreationEvent{m_pUI, OBJECT_TYPE::UI});
	CCore::GetInstance().GetUIManager().SetUI(m_pUI);
}

void CPlayerUIState::BuyCowItems(CPlayer* pPlayer, CItem* pSelected)
{
	std::vector<CItem*>& vecInventory = pPlayer->GetInventory();
	CItem*               pPurchased   = pPlayer->GetItemFromInventory(pSelected->GetInfo().id);
	ItemInfo             info{};
	assert(pPurchased == nullptr);

	pPurchased = new CItem{};
	info       = BASIC_INFO::MAP_DATA[pSelected->GetInfo().id];
	pPurchased->SetInfo(info);
	pPurchased->GetComponent<CTransform>()->SetPos(Vec2{info.location});
	pPurchased->GetComponent<CTransform>()->SetPos(Vec2{info.location});

	CSpriteComponent* pSprite = pPurchased->GetComponent<CSpriteComponent>();
	pSprite->SetTexture(TOOL::TYPES[info.id.kind], LR"(texture\tools.bmp)");

	CCollider* pCollider = pPurchased->GetComponent<CCollider>();
	pCollider->SetScale(Vec2{40.f, 40.f});
	pCollider->SetOffsetPos(Vec2{20.f, 20.f});

	vecInventory.push_back(pPurchased);
	CCore::GetInstance().GetStageManager().GetCurrent().MoveObjectsTo(
		STAGE_TYPE::GARAGE, OBJECT_TYPE::ITEM, pPurchased);
	pSelected->GetComponent<CSpriteComponent>()->SetEnabled(false);
	pSelected->GetComponent<CCollider>()->Enable(false);
}

void CPlayerUIState::BuySeedItem(CPlayer* pPlayer, CItem* pSelected, ANSWER_TYPE answer)
{
	if (false == m_queueUI.empty())
	{
		if (m_selected == ANSWER_TYPE::NONE)
		{
			m_selected = answer;
		}
		m_pUI = m_queueUI.front();
		m_queueUI.pop();
		CCore::GetInstance().GetUIManager().SetUI(m_pUI);
	}
	else
	{
		if (answer == ANSWER_TYPE::OKAY)
		{
			ItemID itemId = g_mapSeed[m_selected];
			if (pPlayer->GetMoney() < BASIC_INFO::MAP_DATA[itemId].price)
			{
				ShowCannotBuy(pPlayer, pSelected);
			}
			else
			{
				pPlayer->SetMoney(pPlayer->GetMoney() - BASIC_INFO::MAP_DATA[itemId].price);
				std::vector<CItem*>& vecInventory = pPlayer->GetInventory();
				CItem*               pPurchased   = pPlayer->GetItemFromInventory(itemId);
				ItemInfo             info{};
				if (nullptr == pPurchased)
				{
					pPurchased = new CItem{};
					info       = BASIC_INFO::MAP_DATA[itemId];
					pPurchased->SetInfo(info);
					pPurchased->GetComponent<CTransform>()->SetPos(Vec2{info.location});
					pPurchased->GetComponent<CTransform>()->SetPos(Vec2{info.location});

					CSpriteComponent* pSprite = pPurchased->GetComponent<CSpriteComponent>();
					pSprite->SetTexture(TOOL::TYPES[info.id.kind], LR"(texture\extra.bmp)");

					CCollider* pCollider = pPurchased->GetComponent<CCollider>();
					pCollider->SetScale(Vec2{40.f, 40.f});
					pCollider->SetOffsetPos(Vec2{20.f, 20.f});

					vecInventory.push_back(pPurchased);
					CCore::GetInstance().GetStageManager().GetCurrent().MoveObjectsTo(
						STAGE_TYPE::GARAGE, OBJECT_TYPE::ITEM, pPurchased);
				}
				else
				{
					for (int i = 0; i < vecInventory.size(); ++i)
					{
						if (itemId == vecInventory[i]->GetInfo().id)
						{
							info = vecInventory[i]->GetInfo();
							++info.currentCount;
							if (info.currentCount > info.remainCount)
							{
								info.currentCount = info.remainCount;
							}
							vecInventory[i]->SetInfo(info);
							break;
						}
					}
				}

				m_pUI             = new CUI{};
				std::wstring text = L"you have " + g_mapPurchasedName[itemId] + L" : " + std::to_wstring(
					info.currentCount);
				m_pUI->SetShowSelection(false);
				m_pUI->SetText(text);
				CCore::GetInstance().GetEventManager().Send<CreationEvent>(
					CreationEvent{m_pUI, OBJECT_TYPE::UI});
				CCore::GetInstance().GetUIManager().SetUI(m_pUI);
			}
		}
		else
		{
			ChangeState(L"IDLE");
		}
		pPlayer->SetCarryingObject(nullptr);
	}
}

bool CPlayerUIState::ChooseToEat(CItem* pSelected, ANSWER_TYPE answer)
{
	return pSelected->GetInfo().canEat && answer == ANSWER_TYPE::OKAY;
}

bool CPlayerUIState::ChooseToBuyTool(CPlayer* pPlayer, CItem* pSelected, ANSWER_TYPE answer)
{
	return false == pPlayer->HasItem(pSelected->GetInfo().id)
			&& pSelected->GetInfo().id.type == ITEM_CATEGORY::TOOL
			&& answer == ANSWER_TYPE::OKAY
			&& pSelected->GetName() != L"SEED";
}

bool CPlayerUIState::ChooseToBuySeed(CPlayer* pPlayer, CItem* pSelected)
{
	return false == pPlayer->HasItem(pSelected->GetInfo().id)
			&& pSelected->GetInfo().id.type == ITEM_CATEGORY::TOOL
			&& pSelected->GetName() == L"SEED";
}
