#pragma once

#include <flecs.h>
#include <AudioEngine.h>

struct PlaySound
{
	int id;
	bool loop = false;
	bool isPlaying = false;
};

void RegisterEcsAudioSystems(flecs::world& world);