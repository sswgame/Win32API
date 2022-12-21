#pragma once

#pragma region CComponent

class CGameObject;

class CComponent
{
	friend class CGameObject;
public:
	CComponent()
		:
		m_pOwner{nullptr}{}
	virtual ~CComponent() = default;

public:
	virtual void Update() = 0;
	virtual void Render(HDC _dc) = 0;
private:
	virtual CComponent* Clone() = 0;

public:
	CGameObject& GetOwner() const { return *m_pOwner; }

private:
	void SetOwner(CGameObject* _pOwner) { m_pOwner = _pOwner; }

private:
	CGameObject* m_pOwner;
};

#pragma endregion

#pragma region IComponent

template <typename T>
class IComponent : public CComponent
{
	friend class CComponent;
public:
	explicit IComponent(COMPONENT_TYPE _type);
	IComponent(const IComponent& _other);
	const IComponent& operator=(const IComponent&) = delete;
	virtual           ~IComponent();

public:
	void Update() override;;
	void Render(HDC _dc) override;;

public:
	int  GetID() const { return m_id; }
	static COMPONENT_TYPE GetType() ;

private:
	const int             m_id;
	static int            s_generator;
	static COMPONENT_TYPE s_componentType;
};

template <typename T>
COMPONENT_TYPE IComponent<T>::s_componentType{};

template <typename T>
int IComponent<T>::s_generator{};

template <typename T>
IComponent<T>::IComponent(COMPONENT_TYPE _type)
	:
	m_id{s_generator++}
{
	s_componentType = _type;
}

template <typename T>
IComponent<T>::IComponent(const IComponent& _other)
	:
	CComponent{ _other },
	m_id{s_generator++} {}

template <typename T>
IComponent<T>::~IComponent() = default;

template <typename T>
void IComponent<T>::Update() {}

template <typename T>
void IComponent<T>::Render(HDC _dc)
{
	UNREFERENCED_PARAMETER(_dc);
}

template <typename T>
COMPONENT_TYPE IComponent<T>::GetType()
{
	return s_componentType;
}
#pragma endregion
