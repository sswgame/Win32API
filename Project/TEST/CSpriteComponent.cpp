#include "pch.h"
#include "CSpriteComponent.h"

#include "CCamera.h"
#include "CCore.h"
#include "CGameObject.h"
#include "CResourceManager.h"
#include "CTexture.h"

CSpriteComponent::CSpriteComponent()
	:
	IComponent<CSpriteComponent>{COMPONENT_TYPE::SPRITE},
	m_pTexture{nullptr},
	m_pInfo{nullptr},
	m_isEnabled{true},
	m_isFixed{false} {}

CSpriteComponent::~CSpriteComponent() = default;

void CSpriteComponent::Render(HDC _dc)
{
	if (false == m_isEnabled)
	{
		return;
	}

	Vec2 pos = CCore::GetInstance().GetCamera().GetRenderPos(GetOwner().GetComponent<CTransform>()->GetPos());
	if (m_isFixed)
	{
		pos = (GetOwner().GetComponent<CTransform>()->GetPos());
	}
	const UINT width  = static_cast<UINT>(m_pInfo->scale.x);
	const UINT height = static_cast<UINT>(m_pInfo->scale.y);

	GdiTransparentBlt(_dc,
	                  static_cast<int>(pos.x),
	                  static_cast<int>(pos.y),
	                  width,
	                  height,
	                  m_pTexture->GetTextureDC(),
	                  static_cast<int>(m_pInfo->pos.x),
	                  static_cast<int>(m_pInfo->pos.y),
	                  width, height,
	                  m_pInfo->backgroundColor);
}

void CSpriteComponent::SetTexture(const std::wstring& _name, const std::wstring& _path, bool _hasXMLInfo)
{
	if (false == _hasXMLInfo)
	{
		m_pInfo = CCore::GetInstance().GetResourceManager().CreateSpriteInfo(_name, _path);
	}
	if (m_pInfo)
	{
		m_pTexture = CCore::GetInstance().GetResourceManager().LoadTexture(_name, _path);
	}
	else
	{
		m_pInfo    = CCore::GetInstance().GetResourceManager().GetSpriteInfo(_name);
		m_pTexture = CCore::GetInstance().GetResourceManager().LoadTexture(m_pInfo->textureName, _path);
	}
	assert(m_pTexture);
}
