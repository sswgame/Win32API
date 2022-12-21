#pragma once
#include "CGameObject.h"

class CNpc : public CGameObject
{
public:
	CNpc();
	virtual ~CNpc();

public:
	OBJECT_TYPE  GetType() override { return OBJECT_TYPE::NPC; };
	CGameObject* Clone() override { return new CNpc{*this}; };
};
