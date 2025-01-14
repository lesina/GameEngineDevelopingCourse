#include <SoundFilePaths.h>

namespace GameEngine::Audio
{
    std::vector<std::string> SoundFilePaths::s_SoundFilePaths;

    const std::string& SoundFilePaths::GetSoundFilePath(unsigned int soundID)
    {
        return s_SoundFilePaths[soundID];
    }

    unsigned int SoundFilePaths::GetSoundFilePathsSize()
    {
        return s_SoundFilePaths.size();
    }

    void SoundFilePaths::AddSoundFilePath(const std::string& filepath)
    {
        s_SoundFilePaths.push_back(filepath);
    }
}