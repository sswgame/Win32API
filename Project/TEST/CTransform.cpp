#include "pch.h"
#include "CTransform.h"

CTransform::CTransform()
	:
	IComponent<CTransform>{COMPONENT_TYPE::TRANSFORM} {}

CTransform::CTransform(const CTransform& _other)
	:
	IComponent<CTransform>{_other},
	m_pos{_other.m_pos},
	m_scale{_other.m_scale} { }

CTransform::~CTransform() = default;
