#include "pch.h"
#include "CStage.h"

#include "CCamera.h"
#include "CCollider.h"
#include "CCore.h"
#include "CFSM.h"
#include "CGameObject.h"
#include "CPlayer.h"
#include "CSpriteComponent.h"
#include "CStageManager.h"
#include "CTile.h"

CPlayer* CStage::s_pPlayer{};
Vec2     CStage::s_playerLocation{};

CStage::CStage()
	:
	m_info{nullptr},
	m_isInitialized{false} {}

CStage::~CStage()
{
	Clear();
}

void CStage::Update()
{
	for (UINT i = 0; i < static_cast<UINT>(OBJECT_TYPE::END); ++i)
	{
		for (CGameObject* pGameObject : m_vecObjects[i])
		{
			if (false == pGameObject->IsDead())
			{
				pGameObject->Update();
			}
		}
	}
	FollowPlayer();
}

void CStage::Render(HDC _dc)
{
	for (UINT i = 0; i < static_cast<UINT>(OBJECT_TYPE::END); ++i)
	{
		if (i == static_cast<UINT>(OBJECT_TYPE::TILE))
		{
			DrawTile(_dc);
		}
		else
		{
			std::vector<CGameObject*>::iterator iter = m_vecObjects[i].begin();
			while (iter != m_vecObjects[i].end())
			{
				if ((*iter)->IsDead())
				{
					iter = m_vecObjects[i].erase(iter);
				}
				else
				{
					(*iter)->Render(_dc);
					++iter;
				}
			}
		}
	}
	for (int i = 0; i < m_vecDeads.size(); ++i)
	{
		delete m_vecDeads[i];
		m_vecDeads[i] = nullptr;
	}
	m_vecDeads.clear();
}

void CStage::DrawTile(HDC _dc)
{
	// 카메라가 보고있는 중심 좌표
	const Vec2 lookAt = CCore::GetInstance().GetCamera().GetLookAtPos();
	const Vec2 vLT    = lookAt - Vec2{CCore::GetInstance().GetResolution()} / 2.f;

	// 카메라가 보고있는 지점의 열, 행 정보
	int col = static_cast<int>(vLT.x) / m_info->tileWidth;
	int row = static_cast<int>(vLT.y) / m_info->tileHeight;

	// 화면을 타일로 가득 채웠을 때 최대 열, 행 개수
	int maxCol = CCore::GetInstance().GetResolution().x / m_info->tileWidth + 1;
	int maxRow = CCore::GetInstance().GetResolution().y / m_info->tileHeight + 1;

	// 화면 우측 초과하지 못하게 값을 보정
	if (col + maxCol > m_info->colCount)
	{
		maxCol -= col + maxCol - m_info->colCount;
	}
	// 화면 좌측 초과하지 못하게 값을 보정
	if (vLT.x < 0.f)
	{
		maxCol += static_cast<int>(vLT.x) / m_info->tileWidth;
		col = 0;
	}

	// 화면 아래로 초과하지 못하게 값을 보정
	if (row + maxRow > m_info->rowCount)
	{
		maxRow -= row + maxRow - m_info->rowCount;
	}

	// 화면 위로 초과하지 못하게 보정
	if (vLT.y < 0.f)
	{
		maxRow += static_cast<int>(vLT.y) / m_info->tileWidth;
		row = 0;
	}

	// 해당 열, 행에 해당하는 타일의 1차원 인덱스 정보
	const int tileIndex = m_info->colCount * row + col;

	// 타일오브젝트 벡터
	const std::vector<CGameObject*>& vecTile = m_vecObjects[static_cast<UINT>(OBJECT_TYPE::TILE)];

	for (int j = 0; j < maxRow; ++j)
	{
		const int startIndex = tileIndex + (m_info->colCount * j);

		for (int i = startIndex; i < startIndex + maxCol; ++i)
		{
			vecTile[i]->Render(_dc);
		}
	}
}

void CStage::AddGameObject(CGameObject* pGameObject, OBJECT_TYPE _type)
{
	m_vecObjects[static_cast<UINT>(_type)].push_back(pGameObject);
}

void CStage::RemoveObject(CGameObject* pGameObject, OBJECT_TYPE _type)
{
	std::vector<CGameObject*>& vecObjects = GetObjects(_type);
	vecObjects.erase(std::remove_if(vecObjects.begin(), vecObjects.end(), [&pGameObject](const CGameObject* _pInner)
	{
		if (pGameObject == _pInner)
		{
			delete pGameObject;
			return true;
		}
		return false;
	}), vecObjects.end());
}

void CStage::Clear()
{
	for (UINT i = 0; i < static_cast<UINT>(OBJECT_TYPE::END); ++i)
	{
		for (const CGameObject* pGameObject : m_vecObjects[i])
		{
			delete pGameObject;
			pGameObject = nullptr;
		}
		m_vecObjects[i].clear();
	}
}

void CStage::Clear(OBJECT_TYPE _type, CGameObject* _pExcept)
{
	for (const CGameObject* pGameObject : m_vecObjects[static_cast<UINT>(_type)])
	{
		if (pGameObject == _pExcept)
		{
			continue;
		}
		delete pGameObject;
		pGameObject = nullptr;
	}

	m_vecObjects[static_cast<UINT>(_type)].clear();
	if (_pExcept)
	{
		m_vecObjects[static_cast<UINT>(_type)].push_back(_pExcept);
	}
}

void CStage::MoveObjectsToNext(OBJECT_TYPE _type)
{
	CStage&                    nextStage  = CCore::GetInstance().GetStageManager().GetNext();
	std::vector<CGameObject*>& vecObjects = nextStage.GetObjects(_type);
	vecObjects.resize(m_vecObjects[static_cast<UINT>(_type)].size());
	vecObjects = std::move(m_vecObjects[static_cast<UINT>(_type)]);
}

void CStage::MoveObjectsTo(STAGE_TYPE _stage, OBJECT_TYPE _type, CGameObject* _pObject)
{
	if (nullptr == _pObject)
	{
		return;
	}

	CStage* pNextStage{};
	if (_stage == STAGE_TYPE::END)
	{
		pNextStage = &CCore::GetInstance().GetStageManager().GetNext();
	}
	else
	{
		pNextStage = &CCore::GetInstance().GetStageManager().GetStage(_stage);
	}
	std::vector<CGameObject*>& vecNextObject = pNextStage->GetObjects(_type);

	std::vector<CGameObject*>& vecCurrentStageObject = GetObjects(_type);
	for (int i = 0; i < vecCurrentStageObject.size(); ++i)
	{
		if (_pObject == vecCurrentStageObject[i])
		{
			std::swap(vecCurrentStageObject[i], vecCurrentStageObject[vecCurrentStageObject.size() - 1]);
			vecCurrentStageObject[vecCurrentStageObject.size() - 1] = nullptr;
			vecCurrentStageObject.resize(vecCurrentStageObject.size() - 1);
			break;
		}
	}
	auto iter = std::find(vecNextObject.begin(), vecNextObject.end(), _pObject);
	if (iter == vecNextObject.end())
	{
		vecNextObject.push_back(_pObject);
	}
}

void CStage::DrawObject(OBJECT_TYPE _type, bool _enabled)
{
	const std::vector<CGameObject*>& vecObjects = GetObjects(_type);
	if (_type == OBJECT_TYPE::ITEM)
	{
		CPlayer*                  pPlayer    = static_cast<CPlayer*>(GetObjects(OBJECT_TYPE::PLAYER)[0]);
		std::pair<CItem*, CItem*> playerItem = pPlayer->GetItems();
		for (int i = 0; i < vecObjects.size(); ++i)
		{
			CItem* pItem = static_cast<CItem*>(vecObjects[i]);
			if (pItem == playerItem.first || pItem == playerItem.second)
			{
				continue;
			}
			CSpriteComponent* pSpriteComponent = vecObjects[i]->GetComponent<CSpriteComponent>();
			if (pSpriteComponent)
			{
				pSpriteComponent->SetEnabled(_enabled);
			}
			CCollider* pCollider = vecObjects[i]->GetComponent<CCollider>();
			if (pCollider)
			{
				pCollider->Enable(_enabled);
			}
		}
	}

	else if (_type == OBJECT_TYPE::TILE || _type == OBJECT_TYPE::PLOT)
	{
		for (int i = 0; i < vecObjects.size(); ++i)
		{
			CSpriteComponent* pSpriteComponent = vecObjects[i]->GetComponent<CSpriteComponent>();
			if (pSpriteComponent)
			{
				pSpriteComponent->SetEnabled(_enabled);
			}
		}
	}
}

void CStage::LoadStage(STAGE_TYPE _stageType, const std::wstring& _filePath)
{
	const std::vector<CTile*>& vecTiles = CCore::GetInstance().GetTileManager().MakeTiles(_stageType, _filePath);
	m_vecObjects[static_cast<int>(OBJECT_TYPE::TILE)].resize(vecTiles.size());

	for (int i = 0; i < vecTiles.size(); ++i)
	{
		m_vecObjects[static_cast<int>(OBJECT_TYPE::TILE)][i] = vecTiles[i]->Clone();
	}

	m_info = &CCore::GetInstance().GetTileManager().GetMapInfos(_stageType);
}

std::vector<CTile*> CStage::GetTilesAround(const CTile* _pTile)
{
	std::vector<CTile*> vecResult{};
	vecResult.reserve(9);

	const MapInfo& mapInfo   = CCore::GetInstance().GetStageManager().GetCurrent().GetMapInfo();
	const Vec2     tileIndex = _pTile->GetTileIndex();

	const int rowFrom = static_cast<int>(tileIndex.x == 0 ? 0 : tileIndex.x - 1);
	const int colFrom = static_cast<int>(tileIndex.y == 0 ? 0 : tileIndex.y - 1);
	const int rowTo = static_cast<int>(tileIndex.x == mapInfo.rowCount - 1 ? tileIndex.x : tileIndex.x + 1);
	const int colTo = static_cast<int>(tileIndex.y == mapInfo.colCount - 1 ? tileIndex.y : tileIndex.y + 1);
	const int tileXCount = mapInfo.colCount;
	const std::vector<CGameObject*>& vecMapTiles = CCore::GetInstance().GetStageManager().GetCurrent().
	                                                                    GetObjects(OBJECT_TYPE::TILE);

	for (size_t i = rowFrom; i <= rowTo; ++i)
	{
		for (size_t j = colFrom; j <= colTo; ++j)
		{
			vecResult.push_back(static_cast<CTile*>(vecMapTiles[i * tileXCount + j]));
		}
	}

	return vecResult;
}

void CStage::DeleteObject(const DeletionEvent* _pEvent)
{
	if (_pEvent->pGameObject && false == _pEvent->pGameObject->IsDead())
	{
		_pEvent->pGameObject->SetDead();
		m_vecDeads.push_back(_pEvent->pGameObject);
	}
}

void CStage::CreateObject(const CreationEvent* _pEvent)
{
	AddGameObject(_pEvent->pGameObject, _pEvent->type);
}

void CStage::ChangeStage(const ChangeStageEvent* _pEvent)
{
	CCore::GetInstance().GetStageManager().ChangeStage(_pEvent->stage);
}

void CStage::ChangeState(const ChangeStateEvent* _pEvent)
{
	_pEvent->pFSM->ChangeState(_pEvent->state);
}

CTile* CStage::GetCurrentTile(CGameObject* _pOwner)
{
	Vec2 pos = _pOwner->GetComponent<CCollider>()->GetFinalPos();

	const int tileXCount = GetMapInfo().colCount;
	const int tileSize   = GetMapInfo().tileWidth;

	if (_pOwner->GetType() != OBJECT_TYPE::PLAYER)
	{
		pos = _pOwner->GetComponent<CTransform>()->GetPos() + tileSize * .5f;
	}
	const size_t col = static_cast<int>(pos.x) / tileSize;
	const size_t row = static_cast<int>(pos.y) / tileSize;

	const std::vector<CGameObject*>& vecTiles = GetObjects(OBJECT_TYPE::TILE);
	CTile*                           pCurrent = static_cast<CTile*>(vecTiles[row * tileXCount + col]);

	return pCurrent;
}

void CStage::FollowPlayer()
{
	if (GetObjects(OBJECT_TYPE::PLAYER).size() <= 0)
	{
		return;
	}

	Vec2 pos = GetObjects(OBJECT_TYPE::PLAYER)[0]->GetComponent<CTransform>()->GetPos();

	const Vec2 resolution    = CCore::GetInstance().GetResolution();
	const int  tileMapWidth  = m_info->tileWidth * m_info->colCount;
	const int  tileMapHeight = m_info->tileHeight * m_info->rowCount;

	const Vec2 minLocation = resolution * .5f;
	const Vec2 maxLocation = Vec2{tileMapWidth, tileMapHeight} - minLocation;

	if (resolution.x > tileMapWidth || resolution.y > tileMapHeight)
	{
		const Vec2 diff = Vec2{(resolution.x - tileMapWidth), (resolution.y - tileMapHeight)} * .5f;

		CCore::GetInstance().GetCamera().SetLookAt(minLocation - diff);
		return;
	}

	if (minLocation.x < pos.x && pos.x < maxLocation.x &&
		minLocation.y < pos.y && pos.y < maxLocation.y)
	{
		CCore::GetInstance().GetCamera().SetLookAt(pos);
	}
	else
	{
		if (pos.x < minLocation.x)
		{
			pos.x = minLocation.x;
		}
		if (pos.y < minLocation.y)
		{
			pos.y = minLocation.y;
		}
		if (pos.x > maxLocation.x)
		{
			pos.x = maxLocation.x;
		}
		if (pos.y > maxLocation.y)
		{
			pos.y = maxLocation.y;
		}
		CCore::GetInstance().GetCamera().SetLookAt(pos);
	}
}
