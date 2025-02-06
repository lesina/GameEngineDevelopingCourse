#include <Camera.h>
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

		world.system<Position, CameraPtr, const Speed>()
			.each([&](flecs::entity e, Position& position, CameraPtr& camera, const Speed& speed)
				{
					if (!Core::g_MainWindowsApplication->IsMouseCaptured() || !Core::g_MainWindowsApplication->IsFocused()) [[unlikely]]
					{
						return;
					}

					Core::InputHandler::MouseMovevement mouseMovement = Core::InputHandler::GetInstance()->GetMouseMovement();

					mouseMovement.dx *= 0.25 * (Math::Constants::PI / 180.f);
					mouseMovement.dy *= 0.25 * (Math::Constants::PI / 180.f);

					camera.ptr->Rotate(mouseMovement.dx, mouseMovement.dy);

					Math::Vector3f currentMoveDir = Math::Vector3f::Zero();
					if (Core::InputHandler::GetInstance()->IsKeyPressed(Core::KeyboardButton::A))
					{
						currentMoveDir = currentMoveDir - camera.ptr->GetRightDir();
					}
					if (Core::InputHandler::GetInstance()->IsKeyPressed(Core::KeyboardButton::D))
					{
						currentMoveDir = currentMoveDir + camera.ptr->GetRightDir();
					}
					if (Core::InputHandler::GetInstance()->IsKeyPressed(Core::KeyboardButton::S))
					{
						currentMoveDir = currentMoveDir - camera.ptr->GetViewDir();
					}
					if (Core::InputHandler::GetInstance()->IsKeyPressed(Core::KeyboardButton::W))
					{
						currentMoveDir = currentMoveDir + camera.ptr->GetViewDir();
					}
					position.x = position.x + currentMoveDir.Normalized().x * speed.value * world.delta_time();
					position.y = position.y + currentMoveDir.Normalized().y * speed.value * world.delta_time();
					position.z = position.z + currentMoveDir.Normalized().z * speed.value * world.delta_time();
					camera.ptr->SetPosition(Math::Vector3f(position.x, position.y, position.z));
				});
	}
};
