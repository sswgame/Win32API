#pragma once
#include "CController.h"

class CItem;

class CPlayerController : public CController
{
public:
	CPlayerController();
	virtual ~CPlayerController();
public:
	void        Update() override;
	CComponent* Clone() override { return new CPlayerController{*this}; };

private:
	std::vector<CItem*>   m_vecInventory;
	std::array<CItem*, 2> m_arrSelected;
};
