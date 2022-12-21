#pragma once

union COLLIDER_ID
{
	struct
	{
		UINT leftID;
		UINT rightID;
	};

	std::int64_t id;
};

class CCollider;

class CColliderManager
{
public:
	CColliderManager();
	~CColliderManager();

public:
	void Update();

	void SetCollisionBetween(OBJECT_TYPE _left, OBJECT_TYPE _right, bool _enabled);
	void OnCollisionEnter(CCollider* _pLeft, CCollider* _pRight);
	void OnCollisionStay(CCollider* _pLeft, CCollider* _pRight);
	void OnCollisionExit(CCollider* _pLeft, CCollider* _pRight);
private:
	void CompareInGroup(const std::vector<CGameObject*>& _vecLeft, const std::vector<CGameObject*>& _vecRight);
	bool IsCollided(const CCollider* _pLeft, const CCollider* _pRight);

private:
	std::array<UINT, OBJECT_TYPE_COUNT> m_arrCheck;
	std::map<std::uint64_t, bool>       m_mapCollisionInfo;
};
