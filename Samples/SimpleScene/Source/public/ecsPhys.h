#pragma once

#include <flecs.h>

struct Position
{
	float x;
	float y;
	float z;
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

struct Timer
{
	float time;
};

struct ToDestroy
{
	float toDestroy;
};

struct Collision
{
	bool isCollided;
};