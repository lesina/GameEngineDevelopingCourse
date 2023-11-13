#pragma once

#include <RHICommon.h>
#include <RHI/Direct3D12/export.h>

namespace GameEngine
{
	namespace Render::HAL
	{
		class DIRECT3D_API D3D12RHI final : public RHI
		{
		public:
			using Ptr = std::shared_ptr<RHI>;

			// Like a factory
			// RHI - "Vulkan" or "D3D12"
			static D3D12RHI::Ptr Create();

		public:
			virtual void Test() override;

		public:
			D3D12RHI() = default;
		};
	}
}