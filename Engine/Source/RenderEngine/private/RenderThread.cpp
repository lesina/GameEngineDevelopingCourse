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
	RenderThread::RenderThread()
	{
		assert(m_CurMainFrame == 0);

		s_MainThreadId = std::this_thread::get_id();

		m_FrameMutexes[m_CurMainFrame].lock();

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
			std::lock_guard<std::mutex> lock(m_FrameMutexes[m_CurrRenderFrame]);

			ProcessCommands();

			m_RenderEngine->Update(m_CurrRenderFrame);

			m_RenderEngine->OnEndFrame();

			OnEndFrame();
		}

		m_ThreadsSynchronizationBarrier.arrive_and_wait();
	}

	void RenderThread::Stop()
	{
		m_FrameMutexes[m_CurMainFrame].unlock();
		m_IsRunning = false;
	}

	bool RenderThread::IsRenderThread()
	{
		return s_RenderThreadId == std::this_thread::get_id();
	}

	void RenderThread::EnqueueCommand(RenderCommand::Task task)
	{
		m_Commands[m_CurMainFrame].emplace_back(new RenderCommand(std::move(task)));

		if (IsRenderThread())
		{
			m_Commands[m_CurMainFrame].front()->DoTask();
			m_Commands[m_CurMainFrame].pop_back();
		}
	}

	void RenderThread::ProcessCommands()
	{
		for (RenderCommand::Ptr& command : m_Commands[m_CurrRenderFrame])
		{
			command->DoTask();
		}

		m_Commands[m_CurrRenderFrame].clear();
	}

	void RenderThread::OnEndFrame()
	{
		SwitchFrame();
	}

	void RenderThread::SwitchFrame()
	{
		if (IsRenderThread())
		{
			m_CurrRenderFrame = GetNextFrame(m_CurrRenderFrame);
		}
		else
		{
			m_FrameMutexes[m_CurMainFrame].unlock();

			m_CurMainFrame = GetNextFrame(m_CurMainFrame);

			m_FrameMutexes[m_CurMainFrame].lock();
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