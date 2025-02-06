#include <GUIPlatform.h>
#include <RHIContext.h>
#include <RHIHelper.h>
#include <D3D12GUIRenderBackend.h>
#include <PackedVariables.h>

#include <imgui.h>
#include <backends/imgui_impl_win32.h>

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace GameEngine
{
	namespace GUI
	{
		void GUIContextPlatform::Init()
		{
			IMGUI_CHECKVERSION();
			ImGui::CreateContext();
			ImGuiIO& io = ImGui::GetIO();
			io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
			io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

			ImGui::StyleColorsDark();

			ImGui_ImplWin32_Init(GameEngine::Core::g_MainWindowsApplication->GetWindowHandle());
		}

		void GUIContextPlatform::InitRenderBackend(std::shared_ptr<Render::HAL::RHIContext> rhiContext)
		{
			switch (Render::HAL::RHIHelper::GetRHIType())
			{
			case Render::HAL::RHIType::D3D12:
				D3D12RenderBackend::Init(rhiContext);
				break;
			default:
				assert(false && "Couldn't init the GUI");
				break;
			}
		}

		bool GUIContextPlatform::UpdateInput(Core::PackedVariables& arguments)
		{
			HWND hWnd; UINT msg; WPARAM wParam; LPARAM lParam;
			arguments.Decode(hWnd, msg, wParam, lParam);
			return ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam) != 0;
		}

		void GUIContextPlatform::NewFrame()
		{
			ImGui_ImplWin32_NewFrame();

			switch (Render::HAL::RHIHelper::GetRHIType())
			{
			case Render::HAL::RHIType::D3D12:
				D3D12RenderBackend::NewFrame();
				break;
			default:
				assert(false && "Couldn't init the GUI");
				break;
			}
		}

		void GUIContextPlatform::Render(ImDrawData* drawData)
		{
			switch (Render::HAL::RHIHelper::GetRHIType())
			{
			case Render::HAL::RHIType::D3D12:
				D3D12RenderBackend::Render(drawData);
				break;
			default:
				assert(false && "Couldn't render GUI");
				break;
			}
		}
	}
}