#include "pch.h"
#include "Window.h"
#include "CCore.h"

CWindow::CWindow()
	:
	m_hWnd{nullptr},
	m_hInstance{GetModuleHandleA(nullptr)},
	m_className{L"GAME"} {}

bool CWindow::Initialize()
{
	RegisterMessageFunc();

	if (false == InitWindow())
	{
		return false;
	}
	CCore::GetInstance().Init(m_hWnd, {640, 640});

	return true;
}

void CWindow::Run()
{
	MSG msg{};

	// 기본 메시지 루프입니다:
	while (true)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			if (WM_QUIT == msg.message)
			{
				break;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			CCore::GetInstance().Update();
		}
	}
}

CWindow::~CWindow()
{
	UnregisterClass(m_className.c_str(), m_hInstance);
	CCore::GetInstance().Release();
}

bool CWindow::InitWindow()
{
	WNDCLASSEXW wcex{};
	wcex.cbSize        = sizeof(WNDCLASSEX);
	wcex.style         = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc   = WndProc;
	wcex.hInstance     = m_hInstance;
	wcex.lpszClassName = m_className.c_str();

	RegisterClassExW(&wcex);

	m_hWnd = CreateWindowW(wcex.lpszClassName, wcex.lpszClassName, WS_OVERLAPPEDWINDOW,
	                       CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, wcex.hInstance, this);

	if (!m_hWnd)
	{
		assert(m_hWnd);
		return false;
	}
	ShowWindow(GetConsoleWindow(), SW_HIDE);
	ShowWindow(m_hWnd, SW_SHOW);
	UpdateWindow(m_hWnd);

	return true;
}

LRESULT CWindow::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (WM_NCCREATE == message)
	{
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
		CWindow* const             pWindow = static_cast<CWindow*>(pCreate->lpCreateParams);
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWindow));
	}
	else
	{
		CWindow* const pWindow = reinterpret_cast<CWindow*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
		if (pWindow && pWindow->m_mapFunc.count(message))
		{
			pWindow->m_mapFunc[message](pWindow, hWnd, message, wParam, lParam);
		}
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}

void CWindow::RegisterMessageFunc()
{
	m_mapFunc.clear();
	m_mapFunc[WM_DESTROY] = &CWindow::OnDestroy;
}

void CWindow::OnDestroy(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(message);
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);
	PostQuitMessage(0);
}
