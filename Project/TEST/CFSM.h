#pragma once
#include "CComponent.h"

class CState;
class CFSM : public IComponent<CFSM>
{
public:
	CFSM();
	CFSM(const CFSM& _other);
	CFSM&   operator=(const CFSM&) = delete;
	virtual ~CFSM();

public:
	void Update() override;

public:
	void                AddState(CState* _pState);
	CState*             FindState(const std::wstring& _state);
	void                SetState(const std::wstring& _state);
	void                ChangeState(const std::wstring& _state);
	const std::wstring& GetStateName() const;
	CComponent*         Clone() override { return new CFSM{*this}; };
private:

private:
	std::map<std::wstring, CState*> m_mapState;
	CState*                         m_pCurrentState;
};

