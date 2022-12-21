#pragma once
#include "CState.h"

class CRayCast;
class CPlayer;

class CPlayerIdle : public CState
{
public:
	CPlayerIdle();
	virtual ~CPlayerIdle();

public:
	void    Update() override;
	CState* Clone() override { return new CPlayerIdle{*this}; };
	void    Enter() override;
	void    Exit() override;

private:
	void ChangeIdleAnimation();
	void Walk(const Vec2& _dir);
	void Run();
	void Select();
	void Use();
	void Confirm();
	void Info();
	void Whistle();
	bool CheckWithTile();
	bool CheckWitheTileAfter();

private:
	float     m_elapsedTime;
	float     m_maxTime;
	bool      m_changeAnimation;
	
};
