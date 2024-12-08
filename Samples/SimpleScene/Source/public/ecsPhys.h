#pragma once

#include <flecs.h>
#include <Quaternion.h>

struct Position
{
	float x;
	float y;
	float z;
	GameEngine::Math::Quaternion<float> rotation = GameEngine::Math::Quaternion<float>::Identity();
};

struct Velocity
{
	float x;
	float y;
	float z;
};

struct Gravity
{
	float x;
	float y;
	float z;
};

struct BouncePlane
{
	float x;
	float y;
	float z;
	float w;
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

struct Speed
{
	float value;
};

