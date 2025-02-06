#pragma once

#include <World/export.h>

namespace GameEngine::World
{
	class WORLD_API WorldParser final
	{
	public:
		using CustomComponentsMap = std::unordered_map<std::string, uint64_t>;

	public:
		WorldParser() = delete;
		~WorldParser() = delete;

	public:
		static Level ParseLevel(const std::string& xmlPath);

		/*
		* The signature suggests that custom value are always single-valued pointers
		* And we return uint64_t because flecs doesn't know about pointers as a value...
		*/
		static uint64_t ParseCustom(const std::string& componentName, const std::string& componentValue);

	public:
		static CustomComponentsMap GetCustomComponents();

	private:
		static CustomComponentsMap s_CustomComponents;
	};
}