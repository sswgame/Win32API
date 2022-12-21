#pragma once
#include "CStage.h"

class CStage_Chicken : public CStage
{
public:
	CStage_Chicken();
	virtual ~CStage_Chicken();
public:
	void Init() override;
	void Enter() override;
	void Exit() override;
private:
	
};
