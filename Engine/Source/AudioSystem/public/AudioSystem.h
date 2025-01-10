#pragma once

#include <AudioSystem/export.h>
#include <fmod.h>
#include <Vector.h>

namespace GameEngine::Audio
{
	class AUDIO_SYSTEM_API AudioManager final
	{
	public:
		AudioManager();
		~AudioManager();

		void SetListenerAttributes(const Math::Vector3f& pos, const Math::Vector3f& viewDir, const Math::Vector3f& upDir);
		void PlaySound(int soundID, const Math::Vector3f& pos, bool loop = false);
		void UpdateSoundPosition(int soundID, const Math::Vector3f& pos);

		void Update(float dt);

	private:
		void InitSystem();
		void Cleanup();

	private:
		FMOD_SYSTEM* m_System = nullptr;
		FMOD_VECTOR m_ListenerPosition = { 0.0f, 0.0f, 0.0f };
		FMOD_VECTOR m_ListenerViewDir  = { 0.0f, 0.0f, 0.0f };
		FMOD_VECTOR m_ListenerUpVec    = { 0.0f, 0.0f, 0.0f };

		std::unordered_map<int, FMOD_CHANNEL*>         m_ActiveChannels;
		std::unordered_map<FMOD_CHANNEL*, FMOD_SOUND*> m_ActiveSounds;
	};
}