#include "pch.h"
#include "CTeleport.h"

#include "CCollider.h"
#include "CCore.h"
#include "CStage.h"
#include "CStageManager.h"

CTeleport::CTeleport()
{
	AddComponent<CCollider>();
}

CTeleport::~CTeleport() = default;

void CTeleport::OnCollisionEnter(CGameObject* _pOther)
{
	if (_pOther->GetType() == OBJECT_TYPE::PLAYER)
	{
		CCore::GetInstance().GetStageManager().GetCurrent().SetPlayerLocation(m_target);
	}
}
