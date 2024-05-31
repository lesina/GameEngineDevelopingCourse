#pragma once

#include <RenderEngine/export.h>
#include <RenderCommand.h>
#include <RenderCore.h>
#include <Threads.h>

namespace GameEngine::Render
{
	class RenderEngine;

	enum class ERC : uint32_t
	{
		CreateRenderObject = 0,
	};

	class RENDER_ENGINE_API RenderThread final
	{
	public:
		using Ptr = std::unique_ptr<RenderThread>;

	public:
		RenderThread();
		~RenderThread();

	public:
		void Run();

		template<typename... Args>
		void EnqueueCommand(ERC command, Args... args);

		bool IsRenderThread() const;
		void OnEndFrame();

		size_t GetMainFrame() const { return m_CurMainFrame; }

	private:
		void ProcessCommands();
		void SwitchFrame();
		size_t GetNextFrame(size_t frameNumber) const;

		std::jthread::id m_RenderThreadId;
		std::jthread::id m_MainThreadId;
		std::unique_ptr<std::jthread> m_Thread;
		std::mutex frameMutex[RenderCore::g_FrameBufferCount];

		RenderEngine* m_RenderEngine;

		size_t m_CurrRenderFrame = 0;
		size_t m_CurMainFrame = 0;

		std::vector<RenderCommand*> m_commands[RenderCore::g_FrameBufferCount];
	};
}