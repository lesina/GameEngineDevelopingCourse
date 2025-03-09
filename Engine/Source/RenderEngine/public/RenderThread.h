#pragma once

#include <RenderEngine/export.h>
#include <RenderCommand.h>
#include <RenderCore.h>
#include <Threads.h>

namespace cqe::Render
{
	namespace RHI
	{
		class Context;
	}

	class RenderEngine;

	class RENDER_ENGINE_API RenderThread final
	{
	public:
		using Ptr = std::unique_ptr<RenderThread>;

	public:
		RenderThread();
		~RenderThread();

	public:
		void Run();
		void Stop();

		void EnqueueCommand(RenderCommand::Task task);

		void OnEndFrame();

		static size_t GetMainFrame() { return s_RenderFrame; }
		static size_t GetRenderFrame() { return s_MainFrame; }

		RenderEngine* GetRenderEngine();

		static bool IsRenderThread();

		void WaitForRenderThread();

	private:
		void ProcessCommands();
		void SwitchFrame();
		size_t GetNextFrame(size_t frameNumber) const;

		inline static std::jthread::id s_RenderThreadId;
		inline static std::jthread::id s_MainThreadId;
		std::unique_ptr<std::jthread> m_Thread;
		std::mutex m_FrameMutexes[RenderCore::g_FrameBufferCount];
		std::barrier<> m_ThreadsSynchronizationBarrier{ 2 };

		RenderEngine* m_RenderEngine = nullptr;

		static size_t s_RenderFrame;
		static size_t s_MainFrame;

		std::vector<RenderCommand::Ptr> m_Commands[RenderCore::g_FrameBufferCount];

		bool m_IsRunning : 1 = false;
	};
}