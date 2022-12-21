#pragma once
#include "CState.h"

class CPlayerWhistle : public CState
{
public:
	CPlayerWhistle();
	virtual ~CPlayerWhistle();
public:
	void    Update() override;
	CState* Clone() override { return new CPlayerWhistle{*this}; };
	void    Enter() override;
	void    Exit() override;
};
