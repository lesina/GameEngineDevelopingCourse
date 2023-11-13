#pragma once

#include <RHI/RHI/export.h>

namespace GameEngine
{
	namespace Render::HAL
	{
		enum class RHIType : uint8_t
		{
			D3D12 = 0,
		};

		const std::unordered_map<std::string, RHIType> k_RHITypeMap =
		{
			{"D3D12", RHIType::D3D12},
		};

		class RHI
		{
		public:
			using Ptr = std::shared_ptr<RHI>;

		public:
			virtual void Test() = 0;

		protected:
			RHI() = default;
		};
	}
}