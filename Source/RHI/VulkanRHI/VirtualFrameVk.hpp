#pragma once

#include "RHI/RHICommon.hpp"
#include "CommandBufferVK.hpp"
#include "BufferVK.hpp"

namespace RHI::Vulkan
{
    class RendererBase;

    struct VirtualFrame
    {
        CommandBufferVK Commands{ vk::CommandBuffer{ } };
        StageBufferVK StagingBuffer;
        vk::Fence CommandQueueFence;
    };

    class VirtualFrameProvider
    {
    public:
        void Init(size_t frameCount, size_t stageBufferSize);
        void Destroy();

        void StartFrame();
        VirtualFrame& GetCurrentFrame();
        VirtualFrame& GetNextFrame();
        const VirtualFrame& GetCurrentFrame() const;
        const VirtualFrame& GetNextFrame() const;
        uint32_t GetPresentImageIndex() const;
        bool IsFrameRunning() const;
        size_t GetFrameCount() const;
        void EndFrame();

    private:
        std::vector<VirtualFrame> mVirtualFrames;
        uint32_t mPresentImageIndex = 0;
        bool mbIsFrameRunning = false;
        size_t mCurrentFrame = 0;
    };
}