#include <LevelEditor/LevelEditor.h>
#include <LevelEditor/LevelSerializer.h>
#include <LevelEditor/ECS/ecsLevelEditor.h>

#include <Filesystem.h>
#include <ECS/ecsSystems.h>
#include <Parser/WorldParser.h>

#include <imgui.h>
#include <imgui_stdlib.h>

namespace GameEngine
{
	namespace Editor
	{
		LevelEditor::LevelEditor(flecs::world& world)
		{
			m_World = world.get_world();
			m_Level = LevelSerializer::Deserialize(Core::g_FileSystem->GetFilePath("Levels/Main.xml").generic_string());

			for (World::LevelObject& levelObject : m_Level->GetLevelObjects())
			{
				AddLevelEditorEntity(levelObject);
			}

			EntitySystem::LevelEditorECS::RegisterLevelEditorEcsSystems(m_World);
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
						static const std::set<std::string> VECTOR3_COMPONENTS = {
							"Position", "Velocity", "Gravity",
						};
						for (World::LevelObject::Component& component : levelObject.GetComponents())
						{
							if (VECTOR3_COMPONENTS.contains(component.first))
							{
								Math::Vector3f vec = ParseStringToFloat3(component.second);

								float arrayPos[3] = { vec.x, vec.y, vec.z };
								ImGui::InputFloat3(component.first.c_str(), arrayPos);
								component.second = 
									std::to_string(arrayPos[0]) + "," +
									std::to_string(arrayPos[1]) + "," +
									std::to_string(arrayPos[2]);
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
			if (ImGui::Button("Add Default Object"))
			{
				AddDefaultObject();
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
			LevelSerializer::Serialize(Core::g_FileSystem->GetFilePath("Levels/Main.xml").generic_string(), m_Level.value());
		}

		void LevelEditor::AddDefaultObject() 
		{
			if (!m_Level.has_value())
			{
				return;
			}

			std::string baseName = "NewObject1";
			std::string objectName = baseName;
			uint16_t counter = 1;

			World::Level::LevelObjectList& objects = m_Level.value().GetLevelObjects();
			while ( std::ranges::find_if(objects, [&](const World::LevelObject& obj) { return obj.GetName() == objectName; }) != objects.end() ) {
				objectName = baseName + std::to_string(++counter);
			}

			World::LevelObject newLevelObject;
			newLevelObject.SetName(objectName.c_str());
			newLevelObject.AddComponent("Position", "0.0,0.0,0.0");
			newLevelObject.AddComponent("Velocity", "0.0,0.0,0.0");
			newLevelObject.AddComponent("Gravity", "0.0,-9.8,0.0");
			newLevelObject.AddComponent("BouncePlane", "0.0,1.0,0.0,5.0");
			newLevelObject.AddComponent("Bounciness", "1.0");
			newLevelObject.AddComponent("GeometryPtr", "Cube");

			m_Level.value().AddLevelObject(newLevelObject);
			AddLevelEditorEntity(m_Level.value().GetLevelObjects().back());
		}

		void LevelEditor::AddLevelEditorEntity(World::LevelObject& levelObject) 
		{
			flecs::entity entity = m_World.entity(levelObject.GetName().c_str());

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

				entity.set(EntitySystem::LevelEditorECS::PositionDesc{ &positionAttribute->second });

				// Can be set to 0 since it doesn't matter now, will be updated by the system
				entity.set(EntitySystem::EditorECS::Position{ 0.0f, 0.0f, 0.0f });
				entity.set(GeometryPtr{
					reinterpret_cast<RenderCore::Geometry*>(
						World::WorldParser::GetCustomComponents()[geometryAttribute->second]
						)
					});
			}
		}
	}
}