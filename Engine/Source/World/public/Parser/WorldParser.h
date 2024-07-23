#pragma once

namespace GameEngine::World
{
	class WorldParser final
	{
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

	private:
		static std::unordered_map<std::string, uint64_t> s_CustomComponents;
	};
}