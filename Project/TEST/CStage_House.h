#pragma once
#include "CStage.h"

class CStage_House : public CStage
{
public:
	CStage_House();
	virtual ~CStage_House();

public:
	void Init() override;
	void Enter() override;
	void Exit() override;

private:
};
