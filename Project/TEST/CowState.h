#pragma once
#include "CState.h"

class CowState : public CState
{
public:
	CowState();
	virtual ~CowState();

public:
	void    Update() override;
	CState* Clone() override { return new CowState{*this}; };
	void    Enter() override;
	void    Exit() override;

private:
	void ChangeIdleAnimation();
private:
	float m_elapsedTime;
	float m_maxTime;
	float m_changeDirTime;
	bool  m_changeDirection;
	bool  m_toWalk;
	DIR   m_dir;
};
