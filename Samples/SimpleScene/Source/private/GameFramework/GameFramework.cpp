#include <Camera.h>
#include <DefaultGeometry.h>
#include <ecsControl.h>
#include <ecsMesh.h>
#include <ecsPhys.h>
#include <ecsAudio.h>
#include <ECS/ecsSystems.h>
#include <GameFramework/GameFramework.h>
#include <Input/Controller.h>
#include <RenderObject.h>
#include <GameWorld.h>

using namespace GameEngine;

void GameFramework::Init()
{
	RegisterComponentsReflection();
	RegisterSystems();

	World::GameWorld::GetInstance()->LoadLevel(
		m_World,
		Core::g_FileSystem->GetFilePath("Levels/Main.xml").generic_string()
	);

	flecs::entity camera = m_World.entity()
		.set(Position{ 0.0f, 12.0f, -10.0f })
		.set(Speed{ 10.f })
		.set(CameraPtr{ Core::g_MainCamera })
		.set(ControllerPtr{ new Core::Controller(Core::g_FileSystem->GetConfigPath("Input_default.ini")) });
}

void GameFramework::RegisterComponentsReflection()
{
	m_World.component<Position>()
		.member<float>("x")
		.member<float>("y")
		.member<float>("z");

	m_World.component<Velocity>()
		.member<float>("x")
		.member<float>("y")
		.member<float>("z");

	m_World.component<GeometryPtr>()
		.member<uint64_t>("ptr");

	m_World.component<ControllerPtr>()
		.member<uint64_t>("ptr");

	m_World.component<Gravity>()
		.member<float>("x")
		.member<float>("y")
		.member<float>("z");

	m_World.component<BouncePlane>()
		.member<float>("x")
		.member<float>("y")
		.member<float>("z")
		.member<float>("w");

	m_World.component<Bounciness>()
		.member<float>("value");

	m_World.component<ShiverAmount>()
		.member<float>("value");

	m_World.component<FrictionAmount>()
		.member<float>("value");

	m_World.component<Speed>()
		.member<float>("value");

	m_World.component<JumpSpeed>()
		.member<float>("value");

	m_World.component<TTL>()
		.member<float>("value");

	m_World.component<DieOnGround>()
		.member<bool>("value");

	m_World.component<Collider>()
		.member<bool>("value");

	m_World.component<JumpOnCollision>()
		.member<bool>("value");

	m_World.component<PlaySound>()
		.member<int>("id")
		.member<bool>("loop");
}

void GameFramework::RegisterSystems()
{
	RegisterEcsMeshSystems(m_World);
	RegisterEcsControlSystems(m_World);
	RegisterEcsAudioSystems(m_World);
}

void GameFramework::Update(float dt)
{
	AudioSystem::g_AudioManager->Update(dt);
}