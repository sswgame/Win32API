#pragma once
#include "CGameObject.h"
#include "CTileManager.h"


struct TileInfo;

class CTile : public CGameObject
{
public:
	CTile();
	virtual ~CTile();

public:
	OBJECT_TYPE  GetType() override { return OBJECT_TYPE::TILE; };
	CGameObject* Clone() override { return new CTile{*this}; };

public:
	TileInfo& GetTileInfo() { return m_info; }
	void      SetTileInfo(const TileInfo& _info) { m_info = _info; }

	void SetTileIndex(const Vec2& _tileIndex)
	{
		m_tileIndex = {static_cast<LONG>(_tileIndex.x), static_cast<LONG>(_tileIndex.y)};
	}

	Vec2 GetTileIndex() const { return m_tileIndex; }

	CTile* GetNextTile(const Vec2& _dir);
	CTile* GetLeft();
	CTile* GetRight();
	CTile* GetUp();
	CTile* GetDown();

	void         SetLyingObject(CGameObject* _pObject) { m_pObject = _pObject; }
	CGameObject* GetLyingObject() const { return m_pObject; }
private:
	TileInfo     m_info;
	POINT        m_tileIndex;
	CGameObject* m_pObject;
};
