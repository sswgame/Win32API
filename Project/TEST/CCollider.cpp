#include "pch.h"
#include "CCollider.h"

#include "CCamera.h"
#include "CCore.h"
#include "CGameObject.h"
#include "CKeyManager.h"
#include "CTransform.h"

CCollider::CCollider()
	:
	IComponent<CCollider>{COMPONENT_TYPE::COLLIDER},
	m_isEnabled{true},
	m_show{false} {}

CCollider::CCollider(const CCollider& _other)
	:
	IComponent<CCollider>{COMPONENT_TYPE::COLLIDER},
	m_offsetPos{_other.m_offsetPos},
	m_scale{_other.m_scale},
	m_finalPos{_other.m_finalPos},
	m_isEnabled{_other.m_isEnabled},
	m_show{_other.m_show} {}

CCollider::~CCollider() = default;

void CCollider::Update()
{
	m_finalPos = GetOwner().GetComponent<CTransform>()->GetPos() + m_offsetPos;

	if (KEY_TAP(KEY::N0))
	{
		m_show = !m_show;
	}
}

void CCollider::Render(HDC _dc)
{
	/*if (false == m_isEnabled)
	{
		return;
	}*/

	if (false == m_show)
	{
		return;
	}

	const HPEN hPen = CCore::GetInstance().GetGreenPen();

	const HBRUSH hBrush = static_cast<HBRUSH>(GetStockObject(HOLLOW_BRUSH));
	const HPEN   hPrevPen = static_cast<HPEN>(SelectObject(_dc, hPen));
	const HBRUSH hPrevBrush = static_cast<HBRUSH>(SelectObject(_dc, hBrush));

	const Vec2 renderPos = CCore::GetInstance().GetCamera().GetRenderPos(m_finalPos);

	Rectangle(_dc,
	          static_cast<int>(renderPos.x - m_scale.x * .5f),
	          static_cast<int>(renderPos.y - m_scale.y * .5f),
	          static_cast<int>(renderPos.x + m_scale.x * .5f),
	          static_cast<int>(renderPos.y + m_scale.y * .5f));
	SelectObject(_dc, hPrevPen);
	SelectObject(_dc, hPrevBrush);
}
