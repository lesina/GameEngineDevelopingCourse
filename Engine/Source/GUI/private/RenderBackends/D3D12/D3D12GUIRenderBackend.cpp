#include <D3D12DescriptorHeap.h>
#include <D3D12GUIRenderBackend.h>
#include <D3D12RHIContext.h>
#include <D3D12RHICore.h>
#include <RenderCore.h>

#include <imgui.h>
#include <backends/imgui_impl_dx12.h>

namespace GameEngine::GUI
{
	Render::HAL::RHIContext::Ptr g_RHIContext = nullptr;

	void SrvDescriptorAlloc(ImGui_ImplDX12_InitInfo* info, D3D12_CPU_DESCRIPTOR_HANDLE* out_cpu_handle, D3D12_GPU_DESCRIPTOR_HANDLE* out_gpu_handle)
	{
		assert(g_RHIContext.get());

		Render::HAL::D3D12RHIContext* d3d12RHI = reinterpret_cast<Render::HAL::D3D12RHIContext*>(g_RHIContext.get());
		d3d12RHI->GetSrvHeap()->Alloc(out_cpu_handle, out_gpu_handle);
	}

	void SrvDescriptorFree(ImGui_ImplDX12_InitInfo* info, D3D12_CPU_DESCRIPTOR_HANDLE cpu_handle, D3D12_GPU_DESCRIPTOR_HANDLE gpu_handle)
	{
		assert(g_RHIContext.get());

		Render::HAL::D3D12RHIContext* d3d12RHI = reinterpret_cast<Render::HAL::D3D12RHIContext*>(g_RHIContext.get());
		d3d12RHI->GetSrvHeap()->Free(cpu_handle, gpu_handle);
	}

	void D3D12RenderBackend::Init(Render::HAL::RHIContext::Ptr rhiContext)
	{
		g_RHIContext = rhiContext;

		Render::HAL::D3D12RHIContext* d3d12RHI = reinterpret_cast<Render::HAL::D3D12RHIContext*>(g_RHIContext.get());

		ImGui_ImplDX12_InitInfo init_info = {};
		init_info.Device = (ID3D12Device*)d3d12RHI->GetDevice()->GetNativeObject();
		init_info.CommandQueue = (ID3D12CommandQueue*)d3d12RHI->GetCommandQueue()->GetNativeObject();
		init_info.NumFramesInFlight = RenderCore::g_FrameBufferCount;
		init_info.RTVFormat = Render::HAL::ConvertToDXGIFormat(d3d12RHI->GetSwapChain()->GetCurrentBackBuffer()->GetFormat());
		init_info.DSVFormat = DXGI_FORMAT_UNKNOWN;

		init_info.SrvDescriptorHeap = d3d12RHI->GetSrvHeap()->GetHandle();
		init_info.SrvDescriptorAllocFn = &SrvDescriptorAlloc;
		init_info.SrvDescriptorFreeFn = &SrvDescriptorFree;
		ImGui_ImplDX12_Init(&init_info);
	}

	void D3D12RenderBackend::NewFrame()
	{
		ImGui_ImplDX12_NewFrame();
	}

	void D3D12RenderBackend::Render(ImDrawData* drawData)
	{
		ImGui_ImplDX12_RenderDrawData(drawData, (ID3D12GraphicsCommandList*)g_RHIContext->GetCommandList()->GetNativeObject());
	}
}