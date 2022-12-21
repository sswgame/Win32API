#pragma once
#include "CTransform.h"

class CGameObject
{
	friend class CStage;
public:
	CGameObject();
	CGameObject(const CGameObject& _other);
	CGameObject& operator=(const CGameObject&) = delete;

	virtual ~CGameObject();

public:
	virtual OBJECT_TYPE  GetType() = 0;
	virtual CGameObject* Clone() = 0;
	virtual void         UpdateSpecific() {};
	virtual void         RenderSpecific(HDC _dc) {};

	virtual void OnCollisionEnter(CGameObject* _pOther) {};
	virtual void OnCollisionStay(CGameObject* _pOther) {};
	virtual void OnCollisionExit(CGameObject* _pOther) {};
public:
	void Update();
	void Render(HDC _dc);

public:
	template <typename T>
	T* AddComponent();

	template <typename T>
	T* GetComponent();

	bool IsDead() const { return m_isDead; }

public:
	void                SetName(const std::wstring& _name) { m_name = _name; }
	const std::wstring& GetName() const { return m_name; }

private:
	void SetDead() { m_isDead = true; }
private:
	std::wstring m_name;
	bool         m_isDead;

	std::array<CComponent*, COMPONENT_TYPE_COUNT> m_arrComponents;
};

template <typename T>
T* CGameObject::AddComponent()
{
	static_assert(std::is_base_of_v<CComponent, T>, "T must derive from Component");
	T*             pComponent                = new T{};
	COMPONENT_TYPE type                      = IComponent<T>::GetType();
	m_arrComponents[static_cast<UINT>(type)] = pComponent;
	pComponent->SetOwner(this);

	return pComponent;
}

template <typename T>
T* CGameObject::GetComponent()
{
	const bool result = std::is_base_of_v<CComponent, T>;
	assert(result);

	COMPONENT_TYPE type = IComponent<T>::GetType();
	return static_cast<T*>(m_arrComponents[static_cast<UINT>(type)]);
}
