#include "pch.h"
#include "CSoundManager.h"
#include "CCore.h"
#include "CSound.h" 

CSoundManager::CSoundManager()
	:
	m_pSound(nullptr),
	m_pBGM(nullptr) {}

CSoundManager::~CSoundManager() {}

void CSoundManager::Init()
{
	if (FAILED(DirectSoundCreate8(NULL, &m_pSound, NULL)))
	{
		MessageBox(nullptr, L"사운드디바이스생성실패", L"SYSTEM ERROR", MB_OK);
		assert(nullptr);
	}

	// 사운드 디바이스 협조레벨 설정.
	const HWND hWnd = CCore::GetInstance().GetHWnd();
	if (FAILED(m_pSound->SetCooperativeLevel(hWnd, DISCL_EXCLUSIVE))) // Flag값 정리
	{
		MessageBox(nullptr, L"사운드디바이스 협조레벨 설정", L"SYSTEM ERROR", MB_OK);
		assert(nullptr);
	}
}

void CSoundManager::RegisterToBGM(CSound* _pSound)
{
	if (m_pBGM != nullptr)
	{
		m_pBGM->Stop(true);
	}
	m_pBGM = _pSound;
}
