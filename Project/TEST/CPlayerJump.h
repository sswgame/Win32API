#pragma once
#include "CState.h"

class CTile;

class CPlayerJump : public CState
{
public:
	CPlayerJump();
	virtual ~CPlayerJump();
public:
	void    Update() override;
	CState* Clone() override { return new CPlayerJump{*this}; }
	void    Enter() override;
	void    Exit() override;

private:
	CTile* m_pDestinationTile;
	Vec2   m_destinationPos;
	Vec2   m_startPos;
	Vec2   m_step;
	bool   m_jumpToXAxis;
	Vec2   m_add;
	float  m_delay;

	float m_totalDuration;
};
