#pragma once
#include "CState.h"

class CCarryRunState : public CState
{
public:
	CCarryRunState();
	virtual ~CCarryRunState();
public:
	void    Update() override;
	CState* Clone() override { return new CCarryRunState{*this}; };
	void    Enter() override;
	void    Exit() override;
};
