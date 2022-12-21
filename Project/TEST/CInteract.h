#pragma once
#include "CComponent.h"

class CInteract : public IComponent<CInteract>
{
public:
	CInteract();
	virtual ~CInteract();

	void Update() override;
public:
	CComponent* Clone() override { return new CInteract{*this}; };

private:
	float m_distance;
};
