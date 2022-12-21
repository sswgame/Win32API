#include "pch.h"
#include "CPlot.h"

#include "CCore.h"
#include "CKeyManager.h"
#include "CSpriteComponent.h"
#include "CTile.h"
#include "Name.h"

CPlot::CPlot()
	:
	m_needDayToGrow{0},
	m_currentGrow{0},
	m_changeStep{0},
	m_pTile{nullptr},
	m_itemId{},
	m_isSeeded{false},
	m_isFinishedDay{false},
	m_isFullyGrowed{false},
	m_isWatered{false} {}

CPlot::~CPlot() {}

namespace
{
	std::vector<std::wstring> vecTurnip =
	{
		PLOT::TYPES[PLOT::TURNIP_0], PLOT::TYPES[PLOT::TURNIP_1], PLOT::TYPES[PLOT::TURNIP_2]
	};

	std::vector<std::wstring> vecPotato =
	{
		PLOT::TYPES[PLOT::TURNIP_0], PLOT::TYPES[PLOT::POTATO_1], PLOT::TYPES[PLOT::POTATO_2]
	};

	std::vector<std::wstring> vecTomato =
	{
		PLOT::TYPES[PLOT::TOMATO_0], PLOT::TYPES[PLOT::TOMATO_1], PLOT::TYPES[PLOT::TOMATO_2],
		PLOT::TYPES[PLOT::TOMATO_3], PLOT::TYPES[PLOT::TOMATO_4]
	};

	std::vector<std::wstring> vecCorn =
	{
		PLOT::TYPES[PLOT::CORN_0], PLOT::TYPES[PLOT::CORN_1], PLOT::TYPES[PLOT::CORN_2], PLOT::TYPES[PLOT::CORN_3],
		PLOT::TYPES[PLOT::CORN_4],
	};

	std::vector<std::wstring> vecGrass =
	{
		EXTRA::TYPES[EXTRA::GRASS_1], EXTRA::TYPES[EXTRA::GRASS_2], EXTRA::TYPES[EXTRA::GRASS_3],
		EXTRA::TYPES[EXTRA::GRASS_0],		
	};
}

void CPlot::UpdateSpecific()
{
	if (nullptr == m_pTile)
	{
		return;
	}

	//test code
	if (KEY_AWAY(KEY::N9))
	{
		FinishDay();
	}
	if (m_isFinishedDay && m_isSeeded && false == m_isFullyGrowed)
	{
		if (m_currentGrow != 0 && m_currentGrow % m_changeStep == 0)
		{
			SpriteInfo* pInfo =
					CCore::GetInstance().GetResourceManager().GetSpriteInfo(
						m_vecTypes[m_currentGrow / m_changeStep - 1]);
			CSpriteComponent* pSprite = m_pTile->GetComponent<CSpriteComponent>();
			pSprite->SetInfo(pInfo);
			if (m_itemId.kind < TOOL::SEED_GRASS)
			{
				pSprite->SetTexture(m_vecTypes[m_currentGrow / m_changeStep - 1], LR"(texture\crops.bmp)");
			}
			else
			{
				pSprite->SetTexture(m_vecTypes[m_currentGrow / m_changeStep - 1], LR"(texture\extra.bmp)");
			}
			if (m_itemId.kind != TOOL::SEED_GRASS)
			{
				m_pTile->GetTileInfo().canMove = false;
			}
		}
		if (m_currentGrow / m_changeStep == m_vecTypes.size() - 1)
		{
			m_isFullyGrowed = true;
		}
		m_isFinishedDay = false;
	}
}

void CPlot::Reset()
{
	switch (m_itemId.kind)
	{
	case TOOL::SEED_TURNIP:
	case TOOL::SEED_POTATO:
		{
			m_isSeeded      = false;
			m_isFullyGrowed = false;
			m_needDayToGrow = 0;
			m_currentGrow   = 0;
			m_changeStep    = 0;
			m_vecTypes.clear();
			SpriteInfo* pInfo = CCore::GetInstance().GetResourceManager().GetSpriteInfo(PLOT::TYPES[PLOT::EMPTY_PLOT]);
			CSpriteComponent* pSprite = m_pTile->GetComponent<CSpriteComponent>();
			pSprite->SetInfo(pInfo);
			pSprite->SetTexture(PLOT::TYPES[PLOT::EMPTY_PLOT], LR"(texture\crops.bmp)");
			m_pTile->GetTileInfo().canMove = true;
		}
		break;
	case TOOL::SEED_TOMATO:
	case TOOL::SEED_CORN:
		{
			m_isFullyGrowed   = false;
			m_currentGrow     = m_needDayToGrow - 3;
			SpriteInfo* pInfo = CCore::GetInstance().GetResourceManager().GetSpriteInfo(
				m_vecTypes[m_vecTypes.size() - 3]);
			CSpriteComponent* pSprite = m_pTile->GetComponent<CSpriteComponent>();
			pSprite->SetInfo(pInfo);
			pSprite->SetTexture(m_vecTypes[m_vecTypes.size() - 3], LR"(texture\crops.bmp)");
		}
		break;
	case TOOL::SEED_GRASS:
		{
			m_isFullyGrowed = false;
			m_currentGrow = 0;
			SpriteInfo* pInfo = CCore::GetInstance().GetResourceManager().GetSpriteInfo(EXTRA::TYPES[EXTRA::GRASS_0]);
			CSpriteComponent* pSprite = m_pTile->GetComponent<CSpriteComponent>();
			pSprite->SetInfo(pInfo);
			pSprite->SetTexture(EXTRA::TYPES[EXTRA::GRASS_0], LR"(texture\crops.bmp)");
		}
		break;
	}
}

void CPlot::SetSeed(ItemID _id)
{
	m_itemId = _id;
	switch (m_itemId.kind)
	{
	case TOOL::SEED_TURNIP:
		m_needDayToGrow = 4;
		m_changeStep = m_needDayToGrow / static_cast<int>(vecTurnip.size() - 1);
		m_vecTypes   = vecTurnip;
		break;
	case TOOL::SEED_POTATO:
		m_needDayToGrow = 6;
		m_changeStep = m_needDayToGrow / static_cast<int>(vecPotato.size() - 1);
		m_vecTypes   = vecPotato;
		break;
	case TOOL::SEED_TOMATO:
		m_needDayToGrow = 9;
		m_changeStep = m_needDayToGrow / static_cast<int>(vecTomato.size() - 1);
		m_vecTypes   = vecTomato;
		break;
	case TOOL::SEED_CORN:
		m_needDayToGrow = 12;
		m_changeStep = m_needDayToGrow / static_cast<int>(vecCorn.size() - 1);
		m_vecTypes   = vecCorn;
		break;
	case TOOL::SEED_GRASS:
		m_needDayToGrow = 9;
		m_changeStep = m_needDayToGrow / static_cast<int>(vecGrass.size() - 1);
		m_vecTypes   = vecGrass;
		break;
	}
}

const std::wstring& CPlot::GetCurrentState()
{
	if (m_vecTypes.size() == 0)
	{
		return PLOT::TYPES[PLOT::EMPTY_PLOT];
	}

	if (IsFullyGrowed())
	{
		if (m_itemId.kind != TOOL::SEED_GRASS)
		{
			return m_vecTypes[m_vecTypes.size() - 1];
		}
		return m_vecTypes[m_vecTypes.size() - 2];
	}

	if (m_currentGrow / m_changeStep == 0)
	{
		return PLOT::TYPES[PLOT::SEEDED_PLOT];
	}
	return m_vecTypes[m_currentGrow / m_changeStep - 1];
}

ItemInfo CPlot::GetGrowedItemInfo() const
{
	assert(m_isFullyGrowed);
	ItemID id{};
	id.type = ITEM_CATEGORY::SELL;

	int index = 0;
	for (int i = 0; i < PLOT::TYPES.size(); ++i)
	{
		if (PLOT::TYPES[i] == m_vecTypes[m_vecTypes.size() - 1])
		{
			index = i;
			break;
		}
	}
	id.kind = index;
	return BASIC_INFO::MAP_DATA[id];
}

void CPlot::FinishDay()
{
	if (m_isSeeded && m_isWatered && m_itemId != ItemID{TOOL::SEED_GRASS, ITEM_CATEGORY::TOOL})
	{
		++m_currentGrow;
		m_isWatered        = false;
		std::wstring state = GetCurrentState();

		SpriteInfo* info = CCore::GetInstance().GetResourceManager().GetSpriteInfo(
			state);
		m_pTile->GetComponent<CSpriteComponent>()->SetInfo(info);
		m_pTile->GetComponent<CSpriteComponent>()->SetTexture(
			state, LR"(texture\crops.bmp)");
	}
	else if (m_isSeeded && m_itemId == ItemID{TOOL::SEED_GRASS, ITEM_CATEGORY::TOOL})
	{
		++m_currentGrow;
	}
	m_isFinishedDay = true;
}
