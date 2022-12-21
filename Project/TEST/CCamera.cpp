#include "pch.h"
#include "CCamera.h"

#include "CCore.h"
#include "CResourceManager.h"
#include "CTexture.h"
#include "CTimer.h"

CCamera::CCamera()
	:
	m_pVeilTexture{nullptr},
	m_isFinished{false} {}

CCamera::~CCamera() = default;

void CCamera::Init()
{
	const Vec2 resolution = CCore::GetInstance().GetResolution();
	m_lookAt              = resolution / 2.f;
	m_centerPos           = m_lookAt;

	if (nullptr == m_pVeilTexture)
	{
		m_pVeilTexture = CCore::GetInstance().GetResourceManager().CreateTexture(
			L"Veil", static_cast<UINT>(resolution.x), static_cast<UINT>(resolution.y));
		assert(m_pVeilTexture);
	}
}

void CCamera::Update()
{
	m_diff = m_lookAt - m_centerPos;
}

void CCamera::Render(HDC _dc)
{
	while (true)
	{
		if (m_queueEffect.empty())
		{
			return;
		}

		CameraEffect& effect = m_queueEffect.front();
		effect.elapsedTime += DS;

		if (effect.elapsedTime > effect.effectTime)
		{
			m_isFinished = true;
			m_queueEffect.pop();
		}
		else
		{
			break;
		}
	}

	const CameraEffect& effect = m_queueEffect.front();

	float fAlpha = 0.f;
	if (CAMERA_EFFECT_TYPE::FADE_OUT == effect.effect)
	{
		fAlpha = effect.elapsedTime / effect.effectTime;
	}
	else if (CAMERA_EFFECT_TYPE::FADE_IN == effect.effect)
	{
		fAlpha = (1.f - (effect.elapsedTime / effect.effectTime));
	}

	const UINT width  = static_cast<UINT>(m_pVeilTexture->GetSize().x);
	const UINT height = static_cast<UINT>(m_pVeilTexture->GetSize().y);

	BLENDFUNCTION bf       = {};
	bf.BlendOp             = AC_SRC_OVER;
	bf.BlendFlags          = 0;
	bf.SourceConstantAlpha = static_cast<BYTE>(fAlpha * 255.f);
	bf.AlphaFormat         = 0;

	if (fAlpha <= 0.f)
	{
		return;
	}

	GdiAlphaBlend(_dc
	            , 0, 0
	            , width, height
	            , m_pVeilTexture->GetTextureDC()
	            , 0, 0
	            , width, height
	            , bf);
}

void CCamera::FadeIn(float _effectTime)
{
	m_isFinished = false;
	CameraEffect effect{};
	effect.effect      = CAMERA_EFFECT_TYPE::FADE_IN;
	effect.effectTime  = _effectTime;
	effect.elapsedTime = 0.f;

	m_queueEffect.push(effect);
}

void CCamera::FadeOut(float _effectTime)
{
	m_isFinished = false;
	CameraEffect effect{};
	effect.effect      = CAMERA_EFFECT_TYPE::FADE_OUT;
	effect.effectTime  = _effectTime;
	effect.elapsedTime = 0.f;

	m_queueEffect.push(effect);
}
