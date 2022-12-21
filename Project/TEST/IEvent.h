#pragma once
#include <string>

class IEvent
{
public:
	virtual ~IEvent() = default;

	IEvent(const std::string& eventName)
		:
		m_id{eventName},
		m_isActive{true} {}

	virtual std::string GetEventName() = 0;

	void SetActive(bool _state) { m_isActive = _state; }
	bool IsActive() const { return m_isActive; }
private:
	std::string m_id;
	bool        m_isActive;
};

template <typename T>
class CEvent : public IEvent
{
	friend class CEventManager;
public:
	CEvent()
		:
		IEvent{s_eventName} {}

	std::string GetEventName() override { return s_eventName; }
private:
	static const std::string s_eventName;
};

template <typename T>
const std::string CEvent<T>::s_eventName = typeid(T).name();
