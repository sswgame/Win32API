#pragma once
#include "CStage.h"

class CStage_Cow : public CStage
{
public:
	CStage_Cow();
	virtual ~CStage_Cow();
public:
	void Init() override;
	void Enter() override;
	void Exit() override;
private:
	
};
