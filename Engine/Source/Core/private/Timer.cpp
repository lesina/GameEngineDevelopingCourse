#include <Timer.h>

namespace GameEngine::Core
{
	Timer::Timer()
	{
		Start();
		Reset();
	}

	void Timer::Start()
	{
		m_bIsStopped = false;
	}

	void Timer::Stop()
	{
		m_bIsStopped = true;
	}

	void Timer::Tick()
	{
		if (m_bIsStopped) [[unlikely]]
		{
			return;
		}

		m_PrevTimePoint = m_CurrTimePoint;
		m_CurrTimePoint = std::chrono::high_resolution_clock::now();
	}

	void Timer::Reset()
	{
		m_PrevTimePoint = m_CurrTimePoint = m_BeginTimePoint = std::chrono::high_resolution_clock::now();
	}

	float Timer::GetDeltaTime() const
	{
		if (m_bIsStopped) [[unlikely]]
		{
			return 0.0f;
		}

		const std::chrono::duration<float, std::ratio<1, 1>> deltaTime = m_CurrTimePoint - m_PrevTimePoint;

		return deltaTime.count();
	}

	float Timer::GetTotalTime() const
	{
		const std::chrono::duration<float, std::ratio<1, 1>> totalTime = m_CurrTimePoint - m_BeginTimePoint;

		return totalTime.count();
	}
}