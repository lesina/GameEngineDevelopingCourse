#include <LevelEditor/LevelSerializer.h>
#include <Parser/WorldParser.h>

#include <tinyxml2.h>
#include <unordered_map>
#include <string.h>

namespace cqe::Editor
{
	[[nodiscard]] World::Level LevelSerializer::Deserialize(const std::string& filepath)
	{
		return World::WorldParser::ParseLevel(filepath);
	}

	void LevelSerializer::Serialize(const std::string& filepath, const World::Level& level, const std::unordered_map<std::string, std::vector<float>>& objectPositions)
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
				const bool bIsPositionAttribute = !std::strcmp(component.first.c_str(), "Position");
				if (bIsPositionAttribute) 
				{
					std::string positionAsString = "";

					std::unordered_map<std::string, std::vector<float>>::const_iterator position = objectPositions.find(levelObject.GetName().c_str());
					for (int i = 0; i < position->second.size(); i++) 
					{
						if (i != 0) 
						{
							positionAsString.append(",");
						}

						std::string number = std::to_string(position->second[i]);
						positionAsString.append(number);
					}

					entityElem->SetAttribute(component.first.c_str(), positionAsString.c_str());
				}
				else 
				{
					entityElem->SetAttribute(component.first.c_str(), component.second.c_str());
				}
			}
		}

		doc.SaveFile(filepath.c_str());
	}
}