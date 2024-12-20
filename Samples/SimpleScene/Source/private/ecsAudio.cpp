#include <memory> 
#include <AudioEngine.h>
#include <ecsAudio.h>
#include <ECS/ecsSystems.h>
#include <ecsPhys.h>

using namespace GameEngine;

void RegisterEcsAudioSystems(flecs::world& world)
{
	world.system<const Position, PlaySound>()
		.each([&](const Position& pos, PlaySound& sound)
	{
		if (!sound.isPlaying)
		{
			sound.isPlaying = true;
			AudioSystem::g_AudioManager->PlaySound(sound.id, Math::Vector3f(pos.x, pos.y, pos.z), sound.loop);
		}
		AudioSystem::g_AudioManager->UpdateSoundPosition(sound.id, Math::Vector3f(pos.x, pos.y, pos.z));
	});
}