#include "pch.h"
#include "CRayCast.h"

#include "CCollider.h"
#include "CCore.h"
#include "CEventManager.h"
#include "CFSM.h"
#include "CMovement.h"
#include "CObstacle.h"

#include "CPlayer.h"
#include "CResourceManager.h"
#include "CSpriteComponent.h"
#include "Name.h"

CRayCast::CRayCast()
	:
	m_distance{0.f},
	m_pTarget{nullptr},
	m_pOwner{nullptr}
{
	AddComponent<CCollider>();
}

CRayCast::~CRayCast() = default;

void CRayCast::UpdateSpecific()
{
	const CCollider* pCollider = m_pOwner->GetComponent<CCollider>();
	const Vec2       pos       = pCollider->GetFinalPos();
	const Vec2       scale     = pCollider->GetScale();

	GetComponent<CTransform>()->SetPos(pos);

	const Vec2 dir    = m_pOwner->GetComponent<CMovement>()->GetDir();
	const Vec2 offset = dir * (scale);

	if (dir == DIR::LEFT || dir == DIR::RIGHT)
	{
		GetComponent<CCollider>()->SetScale(Vec2{scale.x, 1.f});
	}

	else if (dir == DIR::UP || dir == DIR::DOWN)
	{
		GetComponent<CCollider>()->SetScale(Vec2{1.f, scale.y});
	}

	GetComponent<CCollider>()->SetOffsetPos(offset);
}


void CRayCast::OnCollisionEnter(CGameObject* _pOther)
{
	if (false == IsInterSect(_pOther))
	{
		GetComponent<CCollider>()->Enable(false);
		return;
	}
	CFSM* pFsm = GetOwner()->GetComponent<CFSM>();
	m_pTarget  = _pOther;
	GetComponent<CCollider>()->Enable(false);

	if (_pOther->GetType() == OBJECT_TYPE::ITEM)
	{
		CPlayer* pPlayer = static_cast<CPlayer*>(m_pOwner);
		pPlayer->SetCheckRay(true);
		CCore::GetInstance().GetEventManager().Send<ChangeStateEvent>(ChangeStateEvent{pFsm, L"CONFIRM"});
	}
	else if (_pOther->GetType() == OBJECT_TYPE::ANIMAL)
	{
		CPlayer* pPlayer = static_cast<CPlayer*>(m_pOwner);
		pPlayer->SetCheckRay(true);
		CCore::GetInstance().GetEventManager().Send<ChangeStateEvent>(ChangeStateEvent{pFsm, L"CONFIRM"});
	}
	else if (_pOther->GetType() == OBJECT_TYPE::INVOKER)
	{
		if (_pOther->GetName() == L"SLEEP")
		{
			CCore::GetInstance().GetEventManager().Send<ChangeStateEvent>(ChangeStateEvent{pFsm, L"UI"});
		}
		else if (_pOther->GetName() == L"WOOD")
		{
			CPlayer* pPlayer = static_cast<CPlayer*>(m_pOwner);
			if (pPlayer->GetWoodCount() > 0)
			{
				pPlayer->SetWood(pPlayer->GetWoodCount() - 1);
				CObstacle* pObstacle = new CObstacle{};
				pObstacle->SetName(L"WALL");
				pObstacle->SetHP(2);
				pObstacle->SetObstacleType(OBSTACLE_TYPE::WALL);
				pObstacle->GetComponent<CTransform>()->SetPos(_pOther->GetComponent<CTransform>()->GetPos());
				CSpriteComponent* pSpriteComponent = pObstacle->GetComponent<CSpriteComponent>();
				SpriteInfo*       pSpriteInfo      = CCore::GetInstance().GetResourceManager().
				                                                          GetSpriteInfo(
					                                                          EXTRA::TYPES[EXTRA::WOOD_CARRYING]);
				pSpriteComponent->SetTexture(EXTRA::TYPES[EXTRA::WOOD_CARRYING], LR"(texture\extra.bmp)");
				pSpriteComponent->SetInfo(pSpriteInfo);
				pObstacle->GetComponent<CTransform>()->SetScale(pSpriteInfo->scale);
				pPlayer->SetCarryingObject(pObstacle);
				
				CCore::GetInstance().GetEventManager().Send<CreationEvent>(CreationEvent{
					pObstacle, OBJECT_TYPE::OBSTACLE
				});
				m_pTarget = pObstacle;
				pPlayer->SetCheckRay(true);
				CCore::GetInstance().GetEventManager().Send<ChangeStateEvent>(ChangeStateEvent{pFsm, L"CONFIRM"});
			}
		}
	}
}

bool CRayCast::IsInterSect(CGameObject* _pOther)
{
	const Vec2 pos         = GetComponent<CCollider>()->GetFinalPos();
	const Vec2 scale       = GetComponent<CCollider>()->GetScale();
	const Vec2 targetPos   = _pOther->GetComponent<CCollider>()->GetFinalPos();
	const Vec2 targetScale = _pOther->GetComponent<CCollider>()->GetScale();

	const Vec2 dir  = m_pOwner->GetComponent<CMovement>()->GetDir();
	const RECT rect = m_pOwner->GetComponent<CMovement>()->GetIntersect(pos, scale, targetPos, targetScale);
	float      diff = 0;
	if (dir == DIR::LEFT || dir == DIR::RIGHT)
	{
		diff       = static_cast<float>(rect.right - rect.left);
		m_distance = targetScale.x * .5f;
	}
	else if (dir == DIR::DOWN || dir == DIR::UP)
	{
		diff       = static_cast<float>(rect.bottom - rect.top);
		m_distance = targetScale.y * .5f;
	}
	if (diff != 0 && diff >= m_distance)
	{
		return true;
	}
	return false;
}


