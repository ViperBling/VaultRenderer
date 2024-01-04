#include "VaultEngine.hpp"

namespace Core
{
    VaultEngine* VaultEngine::GetInstance()
    {
        static VaultEngine instance;
        return &instance;
    }

    void VaultEngine::Init(Windows::GLFWindow *window, uint32_t width, uint32_t height)
    {
        mWindow = window;

        Renderer::RendererCreateInfo rendererCI;
        rendererCI.Width = width;
        rendererCI.Height = height;
        rendererCI.ApplicationName = "Vultana";
        rendererCI.bEnableValidationLayers = true;

        mRenderer = std::make_unique<Renderer::RendererBase>(mWindow);
        mRenderer->InitContext(rendererCI);
    }

    void VaultEngine::Shutdown()
    {
    }

    void VaultEngine::Tick()
    {
    }
}