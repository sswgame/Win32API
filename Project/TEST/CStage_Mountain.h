#pragma once
#include "CStage.h"

class CStage_Mountain : public CStage
{
public:
	CStage_Mountain();
	virtual ~CStage_Mountain();
public:
	void Init() override;
	void Enter() override;
	void Exit() override;
private:
	std::vector<CGameObject*> m_vecObjects;
};
