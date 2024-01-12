#pragma once

#include "Utilities/Utilities.hpp"
#include "RHI/RHIForward.hpp"

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
        RendererBase(Windows::GLFWindow* window) 
            : mWindow(window)
            , mCommandBuffer(nullptr)
        { }

        virtual void InitContext(const RendererCreateInfo& createInfo);
        virtual void RecreateSwapchain(uint32_t surfaceWidth, uint32_t surfaceHeight);
        virtual void BeginFrame();
        virtual void RenderFrame();
        virtual void EndFrame();
        virtual void Cleanup();
        bool IsFrameRunning() const;
        const RHI::Vulkan::ImageVK& AcquireCurrentSwapchainImage(RHI::ImageUsage::Bits usage);
        RHI::Vulkan::CommandBufferVK& GetCurrentCommandBuffer();
        RHI::Vulkan::StageBufferVK& GetCurrentStageBuffer();
        size_t GetVirtualFrameCount() const { return mVirtualFrames.GetFrameCount(); }
        void SubmitCommandsImmediate(const RHI::Vulkan::CommandBufferVK& commands);
        RHI::Vulkan::CommandBufferVK& GetImmediateCommandBuffer();

        const vk::Instance& GetInstance() const { return mInstance; }
        const vk::PhysicalDevice& GetPhysicalDevice() const { return mPhysicalDevice; }
        const vk::Device& GetDevice() const { return mDevice; }
        const vk::Queue& GetDeviceQueue() const { return mDeviceQueue; }
        const vk::CommandPool& GetCommandPool() const { return mCommandPool; }
        const vk::SwapchainKHR& GetSwapchain() const { return mSwapchain; }
        const vk::SurfaceKHR& GetSurface() const { return mSurface; }
        const vk::Semaphore& GetImageAvailableSemaphore() const { return mImageAvailableSemaphore; }
        const vk::Semaphore& GetRenderFinishedSemaphore() const { return mRenderFinishedSemaphore; }
        RHI::Vulkan::DescriptorCacheVK& GetDescriptorCache() { return mDescriptorCache; }
        const VmaAllocator& GetAllocator() const { return mAllocator; }
        bool IsRenderingEnabled() const { return mbRenderingEnabled; }

        const RHI::Vulkan::ImageVK& AcquireSwapchainImage(size_t index, RHI::ImageUsage::Bits usage);
        const RHI::ImageUsage::Bits GetSwapchainImageUsage(size_t index) const;

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
        RHI::Vulkan::CommandBufferVK mCommandBuffer;
        RHI::Vulkan::VirtualFrameProvider mVirtualFrames;
        RHI::Vulkan::DescriptorCacheVK mDescriptorCache;

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

Renderer::RendererBase& GetCurrentRenderer();