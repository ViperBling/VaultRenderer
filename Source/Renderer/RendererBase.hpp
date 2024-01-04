#pragma once

#include "Utilities/Utilities.hpp"

#include <iostream>
#include <memory>

#include <vma/vk_mem_alloc.h>
#include <vulkan/vulkan.hpp>

namespace Windows
{
    class GLFWindow;
}

namespace Renderer
{
    struct RendererCreateInfo
    {
        const char* ApplicationName = "Vultana";
        uint32_t Width = 1280;
        uint32_t Height = 720;
        bool bEnableValidationLayers = true;
    };

    class RendererBase
    {
    public:
        NOCOPY(RendererBase)
        RendererBase(Windows::GLFWindow* window) : mWindow(window) { }

        virtual void InitContext(const RendererCreateInfo& createInfo);
        virtual void RecreateSwapchain(uint32_t surfaceWidth, uint32_t surfaceHeight);
        virtual void BeginFrame();
        virtual void RenderFrame();
        virtual void EndFrame();
        virtual void Cleanup();

        const vk::Instance& GetInstance() const { return mInstance; }

    private:
        vk::Instance mInstance;
        vk::SurfaceKHR mSurface;
        vk::SurfaceFormatKHR mSurfaceFormat;
        vk::PresentModeKHR mPresentMode;
        vk::Extent2D mSurfaceExtent;

        vk::PhysicalDevice mPhysicalDevice;
        vk::PhysicalDeviceProperties mPhysicalDeviceProperties;
        vk::Device mDevice;
        vk::Queue mDeviceQueue;
        uint32_t mQueueFamilyIndex;

        vk::Semaphore mImageAvailableSemaphore;
        vk::Semaphore mRenderFinishedSemaphore;
        vk::Fence mImmediateFence;

        vk::CommandPool mCommandPool;

        vk::SwapchainKHR mSwapchain;
        vk::DebugUtilsMessengerEXT mDebugMessenger;
        vk::DispatchLoaderDynamic mDynamicDispatch;

        VmaAllocator mAllocator;

        bool mbRenderingEnabled = true;
        uint8_t mInFlightFrames;
        uint32_t mFrameIndex = 0;

        Windows::GLFWindow* mWindow = nullptr;
    };
}