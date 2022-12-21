#include "pch.h"
#include "CColliderManager.h"

#include "CCollider.h"
#include "CCore.h"

#include "CGameObject.h"
#include "CStage.h"
#include "CStageManager.h"

CColliderManager::CColliderManager()
	:
	m_arrCheck{} {}

CColliderManager::~CColliderManager() = default;

void CColliderManager::Update()
{
	for (int i = 0; i < OBJECT_TYPE_COUNT; ++i)
	{
		for (int j = i; j < OBJECT_TYPE_COUNT; ++j)
		{
			if (m_arrCheck[i] & (1 << j))
			{
				CStage& pCurrentStage = CCore::GetInstance().GetStageManager().GetCurrent();
				CompareInGroup(pCurrentStage.GetObjects(static_cast<OBJECT_TYPE>(i)),
				               pCurrentStage.GetObjects(static_cast<OBJECT_TYPE>(j)));
			}
		}
	}
}

void CColliderManager::SetCollisionBetween(OBJECT_TYPE _left, OBJECT_TYPE _right, bool _enabled)
{
	UINT col = static_cast<UINT>(_left);
	UINT row = static_cast<UINT>(_right);

	if (static_cast<UINT>(_left) < static_cast<UINT>(_right))
	{
		col = static_cast<UINT>(_right);
		row = static_cast<UINT>(_left);
	}

	if (_enabled)
	{
		m_arrCheck[row] = (m_arrCheck[row] | (1 << col));
	}
	else
	{
		m_arrCheck[row] = m_arrCheck[row] & ~(1 << col);
	}
}

void CColliderManager::OnCollisionEnter(CCollider* _pLeft, CCollider* _pRight)
{
	_pLeft->GetOwner().OnCollisionEnter(&_pRight->GetOwner());
	_pRight->GetOwner().OnCollisionEnter(&_pLeft->GetOwner());
}

void CColliderManager::OnCollisionStay(CCollider* _pLeft, CCollider* _pRight)
{
	_pLeft->GetOwner().OnCollisionStay(&_pRight->GetOwner());
	_pRight->GetOwner().OnCollisionStay(&_pLeft->GetOwner());
}

void CColliderManager::OnCollisionExit(CCollider* _pLeft, CCollider* _pRight)
{
	_pLeft->GetOwner().OnCollisionExit(&_pRight->GetOwner());
	_pRight->GetOwner().OnCollisionExit(&_pLeft->GetOwner());
}

void CColliderManager::CompareInGroup(const std::vector<CGameObject*>& _vecLeft,
                                      const std::vector<CGameObject*>& _vecRight)
{
	CCollider* pLeft{};
	CCollider* pRight{};

	for (size_t i = 0; i < _vecLeft.size(); ++i)
	{
		pLeft = _vecLeft[i]->GetComponent<CCollider>();
		if (nullptr == pLeft)
		{
			continue;
		}

		for (size_t j = 0; j < _vecRight.size(); ++j)
		{
			pRight = _vecRight[j]->GetComponent<CCollider>();
			if (nullptr == pRight)
			{
				continue;
			}

			COLLIDER_ID colliderID{};
			colliderID.leftID  = pLeft->GetID();
			colliderID.rightID = pRight->GetID();

			std::map<std::uint64_t, bool>::iterator iter = m_mapCollisionInfo.find(colliderID.id);

			if (iter == m_mapCollisionInfo.end())
			{
				m_mapCollisionInfo.insert({colliderID.id, false});
				iter = m_mapCollisionInfo.find(colliderID.id);
			}

			const bool isDead    = (pLeft->GetOwner().IsDead() || pRight->GetOwner().IsDead());

			if (IsCollided(pLeft, pRight))
			{
				if (true == iter->second)
				{
					if (true == isDead)
					{
						OnCollisionExit(pLeft, pRight);
						m_mapCollisionInfo.erase(iter);
					}
					else
					{
						OnCollisionStay(pLeft, pRight);
					}
				}
				else
				{
					if (false == isDead)
					{
						OnCollisionEnter(pLeft, pRight);
						iter->second = true;
					}
				}
			}
			else
			{
				if (true == iter->second)
				{
					OnCollisionExit(pLeft, pRight);
					iter->second = false;
				}
			}
		}
	}
}

bool CColliderManager::IsCollided(const CCollider* _pLeft, const CCollider* _pRight)
{
	const bool isEnabled = (_pLeft->IsEnabled() && _pRight->IsEnabled());
	if (false == isEnabled)
	{
		return false;
	}

	const Vec2 leftPos  = _pLeft->GetFinalPos();
	const Vec2 rightPos = _pRight->GetFinalPos();

	const Vec2 leftScale  = _pLeft->GetScale();
	const Vec2 rightScale = _pRight->GetScale();
	if (_pLeft->GetOwner().GetType() == OBJECT_TYPE::RAYCAST || _pRight->GetOwner().GetType() ==
		OBJECT_TYPE::RAYCAST)
	{
		Vec2 data{};
	}
	float distance{};

	//x축 테스트
	distance = abs(leftPos.x - rightPos.x);
	if (distance > (leftScale.x + rightScale.x) * .5f)
	{
		return false;
	}

	//y축 테스트
	distance = abs(leftPos.y - rightPos.y);
	if (distance > (leftScale.y + rightScale.y) * .5f)
	{
		return false;
	}

	return true;
}
