#include <GameWorld.h>
#include <Parser/WorldParser.h>
#include <AudioDataBase.h>

namespace GameEngine::World
{
	GameWorld* GameWorld::m_Instance = nullptr;

	GameWorld::GameWorld()
	{

	}

	GameWorld* GameWorld::GetInstance()
	{
		if (m_Instance == nullptr) [[unlikely]]
		{
			m_Instance = new GameWorld();
		}

		return m_Instance;
	}

	void GameWorld::LoadLevel(flecs::world& world, const std::string& levelPath)
	{
		// That's a silly action but flecs doesn't work properly without it
		m_World = world.get_world();

		if (m_CurrentLevel.has_value()) [[likely]]
		{
			UnloadCurrentLevel();
		}

		m_CurrentLevel = WorldParser::ParseLevel(levelPath);

		LoadCurrentLevel();
	}

    void GameWorld::LoadCurrentLevel()
    {
        for (const LevelObject& levelObject : m_CurrentLevel.value().GetLevelObjects())
        {
            flecs::entity newEntity = m_World.entity(levelObject.GetName().c_str());

            for (const LevelObject::Component& objComponent : levelObject.GetComponents())
            {
                flecs::entity comp = m_World.lookup(objComponent.first.c_str());
                assert(comp.is_valid());

                newEntity.add(comp);
                assert(newEntity.has(comp.id()));

				const char* compValue = objComponent.second.c_str();

                void* ptr = newEntity.get_mut(comp);
                flecs::cursor cursor = m_World.cursor(comp.id(), ptr);

                int ret = cursor.push();
                ecs_assert(!ret, ECS_INTERNAL_ERROR, NULL);

				std::string strCompValue(compValue);
				bool bIsSoundFilePath = strCompValue.ends_with(".mp3") || 
										strCompValue.ends_with(".wav") || 
										strCompValue.ends_with(".ogg");

				if (bIsSoundFilePath)
				{
					cursor.set_uint(GameEngine::Audio::AudioDataBase::GetSoundFilePathsSize()); // sound id
					GameEngine::Audio::AudioDataBase::AddSoundFilePath(objComponent.second);
				}
				else
				{
					bool bIsCustom = true;
					char* end;
					for (float f = std::strtof(compValue, &end); compValue != end; f = std::strtof(compValue, &end))
					{
						bIsCustom = false;
						compValue = end + 1;
						cursor.set_float(f);
						cursor.next();
					}

					if (bIsCustom)
					{
						cursor.set_uint(WorldParser::ParseCustom(objComponent.first, objComponent.second));
					}
				}
                ret = cursor.pop();
                ecs_assert(!ret, ECS_INTERNAL_ERROR, NULL);
            }
        }
    }

	void GameWorld::UnloadCurrentLevel()
	{
		for (const LevelObject& levelObject : m_CurrentLevel.value().GetLevelObjects())
		{
			flecs::entity entityToRemove = m_World.lookup(levelObject.GetName().c_str());

			entityToRemove.destruct();
		}
	}
}