#include <LevelEditor/LevelSerializer.h>
#include <Parser/WorldParser.h>

#include <tinyxml2.h>

namespace GameEngine::Editor
{
	[[nodiscard]] World::Level LevelSerializer::Deserialize(const std::string& filepath)
	{
		return World::WorldParser::ParseLevel(filepath);
	}

	void LevelSerializer::Serialize(const std::string& filepath, const World::Level& level)
	{
		tinyxml2::XMLDocument doc;

		tinyxml2::XMLElement* root = doc.NewElement("level");
		assert(root != nullptr);

		root->SetAttribute("name", level.GetName().c_str());
		doc.InsertFirstChild(root);

		for (const World::LevelObject& levelObject : level.GetLevelObjects())
		{
			tinyxml2::XMLElement* entityElem = root->InsertNewChildElement("entity");
			assert(entityElem != nullptr);

			entityElem->SetAttribute("Name", levelObject.GetName().c_str());

			for (const World::LevelObject::Component& component : levelObject.GetComponents())
			{
				entityElem->SetAttribute(component.first.c_str(), component.second.c_str());
			}
		}

		doc.SaveFile(filepath.c_str());
	}
}