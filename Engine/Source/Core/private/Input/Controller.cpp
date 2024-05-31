#include <INIReader.h>
#include <Input/Controller.h>
#include <Input/InputHandler.h>

namespace GameEngine::Core
{
	Controller::Controller(FileSystem::Path&& config)
		: m_Config(std::move(config))
	{
		INIReader reader(m_Config.string());
		assert(reader.ParseError() >= 0);

		std::set<std::string> fields = reader.GetFields("Keyboard");

		for (const std::string& field : fields)
		{
			std::string eventName = reader.Get("Keyboard", field, "");

			assert(!m_KeyboardEventMap.contains(field));
			m_KeyboardEventMap.emplace(field, StringToKeyboardButton(eventName));
		}

		fields = reader.GetFields("Mouse");

		for (const std::string& field : fields)
		{
			std::string eventName = reader.Get("Mouse", field, "");

			assert(!m_MouseEventMap.contains(field));
			m_MouseEventMap.emplace(field, StringToMouseButton(eventName));
		}
	}

	bool Controller::IsPressed(const std::string& event)
	{
		if (m_MouseEventMap.contains(event))
		{
			return InputHandler::GetInstance()->IsKeyPressed(m_MouseEventMap[event]);
		}

		if (m_KeyboardEventMap.contains(event))
		{
			return InputHandler::GetInstance()->IsKeyPressed(m_KeyboardEventMap[event]);
		}

		return false;
	}
}