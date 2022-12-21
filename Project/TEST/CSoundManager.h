#pragma once
// sound �뵵 - ��� ���� �߿�
#include <mmsystem.h>
#include <dsound.h>
#include <dinput.h>

// �ڵ�
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
	LPDIRECTSOUND8 m_pSound; // ����ī�� ��ǥ ��ü
	CSound*        m_pBGM;   // ���� ������ BGM Sound
};
