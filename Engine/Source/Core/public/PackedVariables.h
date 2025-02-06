#pragma once

#include <Core/export.h>

namespace GameEngine
{
	namespace Core
	{
		class CORE_API PackedVariables final
		{
		public:
			PackedVariables();
			~PackedVariables();

		public:
			template <typename Arg>
			void Encode(Arg arg)
			{
				if (m_currentDataIdx + sizeof(Arg) > m_dataSize) [[unlikely]]
				{
					Resize();
				}

				std::memcpy(m_data + m_currentDataIdx, &arg, sizeof(Arg));
				m_currentDataIdx += sizeof(Arg);
			}

			template <typename FirstArg, typename... Args>
			void Encode(FirstArg firstArg, Args... args)
			{
				Encode(firstArg);
				Encode(args...);

				m_currentDataIdx = 0;
			}

			template <typename Arg>
			void Decode(Arg& argument)
			{
				std::memcpy(&argument, m_data + m_currentDataIdx, sizeof(Arg));
				m_currentDataIdx += sizeof(Arg);
			}

			template <typename FirstArg, typename... Args>
			void Decode(FirstArg& firstArg, Args&... args)
			{
				Decode(firstArg);
				Decode(args...);

				m_currentDataIdx = 0;
			}

		private:
			void Resize();

		private:
			std::byte* m_data = nullptr;
			size_t m_dataSize = 0;
			size_t m_currentDataIdx = 0;
		};
	}
}