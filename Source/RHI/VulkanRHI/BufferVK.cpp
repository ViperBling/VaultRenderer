#include "BufferVK.hpp"

#include "Renderer/RendererBase.hpp"

#include <cassert>

namespace RHI::Vulkan
{
    Buffer::Buffer(Buffer &&other) noexcept
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

    Buffer::Buffer(size_t size, BufferUsage::Value usage, MemoryUsage memoryUsage)
    {
        this->Init(size, usage, memoryUsage);
    }

    Buffer &Buffer::operator=(Buffer &&other) noexcept
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

    Buffer::~Buffer()
    {
        this->Destroy();
    }

    void Buffer::Init(size_t size, BufferUsage::Value usage, MemoryUsage memoryUsage)
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

    bool Buffer::IsMemoryMapped() const
    {
        return this->mpMapped != nullptr;
    }

    uint8_t *Buffer::MapMemory()
    {
        if (this->mpMapped == nullptr)
        {
            this->mpMapped = RHI::Vulkan::MapMemory(this->mAllocation);
        }
        return this->mpMapped;
    }

    void Buffer::UnmapMemory()
    {
        RHI::Vulkan::UnmapMemory(this->mAllocation);
        this->mpMapped = nullptr;
    }

    void Buffer::FlushMemory()
    {
        this->FlushMemory(this->mSize, 0);
    }

    void Buffer::FlushMemory(size_t size, size_t offset)
    {
        RHI::Vulkan::FlushMemory(this->mAllocation, size, offset);
    }

    void Buffer::CopyData(const uint8_t *data, size_t size, size_t offset)
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

    void Buffer::CopyDataWithFlush(const uint8_t *data, size_t size, size_t offset)
    {
        this->CopyData(data, size, offset);
        if (this->IsMemoryMapped())
        {
            this->FlushMemory(size, offset);
        }
    }

    void Buffer::Destroy()
    {
        if (this->mBuffer)
        {
            if (this->mpMapped != nullptr) { this->UnmapMemory(); }
            DeallocateBuffer(this->mBuffer, this->mAllocation);
            this->mBuffer = vk::Buffer();
        }
    }
}