#include <Camera.h>
#include <ecsControl.h>
#include <ECS/ecsSystems.h>
#include <ecsPhys.h>
#include <flecs.h>
#include <Input/Controller.h>
#include <Input/InputHandler.h>
#include <Vector.h>
#include <DefaultGeometry.h>


void RegisterEcsControlSystems(flecs::world& world)
{
	world.system<const ControllerPtr, const BulletStore>()
		.each([&](const ControllerPtr& controller, const BulletStore& firerate) {
		if (controller.ptr->IsPressed("Fire")) {
			world.entity()
				.set(Position{ 3.0,0.0,0.0 })
				.set(Velocity{ 0.0,0.0,0.0 })
				.set(Speed{ 1 })
				.set(GeometryPtr{ GameEngine::RenderCore::DefaultGeometry::Bullet() })
				.set(Lifetime(5.0));
			int x = 0;
		}
	});

	world.system<Lifetime, Position>()
		.each([&](Lifetime& lifetime, Position& position) {
		lifetime.value -= world.delta_time();
	});
	world.system<const ControllerPtr>()
		.each([&](const ControllerPtr& controller) {
		if (controller.ptr->IsPressed("Fire")) {
			std::string new_name = (std::string("Bullet") + std::to_string(std::rand()));
			flecs::entity new_bullet = world.entity(new_name.c_str());
			new_bullet.set(Position{ 0.0,2.0,0.0 });
			new_bullet.set(Velocity{ 0.0,0.0,0.0 });
			new_bullet.set(Speed{ 10 });
			new_bullet.set(Bounciness{ 0.3 });
			new_bullet.set(BulletStore{ 5 });
		}
	});
	
	world.system<Position, CameraPtr, const Speed, const ControllerPtr>()
		.each([&](flecs::entity e, Position& position, CameraPtr& camera, const Speed& speed, const ControllerPtr& controller)
		{
				GameEngine::Math::Vector3f currentMoveDir = GameEngine::Math::Vector3f::Zero();
				if (controller.ptr->IsPressed("GoLeft"))
				{
					currentMoveDir = currentMoveDir - camera.ptr->GetRightDir();
				}
				if (controller.ptr->IsPressed("GoRight"))
				{
					currentMoveDir = currentMoveDir + camera.ptr->GetRightDir();
				}
				if (controller.ptr->IsPressed("GoBack"))
				{
					currentMoveDir = currentMoveDir - camera.ptr->GetViewDir();
				}
				if (controller.ptr->IsPressed("GoForward"))
				{
					currentMoveDir = currentMoveDir + camera.ptr->GetViewDir();
				}
				position.x = position.x + currentMoveDir.Normalized().x * speed.value * world.delta_time();
				position.y = position.y + currentMoveDir.Normalized().y * speed.value * world.delta_time();
				position.z = position.z + currentMoveDir.Normalized().z * speed.value * world.delta_time();
				camera.ptr->SetPosition(GameEngine::Math::Vector3f(position.x, position.y, position.z));
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
}

