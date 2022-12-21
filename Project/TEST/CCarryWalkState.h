#pragma once
#include "CState.h"

class CCarryWalkState : public CState
{
public:
	CCarryWalkState();
	virtual ~CCarryWalkState();

public:
	void    Update() override;
	CState* Clone() override { return new CCarryWalkState{*this}; };
	void    Enter() override;
	void    Exit() override;
};
