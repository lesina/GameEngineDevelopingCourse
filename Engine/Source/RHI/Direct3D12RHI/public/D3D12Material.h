#pragma once

#include <windows.h>
#include <wrl.h>
#include <dxgi1_4.h>
#include <d3d12.h>
#include <D3Dcompiler.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <DirectXColors.h>
#include <DirectXCollision.h>

#include <d3dx12.h>
#include <DDSTextureLoader.h>
#include <RHIMaterial.h>

namespace GameEngine
{
	namespace Render::HAL
	{
		class D3D12Material final : public RHIMaterial
		{
		public:
			D3D12Material() = delete;
			D3D12Material(
				RHIMaterial::ID id
			);
		};
	}
}