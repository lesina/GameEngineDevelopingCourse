#pragma once

#include <Camera.h>
#include <Constants.h>
#include <Window/IWindow.h>
#include <Window.h>

namespace GameEngine::Core
{
    void OnMouseDown(WPARAM btnState, int x, int y, Window* window)
    {
        window->SetMousePos(x, y);

        SetCapture(GetPlatformWindowHandle(window->GetWindowHandle()));
    }

    void OnMouseUp(WPARAM btnState, int x, int y)
    {
        ReleaseCapture();
    }

    void OnMouseMove(WPARAM btnState, int x, int y, Camera* camera, Window* window)
    {
        Math::Vector2i mousePos = window->GetMousePos();
        if ((btnState & MK_LBUTTON) != 0)
        {
            float dx = 0.25 * static_cast<float>(x - mousePos.x) * Math::Constants::PI / 180.f;
            float dy = 0.25 * static_cast<float>(y - mousePos.y) * Math::Constants::PI / 180.f;

            dy = -dy; // To avoid inverse movement

            camera->Rotate(dx, dy);
        }
        else if ((btnState & MK_RBUTTON) != 0)
        {
            float dx = 0.05f * static_cast<float>(x - mousePos.x);
            float dy = 0.05f * static_cast<float>(y - mousePos.y);

            Math::Vector3f offset = camera->GetViewDir() * (dx - dy);

            Math::Vector3f position = camera->GetPosition();
            position = position + offset;

            camera->SetPosition(position);
        }

        window->SetMousePos(x, y);
    }

    void OnKeyDown(WPARAM btnState, Camera* camera, Window* window)
    {
        Math::Vector3f viewDir = camera->GetViewDir();
        Math::Vector3f up = Math::Vector3f(0.0f, 1.0f, 0.0f);
        Math::Vector3f right = -viewDir.CrossProduct(up).Normalized();

        Math::Vector3f moveDir = (
            viewDir * ((btnState == camera->binds.forward) - (btnState == camera->binds.back)) +
            right * ((btnState == camera->binds.right) - (btnState == camera->binds.left)) +
            up * ((btnState == camera->binds.up) - (btnState == camera->binds.down)) ).Normalized();

        camera->SetSpeed(moveDir * 1.0f);
    }

    void OnKeyUp(WPARAM btnState, Camera* camera)
    {
        camera->SetSpeed(Math::Vector3f(0.0f, 0.0f, 0.0f));
    }
}