#include <CameraManager.h>
#include <ecsControl.h>
#include <Constants.h>
#include <ECS/ecsSystems.h>
#include <ecsPhys.h>
#include <flecs.h>
#include <Input/Controller.h>
#include <Input/InputHandler.h>
#include <Vector.h>

using namespace GameEngine;


static void ProcessButtonPress(const ControllerPtr& controller,
                               const char* actionName,
                               const std::function<void()>& action)
{
	if (controller.ptr->IsPressed(actionName))
	{
		if (!controller.ptr->WasPressed(actionName))
		{
			action();
			controller.ptr->SetWasPressed(actionName);
		}
	}
}

void RegisterEcsControlSystems(flecs::world& world)
{
	world.system<CameraPtr, const Speed, const ControllerPtr>()
		.each([&](flecs::entity e, CameraPtr& cameraPtr, const Speed& speed, const ControllerPtr& controller)
	{
		static const CameraManagerPtr* cameraManagerPtr = world.get<CameraManagerPtr>();

		if (cameraPtr.ptr == cameraManagerPtr->ptr->GetActiveCamera())
		{
			Core::InputHandler::MouseMovevement mouseMovement = Core::InputHandler::GetInstance()->GetMouseMovement();

			mouseMovement.dx *= 0.25 * Math::Constants::PI / 180.f;
			mouseMovement.dy *= 0.25 * Math::Constants::PI / 180.f;

			Core::Camera* camera = cameraPtr.ptr;

			camera->Rotate(mouseMovement.dx, mouseMovement.dy);

			Math::Vector3f currentMoveDir = Math::Vector3f::Zero();
			if (controller.ptr->IsPressed("GoLeft"))
			{
				currentMoveDir = currentMoveDir - camera->GetRightDir();
			}
			if (controller.ptr->IsPressed("GoRight"))
			{
				currentMoveDir = currentMoveDir + camera->GetRightDir();
			}
			if (controller.ptr->IsPressed("GoBack"))
			{
				currentMoveDir = currentMoveDir - camera->GetViewDir();
			}
			if (controller.ptr->IsPressed("GoForward"))
			{
				currentMoveDir = currentMoveDir + camera->GetViewDir();
			}

			Math::Vector3f position = camera->GetPosition() + currentMoveDir.Normalized() * speed.value * world.delta_time();
			camera->SetPosition(position);

			ProcessButtonPress(controller, "CreateCamera",
				[&]() { world.entity().set(CameraPtr{ cameraManagerPtr->ptr->CreateCamera() })
				.set(Speed{ speed.value })
				.set(ControllerPtr{ new Core::Controller(Core::g_FileSystem->GetConfigPath("Input_default.ini")) }); }
			);

			ProcessButtonPress(controller, "NextCamera",
				[&]() { cameraManagerPtr->ptr->SwitchNextCamera(); }
			);
			ProcessButtonPress(controller, "PrevCamera",
				[&]() { cameraManagerPtr->ptr->SwitchPrevCamera(); }
			);

			if (cameraManagerPtr->ptr->GetCamerasCount() > 1)
			{
				ProcessButtonPress(controller, "DeleteCamera",
					[&]() { cameraManagerPtr->ptr->DeleteCamera(camera); e.destruct(); }
				);
			}
		}
	});

	world.system<const Position, Velocity, const ControllerPtr, const BouncePlane, const JumpSpeed>()
		.each([&](const Position& pos, Velocity& vel, const ControllerPtr& controller, const BouncePlane& plane, const JumpSpeed& jump)
	{
		constexpr float planeEpsilon = 0.1f;
		if (plane.x * pos.x + plane.y * pos.y + plane.z * pos.z < plane.w + planeEpsilon)
		{
			if (controller.ptr->IsPressed("Jump"))
			{
				vel.y = jump.value;
			}
		}
	});

	world.system<SavedCameraPtr, const ControllerPtr>()
		.each([&](flecs::entity e, SavedCameraPtr& savedCameraPtr, const ControllerPtr& controller)
	{
		static const CameraManagerPtr* cameraManagerPtr = world.get<CameraManagerPtr>();

		ProcessButtonPress(controller, "SaveCamera",
			[&]() { savedCameraPtr.ptr = cameraManagerPtr->ptr->GetActiveCamera(); }
		);
		ProcessButtonPress(controller, "LoadCamera",
			[&]() { cameraManagerPtr->ptr->SetActiveCamera(savedCameraPtr.ptr); }
		);
	});

	world.system<DeltaFixedCamera>()
		.each([&](flecs::entity e, DeltaFixedCamera& delta)
	{
		static const CameraManagerPtr* cameraManagerPtr = world.get<CameraManagerPtr>();

		if (!e.has(world.lookup("CameraPtr").id()))
		{
			e.set(CameraPtr{ cameraManagerPtr->ptr->CreateCamera() });
		}
	});

	world.system<DeltaFixedCamera, CameraPtr, const Position>()
		.each([&](DeltaFixedCamera& delta, CameraPtr& cameraPtr, const Position& pos)
	{
		if (cameraPtr.ptr)
		{
			cameraPtr.ptr->SetPosition(Math::Vector3f(pos.x + delta.dx, pos.y + delta.dy, pos.z + delta.dz));
		}
	});
}

