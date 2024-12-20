#include <AudioEngine.h>
#include <fmod_errors.h>

namespace GameEngine
{
    namespace AudioSystem
    {
        AudioManager* g_AudioManager = nullptr;

        AudioManager::AudioManager()
        {
            InitSystem();
        }

        AudioManager::~AudioManager()
        {
            Cleanup();   
        }

        bool AudioManager::InitSystem()
        {
            FMOD_RESULT result = FMOD_System_Create(&m_System, FMOD_VERSION);
            if (result != FMOD_OK)
            {
                std::cerr << "Failed to create FMOD system: " << FMOD_ErrorString(result) << std::endl;
                return false;
            }

            result = FMOD_System_Init(m_System, 512, FMOD_INIT_NORMAL, nullptr);
            if (result != FMOD_OK)
            {
                std::cerr << "Failed to initialize FMOD system: " << FMOD_ErrorString(result) << std::endl;
                return false;
            }

            return true;
        }

        void AudioManager::Cleanup()
        {
            for (auto& entry : m_ActiveSounds)
            {
                FMOD_Sound_Release(entry.second);
            }
            m_ActiveSounds.clear();
            m_ActiveChannels.clear();

            if (m_System)
            {
                FMOD_System_Close(m_System);
                FMOD_System_Release(m_System);
                m_System = nullptr;
            }
        }

        void AudioManager::SetListenerAttributes(Math::Vector3f pos, Math::Vector3f viewDir, Math::Vector3f upDir)
        {
            m_ListenerPosition = { pos.x,     pos.y,     pos.z };
            m_ListenerViewDir  = { viewDir.x, viewDir.y, viewDir.z };
            m_ListenerUpVec    = { upDir.x,   upDir.y,   upDir.z };
            FMOD_System_Set3DListenerAttributes(m_System, 0, &m_ListenerPosition, nullptr, &m_ListenerViewDir, &m_ListenerUpVec);
        }

        void AudioManager::PlaySound(int soundID, Math::Vector3f pos, bool loop)
        {
            const auto& it = m_SoundIDToFilePath.find(soundID);
            if (it == m_SoundIDToFilePath.end())
            {
                std::cerr << "Sound ID not found: " << soundID << std::endl;
                return;
            }

            const std::string& filepath = it->second;

            FMOD_SOUND* sound = nullptr;
            FMOD_MODE mode = FMOD_3D;
            if (loop)
            {
                mode |= FMOD_LOOP_NORMAL;
            }

            FMOD_RESULT result = FMOD_System_CreateSound(m_System, filepath.c_str(), mode, nullptr, &sound);
            if (result != FMOD_OK)
            {
                std::cerr << "Failed to load sound: " << filepath << " Error: " << FMOD_ErrorString(result) << std::endl;
                return;
            }

            FMOD_CHANNEL* channel = nullptr;
            FMOD_VECTOR position = { pos.x, pos.y, pos.z };
            FMOD_VECTOR velocity = { 0.0f, 0.0f, 0.0f };

            result = FMOD_System_PlaySound(m_System, sound, nullptr, true, &channel);
            if (result != FMOD_OK)
            {
                std::cerr << "Failed to play sound: " << filepath << " Error: " << FMOD_ErrorString(result) << std::endl;
                FMOD_Sound_Release(sound);
                return;
            }

            FMOD_Channel_Set3DAttributes(channel, &position, &velocity);
            FMOD_Channel_SetPaused(channel, false);

            m_ActiveChannels[soundID] = channel;
            m_ActiveSounds  [channel] = sound;
        }

        void AudioManager::UpdateSoundPosition(int soundID, Math::Vector3f pos)
        {
            const auto& it = m_ActiveChannels.find(soundID);
            if (it == m_ActiveChannels.end())
            {
                std::cerr << "Sound ID not found or not playing: " << soundID << std::endl;
                return;
            }

            FMOD_CHANNEL* channel = it->second;
            FMOD_VECTOR position = { pos.x, pos.y, pos.z };
            FMOD_VECTOR velocity = { 0.0f, 0.0f, 0.0f };
            FMOD_Channel_Set3DAttributes(channel, &position, &velocity);
        }

        void AudioManager::Update(float dt)
        {
            for (std::unordered_map<int, FMOD_CHANNEL*>::iterator it = m_ActiveChannels.begin(); it != m_ActiveChannels.end();)
            {
                FMOD_BOOL isPlaying = false;
                FMOD_Channel_IsPlaying(it->second, &isPlaying);

                if (!isPlaying)
                {
                    FMOD_Sound_Release(m_ActiveSounds[it->second]);
                    m_ActiveSounds.erase(it->second);
                    it = m_ActiveChannels.erase(it);
                }
                else
                {
                    ++it;
                }
            }

            FMOD_System_Update(m_System);
        }
    }
}