#include <AudioSystem.h>
#include <ecsAudio.h>
#include <ECS/ecsSystems.h>
#include <ecsPhys.h>

using namespace GameEngine;

void RegisterEcsAudioSystems(flecs::world& world)
{
	const static Audio::AudioManagerPtr* audioManager = world.get<Audio::AudioManagerPtr>();

	world.system<const Position, PlaySound, const SoundFilePath>()
		.each([&](const Position& pos, PlaySound& sound, const SoundFilePath& path)
	{
		if (!sound.isPlaying)
		{
			sound.isPlaying = true;
			audioManager->ptr->PlaySound(path.id, Math::Vector3f(pos.x, pos.y, pos.z), sound.loop);
		}
		audioManager->ptr->UpdateSoundPosition(path.id, Math::Vector3f(pos.x, pos.y, pos.z));
	});
}