#pragma once

#include <flecs.h>

namespace GameEngine::Core
{
	class CameraManager;
	class Camera;
	class Controller;
}

struct ControllerPtr
{
	GameEngine::Core::Controller* ptr;
};

struct JumpSpeed
{
	float value;
};

struct CameraManagerPtr
{
	GameEngine::Core::CameraManager* ptr;
};

struct CameraPtr
{
	GameEngine::Core::Camera* ptr = nullptr;
};


struct DeltaFixedCamera
{
	float dx;
	float dy;
	float dz;
};

struct SavedCameraPtr
{
	GameEngine::Core::Camera* ptr;
};

void RegisterEcsControlSystems(flecs::world& world);

