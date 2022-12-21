#pragma once
#include "CComponent.h"

class CTransform : public IComponent<CTransform>
{
public:
	CTransform();
	CTransform(const CTransform& _other);
	CTransform& operator=(const CTransform&) = delete;
	virtual     ~CTransform();

public:
	void SetPos(const Vec2 _pos) { m_pos = _pos; }
	void SetScale(const Vec2 _scale) { m_scale = _scale; }

	Vec2 GetPos() const { return m_pos; }
	Vec2 GetScale() const { return m_scale; }

private:
	CComponent* Clone() override { return new CTransform{ *this }; };

private:
	Vec2 m_pos;
	Vec2 m_scale;
};

