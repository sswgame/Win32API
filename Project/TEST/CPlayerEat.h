#pragma once
#include "CState.h"

class CItem;

class CPlayerEat : public CState
{
public:
	CPlayerEat();
	virtual ~CPlayerEat();
public:
	void    Update() override;
	CState* Clone() override { return new CPlayerEat{*this}; };
	void    Enter() override;
	void    Exit() override;

private:
	float  m_delay;
	CItem* m_pItem;
};
