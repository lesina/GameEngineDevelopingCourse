#include <AudioSystem.h>
#include <AudioDataBase.h>
#include <fmod.h>
#include <fmod_errors.h>

namespace GameEngine::Audio
{
    class AudioManagerData final
    {
    public:
        FMOD_SYSTEM* m_System = nullptr;
        FMOD_VECTOR m_ListenerPosition = { 0.0f, 0.0f, 0.0f };
        FMOD_VECTOR m_ListenerViewDir = { 0.0f, 0.0f, 0.0f };
        FMOD_VECTOR m_ListenerUpVec = { 0.0f, 0.0f, 0.0f };

        std::unordered_map<int, FMOD_CHANNEL*>         m_ActiveChannels;
        std::unordered_map<FMOD_CHANNEL*, FMOD_SOUND*> m_ActiveSounds;
    };

    AudioManager::AudioManager()
        : audioManagerData(new AudioManagerData())
    {
        InitSystem();
    }

    AudioManager::~AudioManager()
    {
        Cleanup();
        delete audioManagerData;
    }

    void AudioManager::InitSystem()
    {
        FMOD_RESULT result = FMOD_System_Create(&audioManagerData->m_System, FMOD_VERSION);
        assert(result == FMOD_OK);

        result = FMOD_System_Init(audioManagerData->m_System, 512, FMOD_INIT_NORMAL, nullptr);
        assert(result == FMOD_OK);
    }

    void AudioManager::Cleanup()
    {
        for (std::pair<FMOD_CHANNEL*, FMOD_SOUND*> entry : audioManagerData->m_ActiveSounds)
        {
            FMOD_Sound_Release(entry.second);
        }
        audioManagerData->m_ActiveSounds.clear();
        audioManagerData->m_ActiveChannels.clear();

        if (audioManagerData->m_System)
        {
            FMOD_System_Close(audioManagerData->m_System);
            FMOD_System_Release(audioManagerData->m_System);
            audioManagerData->m_System = nullptr;
        }
    }

    void AudioManager::SetListenerAttributes(const Math::Vector3f& pos, const Math::Vector3f& viewDir, const Math::Vector3f& upDir)
    {
        audioManagerData->m_ListenerPosition = { pos.x,     pos.y,     pos.z };
        audioManagerData->m_ListenerViewDir  = { viewDir.x, viewDir.y, viewDir.z };
        audioManagerData->m_ListenerUpVec    = { upDir.x,   upDir.y,   upDir.z };
        FMOD_System_Set3DListenerAttributes(audioManagerData->m_System, 0,
                                            &audioManagerData->m_ListenerPosition,
                                            nullptr, 
                                            &audioManagerData->m_ListenerViewDir,
                                            &audioManagerData->m_ListenerUpVec);
    }

    struct AudioManagerCallbackData {
        AudioManager* audioManager;
        unsigned int soundID;
    };

    FMOD_RESULT F_CALLBACK ChannelCallback(
        FMOD_CHANNELCONTROL* channelcontrol,
        FMOD_CHANNELCONTROL_TYPE controltype,
        FMOD_CHANNELCONTROL_CALLBACK_TYPE callbacktype,
        void* commanddata1,
        void* commanddata2)
    {
        if (callbacktype == FMOD_CHANNELCONTROL_CALLBACK_END)
        {
            AudioManagerCallbackData* callbackData = nullptr;
            FMOD_Channel_GetUserData(reinterpret_cast<FMOD_CHANNEL*>(channelcontrol), reinterpret_cast<void**>(&callbackData));

            if (callbackData)
            {
                callbackData->audioManager->OnSoundFinished(callbackData->soundID);
            }
        }

        return FMOD_OK;
    }

    void AudioManager::OnSoundFinished(int soundID)
    {
        const std::unordered_map<int, FMOD_CHANNEL*>::iterator& channelIt = audioManagerData->m_ActiveChannels.find(soundID);
        assert(channelIt != audioManagerData->m_ActiveChannels.end());

        FMOD_CHANNEL* channel = channelIt->second;

        FMOD_Sound_Release(audioManagerData->m_ActiveSounds[channel]);

        audioManagerData->m_ActiveChannels.erase(soundID);
        audioManagerData->m_ActiveSounds.erase(channel);
    }

    void AudioManager::PlaySound(unsigned int soundID, const Math::Vector3f& pos, bool loop)
    {
        const std::string& filepath = "../../../../../../../Assets/" + AudioDataBase::GetSoundFilePath(soundID);

        FMOD_SOUND* sound = nullptr;
        FMOD_MODE mode = FMOD_3D;
        if (loop)
        {
            mode |= FMOD_LOOP_NORMAL;
        }

        FMOD_RESULT result = FMOD_System_CreateSound(audioManagerData->m_System, filepath.c_str(), mode, nullptr, &sound);
        assert(result == FMOD_OK);

        FMOD_CHANNEL* channel = nullptr;
        FMOD_VECTOR position = { pos.x, pos.y, pos.z };
        FMOD_VECTOR velocity = { 0.0f, 0.0f, 0.0f };

        result = FMOD_System_PlaySound(audioManagerData->m_System, sound, nullptr, true, &channel);
        assert(result == FMOD_OK);

        AudioManagerCallbackData callbackData = { this, soundID };

        FMOD_Channel_SetUserData(channel, &callbackData);
        FMOD_Channel_SetCallback(channel, ChannelCallback);

        FMOD_Channel_Set3DAttributes(channel, &position, &velocity);
        FMOD_Channel_SetPaused(channel, false);

        audioManagerData->m_ActiveChannels[soundID] = channel;
        audioManagerData->m_ActiveSounds  [channel] = sound;
    }

    void AudioManager::UpdateSoundPosition(unsigned int soundID, const Math::Vector3f& pos)
    {
        const std::unordered_map<int, FMOD_CHANNEL*>::iterator& it = audioManagerData->m_ActiveChannels.find(soundID);
        assert(it != audioManagerData->m_ActiveChannels.end());

        FMOD_CHANNEL* channel = it->second;
        FMOD_VECTOR position = { pos.x, pos.y, pos.z };
        FMOD_VECTOR velocity = { 0.0f, 0.0f, 0.0f };
        FMOD_Channel_Set3DAttributes(channel, &position, &velocity);
    }

    void AudioManager::Update(float dt)
    {
        FMOD_System_Update(audioManagerData->m_System);
    }
}