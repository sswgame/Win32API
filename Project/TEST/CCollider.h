#pragma once
#include "CComponent.h"

class CCollider : public IComponent<CCollider>
{
public:
	CCollider();
	CCollider(const CCollider& _other);
	CCollider& operator=(const CCollider&) = delete;
	virtual    ~CCollider();

public:
	void Update() override;
	void Render(HDC _dc) override;

public:
	void SetOffsetPos(const Vec2 _offsetPos) { m_offsetPos = _offsetPos; }
	void SetScale(const Vec2 _scale) { m_scale = _scale; }

	Vec2 GetOffsetPos() const { return m_offsetPos; }
	Vec2 GetScale() const { return m_scale; }
	Vec2 GetFinalPos() const { return m_finalPos; }

	void Enable(bool _enable) { m_isEnabled = _enable; }
	bool IsEnabled() const { return m_isEnabled; }

private:
	CComponent* Clone() override { return new CCollider{*this}; }

private:
	Vec2 m_offsetPos;
	Vec2 m_scale;
	Vec2 m_finalPos;
	bool m_isEnabled;
	bool m_show;
};
