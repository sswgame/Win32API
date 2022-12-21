#include "pch.h"
#include "CAnimation.h"

#include "CAnimator.h"
#include "CCamera.h"
#include "CCore.h"
#include "CTexture.h"
#include "CTimer.h"

CAnimation::CAnimation()
	:
	m_pAnimator{nullptr},
	m_atlasInfo{},
	m_currentIndex{0},
	m_elapsedTime{0},
	m_isFinished{false} {}

CAnimation::~CAnimation() = default;

void CAnimation::Update()
{
	if (true == m_isFinished)
	{
		return;
	}

	m_elapsedTime += DS;

	if (m_elapsedTime > m_vecFrames[m_currentIndex].duration)
	{
		m_elapsedTime = m_elapsedTime - m_vecFrames[m_currentIndex].duration;
		++m_currentIndex;

		if (m_currentIndex >= m_vecFrames.size())
		{
			m_currentIndex -= 1;
			m_isFinished = true;
		}
	}
}

void CAnimation::Render(HDC _dc)
{
	const Vec2 renderPos = CCore::GetInstance().GetCamera().GetRenderPos(
		m_pAnimator->GetOwner().GetComponent<CTransform>()->GetPos());

	if (false == m_vecFrames[m_currentIndex].isFlipped)
	{
		GdiTransparentBlt(_dc,
		                  static_cast<int>(renderPos.x - m_vecFrames[m_currentIndex].centerPos.x),
		                  static_cast<int>(renderPos.y - m_vecFrames[m_currentIndex].centerPos.y),
		                  static_cast<int>(m_vecFrames[m_currentIndex].size.x),
		                  static_cast<int>(m_vecFrames[m_currentIndex].size.y),
		                  m_atlasInfo.pAtlasTexture->GetTextureDC(),
		                  static_cast<int>(m_vecFrames[m_currentIndex].leftTop.x),
		                  static_cast<int>(m_vecFrames[m_currentIndex].leftTop.y),
		                  static_cast<int>(m_vecFrames[m_currentIndex].size.x),
		                  static_cast<int>(m_vecFrames[m_currentIndex].size.y),
		                  m_atlasInfo.backGroundColor);
	}
	else
	{
		const HDC     tempDC     = CreateCompatibleDC(m_atlasInfo.pAtlasTexture->GetTextureDC());
		const HBITMAP tempBitmap = CreateCompatibleBitmap(m_atlasInfo.pAtlasTexture->GetTextureDC(),
		                                                  m_atlasInfo.width, m_atlasInfo.height);
		const HGDIOBJ old = SelectObject(tempDC, tempBitmap);
		DeleteObject(old);

		StretchBlt(tempDC,
		           static_cast<int>(m_vecFrames[m_currentIndex].size.x - 1),
			0,
		           -static_cast<int>(m_vecFrames[m_currentIndex].size.x),
		           static_cast<int>(m_vecFrames[m_currentIndex].size.y),
			m_atlasInfo.pAtlasTexture->GetTextureDC(),
			static_cast<int>(m_vecFrames[m_currentIndex].leftTop.x),
			static_cast<int>(m_vecFrames[m_currentIndex].leftTop.y),
			static_cast<int>(m_vecFrames[m_currentIndex].size.x),
			static_cast<int>(m_vecFrames[m_currentIndex].size.y),
			SRCCOPY);

		GdiTransparentBlt(_dc,
		                  static_cast<int>(renderPos.x - m_vecFrames[m_currentIndex].centerPos.x),
		                  static_cast<int>(renderPos.y - m_vecFrames[m_currentIndex].centerPos.y),
		                  static_cast<int>(m_vecFrames[m_currentIndex].size.x),
		                  static_cast<int>(m_vecFrames[m_currentIndex].size.y),
		                  tempDC,
		                  0,
		                  0,
		                  static_cast<int>(m_vecFrames[m_currentIndex].size.x),
		                  static_cast<int>(m_vecFrames[m_currentIndex].size.y),
		                  m_atlasInfo.backGroundColor);

		DeleteDC(tempDC);
		DeleteObject(tempBitmap);
	}
}

void CAnimation::Reset()
{
	m_currentIndex = 0;
	m_elapsedTime  = 0.f;
	m_isFinished   = false;
}

float CAnimation::GetTotalDuration() const
{
	float sum = 0;
	for (int i = 0; i < m_vecFrames.size(); ++i)
	{
		sum += m_vecFrames[i].duration;
	}
	return sum;
}
