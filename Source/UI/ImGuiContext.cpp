#include "ImGuiContext.hpp"

#include "Core/VaultEngine.hpp"
#include "Renderer/RendererBase.hpp"
#include "Windows/GLFWindow.hpp"

#include <imgui_impl_glfw.h>
#include <imgui_impl_vulkan.h>

namespace UI
{
    void ImGuiContextVK::Init(const Windows::GLFWindow &window, const vk::RenderPass &renderPass)
    {
        auto& renderer = *Core::VaultEngine::GetInstance()->GetRenderer();

        ImGui::CreateContext();
        ImGui_ImplGlfw_InitForVulkan(window.GetNativeHandle(), true);
        ImGui_ImplVulkan_InitInfo initInfo = {};
        initInfo.Instance = renderer.GetInstance();
        initInfo.PhysicalDevice = renderer.GetPhysicalDevice();
        initInfo.Device = renderer.GetDevice();
        ImGui_ImplVulkan_Init(&initInfo, renderPass);
    }

    void ImGuiContextVK::Destroy()
    {
    }

    void ImGuiContextVK::StartFrame()
    {
    }

    void ImGuiContextVK::RenderFrame(const vk::CommandBuffer &commandBuffer)
    {
    }

    ImTextureID ImGuiContextVK::GetTextureID(const vk::ImageView &view)
    {
        return ImTextureID();
    }

    void ImGuiContextVK::EndFrame()
    {
    }
}