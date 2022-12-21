#pragma once
#include "CCore.h"

class CPlayer;
class CTile;
struct MapInfo;
class CGameObject;

class CStage
{
	friend class CStageManager;
public:
	CStage();
	virtual ~CStage();

public:
	virtual void Init() = 0;
	virtual void Update();
	virtual void Render(HDC _dc);

	virtual void Enter() = 0;
	virtual void Exit() = 0;

private:
	void DrawTile(HDC _dc);

public:
	void                       AddGameObject(CGameObject* pGameObject, OBJECT_TYPE _type);
	void                       RemoveObject(CGameObject* pGameObject, OBJECT_TYPE _type);
	std::vector<CGameObject*>& GetObjects(OBJECT_TYPE _type) { return m_vecObjects[static_cast<UINT>(_type)]; }
	void                       Clear();
	void                       Clear(OBJECT_TYPE _type, CGameObject* _pExcept = nullptr);
	void                       MoveObjectsToNext(OBJECT_TYPE _type);
	void                       MoveObjectsTo(STAGE_TYPE _stage, OBJECT_TYPE _type, CGameObject* _pObject);
	void                       DrawObject(OBJECT_TYPE _type, bool _enabled);

	void LoadStage(STAGE_TYPE _stageType, const std::wstring& _filePath);
public:
	static void    SetPlayerLocation(const Vec2 _pos) { s_playerLocation = _pos; }
	static Vec2    GetPlayerLocation() { return s_playerLocation; }

	const MapInfo& GetMapInfo() const { return *m_info; }
	void           SetInitState(bool _initialized) { m_isInitialized = _initialized; }
	bool           GetInitState() const { return m_isInitialized; }

	CTile*              GetCurrentTile(CGameObject* _pOwner);
	std::vector<CTile*> GetTilesAround(const CTile* _pTile);

private:
	void DeleteObject(const DeletionEvent* _pEvent);
	void CreateObject(const CreationEvent* _pEvent);
	void ChangeStage(const ChangeStageEvent* _pEvent);
	void ChangeState(const ChangeStateEvent* _pEvent);
	void FollowPlayer();

protected:
	static CPlayer* s_pPlayer;
private:
	const MapInfo*            m_info;
	std::vector<CGameObject*> m_vecObjects[static_cast<UINT>(OBJECT_TYPE::END)];
	std::vector<CGameObject*> m_vecDeads;

	static Vec2 s_playerLocation;
	bool        m_isInitialized;
};
