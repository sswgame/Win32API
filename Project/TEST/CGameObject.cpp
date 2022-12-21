#include "pch.h"
#include "CGameObject.h"

#include "CComponent.h"
#include "CTransform.h"


CGameObject::CGameObject()
	:
	m_isDead{false},
	m_arrComponents{}
{
	AddComponent<CTransform>();
}

CGameObject::CGameObject(const CGameObject& _other)
	:
	m_name{_other.m_name},
	m_isDead{false},
	m_arrComponents{}
{
	for (int i = 0; i < _other.m_arrComponents.size(); ++i)
	{
		if (_other.m_arrComponents[i])
		{
			m_arrComponents[i] = _other.m_arrComponents[i]->Clone();
			m_arrComponents[i]->SetOwner(this);
		}
	}
}

CGameObject::~CGameObject()
{
	for (const CComponent* pComponent : m_arrComponents)
	{
		delete pComponent;
		pComponent = nullptr;
	}
}

void CGameObject::Update()
{
	
	for (CComponent* pComponent : m_arrComponents)
	{
		if (pComponent)
		{
			pComponent->Update();
		}
	}
	UpdateSpecific();
}

void CGameObject::Render(HDC _dc)
{
	for (CComponent* pComponent : m_arrComponents)
	{
		if (pComponent)
		{
			pComponent->Render(_dc);
		}
	}
	RenderSpecific(_dc);
}


