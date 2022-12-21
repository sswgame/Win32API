#pragma once
#include "CGameObject.h"

class CInvoker : public CGameObject
{
public:
	CInvoker();
	virtual ~CInvoker();

public:
	OBJECT_TYPE  GetType() override { return OBJECT_TYPE::INVOKER; };
	CGameObject* Clone() override { return new CInvoker{*this}; };
};
