#pragma once


namespace GameEngine
{
	namespace GUI
	{
		class UIWindow
		{
		public:
			UIWindow() = default;
			~UIWindow() = default;

		public:
			virtual void Draw() = 0;
			virtual void Update(float dt) = 0;
			virtual const char* GetName() const { return "UIWindow"; }
		};
	}
}