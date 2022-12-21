#include "pch.h"
#include "CState.h"

#include "CCore.h"
#include "CEventManager.h"

CState::CState(const std::wstring& _name)
	:
	m_name{_name},
	m_pFSM{nullptr} {}

CState::~CState() = default;

void CState::ChangeState(const std::wstring& _state) const
{
	CCore::GetInstance().GetEventManager().Send<ChangeStateEvent>(ChangeStateEvent{m_pFSM, _state});
}

CGameObject& CState::GetOwner() const { return m_pFSM->GetOwner(); }
