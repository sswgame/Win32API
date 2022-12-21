#pragma once
#include "CFSM.h"

class CState
{
	friend class CFSM;
public:
	CState(const std::wstring& _name);
	virtual ~CState();

public:
	virtual void    Update() = 0;
	virtual CState* Clone() = 0;

	virtual void Enter() = 0;
	virtual void Exit() = 0;

public:
	void ChangeState(const std::wstring& _state) const;

public:
	CGameObject&        GetOwner() const;
	const std::wstring& GetStateName() const { return m_name; }

private:
	void SetFSM(CFSM* _pOwner) { m_pFSM = _pOwner; }

private:
	const std::wstring m_name;
	CFSM*              m_pFSM;
};
