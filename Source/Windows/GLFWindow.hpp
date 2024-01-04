#pragma once

#include <vector>
#include <string>
#include <functional>

#include "Renderer/RendererBase.hpp"

#include "Utilities/Math.hpp"
#include "Utilities/KeyCodes.hpp"

struct GLFWwindow;
struct WindowSurface;

namespace Renderer
{
    class RendererBase;
}

bool CheckVulkanPresentationSupport(const vk::Instance& instance, const vk::PhysicalDevice& physicalDevice, uint32_t familyQueueIndex);

namespace Windows
{
    inline void DefaultWindowCallback(const std::string&) { }

    struct WindowCreateInfo
    {
        bool TransparentFramebuffer = false;
        bool Resizeable = true;
        bool TileBar = true;
        std::function<void(const std::string&)> ErrorCallback = DefaultWindowCallback;
        Math::Vector2 Size{ 800.0f, 600.0f };
        Math::Vector2 Position{ 0.0f, 0.0f };
        const char* Title = "LeoRenderer";
    };

    class GLFWindow
    {
    public:
        GLFWwindow* GetNativeHandle() const { return this->mHwnd; }
        std::vector<const char*> GetRequiredExtensions() const;

        GLFWindow(const WindowCreateInfo& options);
        GLFWindow(const GLFWindow&) = delete;
        GLFWindow& operator=(const GLFWindow&) = delete;
        GLFWindow(GLFWindow&& other) noexcept;
        GLFWindow& operator=(GLFWindow&& other) noexcept;
        ~GLFWindow();

        void PollEvents() const;
        bool ShouldClose() const;
        void Close();

        void SetSize(Math::Vector2 size);
        Math::Vector2 GetSize() const;
        void SetPosition(Math::Vector2 position);
        Math::Vector2 GetPosition() const;
        void SetTitle(const char* title);

        Math::Vector2 GetCursorPosition() const;
        void SetCursorPosition(Math::Vector2 position);

        Utilities::CursorMode GetCursorMode() const;
        void SetCursorMode(Utilities::CursorMode mode);

        bool IsKeyPressed(Utilities::KeyCode key) const;
        bool IsKeyReleased(Utilities::KeyCode key) const;

        bool IsMousePressed(Utilities::MouseButton button) const;
        bool IsMouseReleased(Utilities::MouseButton button) const;

        float GetTimeSinceCreation() const;
        void SetTimeSinceCreation(float time);

        void OnResize(std::function<void(GLFWindow&, Math::Vector2)> callback);
        void OnKeyChanged(std::function<void(GLFWindow&, Utilities::KeyCode, bool)> callback);
        void OnMouseChanged(std::function<void(GLFWindow&, Utilities::MouseButton, bool)> callback);

        const WindowSurface& CreateWindowSurface(const Renderer::RendererBase& renderer);

        void SetContext(GLFWwindow* window);

    private:
        GLFWwindow* mHwnd = nullptr;
        std::function<void(GLFWindow&, Math::Vector2)> mOnResize;
        std::function<void(GLFWindow&, Utilities::KeyCode, bool)> mOnKeyChanged;
        std::function<void(GLFWindow&, Utilities::MouseButton, bool)> mOnMouseChanged;
    };
}