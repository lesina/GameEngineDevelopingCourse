#include <AudioSystem.h>
#include <fmod_errors.h>
#include <Parser/WorldParser.h>

namespace GameEngine::Audio
{
    AudioManager::AudioManager()
    {
        InitSystem();
    }

    AudioManager::~AudioManager()
    {
        Cleanup();   
    }

    void AudioManager::InitSystem()
    {
        FMOD_RESULT result = FMOD_System_Create(&m_System, FMOD_VERSION);
        assert(result == FMOD_OK);

        result = FMOD_System_Init(m_System, 512, FMOD_INIT_NORMAL, nullptr);
        assert(result == FMOD_OK);
    }

    void AudioManager::Cleanup()
    {
        for (std::pair<FMOD_CHANNEL*, FMOD_SOUND*> entry : m_ActiveSounds)
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

    void AudioManager::SetListenerAttributes(const Math::Vector3f& pos, const Math::Vector3f& viewDir, const Math::Vector3f& upDir)
    {
        m_ListenerPosition = { pos.x,     pos.y,     pos.z };
        m_ListenerViewDir  = { viewDir.x, viewDir.y, viewDir.z };
        m_ListenerUpVec    = { upDir.x,   upDir.y,   upDir.z };
        FMOD_System_Set3DListenerAttributes(m_System, 0, &m_ListenerPosition, nullptr, &m_ListenerViewDir, &m_ListenerUpVec);
    }

    void AudioManager::PlaySound(int soundID, const Math::Vector3f& pos, bool loop)
    {
        const std::string& filepath = GameEngine::World::WorldParser::GetSoundFilePath(soundID);

        FMOD_SOUND* sound = nullptr;
        FMOD_MODE mode = FMOD_3D;
        if (loop)
        {
            mode |= FMOD_LOOP_NORMAL;
        }

        FMOD_RESULT result = FMOD_System_CreateSound(m_System, filepath.c_str(), mode, nullptr, &sound);
        assert(result == FMOD_OK);

        FMOD_CHANNEL* channel = nullptr;
        FMOD_VECTOR position = { pos.x, pos.y, pos.z };
        FMOD_VECTOR velocity = { 0.0f, 0.0f, 0.0f };

        result = FMOD_System_PlaySound(m_System, sound, nullptr, true, &channel);
        assert(result == FMOD_OK);

        FMOD_Channel_Set3DAttributes(channel, &position, &velocity);
        FMOD_Channel_SetPaused(channel, false);

        m_ActiveChannels[soundID] = channel;
        m_ActiveSounds  [channel] = sound;
    }

    void AudioManager::UpdateSoundPosition(int soundID, const Math::Vector3f& pos)
    {
        const std::unordered_map<int, FMOD_CHANNEL*>::iterator& it = m_ActiveChannels.find(soundID);
        assert(it != m_ActiveChannels.end());

        FMOD_CHANNEL* channel = it->second;
        FMOD_VECTOR position = { pos.x, pos.y, pos.z };
        FMOD_VECTOR velocity = { 0.0f, 0.0f, 0.0f };
        FMOD_Channel_Set3DAttributes(channel, &position, &velocity);
    }

    void AudioManager::Update(float dt)
    {
        std::unordered_map<int, FMOD_CHANNEL*>::iterator it = m_ActiveChannels.begin();
        while (it != m_ActiveChannels.end())
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