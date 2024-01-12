#include "BufferVK.hpp"

#include "Renderer/RendererBase.hpp"

#include <cassert>

namespace RHI::Vulkan
{
    BufferVK::BufferVK(BufferVK &&other) noexcept
    {
        this->mBuffer = other.mBuffer;
        this->mSize = other.mSize;
        this->mAllocation = other.mAllocation;
        this->mpMapped = other.mpMapped;

        other.mBuffer = vk::Buffer();
        other.mSize = 0;
        other.mAllocation = {};
        other.mpMapped = nullptr;
    }

    BufferVK::BufferVK(size_t size, BufferUsage::Value usage, MemoryUsage memoryUsage)
    {
        this->Init(size, usage, memoryUsage);
    }

    BufferVK &BufferVK::operator=(BufferVK &&other) noexcept
    {
        this->Destroy();

        this->mBuffer = other.mBuffer;
        this->mSize = other.mSize;
        this->mAllocation = other.mAllocation;
        this->mpMapped = other.mpMapped;

        other.mBuffer = vk::Buffer();
        other.mSize = 0;
        other.mAllocation = {};
        other.mpMapped = nullptr;

        return *this;
    }

    BufferVK::~BufferVK()
    {
        this->Destroy();
    }

    void BufferVK::Init(size_t size, BufferUsage::Value usage, MemoryUsage memoryUsage)
    {
        constexpr std::array bufferQueueFamilyIndices = { VK_QUEUE_FAMILY_IGNORED };
        this->Destroy();

        this->mSize = size;
        vk::BufferCreateInfo bufferCI {};
        bufferCI.setSize(mSize);
        bufferCI.setUsage(static_cast<vk::BufferUsageFlags>(usage));
        bufferCI.setSharingMode(vk::SharingMode::eExclusive);
        bufferCI.setQueueFamilyIndices(bufferQueueFamilyIndices);

        this->mAllocation = AllocateBuffer(bufferCI, memoryUsage, &this->mBuffer);
    }

    bool BufferVK::IsMemoryMapped() const
    {
        return this->mpMapped != nullptr;
    }

    uint8_t *BufferVK::MapMemory()
    {
        if (this->mpMapped == nullptr)
        {
            this->mpMapped = RHI::Vulkan::MapMemory(this->mAllocation);
        }
        return this->mpMapped;
    }

    void BufferVK::UnmapMemory()
    {
        RHI::Vulkan::UnmapMemory(this->mAllocation);
        this->mpMapped = nullptr;
    }

    void BufferVK::FlushMemory()
    {
        this->FlushMemory(this->mSize, 0);
    }

    void BufferVK::FlushMemory(size_t size, size_t offset)
    {
        RHI::Vulkan::FlushMemory(this->mAllocation, size, offset);
    }

    void BufferVK::CopyData(const uint8_t *data, size_t size, size_t offset)
    {
        assert(offset + size <= this->mSize);
        if (this->mpMapped == nullptr)
        {
            (void)this->MapMemory();
            std::memcpy((void*)(this->mpMapped + offset), (const void*)data, size);
            this->FlushMemory(size, offset);
            this->UnmapMemory();
        }
        else 
        {
            std::memcpy((void*)(this->mpMapped + offset), (const void*)data, size);
        }
    }

    void BufferVK::CopyDataWithFlush(const uint8_t *data, size_t size, size_t offset)
    {
        this->CopyData(data, size, offset);
        if (this->IsMemoryMapped())
        {
            this->FlushMemory(size, offset);
        }
    }

    void BufferVK::Destroy()
    {
        if (this->mBuffer)
        {
            if (this->mpMapped != nullptr) { this->UnmapMemory(); }
            DeallocateBuffer(this->mBuffer, this->mAllocation);
            this->mBuffer = vk::Buffer();
        }
    }

    StageBufferVK::StageBufferVK(size_t byteSize)
        : mBuffer(byteSize, BufferUsage::TRANSFER_SOURCE, MemoryUsage::CPUToGPU)
        , mCurrentOffset(0)
    {
        mBuffer.MapMemory();
    }

    Allocation StageBufferVK::Submit(const uint8_t *data, uint32_t byteSize)
    {
        assert(mCurrentOffset + byteSize < mBuffer.GetSize());

        if (data != nullptr)
        {
            mBuffer.CopyData(data, byteSize, mCurrentOffset);
        }
        mCurrentOffset += byteSize;
        return Allocation{ byteSize, mCurrentOffset - byteSize };
    }

    void StageBufferVK::Flush()
    {
        mBuffer.FlushMemory(mCurrentOffset, 0);
    }

    void StageBufferVK::Reset()
    {
        mCurrentOffset = 0;
    }
}