#pragma once

#include <Core/export.h>
#include <Camera.h>

namespace GameEngine
{
    namespace Core
    {
        class CORE_API CameraManager final
        {
        public:
            using CameraList = std::list<Camera::Ptr>;
            using It = CameraList::iterator;

            CameraManager()
                : m_CurrCameraIt(m_CameraList.end()) {}

            CameraManager(const CameraManager&) = delete;
            CameraManager& operator=(const CameraManager&) = delete;

            CameraManager(CameraManager&&) = default;
            CameraManager& operator=(CameraManager&&) = default;

        public:
            Camera* CreateCamera();
            void DeleteCamera(Camera* camera);
            Camera* GetActiveCamera();
            void SwitchNextCamera();
            void SwitchPrevCamera();
            void SetActiveCamera(Camera* camera);
			size_t GetCamerasCount() const { return m_CameraList.size(); }

        private:
            void AddCamera(Camera* camera);
            It FindIterator(Camera* camera);

        private:
            CameraList m_CameraList;
            It m_CurrCameraIt;

            inline static Math::Vector3f startCameraPosition = Math::Vector3f(0.0f, 12.0f, -10.0f);
            inline static Math::Vector3f startCameraViewDir = Math::Vector3f(0.0f, -6.0f, 12.0f);
        };

        extern CORE_API std::unique_ptr<CameraManager> g_CameraManager;
    }
}