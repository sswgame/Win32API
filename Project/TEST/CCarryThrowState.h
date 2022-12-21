#pragma once
#include "CState.h"

class CTile;
class CItem;

class CCarryThrowState : public CState
{
public:
	CCarryThrowState();
	virtual ~CCarryThrowState();
public:
	void    Update() override;
	CState* Clone() override { return new CCarryThrowState{*this}; };
	void    Enter() override;
	void    Exit() override;

private:
	bool CheckMoneyTile();

private:
	CGameObject* m_pObject;
	CTile*       m_pTile;
};
