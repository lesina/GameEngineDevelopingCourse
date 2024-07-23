#pragma once

namespace GameEngine::World
{
	class ComponentParser final
	{
	public:
		using ComponentName = std::string;
		using ComponentValue = std::string;
		using ComponentConstructor = std::function<uint64_t(ComponentValue)>;

	public:
		ComponentParser() = delete;
		~ComponentParser() = delete;

	public:
		static uint64_t Parse(const ComponentName& name, const ComponentValue& value);

	private:
		static std::unordered_map<ComponentName, ComponentConstructor> s_CustomComponentConstructors;
	};
}