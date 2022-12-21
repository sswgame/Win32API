#include "pch.h"
#include "CResourceManager.h"

#include "CCore.h"
#include "CPathManager.h"
#include "CSound.h"
#include "CTexture.h"
#include "CTileManager.h"

CResourceManager::CResourceManager() = default;

CResourceManager::~CResourceManager()
{
	for (std::pair<const std::wstring, SpriteInfo*>& pair : m_mapSpriteInfos)
	{
		delete pair.second;
		pair.second = nullptr;
	}
	m_mapSpriteInfos.clear();

	for (std::pair<const std::wstring, CTexture*>& pair : m_mapTexture)
	{
		delete pair.second;
		pair.second = nullptr;
	}
	m_mapTexture.clear();

	for (std::pair<const std::wstring, CSound*>& pair : m_mapSound)
	{
		delete pair.second;
		pair.second = nullptr;
	}
	m_mapSound.clear();
}

CTexture* CResourceManager::CreateTexture(const std::wstring& _key, UINT _width, UINT _height)
{
	CTexture* pTexture = FindTexture(_key);

	if (nullptr != pTexture)
	{
		return pTexture;
	}

	pTexture = new CTexture{};
	pTexture->Create(_width, _height);
	pTexture->SetKey(_key);

	m_mapTexture.insert({_key, pTexture});

	return pTexture;
}

CTexture* CResourceManager::LoadTexture(const std::wstring& _key, const std::wstring& _relativePath)
{
	CResource* pTexture = FindTexture(_key);
	if (nullptr != pTexture)
	{
		return static_cast<CTexture*>(pTexture);
	}

	const std::wstring absolutePath = CCore::GetInstance().GetPathManager().GetContentPath();

	pTexture = new CTexture{};
	if (false == pTexture->Load(absolutePath + _relativePath))
	{
		assert(pTexture && "LOAD BITMAP FAILED");
		delete pTexture;
		pTexture = nullptr;
		return nullptr;
	}
	pTexture->SetKey(_key);
	pTexture->SetPath(_relativePath);
	m_mapTexture.insert({_key, static_cast<CTexture*>(pTexture)});

	return static_cast<CTexture*>(pTexture);
}

CTexture* CResourceManager::FindTexture(const std::wstring& _key)
{
	const std::map<std::wstring, CTexture*>::iterator iter = m_mapTexture.find(_key);
	if (iter != m_mapTexture.end())
	{
		return iter->second;
	}
	return nullptr;
}

SpriteInfo* CResourceManager::GetSpriteInfo(const std::wstring& _key)
{
	std::map<std::wstring, SpriteInfo*>::iterator iter = m_mapSpriteInfos.find(_key);
	if (iter != m_mapSpriteInfos.end())
	{
		return iter->second;
	}

	return nullptr;
}

SpriteInfo* CResourceManager::CreateSpriteInfo(const std::wstring& _key, const std::wstring& _filePath)
{
	SpriteInfo* pInfo    = GetSpriteInfo(_key);
	CTexture*   pTexture = LoadTexture(_key, _filePath);
	assert(pTexture);

	if (nullptr != pInfo)
	{
		return pInfo;
	}

	pInfo                  = new SpriteInfo{};
	pInfo->textureName     = _key;
	pInfo->scale           = pTexture->GetSize();
	pInfo->centerPos       = pInfo->scale * .5f;
	pInfo->backgroundColor = RGB(255, 0, 255);
	pInfo->isFlipped       = false;
	pInfo->pos             = Vec2{0, 0};

	m_mapSpriteInfos.insert({_key, pInfo});

	return pInfo;
}

void CResourceManager::LoadSpriteInfos(const std::wstring& _xmlPath)
{
	const std::wstring contentPath = CCore::GetInstance().GetPathManager().GetContentPath();
	//read XML Info
	const std::wstring xmlPath = contentPath + _xmlPath;
	std::string        path    = std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(xmlPath);

	tinyxml2::XMLDocument    doc{};
	const tinyxml2::XMLError error = doc.LoadFile(path.c_str());
	assert(error == tinyxml2::XML_SUCCESS && "ANIM ATLAS LOAD FAILED");

	tinyxml2::XMLElement* pRoot = doc.FirstChildElement("TextureAtlas");
	assert(pRoot);

	const std::string  fileName        = pRoot->Attribute("imagePath");
	const std::wstring fileNameUnicode = std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(fileName);
	const int          R               = atoi(pRoot->Attribute("R"));
	const int          G               = atoi(pRoot->Attribute("G"));
	const int          B               = atoi(pRoot->Attribute("B"));
	//const int A = atoi(pRoot->Attribute("A"));

	tinyxml2::XMLNode* pCurrent = pRoot->FirstChild();
	while (pCurrent)
	{
		std::string currentName = pCurrent->ToElement()->Attribute("n");

		SpriteInfo* pInfo  = new SpriteInfo{};
		pInfo->pos.x       = static_cast<float>(atoi(pCurrent->ToElement()->Attribute("x")));
		pInfo->pos.y       = static_cast<float>(atoi(pCurrent->ToElement()->Attribute("y")));
		pInfo->scale.x     = static_cast<float>(atoi(pCurrent->ToElement()->Attribute("w")));
		pInfo->scale.y     = static_cast<float>(atoi(pCurrent->ToElement()->Attribute("h")));
		pInfo->centerPos.x = static_cast<float>(atoi(pCurrent->ToElement()->Attribute("centerX")));
		pInfo->centerPos.y = static_cast<float>(atoi(pCurrent->ToElement()->Attribute("centerY")));

		std::string isFlipped = pCurrent->ToElement()->Attribute("isFlipped");
		if (isFlipped == "True")
		{
			pInfo->isFlipped = true;
		}
		else
		{
			pInfo->isFlipped = false;
		}

		pInfo->backgroundColor = RGB(R, G, B);
		pInfo->textureName     = fileNameUnicode;
		std::wstring name      = std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(currentName);

		m_mapSpriteInfos.insert({name, pInfo});

		pCurrent = pCurrent->NextSibling();
	}
}


void CResourceManager::ConvertTileInfoToSpriteInfo()
{
	const std::vector<TileInfo>& vecTileInfos = CCore::GetInstance().GetTileManager().GetAllTileInfos();

	for (int i = 0; i < vecTileInfos.size(); ++i)
	{
		SpriteInfo* pInfo      = new SpriteInfo{};
		pInfo->scale           = Vec2{vecTileInfos[i].width, vecTileInfos[i].height};
		pInfo->backgroundColor = RGB(255, 0, 255);
		pInfo->centerPos       = pInfo->scale / 2;
		pInfo->isFlipped       = false;

		const int col      = i % vecTileInfos[i].column;
		const int row      = i / vecTileInfos[i].column;
		pInfo->pos         = Vec2{col, row} * pInfo->scale;
		pInfo->textureName = L"TILES";

		m_mapSpriteInfos.insert({std::to_wstring(vecTileInfos[i].id), pInfo});
	}
}

CSound* CResourceManager::LoadSound(const std::wstring& _key, const std::wstring& _relativePath)
{
	assert(nullptr == FindSound(_key));

	CSound* pSound = new CSound{};

	std::wstring filePath = CCore::GetInstance().GetPathManager().GetContentPath();
	filePath += _relativePath;
	const bool result = pSound->Load(filePath.c_str());

	if (false == result)
	{
		MessageBox(nullptr, L"Sound 로딩 실패!!!", L"리소스 로딩 실패", MB_OK);
		delete pSound;
		return nullptr;
	}

	pSound->SetKey(_key);
	pSound->SetPath(_relativePath);

	// map 에 등록
	m_mapSound.insert({_key, pSound});

	return pSound;
}

CSound* CResourceManager::FindSound(const std::wstring& _key)
{
	std::map<std::wstring, CSound*>::iterator iter = m_mapSound.find(_key);

	if (iter == m_mapSound.end())
	{
		return nullptr;
	}

	return iter->second;
}
