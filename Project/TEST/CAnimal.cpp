#include "pch.h"
#include "CAnimal.h"

#include "CAnimator.h"
#include "CCollider.h"
#include "CCore.h"
#include "CEventManager.h"
#include "CFSM.h"
#include "ChickenState.h"
#include "CKeyManager.h"
#include "CMovement.h"
#include "CowHappyState.h"
#include "CowState.h"
#include "CowWalkState.h"
#include "CPlayer.h"
#include "CSpriteComponent.h"
#include "CStage.h"
#include "CStageManager.h"
#include "CTileManager.h"
#include "Name.h"

namespace
{
	constexpr int CHILD_TO_TEEN    = 3;
	constexpr int TEEN_TO_ADULT    = 3;
	constexpr int PREGNANT_DAY     = 3;
	constexpr int CHICK_TO_CHICKEN = 1;
}

CAnimal::CAnimal(ANIMAL_TYPE _type)
	:
	m_info{_type},
	m_isBeingCarried{false},
	m_isFinished{false},
	m_isFullyGrown{false},
	m_pPlayer{nullptr}
{
	CMovement* pMovement = AddComponent<CMovement>();
	pMovement->SetDir({0, 1});

	CAnimator* pAnimator = AddComponent<CAnimator>();
	const int  tileSize  = CCore::GetInstance().GetStageManager().GetCurrent().GetMapInfo().tileWidth;

	GetComponent<CTransform>()->SetScale(Vec2{tileSize, tileSize} * .8f);

	CCollider* pCollider = AddComponent<CCollider>();
	pCollider->SetOffsetPos(Vec2{0.f, -GetComponent<CTransform>()->GetScale().y * .5f});
	pCollider->SetScale(GetComponent<CTransform>()->GetScale());

	CFSM* pFSM = AddComponent<CFSM>();

	switch (_type)
	{
	case ANIMAL_TYPE::COW:
		m_info.maxGrowLevel = GROW_LEVEL::PREGNANT;
		m_info.maxGrowDay   = TEEN_TO_ADULT;
		m_info.currentLevel = 1;
		m_info.maxHp        = 5;
		m_info.hp           = m_info.maxHp;
		pAnimator->LoadAnimation(LR"(texture\Cow.bmp)", LR"(animation\cow.xml)");
		pAnimator->Play(COW::ANIMATIONS[COW::IDLE_CHILD_FRONT_0], false);
		pFSM->AddState(new CowState{});
		pFSM->AddState(new CowHappyState{});
		pFSM->AddState(new CowWalkState{});
		pFSM->SetState(L"COW");
		break;
	case ANIMAL_TYPE::CHICKEN:
		{
			m_info.maxGrowLevel = GROW_LEVEL::TEEN;
			m_info.maxGrowDay   = CHICK_TO_CHICKEN;
			m_info.maxHp        = 1;
			m_info.hp           = m_info.maxHp;
			pAnimator->LoadAnimation(LR"(texture\Chicken.bmp)", LR"(animation\chicken.xml)");
			pAnimator->Play(CHICKEN::ANIMATIONS[CHICKEN::CHICK_WALK_FRONT_0], false);
			pFSM->AddState(new ChickenState{});
			pFSM->SetState(L"CHICKEN");
		}

		break;
	default: ;
	}

	SetAnimation(_type);
}

CAnimal::~CAnimal() = default;

void CAnimal::UpdateSpecific()
{
	if (KEY_TAP(KEY::N9))
	{
		FinishDay();
		if (m_info.type == ANIMAL_TYPE::COW)
		{
			++m_info.like;
		}
	}
	if (KEY_TAP(KEY::N8))
	{
		if (m_info.type == ANIMAL_TYPE::COW
			&& (m_info.currentLevel == GROW_LEVEL::TEEN || m_info.currentLevel == GROW_LEVEL::ADULT))
		{
			--m_info.hp;
		}
	}
	if (m_info.hp <= 0)
	{
		CCore::GetInstance().GetEventManager().Send<DeletionEvent>(DeletionEvent{this});
	}
	if (m_info.type == ANIMAL_TYPE::COW)
	{
		if (m_info.hp < m_info.maxHp / 2
			&& (m_info.currentLevel == GROW_LEVEL::TEEN || m_info.currentLevel == GROW_LEVEL::ADULT))
		{
			m_info.isSick = true;
		}
		if (m_info.currentLevel == m_info.maxGrowLevel && m_info.growDay == m_info.maxGrowDay)
		{
			--m_info.currentLevel;
			m_info.growDay    = 0;
			m_info.maxGrowDay = PREGNANT_DAY;
			m_info.isPregnant = false;

			CAnimal* pAnimal = new CAnimal{ANIMAL_TYPE::COW};
			pAnimal->GetComponent<CTransform>()->SetPos(GetComponent<CTransform>()->GetPos());
			AnimalInfo info = pAnimal->GetInfo();
			--info.currentLevel;
			info.maxGrowDay = CHILD_TO_TEEN;
			info.like       = m_info.like;
			pAnimal->SetInfo(info);
			CCore::GetInstance().GetEventManager().Send<CreationEvent>(CreationEvent{pAnimal, OBJECT_TYPE::ANIMAL});
		}
	}

	if (m_isFinished)
	{
		m_isFinished = false;
		if (m_info.type == ANIMAL_TYPE::CHICKEN)
		{
			CStage& chicken = CCore::GetInstance().GetStageManager().GetStage(STAGE_TYPE::CHICKEN);
		}
	}
}

void CAnimal::OnCollisionEnter(CGameObject* _pOther)
{
	if (_pOther->GetType() == OBJECT_TYPE::ITEM)
	{
		CItem* pItem = static_cast<CItem*>(_pOther);
		if (m_info.type == ANIMAL_TYPE::COW && pItem->GetInfo().id.kind == TOOL::MILKING_TOOL)
		{
			if (m_info.currentLevel != GROW_LEVEL::ADULT || false == m_info.canMilk)
			{
				return;
			}
			m_info.canMilk = false;

			CItem* pNewItem = new CItem{};
			ItemID id       = ItemID{EXTRA::SMALL_MILK, ITEM_CATEGORY::SELL};
			if (m_info.like >= 10)
			{
				const int count = rand() % 5;

				id = count == 1 ? ItemID{EXTRA::BIG_MILK, ITEM_CATEGORY::SELL} : id;
			}

			pNewItem->SetInfo(BASIC_INFO::MAP_DATA[id]);
			SpriteInfo* info = CCore::GetInstance().GetResourceManager().GetSpriteInfo(
				EXTRA::TYPES[id.kind]);
			pNewItem->GetComponent<CSpriteComponent>()->SetInfo(info);
			pNewItem->GetComponent<CSpriteComponent>()->SetTexture(
				EXTRA::TYPES[id.kind], LR"(texture\extra.bmp)");
			pNewItem->GetComponent<CSpriteComponent>()->SetEnabled(true);
			pNewItem->GetComponent<CTransform>()->SetPos(pItem->GetComponent<CCollider>()->GetFinalPos());
			pNewItem->GetComponent<CTransform>()->SetScale(info->scale);

			CCore::GetInstance().GetEventManager().Send<CreationEvent>(CreationEvent{pNewItem, OBJECT_TYPE::ITEM});

			CPlayer* pPlayer = static_cast<CPlayer*>(CCore::GetInstance().GetStageManager().GetCurrent().GetObjects(
				OBJECT_TYPE::PLAYER)[0]);
			pPlayer->SetCarryingObject(pNewItem);
			pPlayer->GetComponent<CFSM>()->ChangeState(L"CARRY_IDLE");
		}
		else if (m_info.type == ANIMAL_TYPE::COW && pItem->GetInfo().id.kind == TOOL::BRUSH)
		{
			if (false == m_info.isBrushed)
			{
				++m_info.like;
				GetComponent<CFSM>()->ChangeState(L"HAPPY");
			}
			m_info.isBrushed = true;
		}
	}
}

void CAnimal::FinishDay()
{
	++m_info.growDay;

	if (m_info.type == ANIMAL_TYPE::CHICKEN)
	{
		if (m_info.growDay >= m_info.maxGrowDay && false == m_isFullyGrown)
		{
			++m_info.currentLevel;
			m_isFullyGrown = true;
		}

		m_info.canCarry = true;
	}
	else if (m_info.type == ANIMAL_TYPE::COW)
	{
		if (m_info.growDay > m_info.maxGrowDay && false == m_isFullyGrown)
		{
			++m_info.currentLevel;
			if (m_info.currentLevel == GROW_LEVEL::TEEN)
			{
				m_info.growDay    = 0;
				m_info.maxGrowDay = TEEN_TO_ADULT;
			}
			if (m_info.currentLevel == GROW_LEVEL::ADULT)
			{
				m_isFullyGrown = true;
			}
		}
		if (m_info.currentLevel == GROW_LEVEL::ADULT)
		{
			m_info.canMilk = true;
		}
		if (m_info.isPregnant)
		{
			m_info.growDay    = 0;
			m_info.maxGrowDay = PREGNANT_DAY;
			m_info.isPregnant = false;
		}

		m_info.isBrushed = false;
	}

	if (m_info.currentLevel > m_info.maxGrowLevel)
	{
		m_info.currentLevel = m_info.maxGrowLevel;
	}

	m_isFinished = true;
}

void CAnimal::SetAnimation(ANIMAL_TYPE _type)
{
	CAnimator* pAnimator = GetComponent<CAnimator>();
	switch (_type)
	{
	case ANIMAL_TYPE::COW:
		{
			pAnimator->SetAnimGroup(COW::GROUPS::ANIMATIONS[COW::GROUPS::IDLE_0],
			                        {
				                        COW::ANIMATIONS[COW::IDLE_LEFT_0],
				                        COW::ANIMATIONS[COW::IDLE_RIGHT_0],
				                        COW::ANIMATIONS[COW::IDLE_BACK_0],
				                        COW::ANIMATIONS[COW::IDLE_FRONT_0]
			                        });
			pAnimator->SetAnimGroup(COW::GROUPS::ANIMATIONS[COW::GROUPS::IDLE_1],
			                        {
				                        COW::ANIMATIONS[COW::IDLE_LEFT_1],
				                        COW::ANIMATIONS[COW::IDLE_RIGHT_1],
				                        COW::ANIMATIONS[COW::IDLE_BACK_0],
				                        COW::ANIMATIONS[COW::IDLE_FRONT_1]
			                        });
			pAnimator->SetAnimGroup(COW::GROUPS::ANIMATIONS[COW::GROUPS::IDLE_2],
			                        {
				                        COW::ANIMATIONS[COW::IDLE_LEFT_2],
				                        COW::ANIMATIONS[COW::IDLE_RIGHT_2],
				                        COW::ANIMATIONS[COW::IDLE_BACK_0],
				                        COW::ANIMATIONS[COW::IDLE_FRONT_2]
			                        });
			pAnimator->SetAnimGroup(COW::GROUPS::ANIMATIONS[COW::GROUPS::IDLE_FRUSTRATED_0],
			                        {
				                        COW::ANIMATIONS[COW::IDLE_FRUSTRATED_LEFT_0],
				                        COW::ANIMATIONS[COW::IDLE_FRUSTRATED_RIGHT_0],
				                        COW::ANIMATIONS[COW::IDLE_BACK_0],
				                        COW::ANIMATIONS[COW::IDLE_FRUSTRATED_FRONT_0]
			                        });
			pAnimator->SetAnimGroup(COW::GROUPS::ANIMATIONS[COW::GROUPS::IDLE_FRUSTRATED_1],
			                        {
				                        COW::ANIMATIONS[COW::IDLE_FRUSTRATED_LEFT_1],
				                        COW::ANIMATIONS[COW::IDLE_FRUSTRATED_RIGHT_1],
				                        COW::ANIMATIONS[COW::IDLE_BACK_0],
				                        COW::ANIMATIONS[COW::IDLE_FRUSTRATED_FRONT_0]
			                        });
			pAnimator->SetAnimGroup(COW::GROUPS::ANIMATIONS[COW::GROUPS::IDLE_FRUSTRATED_2],
			                        {
				                        COW::ANIMATIONS[COW::IDLE_FRUSTRATED_LEFT_2],
				                        COW::ANIMATIONS[COW::IDLE_FRUSTRATED_RIGHT_2],
				                        COW::ANIMATIONS[COW::IDLE_BACK_0],
				                        COW::ANIMATIONS[COW::IDLE_FRUSTRATED_FRONT_0]
			                        });
			pAnimator->SetAnimGroup(COW::GROUPS::ANIMATIONS[COW::GROUPS::IDLE_FRUSTRATED_3],
			                        {
				                        COW::ANIMATIONS[COW::IDLE_FRUSTRATED_LEFT_3],
				                        COW::ANIMATIONS[COW::IDLE_FRUSTRATED_RIGHT_3],
				                        COW::ANIMATIONS[COW::IDLE_BACK_0],
				                        COW::ANIMATIONS[COW::IDLE_FRUSTRATED_FRONT_0]
			                        });
			pAnimator->SetAnimGroup(COW::GROUPS::ANIMATIONS[COW::GROUPS::IDLE_FRUSTRATED_4],
			                        {
				                        COW::ANIMATIONS[COW::IDLE_FRUSTRATED_LEFT_4],
				                        COW::ANIMATIONS[COW::IDLE_FRUSTRATED_RIGHT_4],
				                        COW::ANIMATIONS[COW::IDLE_BACK_0],
				                        COW::ANIMATIONS[COW::IDLE_FRUSTRATED_FRONT_0]
			                        });
			pAnimator->SetAnimGroup(COW::GROUPS::ANIMATIONS[COW::GROUPS::IDLE_SICK_0],
			                        {
				                        COW::ANIMATIONS[COW::IDLE_SICK_LEFT_0],
				                        COW::ANIMATIONS[COW::IDLE_SICK_RIGHT_0],
				                        COW::ANIMATIONS[COW::IDLE_BACK_0],
				                        COW::ANIMATIONS[COW::IDLE_SICK_FRONT_0],
			                        });
			pAnimator->SetAnimGroup(COW::GROUPS::ANIMATIONS[COW::GROUPS::IDLE_SICK_1],
			                        {
				                        COW::ANIMATIONS[COW::IDLE_SICK_LEFT_1],
				                        COW::ANIMATIONS[COW::IDLE_SICK_RIGHT_1],
				                        COW::ANIMATIONS[COW::IDLE_BACK_0],
				                        COW::ANIMATIONS[COW::IDLE_SICK_FRONT_0],
			                        });
			pAnimator->SetAnimGroup(COW::GROUPS::ANIMATIONS[COW::GROUPS::IDLE_SICK_2],
			                        {
				                        COW::ANIMATIONS[COW::IDLE_SICK_LEFT_2],
				                        COW::ANIMATIONS[COW::IDLE_SICK_RIGHT_2],
				                        COW::ANIMATIONS[COW::IDLE_BACK_0],
				                        COW::ANIMATIONS[COW::IDLE_SICK_FRONT_0],
			                        });
			pAnimator->SetAnimGroup(COW::GROUPS::ANIMATIONS[COW::GROUPS::IDLE_SICK_3],
			                        {
				                        COW::ANIMATIONS[COW::IDLE_SICK_LEFT_3],
				                        COW::ANIMATIONS[COW::IDLE_SICK_RIGHT_3],
				                        COW::ANIMATIONS[COW::IDLE_BACK_0],
				                        COW::ANIMATIONS[COW::IDLE_SICK_FRONT_0],
			                        });
			pAnimator->SetAnimGroup(COW::GROUPS::ANIMATIONS[COW::GROUPS::IDLE_HAPPY_0],
			                        {
				                        COW::ANIMATIONS[COW::IDLE_HAPPY_LEFT_0],
				                        COW::ANIMATIONS[COW::IDLE_HAPPY_RIGHT_0],
				                        COW::ANIMATIONS[COW::IDLE_BACK_0],
				                        COW::ANIMATIONS[COW::IDLE_HAPPY_FRONT_0],
			                        });
			pAnimator->SetAnimGroup(COW::GROUPS::ANIMATIONS[COW::GROUPS::WALK_0],
			                        {
				                        COW::ANIMATIONS[COW::WALK_LEFT_0],
				                        COW::ANIMATIONS[COW::WALK_RIGHT_0],
				                        COW::ANIMATIONS[COW::WALK_BACK_0],
				                        COW::ANIMATIONS[COW::WALK_FRONT_0]
			                        });
			pAnimator->SetAnimGroup(COW::GROUPS::ANIMATIONS[COW::GROUPS::WALK_SICK_0],
			                        {
				                        COW::ANIMATIONS[COW::WALK_SICK_LEFT_0],
				                        COW::ANIMATIONS[COW::WALK_SICK_RIGHT_0],
				                        COW::ANIMATIONS[COW::WALK_BACK_0],
				                        COW::ANIMATIONS[COW::IDLE_SICK_FRONT_0]
			                        });
			pAnimator->SetAnimGroup(COW::GROUPS::ANIMATIONS[COW::GROUPS::WALK_FRUSTRATED_0],
			                        {
				                        COW::ANIMATIONS[COW::WALK_FRUSTRATED_LEFT_0],
				                        COW::ANIMATIONS[COW::WALK_FRUSTRATED_RIGHT_0],
				                        COW::ANIMATIONS[COW::WALK_BACK_0],
				                        COW::ANIMATIONS[COW::WALK_FRUSTRATED_FRONT_0]
			                        });
			pAnimator->SetAnimGroup(COW::GROUPS::ANIMATIONS[COW::GROUPS::IDLE_TEEN_0],
			                        {
				                        COW::ANIMATIONS[COW::IDLE_TEEN_LEFT_1],
				                        COW::ANIMATIONS[COW::IDLE_TEEN_RIGHT_1],
				                        COW::ANIMATIONS[COW::IDLE_TEEN_BACK_0],
				                        COW::ANIMATIONS[COW::IDLE_TEEN_FRONT_0],
			                        });
			pAnimator->SetAnimGroup(COW::GROUPS::ANIMATIONS[COW::GROUPS::IDLE_TEEN_1],
			                        {
				                        COW::ANIMATIONS[COW::IDLE_TEEN_LEFT_1],
				                        COW::ANIMATIONS[COW::IDLE_TEEN_RIGHT_1],
				                        COW::ANIMATIONS[COW::IDLE_TEEN_BACK_0],
				                        COW::ANIMATIONS[COW::IDLE_TEEN_FRONT_0],
			                        });
			pAnimator->SetAnimGroup(COW::GROUPS::ANIMATIONS[COW::GROUPS::IDLE_TEEN_2],
			                        {
				                        COW::ANIMATIONS[COW::IDLE_TEEN_LEFT_2],
				                        COW::ANIMATIONS[COW::IDLE_TEEN_RIGHT_2],
				                        COW::ANIMATIONS[COW::IDLE_TEEN_BACK_0],
				                        COW::ANIMATIONS[COW::IDLE_TEEN_FRONT_0],
			                        });
			pAnimator->SetAnimGroup(COW::GROUPS::ANIMATIONS[COW::GROUPS::IDLE_TEEN_3],
			                        {
				                        COW::ANIMATIONS[COW::IDLE_TEEN_LEFT_3],
				                        COW::ANIMATIONS[COW::IDLE_TEEN_RIGHT_3],
				                        COW::ANIMATIONS[COW::IDLE_TEEN_BACK_0],
				                        COW::ANIMATIONS[COW::IDLE_TEEN_FRONT_0],
			                        });
			pAnimator->SetAnimGroup(COW::GROUPS::ANIMATIONS[COW::GROUPS::IDLE_TEEN_FRUSTRATED_0],
			                        {
				                        COW::ANIMATIONS[COW::IDLE_TEEN_FRUSTRATED_LEFT_0],
				                        COW::ANIMATIONS[COW::IDLE_TEEN_FRUSTRATED_RIGHT_0],
				                        COW::ANIMATIONS[COW::IDLE_TEEN_BACK_0],
				                        COW::ANIMATIONS[COW::IDLE_TEEN_FRUSTRATED_FRONT_0],
			                        });
			pAnimator->SetAnimGroup(COW::GROUPS::ANIMATIONS[COW::GROUPS::IDLE_TEEN_SICK_0],
			                        {
				                        COW::ANIMATIONS[COW::IDLE_TEEN_SICK_LEFT_0],
				                        COW::ANIMATIONS[COW::IDLE_TEEN_SICK_RIGHT_0],
				                        COW::ANIMATIONS[COW::IDLE_TEEN_BACK_0],
				                        COW::ANIMATIONS[COW::IDLE_TEEN_FRUSTRATED_FRONT_0],
			                        });
			pAnimator->SetAnimGroup(COW::GROUPS::ANIMATIONS[COW::GROUPS::IDLE_TEEN_HAPPY_0],
			                        {
				                        COW::ANIMATIONS[COW::IDLE_TEEN_HAPPY_LEFT_0],
				                        COW::ANIMATIONS[COW::IDLE_TEEN_HAPPY_RIGHT_0],
				                        COW::ANIMATIONS[COW::IDLE_TEEN_BACK_0],
				                        COW::ANIMATIONS[COW::IDLE_TEEN_FRONT_0],
			                        });
			pAnimator->SetAnimGroup(COW::GROUPS::ANIMATIONS[COW::GROUPS::WALK_TEEN_0],
			                        {
				                        COW::ANIMATIONS[COW::RUN_TEEN_LEFT_0],
				                        COW::ANIMATIONS[COW::RUN_TEEN_RIGHT_0],
				                        COW::ANIMATIONS[COW::WALK_TEEN_BACK_0],
				                        COW::ANIMATIONS[COW::WALK_TEEN_FRONT_0],
			                        });
			pAnimator->SetAnimGroup(COW::GROUPS::ANIMATIONS[COW::GROUPS::WALK_TEEN_FRUSTRATED_0],
			                        {
				                        COW::ANIMATIONS[COW::WALK_TEEN_FRUSTRATED_LEFT_0],
				                        COW::ANIMATIONS[COW::WALK_TEEN_FRUSTRATED_RIGHT_0],
				                        COW::ANIMATIONS[COW::WALK_TEEN_BACK_0],
				                        COW::ANIMATIONS[COW::WALK_TEEN_FRUSTRATED_FRONT_0],
			                        });
			pAnimator->SetAnimGroup(COW::GROUPS::ANIMATIONS[COW::GROUPS::IDLE_CHILD_0],
			                        {
				                        COW::ANIMATIONS[COW::IDLE_CHILD_LEFT_0],
				                        COW::ANIMATIONS[COW::IDLE_CHILD_RIGHT_0],
				                        COW::ANIMATIONS[COW::IDLE_CHILD_BACK_0],
				                        COW::ANIMATIONS[COW::IDLE_CHILD_FRONT_0]
			                        });
			pAnimator->SetAnimGroup(COW::GROUPS::ANIMATIONS[COW::GROUPS::IDLE_CHILD_1],
			                        {
				                        COW::ANIMATIONS[COW::IDLE_CHILD_LEFT_1],
				                        COW::ANIMATIONS[COW::IDLE_CHILD_RIGHT_1],
				                        COW::ANIMATIONS[COW::IDLE_CHILD_BACK_0],
				                        COW::ANIMATIONS[COW::IDLE_CHILD_FRONT_0]
			                        });
			pAnimator->SetAnimGroup(COW::GROUPS::ANIMATIONS[COW::GROUPS::IDLE_CHILD_2],
			                        {
				                        COW::ANIMATIONS[COW::IDLE_CHILD_LEFT_2],
				                        COW::ANIMATIONS[COW::IDLE_CHILD_RIGHT_2],
				                        COW::ANIMATIONS[COW::IDLE_CHILD_BACK_0],
				                        COW::ANIMATIONS[COW::IDLE_CHILD_FRONT_0]
			                        });
			pAnimator->SetAnimGroup(COW::GROUPS::ANIMATIONS[COW::GROUPS::IDLE_CHILD_FRUSTRATED_0],
			                        {
				                        COW::ANIMATIONS[COW::IDLE_CHILD_FRUSTRATED_LEFT_0],
				                        COW::ANIMATIONS[COW::IDLE_CHILD_FRUSTRATED_RIGHT_0],
				                        COW::ANIMATIONS[COW::IDLE_CHILD_BACK_0],
				                        COW::ANIMATIONS[COW::IDLE_CHILD_FRUSTRATED_FRONT_0]
			                        });
			pAnimator->SetAnimGroup(COW::GROUPS::ANIMATIONS[COW::GROUPS::WALK_CHILD_0],
			                        {
				                        COW::ANIMATIONS[COW::WALK_CHILD_LEFT_0],
				                        COW::ANIMATIONS[COW::WALK_CHILD_RIGHT_0],
				                        COW::ANIMATIONS[COW::WALK_CHILD_BACK_0],
				                        COW::ANIMATIONS[COW::WALK_CHILD_FRONT_0]
			                        });
			pAnimator->SetAnimGroup(COW::GROUPS::ANIMATIONS[COW::GROUPS::WALK_CHILD_1],
			                        {
				                        COW::ANIMATIONS[COW::RUN_CHILD_LEFT_0],
				                        COW::ANIMATIONS[COW::RUN_CHILD_RIGHT_0],
				                        COW::ANIMATIONS[COW::WALK_CHILD_BACK_0],
				                        COW::ANIMATIONS[COW::WALK_CHILD_FRONT_0]
			                        });
			pAnimator->SetAnimGroup(COW::GROUPS::ANIMATIONS[COW::GROUPS::WALK_CHILD_FRUSTRATED_0],
			                        {
				                        COW::ANIMATIONS[COW::WALK_CHILD_FRUSTRATED_LEFT_0],
				                        COW::ANIMATIONS[COW::WALK_CHILD_FRUSTRATED_RIGHT_0],
				                        COW::ANIMATIONS[COW::WALK_CHILD_BACK_0],
				                        COW::ANIMATIONS[COW::WALK_CHILD_FRUSTRATED_FRONT_0]
			                        });
			pAnimator->SetAnimGroup(COW::GROUPS::ANIMATIONS[COW::GROUPS::IDLE_PREGNANT_0],
			                        {
				                        COW::ANIMATIONS[COW::IDLE_PREGNANT_LEFT_0],
				                        COW::ANIMATIONS[COW::IDLE_PREGNANT_RIGHT_0],
				                        COW::ANIMATIONS[COW::IDLE_PREGNANT_BACK_0],
				                        COW::ANIMATIONS[COW::IDLE_PREGNANT_FRONT_0]
			                        });
			pAnimator->SetAnimGroup(COW::GROUPS::ANIMATIONS[COW::GROUPS::IDLE_PREGNANT_1],
			                        {
				                        COW::ANIMATIONS[COW::IDLE_PREGNANT_LEFT_1],
				                        COW::ANIMATIONS[COW::IDLE_PREGNANT_RIGHT_1],
				                        COW::ANIMATIONS[COW::IDLE_PREGNANT_BACK_0],
				                        COW::ANIMATIONS[COW::IDLE_PREGNANT_FRONT_0]
			                        });
			pAnimator->SetAnimGroup(COW::GROUPS::ANIMATIONS[COW::GROUPS::IDLE_PREGNANT_2],
			                        {
				                        COW::ANIMATIONS[COW::IDLE_PREGNANT_LEFT_0],
				                        COW::ANIMATIONS[COW::IDLE_PREGNANT_RIGHT_0],
				                        COW::ANIMATIONS[COW::IDLE_PREGNANT_BACK_0],
				                        COW::ANIMATIONS[COW::IDLE_PREGNANT_HAPPY_FRONT_0]
			                        });
			pAnimator->SetAnimGroup(COW::GROUPS::ANIMATIONS[COW::GROUPS::WALK_PREGNANT_0],
			                        {
				                        COW::ANIMATIONS[COW::WALK_PREGNANT_LEFT_0],
				                        COW::ANIMATIONS[COW::WALK_PREGNANT_RIGHT_0],
				                        COW::ANIMATIONS[COW::WALK_PREGNANT_BACK_0],
				                        COW::ANIMATIONS[COW::WALK_PREGNANT_FRONT_0]
			                        });
		}
		break;
	case ANIMAL_TYPE::CHICKEN:
		{
			pAnimator->SetAnimGroup(CHICKEN::GROUPS::ANIMATIONS[CHICKEN::GROUPS::WALK_0],
			                        {
				                        CHICKEN::ANIMATIONS[CHICKEN::WALK_LEFT_0],
				                        CHICKEN::ANIMATIONS[CHICKEN::WALK_RIGHT_0],
				                        CHICKEN::ANIMATIONS[CHICKEN::WALK_BACK_0],
				                        CHICKEN::ANIMATIONS[CHICKEN::WALK_FRONT_0]
			                        });
			pAnimator->SetAnimGroup(CHICKEN::GROUPS::ANIMATIONS[CHICKEN::GROUPS::CHICK_WALK_0],
			                        {
				                        CHICKEN::ANIMATIONS[CHICKEN::CHICK_WALK_LEFT_0],
				                        CHICKEN::ANIMATIONS[CHICKEN::CHICK_WALK_RIGHT_0],
				                        CHICKEN::ANIMATIONS[CHICKEN::CHICK_WALK_BACK_0],
				                        CHICKEN::ANIMATIONS[CHICKEN::CHICK_WALK_FRONT_0]
			                        });
		}
		break;
	case ANIMAL_TYPE::HORSE: break;
	default: ;
	}
}
