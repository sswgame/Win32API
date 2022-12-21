#pragma once
// sound 용도 - 헤더 순서 중요
#include <mmsystem.h>
#include <dsound.h>
#include <dinput.h>

// 코드
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "dsound.lib")

class CSound;

class CSoundManager
{
public:
	CSoundManager();
	~CSoundManager();

public:
	void           Init();
	LPDIRECTSOUND8 GetSoundDevice() const { return m_pSound; }
	void           RegisterToBGM(CSound* _pSound);
	CSound*        GetCurrentBGM() const { return m_pBGM; }
private:
	LPDIRECTSOUND8 m_pSound; // 사운드카드 대표 객체
	CSound*        m_pBGM;   // 현재 지정된 BGM Sound
};
