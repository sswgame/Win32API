#pragma once
#include "IEvent.h"
#include "CGameObject.h"

class CFSM;
class CCollider;

struct CreationEvent : public CEvent<CreationEvent>
{
	CGameObject* pGameObject;
	OBJECT_TYPE  type;

	CreationEvent(CGameObject* _pGameObject, OBJECT_TYPE _type)
		:
		pGameObject{_pGameObject},
		type{_type} {}
};

struct DeletionEvent : public CEvent<DeletionEvent>
{
	CGameObject* pGameObject;

	DeletionEvent(CGameObject* _pGameObject)
		:
		pGameObject{_pGameObject} {}
};

struct ChangeStageEvent : public CEvent<ChangeStageEvent>
{
	STAGE_TYPE stage;

	ChangeStageEvent(STAGE_TYPE _stage)
		:
		stage{_stage} { }
};

struct ChangeStateEvent : public CEvent<ChangeStateEvent>
{
	CFSM*        pFSM;
	std::wstring state;

	ChangeStateEvent(CFSM* _pFSM, const std::wstring& _state)
		:
		pFSM{_pFSM},
		state{_state} {}
};

struct CollisionEnterEvent : public CEvent<CollisionEnterEvent>
{
	CCollider*     pLeft;
	CCollider*     pRight;

	CollisionEnterEvent(CCollider* _pLeft, CCollider* _pRight)
		:
		pLeft{_pLeft},
		pRight{_pRight} {}
};

struct CollisionPersistEvent : public CEvent<CollisionPersistEvent>
{
	CCollider* pLeft;
	CCollider* pRight;

	CollisionPersistEvent(CCollider* _pLeft, CCollider* _pRight)
		:
		pLeft{_pLeft},
		pRight{_pRight} {}
};

struct CollisionExitEvent : public CEvent<CollisionExitEvent>
{
	CCollider* pLeft;
	CCollider* pRight;

	CollisionExitEvent(CCollider* _pLeft, CCollider* _pRight)
		:
		pLeft{_pLeft},
		pRight{_pRight} {}
};
