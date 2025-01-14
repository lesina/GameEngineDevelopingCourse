#include <tinyxml2.h>

#include <DefaultGeometry.h>
#include <Level.h>
#include <Parser/ComponentParser.h>
#include <Parser/WorldParser.h>

namespace GameEngine::World
{
	std::unordered_map<std::string, uint64_t> WorldParser::s_CustomComponents =
	{
		{"Cube", reinterpret_cast<uint64_t>(RenderCore::DefaultGeometry::Cube())},
	};

	Level WorldParser::ParseLevel(const std::string& xmlPath)
	{
		tinyxml2::XMLDocument doc;
		doc.LoadFile(xmlPath.c_str());

		tinyxml2::XMLElement* levelChild = doc.FirstChildElement("level");
		assert(levelChild != nullptr);

		const tinyxml2::XMLAttribute* levelNameAttr = levelChild->FindAttribute("name");
		assert(levelNameAttr != nullptr);

		std::string levelName = levelNameAttr->Value();
		Level newLevel(levelName);

		tinyxml2::XMLElement* entityElem = levelChild->FirstChildElement("entity");

		while (entityElem)
		{
			LevelObject newLevelObject;

			const tinyxml2::XMLAttribute* entityAttr = entityElem->FirstAttribute();

			while (entityAttr != nullptr)
			{
				if (!std::strcmp(entityAttr->Name(), "Name"))
				{
					newLevelObject.SetName(entityAttr->Value());
				}
				else
				{
					newLevelObject.AddComponent(entityAttr->Name(), entityAttr->Value());
				}

				entityAttr = entityAttr->Next();
			}

			newLevel.AddLevelObject(newLevelObject);

			entityElem = entityElem->NextSiblingElement();
		}

		return newLevel;
	}

	uint64_t WorldParser::ParseCustom(const std::string& componentName, const std::string& componentValue)
	{
		if (s_CustomComponents.contains(componentValue))
		{
			return s_CustomComponents[componentValue];
		}
		else
		{
			return ComponentParser::Parse(componentName, componentValue);
		}
	}
}