#include <CameraManager.h>
#include <Constants.h>
#include <EditorECS/ecsEditor.h>
#include <ECS/ecsSystems.h>
#include <flecs.h>
#include <Input/InputHandler.h>
#include <Vector.h>
#include <Geometry.h>
#include <RenderThread.h>
#include <RenderObject.h>
#include <Window/IWindow.h>

namespace GameEngine::EntitySystem::EditorECS
{
	void RegisterEditorEcsControlSystems(flecs::world& world)
	{
		static const EntitySystem::ECS::RenderThreadPtr* renderThread = world.get<EntitySystem::ECS::RenderThreadPtr>();

		world.system<EntitySystem::ECS::RenderObjectPtr, const Position>()
			.each([&](EntitySystem::ECS::RenderObjectPtr& renderObject, const Position& position)
				{
					if (renderObject.ptr) [[likely]]
					{
						renderObject.ptr->SetPosition(Math::Vector3f(position.x, position.y, position.z), renderThread->ptr->GetMainFrame());
					}
				});

		world.system<CameraPtr, const Speed>()
			.each([&](flecs::entity e, CameraPtr& cameraPtr, const Speed& speed)
				{
					static const CameraManagerPtr* cameraManagerPtr = world.get<CameraManagerPtr>();

					if (cameraPtr.ptr != cameraManagerPtr->ptr->GetActiveCamera() ||
						!Core::g_MainWindowsApplication->IsMouseCaptured() || 
						!Core::g_MainWindowsApplication->IsFocused()) [[unlikely]]
					{
						return;
					}

					Core::InputHandler::MouseMovevement mouseMovement = Core::InputHandler::GetInstance()->GetMouseMovement();

					mouseMovement.dx *= 0.25 * (Math::Constants::PI / 180.f);
					mouseMovement.dy *= 0.25 * (Math::Constants::PI / 180.f);

					Core::Camera* camera = cameraManagerPtr->ptr->GetActiveCamera();

					camera->Rotate(mouseMovement.dx, mouseMovement.dy);

					Math::Vector3f currentMoveDir = Math::Vector3f::Zero();
					if (Core::InputHandler::GetInstance()->IsKeyPressed(Core::KeyboardButton::A))
					{
						currentMoveDir = currentMoveDir - camera->GetRightDir();
					}
					if (Core::InputHandler::GetInstance()->IsKeyPressed(Core::KeyboardButton::D))
					{
						currentMoveDir = currentMoveDir + camera->GetRightDir();
					}
					if (Core::InputHandler::GetInstance()->IsKeyPressed(Core::KeyboardButton::S))
					{
						currentMoveDir = currentMoveDir - camera->GetViewDir();
					}
					if (Core::InputHandler::GetInstance()->IsKeyPressed(Core::KeyboardButton::W))
					{
						currentMoveDir = currentMoveDir + camera->GetViewDir();
					}

					Math::Vector3f position = camera->GetPosition() + currentMoveDir.Normalized() * speed.value * world.delta_time();
					camera->SetPosition(position);
				});
	}
};
