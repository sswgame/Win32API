#pragma once
#include "CState.h"

class CCarryIdleState : public CState
{
public:
	CCarryIdleState();
	virtual ~CCarryIdleState();

public:
	void    Update() override;
	CState* Clone() override { return new CCarryIdleState{*this}; };
	void    Enter() override;
	void    Exit() override;

private:
	void Walk(const Vec2& _dir);
	void Run();
	void Throw();
};
