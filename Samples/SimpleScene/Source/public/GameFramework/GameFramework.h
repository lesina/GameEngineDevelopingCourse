/*
* IMPORTANT!!!
* 
* This file is an interface to the engine
* Please note that any change to this file should be reflected in the engine itself,
* and every engine change of this kind leads to incompatibility with the other samples
*/

#pragma once

#include <export.h>

#include <EntityManager.h>

class GAME_API GameFramework final
{
public:
	using Ptr = std::unique_ptr<GameFramework>;

public:
	GameFramework() = delete;
	GameFramework(flecs::world& world)
	{
		m_World = world.get_world();

		Init();
	}
	~GameFramework() = default;

public:
	void Init();
	void Update(float dt);

private:
	/*
	* This function registers the components for the flecs reflection system,
	* so it can be used in lua and for parsing an xml file
	*/
	void RegisterComponentsReflection();

	/* Any ECS system we declare through C++ */
	void RegisterSystems();

private:
	flecs::world m_World;
};