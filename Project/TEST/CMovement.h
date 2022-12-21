#pragma once
#include "CComponent.h"

class CTile;

class CMovement : public IComponent<CMovement>
{
public:
	CMovement();
	virtual ~CMovement();

public:
	void        Update() override;
	CComponent* Clone() override { return new CMovement{*this}; }

public:
	void   SetDir(const Vec2& _dir) { m_dir = _dir; }
	Vec2   GetDir() const { return m_dir; }
	void   SetSpeed(float _speed) { m_speed = _speed; }
	float  GetSpeed() const { return m_speed; }
	void   SetJump(bool _isJumping) { m_isJumping = _isJumping; }
	bool   IsJumping() const { return m_isJumping; }
	void   CheckCanMove(CGameObject* _pOther);
	CTile* CheckCanJump() const;
	RECT   GetIntersect(const Vec2& _pos, const Vec2& _scale, const Vec2& _targetPos, const Vec2& _targetScale);
private:
	void CheckMoveWithTile();
	void CheckMoveWithCollision(const Vec2& _pos, const Vec2& _targetPos, const Vec2& _intersected);
private:
	Vec2 m_dir;
	bool m_isJumping;
	float m_speed;
};
