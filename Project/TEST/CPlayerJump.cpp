#include "pch.h"
#include "CPlayerJump.h"

#include "CAnimator.h"
#include "CKeyManager.h"
#include "CMovement.h"
#include "Name.h"
#include "CCore.h"
#include "CResourceManager.h"
#include "CSound.h"
#include "CTile.h"
#include "CTimer.h"

CPlayerJump::CPlayerJump()
	:
	CState{L"JUMP"},
	m_pDestinationTile{nullptr},
	m_jumpToXAxis{false},
	m_delay{0},
	m_totalDuration{0} {}

CPlayerJump::~CPlayerJump() = default;

void CPlayerJump::Update()
{
	m_delay += DS;

	if (m_delay < m_totalDuration)
	{
		if (m_jumpToXAxis)
		{
			if (m_delay < m_totalDuration / 2)
			{
				m_add.y = -40.f;
			}
			else
			{
				m_add.y = 40.f;
			}
		}
	}

	m_startPos += (m_step + m_add) * DS;
	GetOwner().GetComponent<CTransform>()->SetPos(m_startPos);

	if (GetOwner().GetComponent<CAnimator>()->GetCurrent().IsFinished())
	{
		if (KEY_PRESSED(KEY::L_CTRL))
		{
			ChangeState(L"RUN");
		}
		ChangeState(L"IDLE");
		m_delay = 0;
	}
}

void CPlayerJump::Enter()
{
	CSound* pSound = CCore::GetInstance().GetResourceManager().FindSound(L"jump");
	pSound->SetPosition(10);
	pSound->Play(false);

	GetOwner().GetComponent<CMovement>()->SetJump(true);
	m_pDestinationTile = GetOwner().GetComponent<CMovement>()->CheckCanJump();

	m_startPos       = GetOwner().GetComponent<CTransform>()->GetPos();
	const Vec2 scale = m_pDestinationTile->GetComponent<CTransform>()->GetScale();
	//m_destinationPos = m_pDestinationTile->GetComponent<CTransform>()->GetPos() + Vec2{scale.x * .5f, scale.y};
	m_destinationPos = m_startPos + Vec2{60.f, 60.f} * GetOwner().GetComponent<CMovement>()->GetDir();
	m_totalDuration  = GetOwner().GetComponent<CAnimator>()->GetCurrent().GetTotalDuration();
	m_step           = (m_destinationPos - m_startPos);

	const Vec2 dir = GetOwner().GetComponent<CMovement>()->GetDir();
	if (dir == DIR::LEFT || dir == DIR::RIGHT)
	{
		m_jumpToXAxis = true;
		if (dir == DIR::LEFT)
		{
			m_step.x -= 40.f;
		}
		else
		{
			m_step.x += 20.f;
		}
	}
	GetOwner().GetComponent<CAnimator>()->PlayGroup(dir, PLAYER::GROUPS::ANIMATIONS[PLAYER::GROUPS::JUMP_0], false);
}

void CPlayerJump::Exit()
{
	m_add              = Vec2{};
	m_pDestinationTile = nullptr;
	m_jumpToXAxis      = false;
	GetOwner().GetComponent<CMovement>()->SetJump(false);
	GetOwner().GetComponent<CAnimator>()->GetCurrent().Reset();
}