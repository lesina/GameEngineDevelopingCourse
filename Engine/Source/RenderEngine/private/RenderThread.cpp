#include <RenderEngine.h>
#include <RenderThread.h>

using namespace cqe::Render;

namespace
{
	// Function to run render thread
	static void RunThisThread(void* thisPtr)
	{
		RenderThread* const self = reinterpret_cast<RenderThread*>(thisPtr);
		self->Run();
	}
}

namespace cqe::Render
{
	size_t RenderThread::s_RenderFrame = 0;
	size_t RenderThread::s_MainFrame = 0;

	RenderThread::RenderThread()
	{
		assert(s_MainFrame == 0);

		s_MainThreadId = std::this_thread::get_id();

		m_FrameMutexes[s_MainFrame].lock();

		m_Thread = std::make_unique<std::jthread>(RunThisThread, this);
		m_Thread->detach();
	}

	RenderThread::~RenderThread()
	{
		delete m_RenderEngine;
	}

	// Render Loop
	void RenderThread::Run()
	{
		s_RenderThreadId = std::this_thread::get_id();

		m_RenderEngine = new RenderEngine();

		m_ThreadsSynchronizationBarrier.arrive_and_wait();

		m_IsRunning = true;

		while (m_IsRunning)
		{
			std::lock_guard<std::mutex> lock(m_FrameMutexes[s_RenderFrame]);

			ProcessCommands();

			m_RenderEngine->Update();

			OnEndFrame();
		}

		m_ThreadsSynchronizationBarrier.arrive_and_wait();
	}

	void RenderThread::Stop()
	{
		m_FrameMutexes[s_MainFrame].unlock();
		m_IsRunning = false;
	}

	bool RenderThread::IsRenderThread()
	{
		return s_RenderThreadId == std::this_thread::get_id();
	}

	void RenderThread::EnqueueCommand(RenderCommand::Task task)
	{
		m_Commands[s_MainFrame].emplace_back(new RenderCommand(std::move(task)));

		if (IsRenderThread())
		{
			m_Commands[s_MainFrame].front()->DoTask();
			m_Commands[s_MainFrame].pop_back();
		}
	}

	void RenderThread::ProcessCommands()
	{
		for (RenderCommand::Ptr& command : m_Commands[s_RenderFrame])
		{
			command->DoTask();
		}

		m_Commands[s_RenderFrame].clear();
	}

	void RenderThread::OnEndFrame()
	{
		SwitchFrame();
	}

	void RenderThread::SwitchFrame()
	{
		if (IsRenderThread())
		{
			s_RenderFrame = GetNextFrame(s_RenderFrame);
		}
		else
		{
			m_FrameMutexes[s_MainFrame].unlock();

			s_MainFrame = GetNextFrame(s_MainFrame);

			m_FrameMutexes[s_MainFrame].lock();
		}
	}

	size_t RenderThread::GetNextFrame(size_t frameNumber) const
	{
		return (frameNumber + 1) % RenderCore::g_FrameBufferCount;
	}

	RenderEngine* RenderThread::GetRenderEngine()
	{
		return m_RenderEngine;
	}

	void RenderThread::WaitForRenderThread()
	{
		m_ThreadsSynchronizationBarrier.arrive_and_wait();
	}
}