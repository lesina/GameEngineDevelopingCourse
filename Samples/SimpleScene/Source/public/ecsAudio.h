#pragma once

#include <flecs.h>
#include <AudioSystem.h>

struct PlaySound
{
	bool loop;
	bool isPlaying = false;
};

struct SoundFilePath
{
	unsigned int id;
};

namespace GameEngine::Audio
{
	struct AudioManagerPtr
	{
		GameEngine::Audio::AudioManager* ptr;
	};
}

void RegisterEcsAudioSystems(flecs::world& world);