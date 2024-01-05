#include "RendererBase.hpp"
#include "Core/VaultEngine.hpp"
#include "Windows/GLFWindow.hpp"

#include <algorithm>
#include <cstring>
#include <optional>
#include <iostream>

#include <glslang/Public/ShaderLang.h>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

struct WindowSurface
{
    vk::SurfaceKHR Surface;
};

namespace Renderer
{
    #define VK_KRONOS_VALIDATION_LAYER_NAME "VK_LAYER_KHRONOS_validation"

    static VKAPI_ATTR VkBool32 VKAPI_CALL ValidationLayerCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageType,
        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
        void* pUserData) 
    {
        std::cerr << pCallbackData->pMessage << std::endl;
        if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
        {
            #if defined(WIN32)
            __debugbreak();
            #endif
        }
        return VK_FALSE;
    }

    std::optional<uint32_t> DetermineQueueFamilyIndex(const vk::Instance& instance, const vk::PhysicalDevice device, const vk::SurfaceKHR& surface)
    {
        auto queueFamilyProperties = device.getQueueFamilyProperties();
        uint32_t index = 0;
        for (const auto& property : queueFamilyProperties)
        {
            if ((property.queueCount > 0) &&
                (device.getSurfaceSupportKHR(index, surface)) &&
                (CheckVulkanPresentationSupport(instance, device, index)) &&
                (property.queueFlags & vk::QueueFlagBits::eGraphics) &&
                (property.queueFlags & vk::QueueFlagBits::eCompute))
            {
                return index;
            }
            index++;
        }
        return { };
    }

    void RendererBase::InitContext(const RendererCreateInfo &createInfo)
    {
        vk::ApplicationInfo appInfo {};
        appInfo.setPApplicationName(createInfo.ApplicationName);
        appInfo.setApplicationVersion(VK_MAKE_VERSION(1, 3, 0));
        appInfo.setPEngineName(createInfo.ApplicationName);
        appInfo.setEngineVersion(VK_MAKE_VERSION(1, 3, 0));
        appInfo.setApiVersion(VK_API_VERSION_1_3);

        auto supportedExt = vk::enumerateInstanceExtensionProperties();
        auto supportedLayers = vk::enumerateInstanceLayerProperties();

        auto extensions = mWindow->GetRequiredExtensions();
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

        GDebugInfoCallback("Renderer", "Enumerating request extensions: ");
        for (auto &ext : extensions)
        {
            GDebugInfoCallback("Renderer", "- " + std::string(ext));

            auto extIt = std::find_if(supportedExt.begin(), supportedExt.end(),
                                      [ext](const vk::ExtensionProperties &extension)
                                      {
                                          return std::strcmp(extension.extensionName.data(), ext) == 0;
                                      });
            if (extIt == supportedExt.end())
            {
                GDebugInfoCallback("Renderer", "Extension " + std::string(ext) + " is not supported");
                break;
            }
        }

        std::vector<const char *> layers;
        if (createInfo.bEnableValidationLayers)
            layers.push_back(VK_KRONOS_VALIDATION_LAYER_NAME);

        GDebugInfoCallback("Renderer", "Enumerating request layers: ");
        for (auto &layer : layers)
        {
            GDebugInfoCallback("Renderer", "- " + std::string(layer));
            auto layerIt = std::find_if(supportedLayers.begin(), supportedLayers.end(),
                                        [layer](const vk::LayerProperties &properties)
                                        {
                                            return std::strcmp(properties.layerName.data(), layer) == 0;
                                        });
            if (layerIt == supportedLayers.end())
            {
                GDebugInfoCallback("Renderer", "Layer " + std::string(layer) + " is not supported");
                break;
            }
        }

        vk::InstanceCreateInfo instanceCI {};
        instanceCI.setPApplicationInfo(&appInfo);
        instanceCI.setPEnabledExtensionNames(extensions);
        instanceCI.setPEnabledLayerNames(layers);

        mInstance = vk::createInstance(instanceCI);
        GDebugInfoCallback("Renderer", "Created vulkan instance");
        mSurface = mWindow->CreateWindowSurface(*this).Surface;
        GDebugInfoCallback("Renderer", "Created surface");

        auto physicalDevices = mInstance.enumeratePhysicalDevices();
        for (const auto& pd : physicalDevices)
        {
            auto properties = pd.getProperties();
            GDebugInfoCallback("Renderer", "- checking " + std::string(properties.deviceName.data()) + "...");

            auto queueFamilyIndex = DetermineQueueFamilyIndex(mInstance, pd, mSurface);
            if (!queueFamilyIndex.has_value())
            {
                GDebugInfoCallback("Renderer", "  - no suitable queue family found");
                continue;
            }
            mPhysicalDevice = pd;
            mPhysicalDeviceProperties = properties;
            mQueueFamilyIndex = queueFamilyIndex.value();
            if (properties.deviceType == vk::PhysicalDeviceType::eDiscreteGpu) { break; }
        }
        GDebugInfoCallback("Renderer", "Selected physical device: " + std::string(mPhysicalDeviceProperties.deviceName.data()));

        auto presentModes = mPhysicalDevice.getSurfacePresentModesKHR(mSurface);
        auto surfaceFormats = mPhysicalDevice.getSurfaceFormatsKHR(mSurface);
        auto surfaceCapabilities = mPhysicalDevice.getSurfaceCapabilitiesKHR(mSurface);

        mPresentMode = vk::PresentModeKHR::eImmediate;
        if (std::find(presentModes.begin(), presentModes.end(), vk::PresentModeKHR::eMailbox) != presentModes.end()) { mPresentMode = vk::PresentModeKHR::eMailbox; }

        mInFlightFrames = surfaceCapabilities.minImageCount + 1;
        mSurfaceFormat = surfaceFormats.front();
        
        for (const auto& format : surfaceFormats)
        {
            if (format.format == vk::Format::eR8G8B8A8Unorm || format.format == vk::Format::eB8G8R8A8Unorm)
            {
                mSurfaceFormat = format;
                break;
            }
        }
        GDebugInfoCallback("Renderer", "Selected surface format: " + std::string(vk::to_string(mSurfaceFormat.format)));
        GDebugInfoCallback("Renderer", "Selected present mode: " + std::string(vk::to_string(mPresentMode)));

        vk::DeviceQueueCreateInfo deviceQueueCI {};
        std::array queuePriorities = { 1.0f };
        deviceQueueCI.setQueueFamilyIndex(mQueueFamilyIndex);
        deviceQueueCI.setQueuePriorities(queuePriorities);

        auto deviceExtensions = {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME,
            VK_KHR_DEPTH_STENCIL_RESOLVE_EXTENSION_NAME,
            VK_KHR_CREATE_RENDERPASS_2_EXTENSION_NAME,
        };
        auto deviceLayers = {
            VK_KRONOS_VALIDATION_LAYER_NAME,
        };

        vk::PhysicalDeviceVulkan12Features features12 {};
        features12.setBufferDeviceAddress(true);
        features12.setDescriptorIndexing(true);

        vk::DeviceCreateInfo deviceCI {};
        deviceCI.setQueueCreateInfos(deviceQueueCI);
        deviceCI.setPEnabledExtensionNames(deviceExtensions);
        deviceCI.setPEnabledLayerNames(deviceLayers);
        deviceCI.setPNext(&features12);

        mDevice = mPhysicalDevice.createDevice(deviceCI);
        mDeviceQueue = mDevice.getQueue(mQueueFamilyIndex, 0);
        GDebugInfoCallback("Renderer", "Created logical device");

        mDynamicDispatch.init(mInstance, mDevice);

        vk::DebugUtilsMessengerCreateInfoEXT debugMessengerCI {};
        debugMessengerCI.setMessageSeverity(vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning | vk::DebugUtilsMessageSeverityFlagBitsEXT::eError);
        debugMessengerCI.setMessageType(vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation | vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance);
        debugMessengerCI.setPfnUserCallback(ValidationLayerCallback);
        mDebugMessenger = mInstance.createDebugUtilsMessengerEXT(debugMessengerCI, nullptr, mDynamicDispatch);

        VmaAllocatorCreateInfo allocatorCI {};
        allocatorCI.physicalDevice = mPhysicalDevice;
        allocatorCI.device = mDevice;
        allocatorCI.instance = mInstance;
        allocatorCI.vulkanApiVersion = appInfo.apiVersion;
        vmaCreateAllocator(&allocatorCI, &mAllocator);
        GDebugInfoCallback("Renderer", "Created allocator");

        glslang::InitializeProcess();
        GDebugInfoCallback("Renderer", "Initialized glslang");

        RecreateSwapchain(surfaceCapabilities.maxImageExtent.width, surfaceCapabilities.maxImageExtent.height);
        GDebugInfoCallback("Renderer", "Created swapchain");

        mImageAvailableSemaphore = mDevice.createSemaphore({});
        mRenderFinishedSemaphore = mDevice.createSemaphore({});
        mImmediateFence = mDevice.createFence({});

        vk::CommandPoolCreateInfo commandPoolCI;
        commandPoolCI.setQueueFamilyIndex(mQueueFamilyIndex);
        commandPoolCI.setFlags(vk::CommandPoolCreateFlagBits::eResetCommandBuffer | vk::CommandPoolCreateFlagBits::eTransient);
        mCommandPool = mDevice.createCommandPool(commandPoolCI);

        // vk::CommandBufferAllocateInfo commandBufferAI;
        // commandBufferAI.setCommandPool(mCommandPool);
        // commandBufferAI.setLevel(vk::CommandBufferLevel::ePrimary);
        // commandBufferAI.setCommandBufferCount(mInFlightFrames);
        
    }

    void RendererBase::RecreateSwapchain(uint32_t surfaceWidth, uint32_t surfaceHeight)
    {
    }

    void RendererBase::BeginFrame()
    {
    }

    void RendererBase::RenderFrame()
    {
    }

    void RendererBase::EndFrame()
    {
    }

    void RendererBase::Cleanup()
    {
    }
}

Renderer::RendererBase& GetCurrentRenderer()
{
    auto renderer = Core::VaultEngine::GetInstance()->GetRenderer();
    assert(renderer);
    return *renderer;
}