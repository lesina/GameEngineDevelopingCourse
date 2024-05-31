#include <EventManager.h>

namespace GameEngine::Core
{
	void EventManager::RegisterCallback(const std::string& eventName, Event callback)
	{
		if (!m_Events.contains(eventName))
		{
			m_Events.emplace(eventName, EventList());
		}

		EventList* listeners = &m_Events[eventName];

		listeners->push_back(callback);
	}

	void EventManager::CallEvent(const std::string& eventName)
	{
		if (!m_Events.contains(eventName))
		{
			return;
		}

		EventList* listeners = &m_Events[eventName];

		// Run all the listeners associated with the event
		for (int idx = 0; idx < listeners->size(); ++idx)
		{
			(*listeners)[idx]();
		}
	}
}