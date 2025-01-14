#include <AudioDataBase.h>

namespace GameEngine::Audio
{
    std::vector<std::string> AudioDataBase::s_SoundFilePaths;

    const std::string& AudioDataBase::GetSoundFilePath(unsigned int soundID)
    {
        return s_SoundFilePaths[soundID];
    }

    unsigned int AudioDataBase::GetSoundFilePathsSize()
    {
        return s_SoundFilePaths.size();
    }

    void AudioDataBase::AddSoundFilePath(const std::string& filepath)
    {
        s_SoundFilePaths.push_back(filepath);
    }
}