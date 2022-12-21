#pragma once
#include "CGameObject.h"

class CTeleport : public CGameObject
{
public:
	CTeleport();
	virtual ~CTeleport();

	void OnCollisionEnter(CGameObject* _pOther) override;
public:
	OBJECT_TYPE  GetType() override { return OBJECT_TYPE::TELEPORTER; };
	CGameObject* Clone() override { return new CTeleport{*this}; }
	void         SetTarget(Vec2 _pos) { m_target = _pos; }
	Vec2         GetTarget() const { return m_target; }
private:
	Vec2 m_target;
};
