#pragma once
#include "CGameObject.h"

class CTile;

enum class ITEM_CATEGORY : UINT
{
	TOOL,
	SELL,
	DEFAULT,
};

union ItemID
{
	struct
	{
		UINT          kind;
		ITEM_CATEGORY type;
	};

	std::uint64_t id;

	bool operator <(const ItemID& _other) const { return id < _other.id; }
	bool operator ==(const ItemID& _other) const { return id == _other.id; }
	bool operator !=(const ItemID& _other) const { return !(*this == _other); }
};

struct ItemInfo
{
	ItemID id{};
	int    price{};
	int    hp{};
	int    remainCount{};
	int    currentCount{};
	bool   canSell{};
	bool   canEat{};
	bool   consumable{};
	Vec2   location{};
};

class CItem : public CGameObject
{
public:
	CItem();
	virtual ~CItem();

public:
	void         UpdateSpecific() override;
	OBJECT_TYPE  GetType() override { return OBJECT_TYPE::ITEM; };
	CGameObject* Clone() override { return new CItem{*this}; };

public:
	void OnCollisionEnter(CGameObject* _pOther) override;
	void OnCollisionExit(CGameObject* _pOther) override;

public:
	void      SetDraw(bool _isEnabled) { m_enabled = _isEnabled; }
	void      SetInfo(const ItemInfo& _info) { m_pInfo = _info; }
	ItemInfo& GetInfo() { return m_pInfo; }
	void      SetOwner(CGameObject* _pObject) { m_pOwner = _pObject; }
	void      SetThrow(bool _isThrown) { m_isThrow = _isThrown; }

	void   SetTile(CTile* _pTile) { m_pTile = _pTile; }
	CTile* GetTile() const { return m_pTile; }
private:
	ItemInfo     m_pInfo;
	bool         m_enabled;
	CGameObject* m_pOwner;
	CTile*       m_pTile;
	bool         m_isThrow;
	float        m_elapsedTime;
	float        m_delay;
	float        m_remain;
};
