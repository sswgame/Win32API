#pragma once
#include "CGameObject.h"

enum class OBSTACLE_TYPE
{
	GRASS,
	BIG_ROCK,
	SMALL_ROCK,
	WOOD,
	WALL,

	END
};

class CObstacle : public CGameObject
{
public:
	CObstacle();
	virtual ~CObstacle();

public:
	void         UpdateSpecific() override;
	OBJECT_TYPE  GetType() override { return OBJECT_TYPE::OBSTACLE; };
	CGameObject* Clone() override { return new CObstacle{*this}; };

	void          SetObstacleType(OBSTACLE_TYPE _type) { m_type = _type; }
	OBSTACLE_TYPE GetObstacleType() const { return m_type; }
	int           GetHP() const { return m_hp; }
	void          SetHP(int _hp) { m_hp = _hp; }
private:
	OBSTACLE_TYPE m_type;
	int           m_hp;
	float         m_elapsedTime;
	float         m_delay;
};
