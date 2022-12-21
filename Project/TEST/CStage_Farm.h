#pragma once
#include "CStage.h"

class CStage_Farm : public CStage
{
public:
	CStage_Farm();
	virtual ~CStage_Farm();

public:
	void Init() override;
	void Enter() override;
	void Exit() override;

private:
	
};
