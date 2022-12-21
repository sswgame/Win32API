#pragma once
#include "CComponent.h"

class CController : public IComponent<CController>
{
public:
	CController();
	virtual ~CController();
};
