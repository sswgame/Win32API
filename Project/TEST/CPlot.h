#pragma once
#include "CGameObject.h"
#include "CItem.h"

class CTile;

class CPlot : public CGameObject
{
public:
	CPlot();
	virtual ~CPlot();

public:
	void         UpdateSpecific() override;
	OBJECT_TYPE  GetType() override { return OBJECT_TYPE::PLOT; }
	CGameObject* Clone() override { return new CPlot{*this}; };

public:
	void Reset();
public:
	void   SetSeed(ItemID _id);
	ItemID GetSeedItemId() const { return m_itemId; }
	void   SetTile(CTile* _pTile) { m_pTile = _pTile; }
	CTile* GetTile() const { return m_pTile; }
	bool   IsSeeded() const { return m_isSeeded; }
	void   SetIsSeeded(bool _seed) { m_isSeeded = _seed; }

	bool                IsFullyGrowed() const { return m_isFullyGrowed; }
	const std::wstring& GetCurrentState();
	ItemInfo            GetGrowedItemInfo() const;

	void FinishDay();

	void SetWatered(bool _watered) { m_isWatered = _watered; }
	bool IsWatered() const { return m_isWatered; }
private:
	//seed
	int                       m_needDayToGrow;
	int                       m_currentGrow;
	int                       m_changeStep;
	std::vector<std::wstring> m_vecTypes;
	CTile*                    m_pTile;
	ItemID                    m_itemId;
	bool                      m_isSeeded;
	bool                      m_isFinishedDay;
	bool                      m_isFullyGrowed;

	bool m_isWatered;
	
};
