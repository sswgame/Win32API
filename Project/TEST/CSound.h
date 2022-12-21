#pragma once
#include "CResource.h"

#include "CSoundManager.h"

class CSound : public CResource
{
public:
	CSound();
	virtual ~CSound();

public:
	bool Load(const std::wstring& _strPath) override;

	// �Ϲ� ���
	void Play(bool _bLoop = false);

	// BGM ���� ���
	void PlayToBGM(bool _bLoop = false);

	void Stop(bool _bReset = false);

	// ���� ���� (0 ~ 100)
	void SetVolume(float _fVolume);

	// �������� ��ġ ����
	void SetPosition(float _fPosition); // 0 ~ 100 ����

private:
	bool LoadWaveSound(const std::wstring& _filePath);
	int  GetDecibel(float _fVolume);


private:
	LPDIRECTSOUNDBUFFER m_pSoundBuffer;
	DSBUFFERDESC        m_tBuffInfo;
	int                 m_iVolume;

};

