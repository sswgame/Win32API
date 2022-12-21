#pragma once
#include "CState.h"

class CowHappyState : public CState
{
public:
	CowHappyState();
	virtual ~CowHappyState();
public:
	void    Update() override;
	CState* Clone() override { return new CowHappyState{*this}; };
	void    Enter() override;
	void    Exit() override;
};
