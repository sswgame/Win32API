#pragma once
#include <functional>
class IEvent;

class IDelegate
{
public:
	IDelegate()          = default;
	virtual ~IDelegate() = default;

	virtual void   Invoke(const IEvent* _pEvent) = 0;
	virtual size_t GetID() const = 0;
private:
};

template <typename Subscriber, typename EventType>
class Delegate : public IDelegate
{
	using Callback = std::function<void(Subscriber*, const EventType*)>;
public:
	Delegate(Subscriber* _pSubscriber, const Callback& _func)
		:
		m_pSubscriber{_pSubscriber},
		m_callback{_func} {}

	void Invoke(const IEvent* _pEvent) override
	{
		m_callback(m_pSubscriber, reinterpret_cast<const EventType*>(_pEvent));
	}

	size_t GetID() const override { return typeid(Subscriber).hash_code() ^ typeid(Callback).hash_code(); }
private:
	Subscriber* m_pSubscriber;
	Callback    m_callback;
};
