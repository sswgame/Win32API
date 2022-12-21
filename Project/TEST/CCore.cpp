#include "pch.h"
#include "CCore.h"

#include "CCamera.h"
#include "CColliderManager.h"
#include "CEventManager.h"
#include "CKeyManager.h"
#include "CPathManager.h"
#include "CStageManager.h"
#include "CResourceManager.h"
#include "CSoundManager.h"
#include "CTileManager.h"
#include "CTimer.h"
#include "CUIManager.h"

CCore* CCore::s_pInstance{};

CCore::CCore()
	:
	m_hWnd{nullptr},
	m_resolution{},
	m_hdc{nullptr},
	m_hBackBufferDC{nullptr},
	m_hBitmap{nullptr},

	m_hGreenPen{nullptr},
	m_hRedPen{nullptr},

	m_pTimer{nullptr},
	m_pStageManager{nullptr},
	m_pKeyManager{nullptr},
	m_pResourceManager{nullptr},
	m_pPathManager{nullptr},
	m_pEventManager{nullptr},
	m_pCamera{nullptr},
	m_pTileManager{nullptr},
	m_pColliderManager{nullptr},
	m_pSoundManager{nullptr},
	m_pUIManager{nullptr} {}

CCore::~CCore()
{
	ReleaseDC(m_hWnd, m_hdc);
	DeleteObject(m_hBitmap);
	DeleteDC(m_hBackBufferDC);

	DeleteObject(m_hGreenPen);
	DeleteObject(m_hRedPen);
}

void CCore::ShowFps() const
{
	static float time = 0.f;
	time += DS;
	if (time >= 1.f)
	{
		std::wstring text = {L"FPS : "};
		text.append(std::to_wstring(m_pTimer->GetFPS()));
		SetWindowText(m_hWnd, text.c_str());

		time = time - 1.f;
	}
}

CCore& CCore::GetInstance()
{
	if (nullptr == s_pInstance)
	{
		s_pInstance = new CCore{};
	}
	return *s_pInstance;
}

void CCore::Init(HWND _hWnd, const POINT& _resolution)
{
	m_hWnd = _hWnd;
	m_hdc  = GetDC(m_hWnd);

	//Set the window with exact client size that we need
	ChangeWindowSize(_resolution, false);

	//Create Pen
	m_hGreenPen = CreatePen(PS_SOLID, 1, RGB(120, 255, 0));
	m_hRedPen   = CreatePen(PS_SOLID, 1, RGB(255, 40, 40));

	//Make DC & Bitmap to use them as a back-buffer
	m_hBitmap       = CreateCompatibleBitmap(m_hdc, m_resolution.x, m_resolution.y);
	m_hBackBufferDC = CreateCompatibleDC(m_hdc);

	const HBITMAP hPrevBitmap = static_cast<HBITMAP>(SelectObject(m_hBackBufferDC, m_hBitmap));
	DeleteObject(hPrevBitmap);

	//Initialized all the managers and the other utilities that need for entire game
	m_pPathManager     = new CPathManager{};
	m_pResourceManager = new CResourceManager{};
	m_pKeyManager      = new CKeyManager{};
	m_pEventManager    = new CEventManager{};
	m_pStageManager    = new CStageManager{};
	m_pTimer           = new CTimer{};
	m_pCamera          = new CCamera{};
	m_pTileManager     = new CTileManager{};
	m_pColliderManager = new CColliderManager{};
	m_pSoundManager    = new CSoundManager{};
	m_pUIManager       = new CUIManager{};

	m_pPathManager->Init();
	m_pKeyManager->Init();
	m_pCamera->Init();
	m_pSoundManager->Init();
	m_pStageManager->Init();
	m_pUIManager->Init();
}

void CCore::Update()
{
	//Manager Update
	m_pTimer->Update();
	ShowFps();

	if (m_pCamera->IsEffectFinished())
	{
		m_pEventManager->Update();
		//1.process Input
		m_pKeyManager->Update();
	}

	//2. UpdateGame()
	m_pCamera->Update();

	m_pStageManager->Update();
	m_pColliderManager->Update();


	//3. Render()
	Rectangle(m_hBackBufferDC, -1, -1, m_resolution.x + 1, m_resolution.y + 1);
	m_pStageManager->Render(m_hBackBufferDC);
	m_pUIManager->Render(m_hBackBufferDC);
	m_pCamera->Render(m_hBackBufferDC);
	BitBlt(m_hdc, 0, 0, m_resolution.x, m_resolution.y, m_hBackBufferDC, 0, 0, SRCCOPY);
}

void CCore::Release()
{
	delete m_pUIManager;
	m_pUIManager = nullptr;

	delete m_pColliderManager;
	m_pColliderManager = nullptr;

	delete m_pTileManager;
	m_pTileManager = nullptr;

	delete m_pCamera;
	m_pCamera = nullptr;

	delete m_pEventManager;
	m_pEventManager = nullptr;

	delete m_pTimer;
	m_pTimer = nullptr;

	delete m_pResourceManager;
	m_pResourceManager = nullptr;

	delete m_pStageManager;
	m_pStageManager = nullptr;

	delete m_pKeyManager;
	m_pKeyManager = nullptr;

	delete m_pPathManager;
	m_pPathManager = nullptr;

	delete m_pSoundManager;
	m_pSoundManager = nullptr;

	delete s_pInstance;
	s_pInstance = nullptr;
}

void CCore::ChangeWindowSize(const POINT& _resolution, bool _menu)
{
	m_resolution = _resolution;

	RECT windowRect = {0, 0, m_resolution.x, m_resolution.y};
	AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, _menu);
	SetWindowPos(m_hWnd, nullptr,
	             0, 0,
	             windowRect.right - windowRect.left,
	             windowRect.bottom - windowRect.top,
	             0);
}
