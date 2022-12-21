#pragma once
#include "CGameObject.h"

class CPlayer;

namespace GROW_LEVEL
{
	enum
	{
		CHILD,
		TEEN,
		ADULT,
		PREGNANT
	};
}

enum class ANIMAL_TYPE
{
	COW,
	CHICKEN,
	HORSE,
};
struct AnimalInfo
{
	ANIMAL_TYPE type;
	int         maxGrowLevel; // 성체가 되는데 필요한 요일 수
	int         currentLevel;
	int         like; // 호감도
	int         growDay;
	int         maxGrowDay;
	int         hp;
	int         maxHp;
	bool        canMilk;
	bool        canCarry;
	bool        isPregnant;
	bool        isBrushed;
	bool        isSick;
};

class CAnimal : public CGameObject
{
public:
	CAnimal(ANIMAL_TYPE _type);
	virtual ~CAnimal();

public:
	void         UpdateSpecific() override;
	CGameObject* Clone() override { return new CAnimal{*this}; };
	OBJECT_TYPE  GetType() override { return OBJECT_TYPE::ANIMAL; };

public:
	const AnimalInfo& GetInfo() const { return m_info; }
	void              SetInfo(const AnimalInfo& _info) { m_info = _info; }
	void              SetCarry(bool _enabled) { m_isBeingCarried = _enabled; }
	bool              IsCarried() const { return m_isBeingCarried; }
	void              SetOwner(CPlayer* _pPlayer) { m_pPlayer = _pPlayer; }

	void OnCollisionEnter(CGameObject* _pOther) override;
	void FinishDay();
	bool IsFullyGrown() const { return m_isFullyGrown; }
private:
	void SetAnimation(ANIMAL_TYPE _type);
private:
	AnimalInfo m_info;
	bool       m_isBeingCarried;
	bool       m_canCarry;
	bool       m_isFinished;
	bool       m_isFullyGrown;

	CPlayer*   m_pPlayer;
};
