#pragma once

class CUIManager;
class CSoundManager;
class CColliderManager;
class CCamera;
class CEventManager;
class CPathManager;
class CResourceManager;
class CStageManager;
class CKeyManager;
class CTimer;
class CTileManager;

class CCore final
{
public:
	static CCore& GetInstance();
	CCore(const CCore&)            = delete;
	CCore& operator=(const CCore&) = delete;

public:
	void Init(HWND _hWnd, const POINT& _resolution);
	void Update();
	void Release();
	void ChangeWindowSize(const POINT& _resolution, bool _menu);

public:
	HWND  GetHWnd() const { return m_hWnd; }
	HDC   GetMainDC() const { return m_hdc; }
	HDC   GetBack() const { return m_hBackBufferDC; }
	POINT GetResolution() const { return m_resolution; }

	HPEN GetRedPen() const { return m_hRedPen; }
	HPEN GetGreenPen() const { return m_hGreenPen; }

	CTimer&           GetTimer() const { return *m_pTimer; }
	CStageManager&    GetStageManager() const { return *m_pStageManager; }
	CKeyManager&      GetKeyManager() const { return *m_pKeyManager; }
	CResourceManager& GetResourceManager() const { return *m_pResourceManager; }
	CPathManager&     GetPathManager() const { return *m_pPathManager; }
	CEventManager&    GetEventManager() const { return *m_pEventManager; }
	CCamera&          GetCamera() const { return *m_pCamera; }
	CTileManager&     GetTileManager() const { return *m_pTileManager; }
	CColliderManager& GetColliderManager() const { return *m_pColliderManager; }
	CSoundManager&    GetSoundManager() const { return *m_pSoundManager; }
	CUIManager&       GetUIManager() const { return *m_pUIManager; }
private:
	CCore();
	~CCore();
	void ShowFps() const;

private:
	static CCore* s_pInstance;

	HWND    m_hWnd;
	POINT   m_resolution;
	HDC     m_hdc;
	HDC     m_hBackBufferDC;
	HBITMAP m_hBitmap;

	HPEN m_hGreenPen;
	HPEN m_hRedPen;

	//Managers
	CTimer*           m_pTimer;
	CStageManager*    m_pStageManager;
	CKeyManager*      m_pKeyManager;
	CResourceManager* m_pResourceManager;
	CPathManager*     m_pPathManager;
	CEventManager*    m_pEventManager;
	CCamera*          m_pCamera;
	CTileManager*     m_pTileManager;
	CColliderManager* m_pColliderManager;
	CSoundManager*    m_pSoundManager;
	CUIManager*       m_pUIManager;
};
