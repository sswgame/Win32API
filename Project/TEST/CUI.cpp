#include "pch.h"
#include "CUI.h"

#include "CCamera.h"
#include "CCore.h"
#include "CPathManager.h"
#include "CSpriteComponent.h"
#include "CUIManager.h"

CUI::CUI()
	:
	m_answer{ANSWER_TYPE::CANCEL},
	m_selection{SELECTION_TYPE::YES_NO},
	m_show{false},
	m_text{},
	m_range{0}
{
	CSpriteComponent* pSprite = AddComponent<CSpriteComponent>();
	pSprite->SetTexture(L"UI", LR"(texture\ui.bmp)", false);
	pSprite->EnableFixed(true);
	const Vec2 scale = pSprite->GetScale();

	CTransform* pTransform = GetComponent<CTransform>();
	pTransform->SetScale(scale);
	pTransform->SetPos({0, 470});
}

CUI::~CUI() = default;

void CUI::SetText(const std::wstring& _text)
{
	m_text = _text;
}

void CUI::Mark(int _type)
{
	if (m_selection == SELECTION_TYPE::COW_CHICKEN || m_selection == SELECTION_TYPE::YES_NO)
	{
		if (_type < 1)
		{
			_type = 1;
		}
		else if (_type > 2)
		{
			_type = 2;
		}
	}
	else if (m_selection == SELECTION_TYPE::SEED)
	{
		if (_type < 3)
		{
			_type = 3;
		}
		else if (_type > 7)
		{
			_type = 7;
		}
	}
	m_range  = _type;
	m_answer = static_cast<ANSWER_TYPE>(_type);
}

void CUI::SetSelectionType(SELECTION_TYPE _type)
{
	m_selection = _type;

	if (m_selection == SELECTION_TYPE::COW_CHICKEN || m_selection == SELECTION_TYPE::YES_NO)
	{
		m_answer = ANSWER_TYPE::CANCEL;
	}
	else
	{
		m_answer = ANSWER_TYPE::TURNIP;
	}
}

void CUI::DrawSelection(HDC _dc)
{
	if (m_show)
	{
		switch (m_selection)
		{
		case SELECTION_TYPE::YES_NO:
			{
				switch (m_answer)
			{
			case ANSWER_TYPE::OKAY:
				{
					TextOut(_dc, 160, 560, L"*OK", 3);
					TextOut(_dc, 400, 560, L" Cancel", 7);
			}
			break;
		case ANSWER_TYPE::CANCEL:
			{
				TextOut(_dc, 180, 560, L" OK", 3);
				TextOut(_dc, 380, 560, L"*Cancel", 7);
			}
			break;
			}
			}
			break;
		case SELECTION_TYPE::COW_CHICKEN:
			{
				switch (m_answer)
			{
			case ANSWER_TYPE::OKAY:
				{
					TextOut(_dc, 160, 560, L"*COW", 4);
					TextOut(_dc, 400, 560, L" CHICKEN", 8);
			}
			break;
		case ANSWER_TYPE::CANCEL:
			{
				TextOut(_dc, 180, 560, L" COW", 4);
				TextOut(_dc, 380, 560, L"*CHICKEN", 8);
			}
			break;
			}
			}
			break;
		case SELECTION_TYPE::SEED:
			{
				switch (m_answer)
			{
			case ANSWER_TYPE::TURNIP:
				{
					TextOut(_dc, 20, 560, L"*TURNIP", 7);
					TextOut(_dc, 160, 560, L" CORN", 5);
					TextOut(_dc, 260, 560, L" TOMATO", 7);
					TextOut(_dc, 400, 560, L" POTATO", 7);
					TextOut(_dc, 520, 560, L" GRASS", 6);
				}
			break;
		case ANSWER_TYPE::CORN:
			{
				TextOut(_dc, 20, 560, L" TURNIP", 7);
				TextOut(_dc, 160, 560, L"*CORN", 5);
				TextOut(_dc, 260, 560, L" TOMATO", 7);
				TextOut(_dc, 400, 560, L" POTATO", 7);
				TextOut(_dc, 520, 560, L" GRASS", 6);
			}
			break;
		case ANSWER_TYPE::TOMATO:
			{
				TextOut(_dc, 20, 560, L" TURNIP", 7);
				TextOut(_dc, 160, 560, L" CORN", 5);
				TextOut(_dc, 260, 560, L"*TOMATO", 7);
				TextOut(_dc, 400, 560, L" POTATO", 7);
				TextOut(_dc, 520, 560, L" GRASS", 6);
			}
			break;
		case ANSWER_TYPE::POTATO:
			{
				TextOut(_dc, 20, 560, L" TURNIP", 7);
				TextOut(_dc, 160, 560, L" CORN", 5);
				TextOut(_dc, 260, 560, L" TOMATO", 7);
				TextOut(_dc, 400, 560, L"*POTATO", 7);
				TextOut(_dc, 520, 560, L" GRASS", 6);
			}
			break;
		case ANSWER_TYPE::GRASS:
			{
				TextOut(_dc, 20, 560, L" TURNIP", 7);
				TextOut(_dc, 160, 560, L" CORN", 5);
				TextOut(_dc, 260, 560, L" TOMATO", 7);
				TextOut(_dc, 400, 560, L" POTATO", 7);
				TextOut(_dc, 520, 560, L"*GRASS", 6);
			}
			break;
			}
			}
			break;
		}
	}
}

void CUI::MoveTo(MOVE_TYPE _type)
{
	switch (_type)
	{
	case MOVE_TYPE::LEFT:
		Mark(--m_range);
		break;
	case MOVE_TYPE::RIGHT:
		Mark(++m_range);
		break;
	}
}
