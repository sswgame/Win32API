#include "pch.h"
#include "CObstacle.h"
#include "CTimer.h"
#include "CCore.h"
#include "CSpriteComponent.h"
#include "CEventManager.h"
#include "CStage.h"
#include "CStageManager.h"
#include "CTile.h"
#include "Events.h"


CObstacle::CObstacle()
	:
	m_type{OBSTACLE_TYPE::END},
	m_hp{0},
	m_elapsedTime{0},
	m_delay{0}
{
	AddComponent<CSpriteComponent>();
	AddComponent<CCollider>();
}

CObstacle::~CObstacle() = default;

void CObstacle::UpdateSpecific()
{
	if (IsDead())
	{
		return;
	}

	if (m_hp <= 0)
	{
		m_elapsedTime += DS;
		m_delay += DS;

		if (m_delay > 0.5f)
		{
			if (m_delay >= 1.f)
			{
				m_delay = 0.f;
			}
			GetComponent<CSpriteComponent>()->SetEnabled(false);
		}
		else if (m_delay <= 0.5f)
		{
			GetComponent<CSpriteComponent>()->SetEnabled(true);
		}

		if (m_elapsedTime >= 3.f)
		{
			m_elapsedTime = 0;

			CTile* pTile = CCore::GetInstance().GetStageManager().GetCurrent().GetCurrentTile(this);

			if (m_type == OBSTACLE_TYPE::BIG_ROCK || m_type == OBSTACLE_TYPE::WOOD)
			{
				const std::vector<CTile*> vecAdjacentTiles = CCore::GetInstance().GetStageManager().GetCurrent().
				                                                                  GetTilesAround(pTile);
				for (int i = 0; i < vecAdjacentTiles.size(); ++i)
				{
					if (vecAdjacentTiles[i]->GetLyingObject() == this)
					{
						vecAdjacentTiles[i]->GetTileInfo().canMove = true;
						vecAdjacentTiles[i]->SetLyingObject(nullptr);
					}
				}
			}
			else
			{
				if (pTile->GetLyingObject() == this)
				{
					pTile->GetTileInfo().canMove = true;
					pTile->SetLyingObject(nullptr);
				}
			}
			CCore::GetInstance().GetEventManager().Send<DeletionEvent>(DeletionEvent{this});
		}
	}
}
