#pragma once

#include <RenderEngine/export.h>

namespace GameEngine::Render
{
	class RENDER_ENGINE_API RenderCommand
	{
	public:
		virtual void DoTask() = 0;
	};

	template<typename LAMBDA, typename... Args>
	class RENDER_ENGINE_API EnqueuedRenderCommand final : public RenderCommand
	{
	public:
		EnqueuedRenderCommand() = delete;

		EnqueuedRenderCommand(LAMBDA&& InLambda, Args... args) :
			lambda(std::forward<LAMBDA>(InLambda)),
			arguments(std::forward<Args>(args)...)
		{
		}

		virtual void DoTask() override
		{
			std::apply(lambda, arguments);
		}

	private:
		LAMBDA lambda;
		std::tuple<Args...> arguments;
	};
}