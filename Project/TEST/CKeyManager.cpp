#include "pch.h"
#include "CKeyManager.h"

#include "CCore.h"

namespace
{
	constexpr int KEY_BINDING[static_cast<int>(KEY::NONE)] =
	{
		'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P',
		'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L',
		'Z', 'X', 'C', 'V', 'B', 'N', 'M',
		'1', '2', '3', '4', '5', '6', '7', '8', '9', '0',
		VK_NUMPAD0, VK_NUMPAD1, VK_NUMPAD2, VK_NUMPAD3, VK_NUMPAD4, VK_NUMPAD5, VK_NUMPAD6, VK_NUMPAD7, VK_NUMPAD8,
		VK_NUMPAD9,
		VK_LEFT, VK_UP, VK_RIGHT, VK_DOWN,
		VK_INSERT, VK_HOME, VK_PRIOR,
		VK_DELETE, VK_END, VK_NEXT,
		VK_LCONTROL, VK_RCONTROL,
		VK_LMENU, VK_RMENU, VK_RETURN, VK_BACK, VK_SPACE, VK_ESCAPE,
		VK_LSHIFT, VK_RSHIFT,
		VK_LBUTTON,VK_RBUTTON,
	};
	constexpr int KEY_COUNT = static_cast<int>(KEY::END);
}

CKeyManager::CKeyManager() = default;

CKeyManager::~CKeyManager() = default;

void CKeyManager::Init()
{
	m_vecKeyInfo.resize(static_cast<size_t>(KEY::NONE), {KEY_STATE::NONE, false});
}

void CKeyManager::Update()
{
	const HWND focusedHandle = GetFocus();

	if (nullptr != focusedHandle)
	{
		for (UINT i = 0; i < static_cast<UINT>(KEY::NONE); ++i)
		{
			if (GetAsyncKeyState(KEY_BINDING[i]) & 0x8000)
			{
				if (false == m_vecKeyInfo[i].isPreviouslyChecked)
				{
					m_vecKeyInfo[i].keyState = KEY_STATE::TAP;
				}
				else
				{
					m_vecKeyInfo[i].keyState = KEY_STATE::PRESSED;
				}
				m_vecKeyInfo[i].isPreviouslyChecked = true;
			}
			else
			{
				if (KEY_STATE::TAP == m_vecKeyInfo[i].keyState
					|| KEY_STATE::PRESSED == m_vecKeyInfo[i].keyState)
				{
					m_vecKeyInfo[i].keyState = KEY_STATE::AWAY;
				}
				else
				{
					m_vecKeyInfo[i].keyState = KEY_STATE::NONE;
				}
				m_vecKeyInfo[i].isPreviouslyChecked = false;
			}
		}

		// Get Mouse Pos
		POINT mousePos{};
		GetCursorPos(&mousePos);
		ScreenToClient(CCore::GetInstance().GetHWnd(), &mousePos);

		m_mousePrevPos = m_mousePos;
		m_mousePos     = mousePos;
	}

	else
	{
		for (UINT i = 0; i < KEY_COUNT; ++i)
		{
			m_vecKeyInfo[i].isPreviouslyChecked = false;
			if (KEY_STATE::TAP == m_vecKeyInfo[i].keyState ||
				KEY_STATE::PRESSED == m_vecKeyInfo[i].keyState)
			{
				m_vecKeyInfo[i].keyState = KEY_STATE::AWAY;
			}

			else if (KEY_STATE::AWAY == m_vecKeyInfo[i].keyState)
			{
				m_vecKeyInfo[i].keyState = KEY_STATE::NONE;
			}
		}
	}
}
