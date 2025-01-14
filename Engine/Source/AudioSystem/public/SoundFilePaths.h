#pragma once

#include <AudioSystem/export.h>

namespace GameEngine::Audio
{
	class AUDIO_SYSTEM_API SoundFilePaths final
	{
	public:
		static const std::string& GetSoundFilePath(unsigned int soundID);
		static void AddSoundFilePath(const std::string& filepath);
		static unsigned int GetSoundFilePathsSize();

	private:
		static std::vector<std::string> s_SoundFilePaths;
	};
}