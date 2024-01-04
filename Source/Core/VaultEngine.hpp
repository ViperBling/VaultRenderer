#pragma once

#include "Windows/GLFWindow.hpp"
#include "Renderer/RendererBase.hpp"

#include <iostream>

namespace Core
{
    class VaultEngine
    {
    public:
        static VaultEngine* GetInstance();

        void Init(Windows::GLFWindow* window, uint32_t width, uint32_t height);
        void Shutdown();
        void Tick();

        Renderer::RendererBase* GetRenderer() const { return mRenderer.get(); }
        Windows::GLFWindow* GetWindow() const { return mWindow; }

    private:
        Windows::GLFWindow* mWindow = nullptr;
        std::unique_ptr<Renderer::RendererBase> mRenderer;
    };
}