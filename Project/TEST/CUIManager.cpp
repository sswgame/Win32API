#include "pch.h"
#include "CUIManager.h"

#include "CCore.h"
#include "CPathManager.h"
#include "CResourceManager.h"
#include "CSound.h"
#include "CTimer.h"
#include "CUI.h"

CUIManager::CUIManager()
	:
	m_pCurrentUI{nullptr},
	m_font{nullptr},
	m_delay{0},
	m_length{0},
	m_height{0},
	m_isShowedAll{false} { }

CUIManager::~CUIManager()
{
	DeleteObject(m_font);
	RemoveFontResource((CCore::GetInstance().GetPathManager().GetContentPath() + LR"(texture\font.ttf)").c_str());
	SendNotifyMessage(HWND_BROADCAST, WM_FONTCHANGE, 0, 0);
}

void CUIManager::Init()
{
	AddFontResource((CCore::GetInstance().GetPathManager().GetContentPath() + LR"(texture\font.ttf)").c_str());
	SendNotifyMessage(HWND_BROADCAST, WM_FONTCHANGE, 0, 0);

	SetBkMode(CCore::GetInstance().GetBack(), -1);
	SetTextColor(CCore::GetInstance().GetBack(), RGB(255, 255, 255));
	m_font = CreateFont(30, 0, 0, 0, 0, 0, 0, 0, ANSI_CHARSET, 0, 0, 0, VARIABLE_PITCH || FF_ROMAN,
	                    TEXT("Retro Gaming"));
	const HFONT old = static_cast<HFONT>(SelectObject(CCore::GetInstance().GetBack(), m_font));
	DeleteObject(old);
}

void CUIManager::Render(HDC _dc)
{
	if (nullptr == m_pCurrentUI)
	{
		return;
	}
	m_length = 0;


	if (false == m_queueLine.empty())
	{
		std::deque<wchar_t>& line = m_queueLine.front();
		if (false == line.empty())
		{
			m_delay += DS;
			wchar_t ch = line.front();
			for (int i = 0; i < m_vecTexts.size(); ++i)
			{
				TextOut(_dc, 25 + m_length, 475 + i * 50, m_vecTexts[i].c_str(),
				        static_cast<int>(m_vecTexts[i].length()));
			}
			TextOut(_dc, 25 + m_length, 475 + m_height, m_text.c_str(), static_cast<int>(m_text.length()));
			if (m_delay >= 0.1f)
			{
				CSound* pSound = CCore::GetInstance().GetResourceManager().FindSound(L"text");
				pSound->Play(false);
				m_text += ch;
				m_length += 25;
				line.pop_front();
				m_delay = 0.f;
			}
		}
		else
		{
			m_vecTexts.push_back(m_text);
			m_height += 50;
			m_text.clear();
			m_queueLine.pop();
		}
	}
	else
	{
		m_isShowedAll = true;
		for (int i = 0; i < m_vecTexts.size(); ++i)
		{
			TextOut(_dc, 25 + m_length, 475 + i * 50, m_vecTexts[i].c_str(), static_cast<int>(m_vecTexts[i].length()));
		}
		m_pCurrentUI->DrawSelection(_dc);
	}
}

void CUIManager::SetUI(CUI* _pUI)
{
	m_pCurrentUI = _pUI;
	if (m_pCurrentUI)
	{
		std::wstring       text = m_pCurrentUI->GetText();
		std::wstringstream ss{text};
		while (std::getline(ss, text, L'\n'))
		{
			std::deque<wchar_t> queue;
			queue.assign(text.begin(), text.end());
			m_queueLine.push(queue);
		}
	}
	if (nullptr == _pUI)
	{
		Clear();
	}
}

void CUIManager::Clear()
{
	m_height = 0;
	m_vecTexts.clear();
	m_text.clear();
	m_isShowedAll = false;
}
