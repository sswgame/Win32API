#pragma once
#include "CState.h"

class CPlayerWalk : public CState
{
public:
	CPlayerWalk();
	virtual ~CPlayerWalk();
public:
	void    Update() override;
	CState* Clone() override { return new CPlayerWalk{ *this }; }
	void    Enter() override;
	void    Exit() override;

private:
};

