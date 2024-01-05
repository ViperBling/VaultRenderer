#include "MemoryAllocatorVK.hpp"

#define VMA_IMPLEMENTATION
#include <vma/vk_mem_alloc.h>

#include "Renderer/RendererBase.hpp"

namespace RHI::Vulkan
{
    VmaMemoryUsage MemoryUsageToNative(MemoryUsage usage)
    {
        constexpr VmaMemoryUsage mappingTable[] = {
            VMA_MEMORY_USAGE_GPU_ONLY,
            VMA_MEMORY_USAGE_CPU_ONLY,
            VMA_MEMORY_USAGE_CPU_TO_GPU,
            VMA_MEMORY_USAGE_GPU_TO_CPU,
            VMA_MEMORY_USAGE_CPU_COPY,
            VMA_MEMORY_USAGE_GPU_LAZILY_ALLOCATED,
        };
        return mappingTable[(size_t)usage];
    }

    VmaAllocator GetVulkanAllocator()
    {
        return GetCurrentRenderer().GetAllocator();
    }

    void DeallocateImage(const vk::Image& image, VmaAllocation allocation)
    {
        vmaDestroyImage(GetVulkanAllocator(), image, allocation);
    }

    void DeallocateBuffer(const vk::Buffer& buffer, VmaAllocation allocation)
    {
        vmaDestroyBuffer(GetVulkanAllocator(), buffer, allocation);
    }

    VmaAllocation AllocateImage(const vk::ImageCreateInfo& imageCreateInfo, MemoryUsage usage, vk::Image* image)
    {
        VmaAllocation allocation = { };
        VmaAllocationCreateInfo allocationInfo = { };
        allocationInfo.usage = MemoryUsageToNative(usage);
        (void)vmaCreateImage(GetVulkanAllocator(), (VkImageCreateInfo*)&imageCreateInfo, &allocationInfo, (VkImage*)image, &allocation, nullptr);
        return allocation;
    }

    VmaAllocation AllocateBuffer(const vk::BufferCreateInfo& bufferCreateInfo, MemoryUsage usage, vk::Buffer* buffer)
    {
        VmaAllocation allocation = { };
        VmaAllocationCreateInfo allocationInfo = { };
        allocationInfo.usage = MemoryUsageToNative(usage);
        (void)vmaCreateBuffer(GetVulkanAllocator(), (VkBufferCreateInfo*)&bufferCreateInfo, &allocationInfo, (VkBuffer*)buffer, &allocation, nullptr);
        return allocation;
    }

    uint8_t* MapMemory(VmaAllocation allocation)
    {
        void* memory = nullptr;
        vmaMapMemory(GetVulkanAllocator(), allocation, &memory);
        return (uint8_t*)memory;
    }

    void UnmapMemory(VmaAllocation allocation)
    {
        vmaUnmapMemory(GetVulkanAllocator(), allocation);
    }

    void FlushMemory(VmaAllocation allocation, size_t byteSize, size_t offset)
    {
        vmaFlushAllocation(GetVulkanAllocator(), allocation, offset, byteSize);
    }
}