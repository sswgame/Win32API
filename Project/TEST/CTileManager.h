#pragma once
#include "CItem.h"
class CTile;

struct TileInfo
{
	int  column;
	int  id;
	int  width;
	int  height;
	bool canJump;
	bool canLayObject;
	bool canMove;
};

union Extra
{
	struct
	{
	float x;
	float y;
	};

	ItemID id;
	
};
struct MapObjectInfo
{
	OBJECT_TYPE type;
	Vec2        location;
	Vec2        size;
	Extra       extra;
};
struct MapInfo
{
	int                                   tileWidth;
	int                                   tileHeight;
	int                                   rowCount;
	int                                   colCount;
	std::vector<int>                      vecTileIDs;
	std::map<std::wstring, MapObjectInfo> vecObjects;
};

class CTileManager
{
public:
	CTileManager();
	~CTileManager();

public:
	void LoadTiles(const std::wstring& _filePath, const std::wstring& _xmlPath);
	void LoadMap(STAGE_TYPE _stageType, const std::wstring& _filePath);

	const std::vector<TileInfo>& GetAllTileInfos() const { return m_vecTileInfos; }
	const MapInfo&               GetMapInfos(STAGE_TYPE _stageType);
	std::vector<CTile*>          MakeTiles(STAGE_TYPE _stageType, const std::wstring& _mapPath);

	
private:
	std::vector<CTile*> FindTiles(STAGE_TYPE _stageType);

private:
	std::map<STAGE_TYPE, std::vector<CTile*>> m_mapTiles;
	std::vector<TileInfo>                     m_vecTileInfos;
	std::map<STAGE_TYPE, MapInfo>             m_mapMapInfos;
	bool                                        m_isLoaded;
};
