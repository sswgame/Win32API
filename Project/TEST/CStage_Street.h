#pragma once
#include "CStage.h"

class CStage_Street : public CStage
{
public:
	CStage_Street();
	virtual ~CStage_Street();
public:
	void Init() override;
	void Enter() override;
	void Exit() override;
private:
	
};
