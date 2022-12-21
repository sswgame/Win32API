#include "pch.h"
#include "CTexture.h"
#include "CCore.h"

CTexture::CTexture()
	:
	m_hBitmap{nullptr},
	m_hBitmapDC{nullptr},
	m_bitmapInfo{} {}

CTexture::~CTexture()
{
	DeleteObject(m_hBitmap);
	DeleteDC(m_hBitmapDC);
}

void CTexture::Create(UINT _width, UINT _height)
{
	m_hBitmap   = CreateCompatibleBitmap(CCore::GetInstance().GetMainDC(), _width, _height);
	m_hBitmapDC = CreateCompatibleDC(CCore::GetInstance().GetMainDC());

	assert(m_hBitmap);
	assert(m_hBitmapDC);

	const HGDIOBJ hPrevBitmap = SelectObject(m_hBitmapDC, m_hBitmap);
	DeleteObject(hPrevBitmap);

	GetObject(m_hBitmap, sizeof(BITMAP), &m_bitmapInfo);
}

bool CTexture::Load(const std::wstring& _fullPath)
{
	m_hBitmap = static_cast<HBITMAP>(LoadImage(nullptr, _fullPath.c_str(), IMAGE_BITMAP, 0, 0,
	                                           LR_CREATEDIBSECTION | LR_LOADFROMFILE | LR_DEFAULTSIZE));
	if (nullptr == m_hBitmap)
	{
		assert(false && "LOAD IMAGE FAILED");
		return false;
	}

	m_hBitmapDC = CreateCompatibleDC(CCore::GetInstance().GetMainDC());

	const HBITMAP hPrevBitmap = static_cast<HBITMAP>(SelectObject(m_hBitmapDC, m_hBitmap));
	DeleteObject(hPrevBitmap);

	if (false == GetObject(m_hBitmap, sizeof(BITMAP), &m_bitmapInfo))
	{
		assert(false && "LOAD BITMAP DATA FAILED");
		return false;
	}

	return true;
}
