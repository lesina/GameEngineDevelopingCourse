#pragma once

#include <flecs.h>

// This define is essential for the scripts to expose the ECS components to lua syntax
// There is a task to rework this behavior
#undef ECS_META_IMPL
#ifndef GAME_FRAMEWORK
#define ECS_META_IMPL EXTERN // Ensure meta symbols are only defined once
#endif

ECS_STRUCT(Position,
{
	float x;
	float y;
	float z;
});

ECS_STRUCT(Velocity,
{
	float x;
	float y;
	float z;
});

ECS_STRUCT(Gravity,
{
	float x;
	float y;
	float z;
});

ECS_STRUCT(BouncePlane,
{
	float x;
	float y;
	float z;
	float w;
});

ECS_STRUCT(Bounciness,
{
	float value;
});

ECS_STRUCT(ShiverAmount,
{
	float value;
});

ECS_STRUCT(FrictionAmount,
{
	float value;
});

ECS_STRUCT(Speed,
{
	float value;
});

ECS_STRUCT(Timer,
{
	float time;
});

ECS_STRUCT(Obstacle,
{
	bool isCollided;
});