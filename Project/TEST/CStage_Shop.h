#pragma once
#include "CStage.h"

class CStage_Shop : public CStage
{
public:
	CStage_Shop();
	virtual ~CStage_Shop();
public:
	void Init() override;
	void Enter() override;
	void Exit() override;
private:
	void RemoveItem();
	
};
