#include "pch.h"
#include "CTileManager.h"

#include "CCore.h"
#include "CPathManager.h"
#include "CResourceManager.h"
#include "CSpriteComponent.h"
#include "CTile.h"

namespace
{
	std::set<int> setJump = {208, 372, 373, 374, 375, 376, 406};
}
CTileManager::CTileManager()
	:
	m_isLoaded{false} { }

CTileManager::~CTileManager()
{
	for (auto& data : m_mapTiles)
	{
		for (size_t i = 0; i < data.second.size(); ++i)
		{
			delete data.second[i];
			data.second[i] = nullptr;
		}
		data.second.clear();
	}
	m_mapTiles.clear();
}

void CTileManager::LoadTiles(const std::wstring& _filePath, const std::wstring& _xmlPath)
{
	CCore::GetInstance().GetResourceManager().LoadTexture(L"TILES", _filePath);

	const std::wstring contentPath = CCore::GetInstance().GetPathManager().GetContentPath();
	const std::wstring xmlPath     = contentPath + _xmlPath;
	std::string        path        = std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(xmlPath);

	tinyxml2::XMLDocument    doc{};
	const tinyxml2::XMLError error = doc.LoadFile(path.c_str());
	assert(error == tinyxml2::XML_SUCCESS && "MAPINFO LOAD FAILED");

	tinyxml2::XMLElement* pRoot = doc.FirstChildElement("tileset");
	assert(pRoot);

	const int tileCount  = atoi(pRoot->Attribute("tilecount"));
	const int tileWidth  = atoi(pRoot->Attribute("tilewidth"));
	const int tileHeight = atoi(pRoot->Attribute("tileheight"));
	const int column     = atoi(pRoot->Attribute("columns"));
	m_vecTileInfos.reserve(tileCount);

	for (int i = 0; i < tileCount; ++i)
	{
		TileInfo info{};

		info.id = i;
		if (setJump.count(info.id) != 0)
		{
			info.canJump = true;
		}
		info.width  = tileWidth;
		info.height = tileHeight;
		info.column = column;

		m_vecTileInfos.push_back(info);
	}

	CCore::GetInstance().GetResourceManager().ConvertTileInfoToSpriteInfo();
}

void CTileManager::LoadMap(STAGE_TYPE _stageType, const std::wstring& _filePath)
{
	std::map<STAGE_TYPE, MapInfo>::iterator iter = m_mapMapInfos.find(_stageType);
	if (iter != m_mapMapInfos.end())
	{
		return;
	}

	const std::wstring contentPath = CCore::GetInstance().GetPathManager().GetContentPath();
	const std::wstring xmlPath     = contentPath + _filePath;
	std::string        path        = std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(xmlPath);

	tinyxml2::XMLDocument    doc{};
	const tinyxml2::XMLError error = doc.LoadFile(path.c_str());
	assert(error == tinyxml2::XML_SUCCESS && "MAPINFO LOAD FAILED");

	tinyxml2::XMLElement* pRoot = doc.FirstChildElement("map");
	assert(pRoot);

	tinyxml2::XMLNode*    pCurrent = pRoot->FirstChildElement();
	tinyxml2::XMLElement* pElement{};

	const int col        = atoi(pRoot->Attribute("width"));
	const int row        = atoi(pRoot->Attribute("height"));
	const int tileWidth  = atoi(pRoot->Attribute("tilewidth"));
	const int tileHeight = atoi(pRoot->Attribute("tileheight"));

	MapInfo info{};
	info.vecTileIDs.reserve(col * row);
	info.tileWidth    = tileWidth;
	info.tileHeight   = tileHeight;
	info.colCount     = col;
	info.rowCount     = row;
	std::string layer = "layer";

	while (pCurrent)
	{
		if (pCurrent->Value() == layer)
		{
			pElement = pCurrent->FirstChild()->FirstChildElement();
			break;
		}
		pCurrent = pCurrent->NextSiblingElement();
	}
	while (pElement)
	{
		info.vecTileIDs.push_back(pElement->IntAttribute("gid") - 1);
		pElement = pElement->NextSiblingElement();
	}

	pCurrent = pCurrent->NextSibling();
	if (pCurrent && pCurrent->ToElement()->Attribute("name") == std::string{"CanMove"})
	{
		pElement = pCurrent->FirstChild()->FirstChildElement();
		int i    = 0;

		while (pElement)
		{
			if (pElement->FindAttribute("gid"))
			{
				m_vecTileInfos[info.vecTileIDs[i]].canMove = true;
			}
			++i;
			pElement = pElement->NextSiblingElement();
		}
	}
	pCurrent = pCurrent->NextSibling();
	if (pCurrent && pCurrent->Value() == std::string{"objectgroup"})
	{
		pElement = pCurrent->FirstChildElement();
		while (pElement)
		{
			std::string  name       = pElement->Attribute("name");
			std::string  type       = pElement->Attribute("type");
			Vec2         location   = Vec2{pElement->IntAttribute("x"), pElement->IntAttribute("y")};
			Vec2         size       = Vec2{pElement->IntAttribute("width"), pElement->IntAttribute("height")};
			std::wstring objectName = std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(name);

			MapObjectInfo objectInfo{};
			objectInfo.location = location;
			objectInfo.size     = size;
			if (type == "TELEPORT")
			{
				objectInfo.type             = OBJECT_TYPE::TELEPORTER;
				tinyxml2::XMLElement* pData = pElement->FirstChildElement();
				if (pData)
				{
					pData->FirstChildElement()->QueryFloatAttribute("value", &objectInfo.extra.x);
					pData->FirstChildElement()->NextSiblingElement()->QueryFloatAttribute("value", &objectInfo.extra.y);
				}
			}
			else if (type == "TOOL")
			{
				objectInfo.type          = OBJECT_TYPE::ITEM;
				objectInfo.extra.id.type = ITEM_CATEGORY::TOOL;
				objectInfo.extra.id.kind = static_cast<UINT>(pElement->IntAttribute("id"));
				objectInfo.size          = Vec2{40, 40};
			}
			else if (type == "OBSTACLE")
			{
				if (name == "BIG" || name == "TREE")
				{
					objectInfo.size = Vec2{80, 80};
				}
				else if (name == "SMALL" || name == "GRASS")
				{
					objectInfo.size = Vec2{40, 40};
				}
				objectInfo.type = OBJECT_TYPE::OBSTACLE;
				objectName += pElement->IntAttribute("id");
			}
			else if (name == "FRUIT")
			{
				objectName += pElement->IntAttribute("id");
				objectInfo.type = OBJECT_TYPE::ITEM;
			}
			else if (name == "NPC")
			{
				objectInfo.type = OBJECT_TYPE::NPC;
			}
			info.vecObjects.insert({objectName, objectInfo});
			pElement = pElement->NextSiblingElement();
		}
	}
	m_mapMapInfos.insert({_stageType, std::move(info)});
}

const MapInfo& CTileManager::GetMapInfos(STAGE_TYPE _stageType)
{
	std::map<STAGE_TYPE, MapInfo>::iterator iter = m_mapMapInfos.find(_stageType);
	assert(iter != m_mapMapInfos.end());
	return iter->second;
}

std::vector<CTile*> CTileManager::MakeTiles(STAGE_TYPE _stageType, const std::wstring& _mapPath)
{
	if (false == m_isLoaded)
	{
		LoadTiles(LR"(texture\newTileSet.bmp)", LR"(map\tileSet.xml)");
		m_isLoaded = true;
	}

	std::vector<CTile*> vecTiles = FindTiles(_stageType);
	if (false == vecTiles.empty())
	{
		return vecTiles;
	}

	LoadMap(_stageType, _mapPath);

	const std::vector<TileInfo>& tileInfos = GetAllTileInfos();
	const MapInfo&               mapInfo   = GetMapInfos(_stageType);

	vecTiles.reserve(mapInfo.colCount * mapInfo.rowCount);

	for (size_t i = 0; i < mapInfo.vecTileIDs.size(); ++i)
	{
		CTile* pTile = new CTile{};

		pTile->SetTileInfo(tileInfos[mapInfo.vecTileIDs[i]]);
		CSpriteComponent* pSpriteComponent = pTile->GetComponent<CSpriteComponent>();
		pTile->SetName(std::to_wstring(mapInfo.vecTileIDs[i]));
		pSpriteComponent->SetTexture(std::to_wstring(mapInfo.vecTileIDs[i]), LR"(texture\newTileSet.bmp)");
		pSpriteComponent->SetEnabled(true);

		Vec2 pos{};
		pos.x = static_cast<float>(i % mapInfo.colCount);
		pos.y = static_cast<float>(i / mapInfo.colCount);

		pTile->SetTileIndex({pos.y, pos.x});
		pTile->GetComponent<CTransform>()->SetPos(pos * pSpriteComponent->GetScale());
		pTile->GetComponent<CTransform>()->SetScale(Vec2{pTile->GetTileInfo().width, pTile->GetTileInfo().height});

		vecTiles.push_back(pTile);
	}
	m_mapTiles.insert({_stageType, std::move(vecTiles)});

	return m_mapTiles[_stageType];
}

std::vector<CTile*> CTileManager::FindTiles(STAGE_TYPE _stageType)
{
	auto iter = m_mapTiles.find(_stageType);
	if (iter != m_mapTiles.end())
	{
		return iter->second;
	}
	return std::vector<CTile*>{};
}
