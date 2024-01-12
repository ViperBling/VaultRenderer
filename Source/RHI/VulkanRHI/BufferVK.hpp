#pragma once

#include "RHI/RHICommon.hpp"

#include "MemoryAllocatorVK.hpp"

namespace RHI::Vulkan
{
    class BufferVK
    {
    public:
        BufferVK() = default;
        BufferVK(const BufferVK&) = delete;
        BufferVK& operator=(const BufferVK&) = delete;
        BufferVK(BufferVK&& other) noexcept;
        BufferVK(size_t size, BufferUsage::Value usage, MemoryUsage memoryUsage);
        BufferVK& operator=(BufferVK&& other) noexcept;
        virtual ~BufferVK();

        void Init(size_t size, BufferUsage::Value usage, MemoryUsage memoryUsage);

        vk::Buffer GetNativeBuffer() const { return mBuffer; }
        size_t GetSize() const { return mSize; }

        bool IsMemoryMapped() const;
        uint8_t* MapMemory();
        void UnmapMemory();
        void FlushMemory();
        void FlushMemory(size_t size, size_t offset = 0);
        void CopyData(const uint8_t* data, size_t size, size_t offset = 0);
        void CopyDataWithFlush(const uint8_t* data, size_t size, size_t offset = 0);

    private:
        void Destroy();
        
    private:
        vk::Buffer mBuffer;
        size_t mSize = 0;
        VmaAllocation mAllocation = VK_NULL_HANDLE;
        uint8_t* mpMapped = nullptr;
    };

    using BufferVKReference = std::reference_wrapper<const BufferVK>;

    struct Allocation
    {
        uint32_t Size;
        uint32_t Offset;
    };

    class StageBufferVK
    {
    public:
        StageBufferVK(size_t byteSize);

        Allocation Submit(const uint8_t *data, uint32_t byteSize);
        void Flush();
        void Reset();
        BufferVK &GetBuffer() { return this->mBuffer; }
        const BufferVK &GetBuffer() const { return this->mBuffer; }
        uint32_t GetCurrentOffset() const { return this->mCurrentOffset; }

        template <typename T>
        Allocation Submit(ArrayView<const T> view)
        {
            return this->Submit((const uint8_t *)view.data(), uint32_t(view.size() * sizeof(T)));
        }

        template <typename T>
        Allocation Submit(ArrayView<T> view)
        {
            return this->Submit((const uint8_t *)view.data(), uint32_t(view.size() * sizeof(T)));
        }

        template <typename T>
        Allocation Submit(const T *value)
        {
            return this->Submit((uint8_t *)value, uint32_t(sizeof(T)));
        }
    
    private:
        BufferVK mBuffer;
        uint32_t mCurrentOffset;
    };
}