#include <D3D12RHI.h>
#include <windows.h>

namespace GameEngine
{
	namespace Render::HAL
	{
		D3D12RHI::Ptr D3D12RHI::Create()
		{
			return std::make_shared<D3D12RHI>();
		}

		void D3D12RHI::Test()
		{
			OutputDebugString("TEST");
		}
	}
}