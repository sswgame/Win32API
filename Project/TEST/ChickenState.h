#pragma once
#include "CState.h"

class ChickenState : public CState
{
public:
	ChickenState();
	virtual ~ChickenState();

public:
	void    Update() override;
	CState* Clone() override { return new ChickenState{*this}; };
	void    Enter() override;
	void    Exit() override;

private:
	float m_elapsedTime;
	float m_maxTime;
	float m_changeDirTime;
	bool  m_changeDirection;
	bool  m_toWalk;
	DIR   m_dir;
};
