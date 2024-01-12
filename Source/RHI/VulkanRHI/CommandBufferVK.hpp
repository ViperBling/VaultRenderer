#pragma once

#include "Utilities/Utilities.hpp"
#include "RHI/RHICommon.hpp"

#include "BufferVK.hpp"
#include "ImageVK.hpp"
#include "RenderPassVK.hpp"

namespace RHI::Vulkan
{
    struct NativeRenderPass;

    struct ImageInfo
    {
        ImageVKReference Resource;
        ImageUsage::Bits Usage = ImageUsage::UNKNOWN;
        uint32_t MipLevel = 0;
        uint32_t Layer = 0;
    };

    struct BufferInfo
    {
        BufferVKReference Resource;
        uint32_t Offset = 0;
    };

    class CommandBufferVK
    {
    public:
        CommandBufferVK(vk::CommandBuffer cmdBuffer)
            : mCmdBuffer(std::move(cmdBuffer)) {}
        
        const vk::CommandBuffer& GetNativeCmdBuffer() const { return mCmdBuffer; }
        void Begin();
        void End();
        void BeginPass(const NativeRenderPass& renderPass);
        void EndPass(const NativeRenderPass& renderPass);
        void Draw(uint32_t vertexCount, uint32_t instanceCount);
        void Draw(uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance);
        void DrawIndexed(uint32_t indexCount, uint32_t instanceCount);
        void DrawIndexed(uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, uint32_t vertexOffset, uint32_t firstInstance);
        void BindIndexBufferUInt32(const BufferVK& indexBuffer);
        void BindIndexBufferUInt16(const BufferVK& indexBuffer);
        void SetViewport(const Viewport& viewport);
        void SetScissor(const Rect2D& scissor);
        void SetRenderArea(const ImageVK& image);

        void PushConstants(const NativeRenderPass& renderPass, const uint8_t* data, size_t size);
        void Dispatch(uint32_t x, uint32_t y, uint32_t z);
        
        void CopyImage(const ImageInfo& src, const ImageInfo& dst);
        void CopyBuffer(const BufferInfo& src, const BufferInfo& dst, size_t byteSize);
        void CopyBufferToImage(const BufferInfo& src, const ImageInfo& dst);
        void CopyImageToBuffer(const ImageInfo& src, const BufferInfo& dst);
        
        void BlitImage(const ImageVK& src, ImageUsage::Bits srcUsage, const ImageVK& dst, ImageUsage::Bits dstUsage, BlitFilter filter);
        void GenerateMipLevels(const ImageVK& image, ImageUsage::Bits initialUsage, BlitFilter filter);
    
        void TransferLayout(const ImageVK& image, ImageUsage::Bits oldLayout, ImageUsage::Bits newLayout);
        void TransferLayout(ArrayView<ImageVKReference> images, ImageUsage::Bits oldLayout, ImageUsage::Bits newLayout);
        void TransferLayout(ArrayView<ImageVK> images, ImageUsage::Bits oldLayout, ImageUsage::Bits newLayout);

        template<typename... Buffers>
        void BindVertexBuffers(const Buffers&... vertexBuffers)
        {
            constexpr size_t BufferCount = sizeof...(Buffers);
            std::array buffers = { vertexBuffers.GetNativeBuffer()... };
            uint64_t offsets[BufferCount] = { 0 };
            this->GetNativeCmdBuffer().bindVertexBuffers(0, BufferCount, buffers.data(), offsets);
        }

        template<typename T>
        void PushConstants(const NativeRenderPass& renderPass, ArrayView<const T> constants)
        {
            this->PushConstants(renderPass, (const uint8_t*)constants.data(), constants.size() * sizeof(T));
        }

        template<typename T>
        void PushConstants(const NativeRenderPass& renderPass, const T* constants)
        {
            this->PushConstants(renderPass, (const uint8_t*)constants, sizeof(T));
        }

    private:
        vk::CommandBuffer mCmdBuffer;
    };
}