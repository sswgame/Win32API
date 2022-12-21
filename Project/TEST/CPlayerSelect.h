#pragma once
#include "CState.h"

class CPlayerSelect : public CState
{
public:
	CPlayerSelect();
	virtual ~CPlayerSelect();
public:
	void    Update() override;
	CState* Clone() override { return new CPlayerSelect{*this}; };
	void    Enter() override;
	void    Exit() override;

private:
};
