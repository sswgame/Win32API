#pragma once
#include "CState.h"

class CItem;
class CPlayer;
enum class ANSWER_TYPE;
class CUI;

class CPlayerUIState : public CState
{
public:
	CPlayerUIState();
	virtual ~CPlayerUIState();
public:
	void    Update() override;
	CState* Clone() override { return new CPlayerUIState{*this}; }
	void    Enter() override;
	void    Exit() override;

private:
	void BuyingAnimal(CPlayer* pPlayer, ANSWER_TYPE answer);
	void ShowCannotBuy(CPlayer* pPlayer, CItem* pSelected);
	void BuyCowItems(CPlayer* pPlayer, CItem* pSelected);
	void BuySeedItem(CPlayer* pPlayer, CItem* pSelected, ANSWER_TYPE answer);

	bool ChooseToEat(CItem* pSelected, ANSWER_TYPE answer);
	bool ChooseToBuyTool(CPlayer* pPlayer, CItem* pSelected, ANSWER_TYPE answer);
	bool ChooseToBuySeed(CPlayer* pPlayer, CItem* pSelected);

private:
	CUI*         m_pUI;
	CGameObject* m_pObject;

	std::queue<CUI*> m_queueUI;
	ANSWER_TYPE      m_selected;
	bool             m_madeDecision;
};
