#pragma once

#include <AudioEngine/export.h>
#include <fmod.h>
#include <Vector.h>

namespace GameEngine::AudioSystem
{
	class AUDIO_SYSTEM_API AudioManager final
	{
	public:
		AudioManager();
		~AudioManager();

		void SetListenerAttributes(Math::Vector3f pos, Math::Vector3f viewDir, Math::Vector3f upDir);
		void PlaySound(int soundID, Math::Vector3f pos, bool loop = false);
		void UpdateSoundPosition(int soundID, Math::Vector3f pos);

		void Update(float dt);

	private:
		bool InitSystem();
		void Cleanup();

	private:
		FMOD_SYSTEM* m_System = nullptr;
		FMOD_VECTOR m_ListenerPosition = { 0.0f, 0.0f, 0.0f };
		FMOD_VECTOR m_ListenerViewDir  = { 0.0f, 0.0f, 0.0f };
		FMOD_VECTOR m_ListenerUpVec    = { 0.0f, 0.0f, 0.0f };

		std::unordered_map<int, FMOD_CHANNEL*>         m_ActiveChannels;
		std::unordered_map<FMOD_CHANNEL*, FMOD_SOUND*> m_ActiveSounds;

		std::unordered_map<int, std::string> m_SoundIDToFilePath = {
			{1, "sound.mp3"},
			// Add sounds here
		};
	};

	extern AUDIO_SYSTEM_API AudioManager* g_AudioManager;
}