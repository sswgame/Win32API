#include "pch.h"
#include "CFSM.h"

#include "CState.h"

CFSM::CFSM()
	:
	IComponent<CFSM>{COMPONENT_TYPE::FSM},
	m_pCurrentState{nullptr}
{}

CFSM::CFSM(const CFSM& _other)
	:
	IComponent<CFSM>{COMPONENT_TYPE::FSM},
	m_pCurrentState{_other.m_pCurrentState}
{
	for (const std::pair<const std::wstring, CState*>& data : _other.m_mapState)
	{
		m_mapState.insert({ data.first,data.second->Clone() });
	}
}

CFSM::~CFSM()
{
	std::map<std::wstring, CState*>::iterator iter = m_mapState.begin();
	while (iter != m_mapState.end())
	{
		delete iter->second;
		iter->second = nullptr;
		iter = m_mapState.erase(iter);
	}
	m_mapState.clear();
}

void CFSM::Update()
{
	assert(m_pCurrentState);
	m_pCurrentState->Update();
}

void CFSM::AddState(CState* _pState)
{
	m_mapState.insert({_pState->GetStateName(), _pState});
	_pState->SetFSM(this);
}

CState* CFSM::FindState(const std::wstring& _state)
{
	std::map<std::wstring, CState*>::iterator iter = m_mapState.find(_state);
	if (iter != m_mapState.end())
	{
		return iter->second;
	}
	return nullptr;
}

void CFSM::SetState(const std::wstring& _state)
{
	m_pCurrentState = FindState(_state);
	assert(m_pCurrentState);
}

void CFSM::ChangeState(const std::wstring& _state)
{
	m_pCurrentState->Exit();
	SetState(_state);
	m_pCurrentState->Enter();
}

inline const std::wstring& CFSM::GetStateName() const { return m_pCurrentState->GetStateName(); }


