#pragma once

#include <GUI/export.h>

#include <RHIContext.h>

struct ImDrawData;

namespace GameEngine
{
	namespace GUI
	{
		class GUI_API D3D12RenderBackend final
		{
		public:
			D3D12RenderBackend() = delete;
			D3D12RenderBackend(D3D12RenderBackend&) = delete;
			D3D12RenderBackend operator=(D3D12RenderBackend&) = delete;

		public:
			static void Init(Render::HAL::RHIContext::Ptr rhiContext);
			static void Render(ImDrawData* drawData);
			static void NewFrame();
		};
	}
}