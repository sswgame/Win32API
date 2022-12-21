#include "pch.h"
#include "CItem.h"

#include "CCollider.h"
#include "CColliderManager.h"
#include "CCore.h"
#include "CEventManager.h"
#include "Name.h"
#include "CSpriteComponent.h"
#include "CTile.h"
#include "CTimer.h"

namespace
{
	std::vector<std::pair<std::wstring, std::wstring>> vecCrashed =
	{
		{PLOT::TYPES[PLOT::TURNIP_2], PLOT::TYPES[PLOT::TURNIP_3]},
		{PLOT::TYPES[PLOT::POTATO_2], PLOT::TYPES[PLOT::POTATO_3]},
		{PLOT::TYPES[PLOT::TOMATO_4], PLOT::TYPES[PLOT::TOMATO_5]},
		{PLOT::TYPES[PLOT::CORN_4], PLOT::TYPES[PLOT::CORN_5]},
		{EXTRA::TYPES[EXTRA::GRASS_3], EXTRA::TYPES[EXTRA::GRASS_0]},
		{EXTRA::TYPES[EXTRA::EGG], EXTRA::TYPES[EXTRA::EGG_CRASHED]},
		{EXTRA::TYPES[EXTRA::WOOD], EXTRA::TYPES[EXTRA::WOOD_CRASHED]},
	};

	std::set<ItemID> setCowItems =
	{
		{TOOL::SPECIAL_MEDICINE, ITEM_CATEGORY::TOOL},
		{TOOL::MEDICINE, ITEM_CATEGORY::TOOL},
		{TOOL::MILKING_TOOL, ITEM_CATEGORY::TOOL},
		{TOOL::BRUSH, ITEM_CATEGORY::TOOL},
	};
}


CItem::CItem()
	:
	m_pInfo{},
	m_enabled{false},
	m_pOwner{nullptr},
	m_isThrow{false},
	m_elapsedTime{0.f},
	m_delay{0.f},
	m_remain{0.f},
	m_pTile{nullptr}
{
	AddComponent<CCollider>();
	AddComponent<CSpriteComponent>();
}

CItem::~CItem() = default;

void CItem::UpdateSpecific()
{
	if (m_isThrow)
	{
		m_elapsedTime += DS;
		m_delay += DS;
		for (int i = 0; i < vecCrashed.size(); ++i)
		{
			if (GetComponent<CSpriteComponent>()->GetInfo()->textureName == vecCrashed[i].first)
			{
				SpriteInfo* info = CCore::GetInstance().GetResourceManager().GetSpriteInfo(
					PLOT::TYPES[PLOT::SEEDED_PLOT]);
				GetComponent<CSpriteComponent>()->SetInfo(info);
				if (i < 4)
				{
					GetComponent<CSpriteComponent>()->SetTexture(vecCrashed[i].second, LR"(texture\crops.bmp)");
				}
				else
				{
					GetComponent<CSpriteComponent>()->SetTexture(vecCrashed[i].second, LR"(texture\extra.bmp)");
				}
			}
		}

		if (m_delay > 0.5f)
		{
			if (m_delay >= 1.f)
			{
				m_delay = 0.f;
			}
			GetComponent<CSpriteComponent>()->SetEnabled(false);
		}
		else if (m_delay <= 0.5f)
		{
			GetComponent<CSpriteComponent>()->SetEnabled(true);
		}

		if (m_elapsedTime >= 3.f)
		{
			m_elapsedTime = 0;
			m_isThrow     = false;
			CCore::GetInstance().GetEventManager().Send<DeletionEvent>(DeletionEvent{this});
		}
	}

	if (true == GetComponent<CCollider>()->IsEnabled()
		&& GetComponent<CCollider>()->GetScale() == Vec2{20.f, 20.f})
	{
		m_remain += DS;
		if (m_remain > 0.05f)
		{
			GetComponent<CCollider>()->Enable(false);
			m_remain = 0;
		}
	}
}

void CItem::OnCollisionEnter(CGameObject* _pOther)
{
	if (_pOther->GetType() == OBJECT_TYPE::ANIMAL
		&& setCowItems.count(m_pInfo.id) != 0)
	{
		GetComponent<CCollider>()->SetScale(Vec2{40.f, 40.f});
		GetComponent<CTransform>()->SetPos(Vec2{-999, 0});
	}
}

void CItem::OnCollisionExit(CGameObject* _pOther)
{
	if (_pOther->GetType() == OBJECT_TYPE::ANIMAL
		&& setCowItems.count(m_pInfo.id) != 0)
	{
		GetComponent<CCollider>()->Enable(false);
	}
}
