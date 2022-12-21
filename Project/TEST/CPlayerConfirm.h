#pragma once
#include "CState.h"


class CItem;

class CPlayerConfirm : public CState
{
public:
	CPlayerConfirm();
	virtual ~CPlayerConfirm();
public:
	void    Update() override;
	CState* Clone() override { return new CPlayerConfirm{*this}; };
	void    Enter() override;
	void    Exit() override;

private:
	Vec2 m_step;
	Vec2 m_startPos;

	CItem* m_pItem;
};
