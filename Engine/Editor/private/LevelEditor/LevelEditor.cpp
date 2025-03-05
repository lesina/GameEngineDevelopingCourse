#include <LevelEditor/LevelEditor.h>
#include <LevelEditor/LevelSerializer.h>
#include <LevelEditor/ECS/ecsLevelEditor.h>

#include <Filesystem.h>
#include <ECS/ecsSystems.h>
#include <Parser/WorldParser.h>

#include <imgui.h>
#include <imgui_stdlib.h>
#include <regex>
#include <array>

namespace cqe
{
	namespace Editor
	{
		LevelEditor::LevelEditor(flecs::world& world)
		{
			m_Level = LevelSerializer::Deserialize(Core::g_FileSystem->GetFilePath("Levels/Main.xml").generic_string());

			for (World::LevelObject& levelObject : m_Level->GetLevelObjects())
			{
				flecs::entity entity = world.entity(levelObject.GetName().c_str());

				World::LevelObject::ComponentList& componentList = levelObject.GetComponents();

				World::LevelObject::ComponentList::iterator positionAttribute = std::ranges::find_if(componentList,
					[](World::LevelObject::Component& component)
					{
						return !std::strcmp(component.first.c_str(), "Position");
					}
				);

				World::LevelObject::ComponentList::iterator geometryAttribute = std::ranges::find_if(componentList,
					[](World::LevelObject::Component& component)
					{
						return !std::strcmp(component.first.c_str(), "GeometryPtr");
					}
				);

				if (positionAttribute != componentList.end() &&
					geometryAttribute != componentList.end())
				{
					assert(World::WorldParser::GetCustomComponents().contains(geometryAttribute->second));

					std::string levelObjectName = levelObject.GetName().c_str();
					std::array<float, 3> levelObjectPositions = {0.0, 0.0, 0.0};
					std::pair<std::string, std::array<float, 3>> storePositionInEditor (levelObjectName, levelObjectPositions);
					m_objectPositions.insert(storePositionInEditor);

					std::string positionAttributeDesc = positionAttribute->second;
					FromStringToFloat3(positionAttributeDesc, ",", m_objectPositions[levelObjectName]);
					entity.set(EntitySystem::LevelEditorECS::PositionDesc{ &m_objectPositions[levelObjectName]});

					// Can be set to 0 since it doesn't matter now, will be updated by the system
					entity.set(EntitySystem::EditorECS::Position{ 0.0f, 0.0f, 0.0f });
					entity.set(GeometryPtr{
						reinterpret_cast<RenderCore::Geometry*>(
							World::WorldParser::GetCustomComponents()[geometryAttribute->second]
							)
						});
				}
			}

			EntitySystem::LevelEditorECS::RegisterLevelEditorEcsSystems(world);
		}

		void LevelEditor::Draw()
		{
			ImGui::Begin(GetName());

			if (m_Level.has_value()) [[likely]]
			{
				for (World::LevelObject& levelObject : m_Level->GetLevelObjects())
				{
					if (ImGui::TreeNode(levelObject.GetName().c_str()))
					{
						for (World::LevelObject::Component& component : levelObject.GetComponents())
						{
							const bool bIsPositionAttribute = !std::strcmp(component.first.c_str(), "Position");
							if (bIsPositionAttribute) 
							{
								std::array<float, 3>* position = &m_objectPositions[levelObject.GetName().c_str()];
								ImGui::InputFloat3(component.first.c_str(), position->data());
							}
							else 
							{
								ImGui::InputText(component.first.c_str(), &component.second);
							}
						}

						ImGui::TreePop();
					}
				}
			}

			if (ImGui::Button("Save"))
			{
				m_SaveButtonMessageTimer.Reset();
				m_SaveButtonPressed = true;

				Save();
			}

			if (m_SaveButtonPressed)
			{
				ImGui::SameLine();
				ImGui::Text("Saved!");
			}

			ImGui::End();
		}

		void LevelEditor::Update(float dt)
		{
			m_SaveButtonMessageTimer.Tick();

			if (m_SaveButtonMessageTimer.GetTotalTime() > m_TimeToShowSaveButtonMessage)
			{
				m_SaveButtonPressed = false;
			}
		}

		void LevelEditor::Save()
		{
			assert(m_Level.has_value());
			LevelSerializer::Serialize(Core::g_FileSystem->GetFilePath("Levels/Main.xml").generic_string(), m_Level.value(), m_objectPositions);
		}

		void LevelEditor::FromStringToFloat3(const std::string inputString, const std::string delimiter, std::array<float, 3>& outFloat3)
		{
			const std::regex del(delimiter);
			std::sregex_token_iterator posBegin = std::sregex_token_iterator(inputString.begin(), inputString.end(), del, -1);
			const std::sregex_token_iterator posEnd = std::sregex_token_iterator();

			for (int i = 0; i < 2; i++) 
			{
				std::string numberStr = *posBegin;
				outFloat3[i] = std::stof(numberStr);

				posBegin++;
			}
		}
	}
}