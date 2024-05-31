#pragma once

#include <flecs.h>
#include <Vector.h>

struct Position
{
	GameEngine::Math::Vector3f value;
};

struct Velocity
{
	GameEngine::Math::Vector3f value;
};

struct Gravity
{
	GameEngine::Math::Vector3f value;
};

struct BouncePlane
{
	GameEngine::Math::Vector4f value;
};

struct Bounciness
{
	float value;
};

struct ShiverAmount
{
	float value;
};

struct FrictionAmount
{
	float value;
};

using Speed = float;

void RegisterEcsPhysSystems(flecs::world& world);

