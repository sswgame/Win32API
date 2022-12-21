#pragma once
class CWindow
{
public:
	CWindow();
	~CWindow();

public:
	bool Initialize();
	void Run();

private:
	bool InitWindow();

	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	void RegisterMessageFunc();
	void OnDestroy(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

private:
	using WindowMessageFunc = std::function<void(CWindow* pWindow, HWND hWnd, UINT message, WPARAM wParam,
	                                             LPARAM   lParam)>;
	std::map<UINT, WindowMessageFunc> m_mapFunc;

	HWND         m_hWnd;
	HINSTANCE    m_hInstance;
	std::wstring m_className;
};
