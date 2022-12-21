#pragma once

enum class KEY
{
	Q, W, E, R, T, Y, U, I, O, P,
	A, S, D, F, G, H, J, K, L,
	Z, X, C, V, B, N, M,
	N1, N2, N3, N4, N5, N6, N7, N8, N9, N0,
	NUMPAD_0, NUMPAD_1, NUMPAD_2, NUMPAD_3, NUMPAD_4, NUMPAD_5, NUMPAD_6, NUMPAD_7, NUMPAD_8, NUMPAD_9,
	LEFT, UP, RIGHT, DOWN,
	INSERT, HOME, PAGE_UP,
	DEL, END, PAGE_DOWN,
	L_CTRL, R_CTRL,
	L_ALT, R_ALT,
	ENTER, BACK_SPACE, SPACE, ESC,
	L_SHIFT, R_SHIFT,
	MOUSE_LBUTTON, MOUSE_RBUTTON,
	NONE
};

enum class KEY_STATE
{
	TAP,
	PRESSED,
	AWAY,
	NONE,
};

struct KeyInfo
{
	KEY_STATE keyState;
	bool      isPreviouslyChecked;
};

class CKeyManager
{
public:
	CKeyManager();
	~CKeyManager();

public:
	void Init();
	void Update();

	KEY_STATE GetKeyState(const KEY& _key) const { return m_vecKeyInfo[static_cast<UINT>(_key)].keyState; }
	Vec2      GetMousePos() const { return m_mousePos; }
	Vec2      GetMouseDir() const { return {m_mousePos - m_mousePrevPos}; }

private:
	std::vector<KeyInfo> m_vecKeyInfo;
	Vec2                 m_mousePos;
	Vec2                 m_mousePrevPos;
};
