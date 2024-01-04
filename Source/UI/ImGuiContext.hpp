#pragma once

#include <imgui.h>

namespace vk
{
    class CommandBuffer;
    class RenderPass;
    class ImageView;
    class Sampler;
}

namespace Renderer
{
    class RendererBase;
}

namespace Windows
{
    class GLFWindow;
}

namespace UI
{

    class ImGuiContextVK
    {
    public:
        static void Init(const Windows::GLFWindow& window, const vk::RenderPass& renderPass);
        static void Destroy();
        static void StartFrame();
        static void RenderFrame(const vk::CommandBuffer& commandBuffer);
        // static ImTextureID GetTextureID(const Image& image);
        static ImTextureID GetTextureID(const vk::ImageView& view);
        static void EndFrame();
    };
}

