#pragma once
#include "CState.h"

class CItem;

enum class ExhaustedLevel
{
	FULL,
	ONE_FOURTH,
	HALF,
	QUARTER,
	ZERO,
};
class CPlayerUse : public CState
{
public:
	CPlayerUse();
	virtual ~CPlayerUse();

public:
	void    Update() override;
	CState* Clone() override { return new CPlayerUse{*this}; };
	void    Enter() override;
	void    Exit() override;

private:
	ExhaustedLevel m_exhausted;
	bool           m_isUsed;
};
