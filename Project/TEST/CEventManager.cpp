#include "pch.h"

#include "CEventManager.h"
#include "IEvent.h"

CEventManager::CEventManager() = default;

CEventManager::~CEventManager()
{
	if (m_queueEvents.size())
	{
		for (int i = 0; i < m_queueEvents.size(); ++i)
		{
			delete m_queueEvents[i];
		}
		m_queueEvents.clear();
	}
	if (m_mapCallbacks.size())
	{
		std::map<std::string, std::vector<IDelegate*>>::iterator iter = m_mapCallbacks.begin();
		while (iter != m_mapCallbacks.end())
		{
			for (const IDelegate* pDelegate : iter->second)
			{
				delete pDelegate;
			}
			iter->second.clear();
			iter = m_mapCallbacks.erase(iter);
		}
	}
}

void CEventManager::Update()
{
	DispatchEvents();
}

void CEventManager::DispatchEvents()
{
	bool         isChangeEvent = false;
	for (size_t i = 0; i < m_queueEvents.size(); ++i)
	{
		if (false == isChangeEvent)
		{
			std::map<std::string, std::vector<IDelegate*>>::iterator iter = m_mapCallbacks.find(
				m_queueEvents[i]->GetEventName());
			if (iter == m_mapCallbacks.end())
			{
				m_queueEvents[i]->SetActive(false);
				continue;
			}


			for (IDelegate* pDelegate : iter->second)
			{
				pDelegate->Invoke(m_queueEvents[i]);
			}

			if (m_queueEvents[i]->GetEventName() == ChangeStageEvent::s_eventName)
			{
				isChangeEvent = true;
			}
		}
		m_queueEvents[i]->SetActive(false);
	}
	ClearBuffer();
}

void CEventManager::ClearBuffer()
{
	for (const IEvent* pEvent : m_queueEvents)
	{
		if (false == pEvent->IsActive())
		{
			delete pEvent;
			m_queueEvents.pop_front();
		}
	}
}
