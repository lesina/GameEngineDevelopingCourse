#include <Camera.h>
#include <DefaultGeometry.h>
#include <Game.h>
#include <GameObject.h>
#include <Windows.h>
#include <array>

namespace GameEngine
{

	Game::Game(
		std::function<bool()> PlatformLoopFunc
	) :
		PlatformLoop(PlatformLoopFunc)
	{
		Core::g_MainCamera = new Core::Camera();
		Core::g_MainCamera->SetPosition(Math::Vector3f(0.0f, 6.0f, -6.0f));
		Core::g_MainCamera->SetViewDir(Math::Vector3f(0.0f, -6.0f, 6.0f).Normalized());

		m_renderThread = std::make_unique<Render::RenderThread>();

		// How many objects do we want to create
		for (int i = 0; i < 3; ++i)
		{
			m_Objects.push_back(new GameObject());
			Render::RenderObject** renderObject = m_Objects.back()->GetRenderObjectRef();
			m_renderThread->EnqueueCommand(Render::ERC::CreateRenderObject, RenderCore::DefaultGeometry::Cube(), renderObject);
		}
	}

	void Game::Run()
	{
		assert(PlatformLoop != nullptr);

		m_GameTimer.Reset();

		bool quit = false;
		while (!quit)
		{
			m_GameTimer.Tick();
			float dt = m_GameTimer.GetDeltaTime();

			Update(dt);
			
			// The most common idea for such a loop is that it returns false when quit is required, or true otherwise
			quit = !PlatformLoop();

			m_renderThread->OnEndFrame();
		}
	}

	bool IsKeyPressed(int keyCode) {
		return GetAsyncKeyState(keyCode) & 0x8000;
	}

	void Game::Update(float dt)
	{
		for (int i = 0; i < m_Objects.size(); ++i)
		{
			float speed = 5.0f;
			Math::Vector3f pos = m_Objects[i]->GetPosition();

			if (i == 0)
			{
				UpdatePositionForObject0(pos, dt, speed);
			}
			else if (i == 1)
			{
				UpdatePositionForObject1(pos, dt, speed);
			}
			else if (i == 2)
			{
				UpdatePositionForObject2(pos, dt, speed);
			}

			m_Objects[i]->SetPosition(pos, m_renderThread->GetMainFrame());
		}
	}
	void Game::UpdatePositionForObject0(Math::Vector3f& pos, float dt, float speed)
	{
		if (GetKeyState('A') & 0x8000) pos.x -= speed * dt;
		if (GetKeyState('D') & 0x8000) pos.x += speed * dt;
		if (GetKeyState('W') & 0x8000) pos.x += speed * dt;
		if (GetKeyState('S') & 0x8000) pos.x -= speed * dt;
	}
	void Game::UpdatePositionForObject1(Math::Vector3f& pos, float dt, float speed)
	{
		if (GetKeyState('A') & 0x8000) pos.x += speed * dt;
		if (GetKeyState('D') & 0x8000) pos.x -= speed * dt;
		if (GetKeyState('W') & 0x8000) pos.y += speed * dt;
		if (GetKeyState('S') & 0x8000) pos.y -= speed * dt;
	}

	void Game::UpdatePositionForObject2(Math::Vector3f& pos, float dt, float speed)
	{
		if (GetKeyState('A') & 0x8000) pos.y -= speed * dt;
		if (GetKeyState('D') & 0x8000) pos.y += speed * dt;
		if (GetKeyState('W') & 0x8000) pos.z += speed * dt;
		if (GetKeyState('S') & 0x8000) pos.z -= speed * dt;
	}
}