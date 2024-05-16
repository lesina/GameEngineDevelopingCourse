#pragma once

#include <Core/export.h>

namespace GameEngine::Core
{
	class CORE_API ActionMapEventManager final
	{
    public:
        using Event = std::function<void()>;
        using EventList = std::vector<Event>;
        using Ptr = std::unique_ptr<ActionMapEventManager>;

	public:
        ActionMapEventManager() = default;
        ~ActionMapEventManager() = default;

    public:
        void RegisterCallback(const std::string& eventName, Event callback);
        void CallEvent(const std::string& eventName);

    private:
        std::unordered_map<std::string, EventList> m_Events;
	};
}