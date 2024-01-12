#pragma once

#include "RHI/RHICommon.hpp"

#include <cstdint>
#include <cstddef>

struct VmaAllocator_T;
struct VmaAllocation_T;
using VmaAllocator = VmaAllocator_T*;
using VmaAllocation = VmaAllocation_T*;

namespace vk
{
    class Image;
    class Buffer;
    struct ImageCreateInfo;
    struct BufferCreateInfo;
}

namespace Renderer
{
    class RendererBase;
}

namespace RHI::Vulkan
{
    VmaAllocator GetVulkanAllocator();
    void DeallocateImage(const vk::Image& image, VmaAllocation allocation);
    void DeallocateBuffer(const vk::Buffer& buffer, VmaAllocation allocation);
    VmaAllocation AllocateImage(const vk::ImageCreateInfo& imageCreateInfo, MemoryUsage usage, vk::Image* image);
    VmaAllocation AllocateBuffer(const vk::BufferCreateInfo& bufferCreateInfo, MemoryUsage usage, vk::Buffer* buffer);
    uint8_t* MapMemory(VmaAllocation allocation);
    void UnmapMemory(VmaAllocation allocation);
    void FlushMemory(VmaAllocation allocation, size_t byteSize, size_t offset);
}