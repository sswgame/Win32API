#pragma once
#include "CState.h"

class CowWalkState : public CState
{
public:
	CowWalkState();
	virtual ~CowWalkState();
public:
	void    Update() override;
	CState* Clone() override { return new CowWalkState{*this}; };
	void    Enter() override;
	void    Exit() override;

private:
	float m_elapsedTime;
	float m_maxTime;
};
