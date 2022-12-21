#pragma once

#ifdef _DEBUG
#define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif

#define KEY_PRESSED(key)	CCore::GetInstance().GetKeyManager().GetKeyState(key) == KEY_STATE::PRESSED
#define KEY_TAP(key)		CCore::GetInstance().GetKeyManager().GetKeyState(key) == KEY_STATE::TAP
#define KEY_AWAY(key)		CCore::GetInstance().GetKeyManager().GetKeyState(key) == KEY_STATE::AWAY
#define DS					CCore::GetInstance().GetTimer().GetDeltaTime()
