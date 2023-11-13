#include <RHI.h>
#include <D3D12RHI.h>

namespace GameEngine
{
	namespace Render::HAL
	{
		RHI::Ptr RHIHelper::CreateRHI(const std::string& RHIName)
		{
			const auto& it = k_RHITypeMap.find(RHIName);

			assert(it != k_RHITypeMap.end());

			switch (it->second)
			{
			case RHIType::D3D12:
				return D3D12RHI::Create();
			default:
				assert(false && std::format("{} is not supported on the current OS", RHIName).c_str());
				return D3D12RHI::Create();
			}
		}
	}
}