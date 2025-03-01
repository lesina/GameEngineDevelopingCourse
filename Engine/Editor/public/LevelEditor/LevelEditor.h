#pragma once

#include <Level.h>
#include <UIWindow.h>
#include <Timer.h>
#include <flecs.h>

namespace cqe
{
	namespace Editor
	{
		class LevelEditor final : public GUI::UIWindow
		{
		public:
			LevelEditor() = delete;
			LevelEditor(flecs::world& world);

		public:
			virtual void Draw() override;
			virtual void Update(float dt) override;
			virtual const char* GetName() const override { return "Level Editor"; }

		private:
			void Save();
			std::vector<float> FromStringToFloat3(const std::string inputString, const std::string delimiter);

		private:
			Core::Timer m_SaveButtonMessageTimer;
			bool m_SaveButtonPressed = false;
			float m_TimeToShowSaveButtonMessage = 3.f;

			std::optional<World::Level> m_Level = std::nullopt;
		};
	}
}