#pragma once
#include "CResource.h"

class CTexture : public CResource
{
public:
	CTexture();
	virtual ~CTexture();

public:
	void Create(UINT _width, UINT _height);

public:
	HDC  GetTextureDC() const { return m_hBitmapDC; }
	Vec2 GetSize() const { return Vec2{m_bitmapInfo.bmWidth, m_bitmapInfo.bmHeight}; }

private:
	bool Load(const std::wstring& _fullPath) override;

private:
	HBITMAP m_hBitmap;
	HDC     m_hBitmapDC;
	BITMAP  m_bitmapInfo;
};
