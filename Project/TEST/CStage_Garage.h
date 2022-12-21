#pragma once
#include "CStage.h"

class CItem;

class CStage_Garage : public CStage
{
public:
	CStage_Garage();
	virtual ~CStage_Garage();
public:
	void Init() override;
	void Enter() override;
	void Exit() override;
private:
};
