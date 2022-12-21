#include "pch.h"
#include "CInteract.h"

#include "CKeyManager.h"
#include "CMovement.h"
#include "CCore.h"

CInteract::CInteract()
	:
	IComponent<CInteract>{COMPONENT_TYPE::INTERACT},
	m_distance{60.f}
{}

CInteract::~CInteract() = default;

void CInteract::Update()
{
	CMovement* pMovement = GetOwner().GetComponent<CMovement>();
	if (nullptr == pMovement)
	{
		return;
	}

	if (KEY_TAP(KEY::SPACE))
	{
		Vec2 dir      = pMovement->GetDir();
		Vec2 startPos = GetOwner().GetComponent<CTransform>()->GetPos();

		Vec2 endPos = startPos + (dir * m_distance);


	}
}
