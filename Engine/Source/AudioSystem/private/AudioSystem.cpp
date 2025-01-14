#include <AudioSystem.h>
#include <SoundFilePaths.h>
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
    {
        InitSystem();
    }

    AudioManager::~AudioManager()
    {
        Cleanup();   
    }

    void AudioManager::InitSystem()
    {
        this->data = new AudioManagerData;

        FMOD_RESULT result = FMOD_System_Create(&static_cast<AudioManagerData*>(this->data)->m_System, FMOD_VERSION);
        assert(result == FMOD_OK);

        result = FMOD_System_Init(static_cast<AudioManagerData*>(this->data)->m_System, 512, FMOD_INIT_NORMAL, nullptr);
        assert(result == FMOD_OK);
    }

    void AudioManager::Cleanup()
    {
        for (std::pair<FMOD_CHANNEL*, FMOD_SOUND*> entry : static_cast<AudioManagerData*>(this->data)->m_ActiveSounds)
        {
            FMOD_Sound_Release(entry.second);
        }
        static_cast<AudioManagerData*>(this->data)->m_ActiveSounds.clear();
        static_cast<AudioManagerData*>(this->data)->m_ActiveChannels.clear();

        if (static_cast<AudioManagerData*>(this->data)->m_System)
        {
            FMOD_System_Close(static_cast<AudioManagerData*>(this->data)->m_System);
            FMOD_System_Release(static_cast<AudioManagerData*>(this->data)->m_System);
            static_cast<AudioManagerData*>(this->data)->m_System = nullptr;
        }

        delete static_cast<AudioManagerData*>(this->data);
    }

    void AudioManager::SetListenerAttributes(const Math::Vector3f& pos, const Math::Vector3f& viewDir, const Math::Vector3f& upDir)
    {
        static_cast<AudioManagerData*>(this->data)->m_ListenerPosition = { pos.x,     pos.y,     pos.z };
        static_cast<AudioManagerData*>(this->data)->m_ListenerViewDir  = { viewDir.x, viewDir.y, viewDir.z };
        static_cast<AudioManagerData*>(this->data)->m_ListenerUpVec    = { upDir.x,   upDir.y,   upDir.z };
        FMOD_System_Set3DListenerAttributes(static_cast<AudioManagerData*>(this->data)->m_System, 0, 
                                            &static_cast<AudioManagerData*>(this->data)->m_ListenerPosition, 
                                            nullptr, 
                                            &static_cast<AudioManagerData*>(this->data)->m_ListenerViewDir,
                                            &static_cast<AudioManagerData*>(this->data)->m_ListenerUpVec);
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
            AudioManagerCallbackData* data = nullptr;
            FMOD_Channel_GetUserData(reinterpret_cast<FMOD_CHANNEL*>(channelcontrol), reinterpret_cast<void**>(&data));

            if (data)
            {
                data->audioManager->OnSoundFinished(data->soundID);
            }
        }

        return FMOD_OK;
    }

    void AudioManager::OnSoundFinished(int soundID)
    {
        const std::unordered_map<int, FMOD_CHANNEL*>::iterator& channelIt = static_cast<AudioManagerData*>(this->data)->m_ActiveChannels.find(soundID);
        assert(channelIt != static_cast<AudioManagerData*>(this->data)->m_ActiveChannels.end());

        FMOD_CHANNEL* channel = channelIt->second;

        FMOD_Sound_Release(static_cast<AudioManagerData*>(this->data)->m_ActiveSounds[channel]);

        static_cast<AudioManagerData*>(this->data)->m_ActiveChannels.erase(soundID);
        static_cast<AudioManagerData*>(this->data)->m_ActiveSounds.erase(channel);
    }

    void AudioManager::PlaySound(unsigned int soundID, const Math::Vector3f& pos, bool loop)
    {
        const std::string& filepath = "../../../../../../../Assets/" + SoundFilePaths::GetSoundFilePath(soundID);

        FMOD_SOUND* sound = nullptr;
        FMOD_MODE mode = FMOD_3D;
        if (loop)
        {
            mode |= FMOD_LOOP_NORMAL;
        }

        FMOD_RESULT result = FMOD_System_CreateSound(static_cast<AudioManagerData*>(this->data)->m_System, filepath.c_str(), mode, nullptr, &sound);
        assert(result == FMOD_OK);

        FMOD_CHANNEL* channel = nullptr;
        FMOD_VECTOR position = { pos.x, pos.y, pos.z };
        FMOD_VECTOR velocity = { 0.0f, 0.0f, 0.0f };

        result = FMOD_System_PlaySound(static_cast<AudioManagerData*>(this->data)->m_System, sound, nullptr, true, &channel);
        assert(result == FMOD_OK);

        AudioManagerCallbackData data = { this, soundID };

        FMOD_Channel_SetUserData(channel, &data);
        FMOD_Channel_SetCallback(channel, ChannelCallback);

        FMOD_Channel_Set3DAttributes(channel, &position, &velocity);
        FMOD_Channel_SetPaused(channel, false);

        static_cast<AudioManagerData*>(this->data)->m_ActiveChannels[soundID] = channel;
        static_cast<AudioManagerData*>(this->data)->m_ActiveSounds  [channel] = sound;
    }

    void AudioManager::UpdateSoundPosition(unsigned int soundID, const Math::Vector3f& pos)
    {
        const std::unordered_map<int, FMOD_CHANNEL*>::iterator& it = static_cast<AudioManagerData*>(this->data)->m_ActiveChannels.find(soundID);
        assert(it != static_cast<AudioManagerData*>(this->data)->m_ActiveChannels.end());

        FMOD_CHANNEL* channel = it->second;
        FMOD_VECTOR position = { pos.x, pos.y, pos.z };
        FMOD_VECTOR velocity = { 0.0f, 0.0f, 0.0f };
        FMOD_Channel_Set3DAttributes(channel, &position, &velocity);
    }

    void AudioManager::Update(float dt)
    {
        FMOD_System_Update(static_cast<AudioManagerData*>(this->data)->m_System);
    }
}