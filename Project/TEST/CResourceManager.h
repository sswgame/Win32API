#pragma once

class CSound;

struct SpriteInfo
{
	std::wstring textureName;
	Vec2         pos;
	Vec2         scale;
	Vec2         centerPos;
	bool         isFlipped;
	COLORREF     backgroundColor;
};

class CTexture;

class CResourceManager
{
public:
	CResourceManager();
	~CResourceManager();

public:
	CTexture* CreateTexture(const std::wstring& _key, UINT _width, UINT _height);
	CTexture* LoadTexture(const std::wstring& _key, const std::wstring& _relativePath);
	CTexture* FindTexture(const std::wstring& _key);

	SpriteInfo* GetSpriteInfo(const std::wstring& _key);
	SpriteInfo* CreateSpriteInfo(const std::wstring& _key, const std::wstring& _filePath);
	void        LoadSpriteInfos(const std::wstring& _xmlPath);
	void        ConvertTileInfoToSpriteInfo();

	CSound* LoadSound(const std::wstring& _key, const std::wstring& _relativePath);
	CSound* FindSound(const std::wstring& _key);

private:
	std::map<std::wstring, CTexture*>   m_mapTexture;
	std::map<std::wstring, SpriteInfo*> m_mapSpriteInfos;

	std::map<std::wstring, CSound*> m_mapSound;
};
