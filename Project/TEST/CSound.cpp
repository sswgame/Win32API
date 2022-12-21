#include "pch.h"
#include "CSound.h"

#include "CCore.h"


CSound::CSound()
	:
	m_pSoundBuffer(nullptr),
	m_tBuffInfo{},
	m_iVolume(0) {}

CSound::~CSound()
{
	if (nullptr != m_pSoundBuffer)
	{
		m_pSoundBuffer->Release();
	}
}

bool CSound::Load(const std::wstring& _strPath)
{
	if (nullptr == CCore::GetInstance().GetSoundManager().GetSoundDevice())
	{
		assert(nullptr); // 사운드 객체 생성되지 않음
	}

	// 확장자 이름 구별하기
	wchar_t szExt[10] = {0};
	_wsplitpath_s(_strPath.c_str(), nullptr, 0, nullptr, 0, nullptr, 0, szExt, 10);

	if (!wcscmp(szExt, L".wav")) // WAV 파일 로드
	{
		if (false == LoadWaveSound(_strPath))
		{
			assert(nullptr);
		}
	}
	else
	{
		assert(nullptr);
	}

	return true;
}

bool CSound::LoadWaveSound(const std::wstring& _filePath)
{
	HMMIO hFile{}; // File Handle

	std::wstring strFilePath = _filePath;

	//CreateFile
	hFile = mmioOpen(const_cast<LPWSTR>(strFilePath.c_str()), nullptr, MMIO_READ); //wave파일을 연다.

	if (nullptr == hFile)
	{
		MessageBox(nullptr, L"사운드 리소스 경로에 파일 없음", L"사운드 로딩 실패", MB_OK);
		return false;
	}

	//Chunk 청크 구조체, 문자열로 색인을 인식해서 WaveFormat 및 버퍼선언정보를 읽어온다.
	MMCKINFO pParent{};
	memset(&pParent, 0, sizeof(pParent));
	pParent.fccType = mmioFOURCC('W', 'A', 'V', 'E');
	mmioDescend(hFile, &pParent, nullptr, MMIO_FINDRIFF);

	MMCKINFO pChild{};
	memset(&pChild, 0, sizeof(pChild));
	pChild.ckid = mmioFOURCC('f', 'm', 't', ' ');
	mmioDescend(hFile, &pChild, &pParent, MMIO_FINDCHUNK);

	WAVEFORMATEX wft{};
	memset(&wft, 0, sizeof(wft));
	mmioRead(hFile, reinterpret_cast<char*>(&wft), sizeof(wft));

	mmioAscend(hFile, &pChild, 0);
	pChild.ckid = mmioFOURCC('d', 'a', 't', 'a');
	mmioDescend(hFile, &pChild, &pParent, MMIO_FINDCHUNK);


	memset(&m_tBuffInfo, 0, sizeof(DSBUFFERDESC));
	m_tBuffInfo.dwBufferBytes = pChild.cksize;
	m_tBuffInfo.dwSize        = sizeof(DSBUFFERDESC);
	m_tBuffInfo.dwFlags       = DSBCAPS_STATIC | DSBCAPS_LOCSOFTWARE | DSBCAPS_CTRLVOLUME;
	m_tBuffInfo.lpwfxFormat   = &wft;

	if (FAILED(
		CCore::GetInstance().GetSoundManager().GetSoundDevice()->CreateSoundBuffer(&m_tBuffInfo, &m_pSoundBuffer, NULL
		)))
	{
		MessageBox(nullptr, L"사운드버퍼생성실패", L"", MB_OK);
		return false;
	}

	void* pWrite1 = nullptr;
	void* pWrite2 = nullptr;
	DWORD dwlength1{};
	DWORD dwlength2{};

	m_pSoundBuffer->Lock(0, pChild.cksize, &pWrite1, &dwlength1
	                   , &pWrite2, &dwlength2, 0L);

	if (pWrite1 != nullptr)
	{
		mmioRead(hFile, static_cast<char*>(pWrite1), dwlength1);
	}
	if (pWrite2 != nullptr)
	{
		mmioRead(hFile, static_cast<char*>(pWrite2), dwlength2);
	}

	m_pSoundBuffer->Unlock(pWrite1, dwlength1, pWrite2, dwlength2);

	mmioClose(hFile, 0);

	// 초기 음량 절반으로 설정
	SetVolume(50.f);

	return true;
}


void CSound::Play(bool _bLoop)
{
	// Play 함수의 1번째 2번째 인자는 0 으로 이미 예약되어있다.
	// 3번째 변수는 사운드를 반복재생 할 것인지 아닌지를 결정한다.
	if (_bLoop)
	{
		m_pSoundBuffer->Play(0, 0, DSBPLAY_LOOPING);
	}
	else
	{
		m_pSoundBuffer->Play(0, 0, 0);
	}
}

void CSound::PlayToBGM(bool _bLoop)
{
	CCore::GetInstance().GetSoundManager().RegisterToBGM(this);

	// Play 함수의 1번째 2번째 인자는 0 으로 이미 예약되어있다.
	// 3번째 변수는 사운드를 반복재생 할 것인지 아닌지를 결정한다.
	if (_bLoop)
	{
		m_pSoundBuffer->Play(0, 0, DSBPLAY_LOOPING);
	}
	else
	{
		m_pSoundBuffer->Play(0, 0, 0);
	}
}

void CSound::Stop(bool _bReset)
{
	m_pSoundBuffer->Stop();

	if (_bReset)
	{
		m_pSoundBuffer->SetCurrentPosition(0);
	}
}


void CSound::SetVolume(float _fVolume)
{
	m_iVolume = GetDecibel(_fVolume);
	m_pSoundBuffer->SetVolume(m_iVolume);
}

void CSound::SetPosition(float _fPosition)
{
	Stop(true);

	DWORD dwBytes = static_cast<DWORD>((_fPosition / 100.f) * (float)m_tBuffInfo.dwBufferBytes);
	m_pSoundBuffer->SetCurrentPosition(dwBytes);

	Play();
}

int CSound::GetDecibel(float _fVolume)
{
	if (_fVolume > 100.f)
	{
		_fVolume = 100.f;
	}
	else if (_fVolume <= 0.f)
	{
		_fVolume = 0.00001f;
	}

	// 1 ~ 100 사이값을 데시벨 단위로 변경
	int iVolume = static_cast<LONG>(-2000.0 * log10(100.f / _fVolume));

	if (iVolume < -10000)
	{
		iVolume = -10000;
	}
	return iVolume;
}
