#pragma once
#include "CState.h"

class CPlayerRun : public CState
{
public:
	CPlayerRun();
	virtual ~CPlayerRun();
public:
	void    Update() override;
	CState* Clone() override { return new CPlayerRun{ *this }; };
	void    Enter() override;
	void    Exit() override;

private:
};
