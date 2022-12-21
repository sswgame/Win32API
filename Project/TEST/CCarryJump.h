#pragma once
#include "CState.h"

class CTile;

class CCarryJump : public CState
{
public:
	CCarryJump();
	virtual ~CCarryJump();
public:
	void    Update() override;
	CState* Clone() override { return new CCarryJump{*this}; };
	void    Enter() override;
	void    Exit() override;

private:
	CTile* m_pDestinationTile;
	Vec2   m_destinationPos;
	Vec2   m_startPos;
	Vec2   m_step;
	Vec2   m_add;
	bool   m_jumpToXAxis;

	float m_delay;
	float m_totalDuration;
};
