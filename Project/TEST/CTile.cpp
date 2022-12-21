#include "pch.h"
#include "CTile.h"

#include "CCore.h"
#include "CSpriteComponent.h"
#include "CStage.h"
#include "CStageManager.h"

CTile::CTile()
	:
	m_info{},
	m_pObject{nullptr}
{
	AddComponent<CSpriteComponent>();
}

CTile::~CTile() = default;

CTile* CTile::GetNextTile(const Vec2& _dir)
{
	if (_dir == DIR::UP)
	{
		return GetUp();
	}
	if (_dir == DIR::RIGHT)
	{
		return GetRight();
	}
	if (_dir == DIR::LEFT)
	{
		return GetLeft();
	}
	if (_dir == DIR::DOWN)
	{
		return GetDown();
	}

	return nullptr;
}

CTile* CTile::GetLeft()
{
	const int tileXCount = CCore::GetInstance().GetStageManager().GetCurrent().GetMapInfo().colCount;
	const std::vector<CGameObject*>& vecTiles = CCore::GetInstance().GetStageManager().GetCurrent().GetObjects(
		OBJECT_TYPE::TILE);

	if (m_tileIndex.y > 0)
	{
		return static_cast<CTile*>(vecTiles[static_cast<size_t>(m_tileIndex.x) * tileXCount + m_tileIndex.y - 1]);
	}
	return nullptr;
}

CTile* CTile::GetRight()
{
	const int tileXCount = CCore::GetInstance().GetStageManager().GetCurrent().GetMapInfo().colCount;
	const std::vector<CGameObject*>& vecTiles = CCore::GetInstance().GetStageManager().GetCurrent().GetObjects(
		OBJECT_TYPE::TILE);

	if (m_tileIndex.y < tileXCount - 1)
	{
		return static_cast<CTile*>(vecTiles[static_cast<size_t>(m_tileIndex.x) * tileXCount + m_tileIndex.y + 1]);
	}
	return nullptr;
}

CTile* CTile::GetUp()
{
	const int tileXCount = CCore::GetInstance().GetStageManager().GetCurrent().GetMapInfo().colCount;
	const std::vector<CGameObject*>& vecTiles = CCore::GetInstance().GetStageManager().GetCurrent().GetObjects(
		OBJECT_TYPE::TILE);

	if (m_tileIndex.x > 0)
	{
		return static_cast<CTile*>(vecTiles[static_cast<size_t>(m_tileIndex.x - 1) * tileXCount + m_tileIndex.y]);
	}
	return nullptr;
}

CTile* CTile::GetDown()
{
	const int tileXCount = CCore::GetInstance().GetStageManager().GetCurrent().GetMapInfo().colCount;
	const int tileYCount = CCore::GetInstance().GetStageManager().GetCurrent().GetMapInfo().rowCount;
	const std::vector<CGameObject*>& vecTiles = CCore::GetInstance().GetStageManager().GetCurrent().GetObjects(
		OBJECT_TYPE::TILE);

	if (m_tileIndex.x < tileYCount - 1)
	{
		return static_cast<CTile*>(vecTiles[static_cast<size_t>(m_tileIndex.x + 1) * tileXCount + m_tileIndex.y]);
	}
	return nullptr;
}
