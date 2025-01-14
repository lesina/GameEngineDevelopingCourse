#pragma once

#include <AudioSystem/export.h>
#include <Vector.h>

namespace GameEngine::Audio
{
	class AudioManagerData;

	class AUDIO_SYSTEM_API AudioManager final
	{
	public:
		AudioManager();
		~AudioManager();

		void SetListenerAttributes(const Math::Vector3f& pos, const Math::Vector3f& viewDir, const Math::Vector3f& upDir);
		void PlaySound(unsigned int soundID, const Math::Vector3f& pos, bool loop = false);
		void UpdateSoundPosition(unsigned int soundID, const Math::Vector3f& pos);
		void OnSoundFinished(int soundID);

		void Update(float dt);

	private:
		void InitSystem();
		void Cleanup();

	private:
		AudioManagerData* audioManagerData;
	};
}