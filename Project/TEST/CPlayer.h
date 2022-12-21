#pragma once
#include "CGameObject.h"
union ItemID;
class CRayCast;
class CItem;

class CPlayer : public CGameObject
{
public:
	CPlayer();
	virtual ~CPlayer();

public:
	void UpdateSpecific() override;
	void OnCollisionEnter(CGameObject* _pOther) override;
	void OnCollisionStay(CGameObject* _pOther) override;
public:
	CGameObject* Clone() override { return new CPlayer{*this}; };
	OBJECT_TYPE  GetType() override { return OBJECT_TYPE::PLAYER; }

public:
	void Use();

private:
	void DoSeed(ItemID _id);
	void DoHoe(ItemID _id);
	void DoWater(ItemID _id);
	void DoSickle(ItemID _id);
	void DoHammer(ItemID _id);
	void DoBrush();
	void DoMilking();
	void DoPainting();
	void DoAxe(ItemID _id);
	void DoMedicine(ItemID _id);
	void DoBell();
	void DoFeed(ItemID _id);

public:
	void SetHp(int _hp);
	int  GetHP() const { return m_Hp; }

	void                      SetMaxHP(int _max) { m_maxHp = _max; }
	int                       GetMaxHP() const { return m_maxHp; }
	CRayCast*                 GetRay() const { return m_pRayCast; }
	void                      SetRay(CRayCast* _pRay) { m_pRayCast = _pRay; }
	CItem*                    GetCurrentItem();
	void                      SelectItem();
	void                      SetItem(CItem* _pItem) { m_arrSelected[m_currentIndex] = _pItem; }
	bool                      IsFull() const { return (m_arrSelected[0] && m_arrSelected[1]); }
	std::pair<CItem*, CItem*> GetItems() const { return {m_arrSelected[0], m_arrSelected[1]}; }

	void         SetCarryingObject(CGameObject* _pObject) { m_pCarrying = _pObject; }
	CGameObject* GetCarryingObject() const { return m_pCarrying; }

	void SetGrass(int _count);
	int  GetGrassCount() const { return m_grassCount; }

	void SetWood(int _count);
	int  GetWoodCount() const { return m_woodCount; }

	void SetMoney(int _count) { m_money = _count; }
	int  GetMoney() const { return m_money; }

	void SetInfoEnabled(bool _enabled) { m_show = _enabled; }
	bool IsInfo() const { return m_show; }

	bool IsBuyingAnimal() const { return m_isBuyingAnimal; }
	void SetBuyingAnimal(bool _enabled) { m_isBuyingAnimal = _enabled; }

	bool HasItem(ItemID _id);
	void InsertIntoInventory(CItem* _pItem);
	void ChangeTool(CItem* _pItem);

	std::vector<CItem*>& GetInventory() { return m_vecInventory; }
	CItem*               GetItemFromInventory(ItemID _id);
	void                 DeleteFromInventory(CItem* _pItem);

	void SetSleep(bool _enable) { m_isToSleep = _enable; }
	bool IsGoingToSleep() const { return m_isToSleep; }

	bool IsFarmUpdated() const { return m_isUpdatingFarm; }
	void SetFarmUpdated(bool _enable) { m_isUpdatingFarm = _enable; }
	void UpdateFarm();

	int  GetCowFeedCount() const { return m_cowFeedCount; }
	int  GetChickenFeedCount() const { return m_chickenFeedCount; }
	void SetCowFeedCount(int _count) { m_cowFeedCount = _count; }
	void SetChickenFeedCount(int _count) { m_chickenFeedCount = _count; }

	bool IsCheckedWithRay() const { return m_withRay; }
	bool IsCheckedWithTile() const { return m_withTile; }
	bool IsCheckedWithTileAfter() const { return m_withTileAfter; }

	void SetCheckRay(bool _enable) { m_withRay = _enable; }
	void SetCheckTile(bool _enable) { m_withTile = _enable; }
	void SetCheckTileAfter(bool _enable) { m_withTileAfter = _enable; }
	bool IsBuyingItem() const { return m_isBuyingItem; }
	void SetBuyingItem(bool _enable) { m_isBuyingItem = _enable; }

private:
	void SetAnimations();
	void UseItem();
private:
	int                   m_Hp;
	int                   m_maxHp;
	int                   m_woodCount;
	int                   m_grassCount;
	int                   m_money;
	std::array<CItem*, 2> m_arrSelected;
	std::vector<CItem*>   m_vecInventory;
	int                   m_currentIndex;
	CGameObject*          m_pCarrying;
	CRayCast*             m_pRayCast;
	bool                  m_show;
	bool                  m_isBuyingAnimal;
	bool                  m_isToSleep;
	bool                  m_isUpdatingFarm;

	bool m_withRay;
	bool m_withTile;
	bool m_isBuyingItem;
	bool m_withTileAfter;

	int m_cowFeedCount;
	int m_chickenFeedCount;
};
