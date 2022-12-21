#include "pch.h"
#include "CPlayerUse.h"

#include "CAnimator.h"
#include "CColliderManager.h"
#include "CCore.h"
#include "CItem.h"
#include "CMovement.h"
#include "CPlayer.h"
#include "Name.h"

CPlayerUse::CPlayerUse()
	:
	CState{L"USE"},
	m_exhausted{ExhaustedLevel::FULL},
	m_isUsed{false} {}

CPlayerUse::~CPlayerUse() = default;

void CPlayerUse::Update()
{
	CPlayer&  owner = static_cast<CPlayer&>(GetOwner());
	const int maxHp = owner.GetMaxHP();
	const int hp    = owner.GetHP();

	if (hp == maxHp)
	{
		m_exhausted = ExhaustedLevel::FULL;
	}

	if (hp <= 0)
	{
		m_exhausted = ExhaustedLevel::ZERO;
		GetOwner().GetComponent<CAnimator>()->Play(PLAYER::ANIMATIONS[PLAYER::EXHAUSTED_LEVEL_3], false);
	}

	if (GetOwner().GetComponent<CAnimator>()->GetCurrent().IsFinished())
	{
		if (hp > 0 && false == m_isUsed)
		{
			m_isUsed = true;
			owner.Use();
		}

		if (hp <= static_cast<int>(maxHp * 0.5f) && m_exhausted == ExhaustedLevel::FULL)
		{
			m_exhausted = ExhaustedLevel::ONE_FOURTH;
			GetOwner().GetComponent<CAnimator>()->Play(PLAYER::ANIMATIONS[PLAYER::EXHAUSTED_LEVEL_0], false);
		}
		else if (hp <= static_cast<int>(maxHp * 0.25f) && m_exhausted == ExhaustedLevel::ONE_FOURTH)
		{
			m_exhausted = ExhaustedLevel::HALF;
			GetOwner().GetComponent<CAnimator>()->Play(PLAYER::ANIMATIONS[PLAYER::EXHAUSTED_LEVEL_1], false);
		}
		else if (hp <= static_cast<int>(maxHp * 0.125f) && m_exhausted == ExhaustedLevel::HALF)
		{
			m_exhausted = ExhaustedLevel::QUARTER;
			GetOwner().GetComponent<CAnimator>()->Play(PLAYER::ANIMATIONS[PLAYER::EXHAUSTED_LEVEL_2], false);
		}
		else
		{
			ChangeState(L"IDLE");
		}
	}
}

void CPlayerUse::Enter()
{
	CItem* pItem = static_cast<CPlayer&>(GetOwner()).GetCurrentItem();
	if (nullptr == pItem)
	{
		ChangeState(L"IDLE");
		return;
	}
	const ITEM_CATEGORY type = static_cast<ITEM_CATEGORY>(HIWORD(pItem->GetInfo().id.type));
	const UINT          kind = LOWORD(pItem->GetInfo().id.kind);
	if (type == ITEM_CATEGORY::TOOL)
	{
		const Vec2 dir = GetOwner().GetComponent<CMovement>()->GetDir();
		switch (kind)
		{
		case TOOL::NORMAL_AXE:
			GetOwner().GetComponent<CAnimator>()->PlayGroup(
				dir, PLAYER::GROUPS::ANIMATIONS[PLAYER::GROUPS::NORMAL_AXE_0], false);
			break;
		case TOOL::SPECIAL_AXE:
			GetOwner().GetComponent<CAnimator>()->PlayGroup(
				dir, PLAYER::GROUPS::ANIMATIONS[PLAYER::GROUPS::SPECIAL_AXE_0], false);
			break;
		case TOOL::NORMAL_HAMMER:
			GetOwner().GetComponent<CAnimator>()->PlayGroup(
				dir, PLAYER::GROUPS::ANIMATIONS[PLAYER::GROUPS::NORMAL_HAMMER_0], false);
			break;
		case TOOL::SPECIAL_HAMMER:
			GetOwner().GetComponent<CAnimator>()->PlayGroup(
				dir, PLAYER::GROUPS::ANIMATIONS[PLAYER::GROUPS::SPECIAL_HAMMER_0], false);
			break;
		case TOOL::NORMAL_HOE:
			GetOwner().GetComponent<CAnimator>()->PlayGroup(
				dir, PLAYER::GROUPS::ANIMATIONS[PLAYER::GROUPS::NORMAL_HOE_0], false);
			break;
		case TOOL::SPECIAL_HOE:
			GetOwner().GetComponent<CAnimator>()->PlayGroup(
				dir, PLAYER::GROUPS::ANIMATIONS[PLAYER::GROUPS::SPECIAL_HOE_0], false);
			break;
		case TOOL::NORMAL_SICKLE:
			GetOwner().GetComponent<CAnimator>()->PlayGroup(
				dir, PLAYER::GROUPS::ANIMATIONS[PLAYER::GROUPS::NORMAL_SICKLE_0], false);
			break;
		case TOOL::SPECIAL_SICKLE:
			GetOwner().GetComponent<CAnimator>()->Play(
				PLAYER::ANIMATIONS[PLAYER::SPECIAL_SICKLE_FRONT_0], false);
			break;
		case TOOL::NORMAL_WATER:
			GetOwner().GetComponent<CAnimator>()->PlayGroup(
				dir, PLAYER::GROUPS::ANIMATIONS[PLAYER::GROUPS::NORMAL_WATER_0], false);
			break;
		case TOOL::SPECIAL_WATER:
			GetOwner().GetComponent<CAnimator>()->Play(
				PLAYER::ANIMATIONS[PLAYER::SPECIAL_WATER_FRONT_0], false);
			break;
		case TOOL::BRUSH:
			GetOwner().GetComponent<CAnimator>()->PlayGroup(
				dir, PLAYER::GROUPS::ANIMATIONS[PLAYER::GROUPS::BRUSH_0], false);
			break;
		case TOOL::MILKING_TOOL:
			GetOwner().GetComponent<CAnimator>()->PlayGroup(
				dir, PLAYER::GROUPS::ANIMATIONS[PLAYER::GROUPS::MILKING_0], false);
			break;
		//TODO::PAINT
		case TOOL::SEED_CORN:
		case TOOL::SEED_GRASS:
		case TOOL::SEED_POTATO:
		case TOOL::SEED_TOMATO:
		case TOOL::SEED_TURNIP:
			GetOwner().GetComponent<CAnimator>()->Play(
				PLAYER::ANIMATIONS[PLAYER::SEED_FRONT_0], false);
			break;
		case TOOL::BELL:
			GetOwner().GetComponent<CAnimator>()->PlayGroup(
				dir, PLAYER::GROUPS::ANIMATIONS[PLAYER::GROUPS::BELL_0], false);
			break;
		case TOOL::SPECIAL_MEDICINE:
		case TOOL::MEDICINE:
			GetOwner().GetComponent<CAnimator>()->PlayGroup(
				dir, PLAYER::GROUPS::ANIMATIONS[PLAYER::GROUPS::SELECT_0], false);
			break;
		case TOOL::CHICKEN_FOOD:
		case TOOL::COW_FOOD:
			GetOwner().GetComponent<CAnimator>()->PlayGroup(
				dir, PLAYER::GROUPS::ANIMATIONS[PLAYER::GROUPS::FEED_0], false);
		default:
			break;
		}
	}
	GetOwner().GetComponent<CAnimator>()->GetCurrent().Reset();
}

void CPlayerUse::Exit()
{
	GetOwner().GetComponent<CAnimator>()->GetCurrent().Reset();

	m_isUsed = false;
}
