#pragma once

#include <Core/export.h>

namespace GameEngine
{
	namespace Core
	{
		class CORE_API Timer final
		{
		public:
			Timer();
			~Timer() = default;

		public:
			void Start();
			void Stop();
			void Tick();
			void Reset();
			float GetDeltaTime() const;
			float GetTotalTime() const;

		private:
			bool m_bIsStopped = false;

			std::chrono::high_resolution_clock::time_point m_PrevTimePoint;
			std::chrono::high_resolution_clock::time_point m_CurrTimePoint;
			std::chrono::high_resolution_clock::time_point m_BeginTimePoint;
		};
	}
}