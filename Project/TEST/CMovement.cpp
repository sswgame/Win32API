#include "pch.h"
#include "CMovement.h"

#include "CCollider.h"
#include "CCore.h"
#include "CStage.h"
#include "CStageManager.h"
#include "CTile.h"
#include "CTimer.h"

CMovement::CMovement()
	:
	IComponent<CMovement>{COMPONENT_TYPE::MOVE},
	m_isJumping{false},
	m_speed{0} {}

CMovement::~CMovement() = default;

void CMovement::Update()
{
	CheckMoveWithTile();
}

void CMovement::CheckCanMove(CGameObject* _pOther)
{
	const Vec2 pos   = GetOwner().GetComponent<CCollider>()->GetFinalPos();
	const Vec2 scale = GetOwner().GetComponent<CCollider>()->GetScale();

	const Vec2 targetPos   = _pOther->GetComponent<CCollider>()->GetFinalPos();
	const Vec2 targetScale = _pOther->GetComponent<CCollider>()->GetScale();

	const RECT intersect = GetIntersect(pos, scale, targetPos, targetScale);

	CheckMoveWithCollision(pos, targetPos, Vec2{intersect.right - intersect.left, intersect.bottom - intersect.top});
}

CTile* CMovement::CheckCanJump() const
{
	if (false == GetOwner().GetComponent<CCollider>()->IsEnabled())
	{
		return nullptr;
	}

	CTile* pCurrentTile = CCore::GetInstance().GetStageManager().GetCurrent().GetCurrentTile(&GetOwner());
	CTile* pNextTile    = pCurrentTile->GetNextTile(m_dir);
	if (nullptr == pNextTile || false == pNextTile->GetTileInfo().canJump)
	{
		return nullptr;
	}

	CTile* pDestinationTile = pNextTile->GetNextTile(m_dir);
	if (nullptr == pDestinationTile
		|| nullptr != pDestinationTile->GetLyingObject()
		|| false == pDestinationTile->GetTileInfo().canMove)
	{
		return nullptr;
	}

	return pDestinationTile;
}

void CMovement::CheckMoveWithTile()
{
	if (false == GetOwner().GetComponent<CCollider>()->IsEnabled() || true == m_isJumping)
	{
		return;
	}

	//object position & scale
	const Vec2 collisionPos   = GetOwner().GetComponent<CCollider>()->GetFinalPos();
	const Vec2 collisionScale = GetOwner().GetComponent<CCollider>()->GetScale();

	const int tileXCount = CCore::GetInstance().GetStageManager().GetCurrent().GetMapInfo().colCount;
	const int tileSize   = CCore::GetInstance().GetStageManager().GetCurrent().GetMapInfo().tileWidth;

	const int col = static_cast<int>(collisionPos.x) / tileSize;
	const int row = static_cast<int>(collisionPos.y) / tileSize;

	const std::vector<CGameObject*>& vecTiles = CCore::GetInstance().GetStageManager().GetCurrent().GetObjects(
		OBJECT_TYPE::TILE);
	CTile* pCurrent = static_cast<CTile*>(vecTiles[row * tileXCount + col]);

	const Vec2 tilePos = pCurrent->GetComponent<CTransform>()->GetPos();
	const Vec2 dir     = GetOwner().GetComponent<CMovement>()->GetDir();
	Vec2       pos     = GetOwner().GetComponent<CTransform>()->GetPos();

	if (dir == DIR::LEFT)
	{
		if (tilePos.x > collisionPos.x - collisionScale.x * .5f)
		{
			if (nullptr == pCurrent->GetLeft() || false == pCurrent->GetLeft()->GetTileInfo().canMove)
			{
				pos.x += (tilePos.x - (collisionPos.x - collisionScale.x * .5f));
				GetOwner().GetComponent<CTransform>()->SetPos(pos);
			}
		}
	}
	if (dir == DIR::RIGHT)
	{
		if (tilePos.x + tileSize < collisionPos.x + collisionScale.x * .5f)
		{
			if (nullptr == pCurrent->GetRight() || false == pCurrent->GetRight()->GetTileInfo().canMove)
			{
				pos.x += (tilePos.x + tileSize - (collisionPos.x + collisionScale.x * .5f));
				GetOwner().GetComponent<CTransform>()->SetPos(pos);
			}
		}
	}
	if (dir == DIR::UP)
	{
		if (tilePos.y - tileSize * .3f >= collisionPos.y - collisionScale.y * .5f)
		{
			if (nullptr == pCurrent->GetUp() || false == pCurrent->GetUp()->GetTileInfo().canMove)
			{
				pos.y += (tilePos.y - tileSize * .3f - (collisionPos.y - collisionScale.y * .5f));
				GetOwner().GetComponent<CTransform>()->SetPos(pos);
			}
		}
	}
	if (dir == DIR::DOWN)
	{
		if (tilePos.y + tileSize < collisionPos.y + collisionScale.y * .5f)
		{
			if (nullptr == pCurrent->GetDown() || false == pCurrent->GetDown()->GetTileInfo().canMove)
			{
				pos.y += (tilePos.y + tileSize - (collisionPos.y + collisionScale.y * .5f));
				GetOwner().GetComponent<CTransform>()->SetPos(pos);
			}
		}
	}
}

void CMovement::CheckMoveWithCollision(const Vec2& _pos, const Vec2& _targetPos, const Vec2& _intersected)
{
	const Vec2 ownerPos = GetOwner().GetComponent<CTransform>()->GetPos();
	Vec2       dir{};
	dir.x       = (_pos.x - _targetPos.x) > 0 ? -1.f : 1.f;
	dir.y       = (_pos.y - _targetPos.y) > 0 ? -1.f : 1.f;
	float speed = m_speed * DS;

	if (_intersected.x < _intersected.y) //y축 변화량이 더 클 때,
	{
		if (dir.x < 0 && dir.y < 0) //우하단 방향으로 이동 
		{
			GetOwner().GetComponent<CTransform>()->SetPos(Vec2{ownerPos.x + _intersected.x, ownerPos.y + speed});
		}
		else if (dir.x < 0 && dir.y > 0) //우상단 방향으로 이동
		{
			GetOwner().GetComponent<CTransform>()->SetPos(Vec2{ownerPos.x + _intersected.x, ownerPos.y - speed});
		}

		else if (dir.x > 0 && dir.y < 0) //좌하단 방향으로 이동
		{
			GetOwner().GetComponent<CTransform>()->SetPos(Vec2{ownerPos.x - _intersected.x, ownerPos.y + speed});
		}
		else if (dir.x > 0 && dir.y > 0) //좌상단 방향으로 이동
		{
			GetOwner().GetComponent<CTransform>()->SetPos(Vec2{ownerPos.x - _intersected.x, ownerPos.y - speed});
		}
	}
	else if (_intersected.x > _intersected.y) //x축 변화량이 더 클 때,
	{
		if (dir.x < 0 && dir.y < 0) // 좌상단 방향으로 이동
		{
			GetOwner().GetComponent<CTransform>()->SetPos(Vec2{ownerPos.x + speed, ownerPos.y + _intersected.y});
		}
		else if (dir.x < 0 && dir.y > 0) //좌하단 방향으로 이동
		{
			GetOwner().GetComponent<CTransform>()->SetPos(Vec2{ownerPos.x + speed, ownerPos.y - _intersected.y});
		}

		else if (dir.x > 0 && dir.y < 0) //우상단 방향으로 이동
		{
			GetOwner().GetComponent<CTransform>()->SetPos(Vec2{ownerPos.x - speed, ownerPos.y + _intersected.y});
		}
		else if (dir.x > 0 && dir.y > 0) //우하단 방향으로 이동 
		{
			GetOwner().GetComponent<CTransform>()->SetPos(Vec2{ownerPos.x - speed, ownerPos.y - _intersected.y});
		}
	}
}

RECT CMovement::GetIntersect(const Vec2& _pos, const Vec2& _scale, const Vec2& _targetPos, const Vec2& _targetScale)
{
	const RECT owner = {
		static_cast<LONG>(_pos.x - _scale.x * .5f),
		static_cast<LONG>(_pos.y - _scale.y * .5f),
		static_cast<LONG>(_pos.x + _scale.x * .5f),
		static_cast<LONG>(_pos.y + _scale.y * .5f)
	};

	const RECT target = {
		static_cast<LONG>(_targetPos.x - _targetScale.x * .5f),
		static_cast<LONG>(_targetPos.y - _targetScale.y * .5f),
		static_cast<LONG>(_targetPos.x + _targetScale.x * .5f),
		static_cast<LONG>(_targetPos.y + _targetScale.y * .5f)
	};

	RECT rect{};
	IntersectRect(&rect, &owner, &target);

	return rect;
}
