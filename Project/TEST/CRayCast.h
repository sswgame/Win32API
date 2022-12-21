#pragma once
#include "CGameObject.h"

class CRayCast : public CGameObject
{
public:
	CRayCast();
	virtual ~CRayCast();

public:
	void         UpdateSpecific() override;
	OBJECT_TYPE  GetType() override { return OBJECT_TYPE::RAYCAST; };
	CGameObject* Clone() override { return new CRayCast{*this}; }

	void OnCollisionEnter(CGameObject* _pOther) override;
public:
	CGameObject* GetOwner() const { return m_pOwner; }
	void         SetOwner(CGameObject* _pObject) { m_pOwner = _pObject; }

	CGameObject* GetTarget() const { return m_pTarget; }
	void         SetTarget(CGameObject* _pTarget) { m_pTarget = nullptr; }

	bool IsInterSect(CGameObject* _pOther);
private:
	float        m_distance;
	CGameObject* m_pTarget;
	CGameObject* m_pOwner;
};
