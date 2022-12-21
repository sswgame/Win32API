#pragma once
#include "pch.h"
#include "IDelegate.h"
#include "IEvent.h"

class IDelegate;
class IEvent;

class CEventManager
{
public:
	CEventManager();
	~CEventManager();

public:
	void Update();

	void DispatchEvents();

	template <typename EventType, typename... Args>
	void Send(Args ..._args);

	template <typename Subscriber, typename EventType>
	void RegisterCallback(Subscriber* _pSubscriber, std::function<void(Subscriber*, const EventType*)> _func);

	template <typename Subscriber, typename EventType>
	void UnRegisterCallback(Subscriber* _pSubscriber, std::function<void(Subscriber*, const EventType*)> _func);

private:
	void ClearBuffer();

private:
	std::deque<IEvent*>                            m_queueEvents;
	std::map<std::string, std::vector<IDelegate*>> m_mapCallbacks;
	std::deque<IEvent*>                            m_queuePending;
};

template <typename Subscriber, typename EventType>
void CEventManager::RegisterCallback(Subscriber* _pSubscriber, std::function<void(Subscriber*, const EventType*)> _func)
{
	const std::string eventName = EventType::s_eventName;

	const std::map<std::string, std::vector<IDelegate*>>::iterator iter = m_mapCallbacks.find(eventName);
	if (iter != m_mapCallbacks.end())
	{
		iter->second.push_back(new Delegate<Subscriber, EventType>{_pSubscriber, _func});
	}
	else
	{
		std::vector<IDelegate*> vecCallbacks{};
		vecCallbacks.push_back(new Delegate<Subscriber, EventType>{_pSubscriber, _func});
		m_mapCallbacks.insert({eventName, vecCallbacks});
	}
}

template <typename Subscriber, typename EventType>
void CEventManager::UnRegisterCallback(Subscriber*                                        _pSubscriber,
                                       std::function<void(Subscriber*, const EventType*)> _func)
{
	const std::string eventName = EventType::s_eventName;

	const std::map<std::string, std::vector<IDelegate*>>::iterator iter = m_mapCallbacks.find(eventName);

	Delegate<Subscriber, EventType> delegate = {_pSubscriber, _func};

	if (iter != m_mapCallbacks.end())
	{
		std::vector<IDelegate*>::iterator deleteIter = iter->second.begin();
		while (deleteIter != iter->second.end())
		{
			if ((*deleteIter)->GetID() == delegate.GetID())
			{
				delete (*deleteIter);

				iter->second.erase(deleteIter);
				break;
			}
			++deleteIter;
		}
	}
}

template <typename EventType, typename ... Args>
void CEventManager::Send(Args ..._args)
{
	IEvent* pEvent = new EventType{std::forward<Args>(_args)...};
	m_queueEvents.push_back(pEvent);
}
