#include <RHIHelper.h>
#include <D3D12RHIContext.h>

namespace GameEngine
{
	namespace Render::HAL
	{
		RHIType RHIHelper::s_rhiType = RHIType::D3D12;

		RHIContext::Ptr RHIHelper::CreateRHI(const std::string& RHIName)
		{
			const auto& it = k_RHITypeMap.find(RHIName);

			assert(it != k_RHITypeMap.end());

			s_rhiType = it->second;

			switch (s_rhiType)
			{
			case RHIType::D3D12:
				return std::make_shared<D3D12RHIContext>();
			default:
				assert(false && std::format("{} is not supported on the current OS", RHIName).c_str());
				return std::make_shared<D3D12RHIContext>();
			}
		}

		RHIType RHIHelper::GetRHIType()
		{
			return s_rhiType;
		}
	}
}