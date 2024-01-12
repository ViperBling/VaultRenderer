#include "CommandBufferVK.hpp"
#include "CommonVK.hpp"

namespace RHI::Vulkan
{
    static vk::ImageMemoryBarrier GetImageMemoryBarrier(const ImageVK& image, ImageUsage::Bits oldLayout, ImageUsage::Bits newLayout)
    {
        auto subresourceRange = GetDefaultImageSubresourceRange(image);
        vk::ImageMemoryBarrier barrier;
        barrier
            .setSrcAccessMask(ImageUsageToAccessFlags(oldLayout))
            .setDstAccessMask(ImageUsageToAccessFlags(newLayout))
            .setOldLayout(ImageUsageToImageLayout(oldLayout))
            .setNewLayout(ImageUsageToImageLayout(newLayout))
            .setSrcQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED)
            .setDstQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED)
            .setImage(image.GetNativeHandle())
            .setSubresourceRange(subresourceRange);

        return barrier;
    }

    void CommandBufferVK::Begin()
    {
        vk::CommandBufferBeginInfo cmdBI {};
        cmdBI.setFlags(vk::CommandBufferUsageFlagBits::eOneTimeSubmit);
        mCmdBuffer.begin(cmdBI);
    }

    void CommandBufferVK::End()
    {
        mCmdBuffer.end();
    }

    void CommandBufferVK::BeginPass(const NativeRenderPass &renderPass)
    {
        if (renderPass.RenderPassHandle)
        {
            vk::RenderPassBeginInfo rpBI {};
            rpBI.setRenderPass(renderPass.RenderPassHandle);
            rpBI.setFramebuffer(renderPass.Framebuffer);
            rpBI.setRenderArea(renderPass.RenderArea);
            rpBI.setClearValues(renderPass.ClearValues);

            mCmdBuffer.beginRenderPass(rpBI, vk::SubpassContents::eInline);
        }

        vk::Pipeline pipeline = renderPass.Pipeline;
        vk::PipelineLayout pipelineLayout = renderPass.PipelineLayout;
        vk::PipelineBindPoint pipelineType = renderPass.PipelineType;
        vk::DescriptorSet descriptorSet = renderPass.DescriptorSet;

        if (pipeline) { mCmdBuffer.bindPipeline(pipelineType, pipeline); }
        if (descriptorSet) { mCmdBuffer.bindDescriptorSets(pipelineType, pipelineLayout, 0, descriptorSet, {}); }
    }

    void CommandBufferVK::EndPass(const NativeRenderPass &renderPass)
    {
        if (renderPass.RenderPassHandle)
        {
            mCmdBuffer.endRenderPass();
        }
    }

    void CommandBufferVK::Draw(uint32_t vertexCount, uint32_t instanceCount)
    {
        mCmdBuffer.draw(vertexCount, instanceCount, 0, 0);
    }

    void CommandBufferVK::Draw(uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance)
    {
        mCmdBuffer.draw(vertexCount, instanceCount, firstVertex, firstInstance);
    }

    void CommandBufferVK::DrawIndexed(uint32_t indexCount, uint32_t instanceCount)
    {
        mCmdBuffer.drawIndexed(indexCount, instanceCount, 0, 0, 0);
    }

    void CommandBufferVK::DrawIndexed(uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, uint32_t vertexOffset, uint32_t firstInstance)
    {
        mCmdBuffer.drawIndexed(indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
    }

    void CommandBufferVK::BindIndexBufferUInt32(const BufferVK &indexBuffer)
    {
        mCmdBuffer.bindIndexBuffer(indexBuffer.GetNativeBuffer(), 0, vk::IndexType::eUint32);
    }

    void CommandBufferVK::BindIndexBufferUInt16(const BufferVK &indexBuffer)
    {
        mCmdBuffer.bindIndexBuffer(indexBuffer.GetNativeBuffer(), 0, vk::IndexType::eUint16);
    }

    void CommandBufferVK::SetViewport(const Viewport &viewport)
    {
        vk::Viewport vp {};
        vp.setX(viewport.OffsetWidth);
        vp.setY(viewport.OffsetHeight + viewport.Height);
        vp.setWidth(viewport.Width);
        vp.setHeight(-viewport.Height);
        vp.setMinDepth(viewport.MinDepth);
        vp.setMaxDepth(viewport.MaxDepth);
        mCmdBuffer.setViewport(0, vp);
    }

    void CommandBufferVK::SetScissor(const Rect2D &scissor)
    {
        vk::Rect2D rect {};
        rect.setOffset({ scissor.OffsetWidth, scissor.OffSetHeight });
        rect.setExtent({ scissor.Width, scissor.Height });
        mCmdBuffer.setScissor(0, rect);
    }

    void CommandBufferVK::SetRenderArea(const ImageVK &image)
    {
        SetViewport(Viewport(0, 0, image.GetWidth(), image.GetHeight(), 0.0f, 1.0f));
        SetScissor(Rect2D(0, 0, image.GetWidth(), image.GetHeight()));
    }

    void CommandBufferVK::PushConstants(const NativeRenderPass &renderPass, const uint8_t *data, size_t size)
    {
        constexpr size_t maxPushConstantByteSize = 128;
        std::array<uint8_t, maxPushConstantByteSize> pushConstants {};

        std::memcpy(pushConstants.data(), data, size);

        mCmdBuffer.pushConstants(renderPass.PipelineLayout, PipelineTypeToShaderStages(renderPass.PipelineType), 0, size, pushConstants.data());
    }

    void CommandBufferVK::Dispatch(uint32_t x, uint32_t y, uint32_t z)
    {
        mCmdBuffer.dispatch(x, y, z);
    }

    void CommandBufferVK::CopyImage(const ImageInfo &src, const ImageInfo &dst)
    {
        auto srcRange = GetDefaultImageSubresourceRange(src.Resource.get());
        auto dstRange = GetDefaultImageSubresourceRange(dst.Resource.get());

        std::array<vk::ImageMemoryBarrier, 2> barriers {};
        size_t barrierCount = 0;

        vk::ImageMemoryBarrier toSrcBarrier {};
        toSrcBarrier.setSrcAccessMask(ImageUsageToAccessFlags(src.Usage));
        toSrcBarrier.setDstAccessMask(vk::AccessFlagBits::eTransferRead);
        toSrcBarrier.setOldLayout(ImageUsageToImageLayout(src.Usage));
        toSrcBarrier.setNewLayout(vk::ImageLayout::eTransferSrcOptimal);
        toSrcBarrier.setSrcQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED);
        toSrcBarrier.setDstQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED);
        toSrcBarrier.setImage(src.Resource.get().GetNativeImage());
        toSrcBarrier.setSubresourceRange(srcRange);

        vk::ImageMemoryBarrier toDstBarrier {};
        toDstBarrier.setSrcAccessMask(ImageUsageToAccessFlags(dst.Usage));
        toDstBarrier.setDstAccessMask(vk::AccessFlagBits::eTransferWrite);
        toDstBarrier.setOldLayout(ImageUsageToImageLayout(dst.Usage));
        toDstBarrier.setNewLayout(vk::ImageLayout::eTransferDstOptimal);
        toDstBarrier.setSrcQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED);
        toDstBarrier.setDstQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED);
        toDstBarrier.setImage(dst.Resource.get().GetNativeImage());
        toDstBarrier.setSubresourceRange(dstRange);

        if (src.Usage != ImageUsage::TRANSFER_SOURCE) { barriers[barrierCount++] = toSrcBarrier; }
        if (dst.Usage != ImageUsage::TRANSFER_DESTINATION) { barriers[barrierCount++] = toDstBarrier; }

        if (barrierCount > 0)
        {
            mCmdBuffer.pipelineBarrier(
                ImageUsageToPipelineStage(src.Usage) | ImageUsageToPipelineStage(dst.Usage),
                vk::PipelineStageFlagBits::eTransfer,
                vk::DependencyFlags {},
                {},
                {},
                barriers
            );
        }
        auto srcLayers = GetDefaultImageSubresourceLayers(src.Resource.get(), src.MipLevel, src.Layer);
        auto dstLayers = GetDefaultImageSubresourceLayers(dst.Resource.get(), dst.MipLevel, dst.Layer);

        vk::ImageCopy copyInfo {};
        copyInfo.setSrcSubresource(srcLayers);
        copyInfo.setDstSubresource(dstLayers);
        copyInfo.setSrcOffset(0);
        copyInfo.setDstOffset(0);
        copyInfo.setExtent(vk::Extent3D(dst.Resource.get().GetMipLevelWidth(dst.MipLevel), dst.Resource.get().GetMipLevelHeight(dst.MipLevel), 1));

        mCmdBuffer.copyImage(
            src.Resource.get().GetNativeImage(),
            vk::ImageLayout::eTransferSrcOptimal,
            dst.Resource.get().GetNativeImage(),
            vk::ImageLayout::eTransferDstOptimal,
            copyInfo
        );
    }

    void CommandBufferVK::CopyBuffer(const BufferInfo &src, const BufferInfo &dst, size_t byteSize)
    {
        assert(src.Resource.get().GetSize() >= src.Offset + byteSize);
        assert(dst.Resource.get().GetSize() >= dst.Offset + byteSize);

        vk::BufferCopy bufferCopyInfo {};
        bufferCopyInfo.setSrcOffset(src.Offset);
        bufferCopyInfo.setDstOffset(dst.Offset);
        bufferCopyInfo.setSize(byteSize);

        mCmdBuffer.copyBuffer(src.Resource.get().GetNativeBuffer(), dst.Resource.get().GetNativeBuffer(), bufferCopyInfo);
    }

    void CommandBufferVK::CopyBufferToImage(const BufferInfo &src, const ImageInfo &dst)
    {
        if (dst.Usage != ImageUsage::TRANSFER_DESTINATION)
        {
            auto dstRange = GetDefaultImageSubresourceRange(dst.Resource.get());

            vk::ImageMemoryBarrier toDstBarrier {};
            toDstBarrier.setSrcAccessMask(ImageUsageToAccessFlags(dst.Usage));
            toDstBarrier.setDstAccessMask(vk::AccessFlagBits::eTransferWrite);
            toDstBarrier.setOldLayout(ImageUsageToImageLayout(dst.Usage));
            toDstBarrier.setNewLayout(vk::ImageLayout::eTransferDstOptimal);
            toDstBarrier.setSrcQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED);
            toDstBarrier.setDstQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED);
            toDstBarrier.setImage(dst.Resource.get().GetNativeImage());
            toDstBarrier.setSubresourceRange(dstRange);

            mCmdBuffer.pipelineBarrier(
                ImageUsageToPipelineStage(dst.Usage),
                vk::PipelineStageFlagBits::eTransfer,
                vk::DependencyFlags {},
                {},
                {},
                toDstBarrier
            );
        }

        auto dstLayers = GetDefaultImageSubresourceLayers(dst.Resource.get(), dst.MipLevel, dst.Layer);

        vk::BufferImageCopy bufferImageCopyInfo {};
        bufferImageCopyInfo.setBufferOffset(src.Offset);
        bufferImageCopyInfo.setBufferRowLength(0);
        bufferImageCopyInfo.setBufferImageHeight(0);
        bufferImageCopyInfo.setImageSubresource(dstLayers);
        bufferImageCopyInfo.setImageOffset({ 0, 0, 0 });
        bufferImageCopyInfo.setImageExtent({ dst.Resource.get().GetMipLevelWidth(dst.MipLevel), dst.Resource.get().GetMipLevelHeight(dst.MipLevel), 1 });

        mCmdBuffer.copyBufferToImage(src.Resource.get().GetNativeBuffer(), dst.Resource.get().GetNativeImage(), vk::ImageLayout::eTransferDstOptimal, bufferImageCopyInfo);
    }

    void CommandBufferVK::CopyImageToBuffer(const ImageInfo &src, const BufferInfo &dst)
    {
        if (src.Usage != ImageUsage::TRANSFER_SOURCE)
        {
            auto srcRange = GetDefaultImageSubresourceRange(src.Resource.get());

            vk::ImageMemoryBarrier toSrcBarrier {};
            toSrcBarrier.setSrcAccessMask(ImageUsageToAccessFlags(src.Usage));
            toSrcBarrier.setDstAccessMask(vk::AccessFlagBits::eTransferRead);
            toSrcBarrier.setOldLayout(ImageUsageToImageLayout(src.Usage));
            toSrcBarrier.setNewLayout(vk::ImageLayout::eTransferSrcOptimal);
            toSrcBarrier.setSrcQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED);
            toSrcBarrier.setDstQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED);
            toSrcBarrier.setImage(src.Resource.get().GetNativeImage());
            toSrcBarrier.setSubresourceRange(srcRange);

            mCmdBuffer.pipelineBarrier(
                ImageUsageToPipelineStage(src.Usage),
                vk::PipelineStageFlagBits::eTransfer,
                vk::DependencyFlags {},
                {},
                {},
                toSrcBarrier
            );
        }

        auto srcLayers = GetDefaultImageSubresourceLayers(src.Resource.get(), src.MipLevel, src.Layer);

        vk::BufferImageCopy bufferImageCopyInfo {};
        bufferImageCopyInfo.setBufferOffset(dst.Offset);
        bufferImageCopyInfo.setBufferRowLength(0);
        bufferImageCopyInfo.setBufferImageHeight(0);
        bufferImageCopyInfo.setImageSubresource(srcLayers);
        bufferImageCopyInfo.setImageOffset({ 0, 0, 0 });
        bufferImageCopyInfo.setImageExtent({ src.Resource.get().GetMipLevelWidth(src.MipLevel), src.Resource.get().GetMipLevelHeight(src.MipLevel), 1 });

        mCmdBuffer.copyImageToBuffer(src.Resource.get().GetNativeImage(), vk::ImageLayout::eTransferSrcOptimal, dst.Resource.get().GetNativeBuffer(), bufferImageCopyInfo);
    }

    void CommandBufferVK::BlitImage(const ImageVK &src, ImageUsage::Bits srcUsage, const ImageVK &dst, ImageUsage::Bits dstUsage, BlitFilter filter)
    {
        auto srcRange = GetDefaultImageSubresourceRange(src);
        auto dstRange = GetDefaultImageSubresourceRange(dst);

        std::array<vk::ImageMemoryBarrier, 2> barriers {};
        size_t barrierCount = 0;

        vk::ImageMemoryBarrier toSrcBarrier {};
        toSrcBarrier.setSrcAccessMask(ImageUsageToAccessFlags(srcUsage));
        toSrcBarrier.setDstAccessMask(vk::AccessFlagBits::eTransferRead);
        toSrcBarrier.setOldLayout(ImageUsageToImageLayout(srcUsage));
        toSrcBarrier.setNewLayout(vk::ImageLayout::eTransferSrcOptimal);
        toSrcBarrier.setSrcQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED);
        toSrcBarrier.setDstQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED);
        toSrcBarrier.setImage(src.GetNativeImage());
        toSrcBarrier.setSubresourceRange(srcRange);

        vk::ImageMemoryBarrier toDstBarrier {};
        toDstBarrier.setSrcAccessMask(ImageUsageToAccessFlags(dstUsage));
        toDstBarrier.setDstAccessMask(vk::AccessFlagBits::eTransferWrite);
        toDstBarrier.setOldLayout(ImageUsageToImageLayout(dstUsage));
        toDstBarrier.setNewLayout(vk::ImageLayout::eTransferDstOptimal);
        toDstBarrier.setSrcQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED);
        toDstBarrier.setDstQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED);
        toDstBarrier.setImage(dst.GetNativeImage());
        toDstBarrier.setSubresourceRange(dstRange);

        if (srcUsage != ImageUsage::TRANSFER_SOURCE) { barriers[barrierCount++] = toSrcBarrier; }
        if (dstUsage != ImageUsage::TRANSFER_DESTINATION) { barriers[barrierCount++] = toDstBarrier; }

        if (barrierCount > 0)
        {
            mCmdBuffer.pipelineBarrier(
                ImageUsageToPipelineStage(srcUsage) | ImageUsageToPipelineStage(dstUsage),
                vk::PipelineStageFlagBits::eTransfer,
                vk::DependencyFlags {},
                {},
                {},
                barriers
            );
        }

        auto srcLayers = GetDefaultImageSubresourceLayers(src);
        auto dstLayers = GetDefaultImageSubresourceLayers(dst);

        vk::ImageBlit blitInfo {};
        blitInfo.setSrcSubresource(srcLayers);
        blitInfo.setDstSubresource(dstLayers);
        blitInfo.setSrcOffsets(
            vk::Offset3D(0, 0, 0),
            vk::Offset3D((int32_t)src.GetWidth(), (int32_t)src.GetHeight(), 1)
        );
        blitInfo.setDstOffsets(
            vk::Offset3D(0, 0, 0),
            vk::Offset3D((int32_t)dst.GetWidth(), (int32_t)dst.GetHeight(), 1)
        );

        mCmdBuffer.blitImage(
            src.GetNativeImage(),
            vk::ImageLayout::eTransferSrcOptimal,
            dst.GetNativeImage(),
            vk::ImageLayout::eTransferDstOptimal,
            blitInfo,
            BlitFilterToNative(filter)
        );
    }

    void CommandBufferVK::GenerateMipLevels(const ImageVK &image, ImageUsage::Bits initialUsage, BlitFilter filter)
    {
        if (image.GetMipLevelCount() < 2) { return; }

        auto srcRange = GetDefaultImageSubresourceRange(image);
        auto dstRange = GetDefaultImageSubresourceRange(image);
        auto srcLayer = GetDefaultImageSubresourceLayers(image);
        auto dstLayer = GetDefaultImageSubresourceLayers(image);
        
        auto srcUsage = initialUsage;

        uint32_t srcWidth = image.GetWidth();
        uint32_t srcHeight = image.GetHeight();
        uint32_t dstWidth = image.GetWidth();
        uint32_t dstHeight = image.GetHeight();

        for (size_t i = 0; i + 1 < image.GetMipLevelCount(); i++)
        {
            srcWidth = dstWidth;
            srcHeight = dstHeight;
            dstWidth = std::max(1u, srcWidth / 2);
            dstHeight = std::max(1u, srcHeight / 2);

            srcLayer.setMipLevel(i);
            srcRange.setBaseMipLevel(i);
            srcRange.setLevelCount(1);

            dstLayer.setMipLevel(i + 1);
            dstRange.setBaseMipLevel(i + 1);
            dstRange.setLevelCount(1);

            std::array<vk::ImageMemoryBarrier, 2> barriers {};
            barriers[0] // to transfer source
                .setSrcAccessMask(ImageUsageToAccessFlags(srcUsage))
                .setDstAccessMask(ImageUsageToAccessFlags(ImageUsage::TRANSFER_SOURCE))
                .setOldLayout(ImageUsageToImageLayout(srcUsage))
                .setNewLayout(ImageUsageToImageLayout(ImageUsage::TRANSFER_SOURCE))
                .setSrcQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED)
                .setDstQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED)
                .setImage(image.GetNativeImage())
                .setSubresourceRange(srcUsage);

            barriers[1] // to transfer distance
                .setSrcAccessMask(ImageUsageToAccessFlags(ImageUsage::UNKNOWN))
                .setDstAccessMask(ImageUsageToAccessFlags(ImageUsage::TRANSFER_DESTINATION))
                .setOldLayout(ImageUsageToImageLayout(ImageUsage::UNKNOWN))
                .setNewLayout(ImageUsageToImageLayout(ImageUsage::TRANSFER_DESTINATION))
                .setSrcQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED)
                .setDstQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED)
                .setImage(image.GetNativeImage())
                .setSubresourceRange(dstRange);

            mCmdBuffer.pipelineBarrier(
                vk::PipelineStageFlagBits::eTransfer,
                vk::PipelineStageFlagBits::eTransfer,
                {},
                {},
                {},
                barriers
            );
            srcUsage = ImageUsage::TRANSFER_DESTINATION;

            vk::ImageBlit blitInfo {};
            blitInfo.setSrcSubresource(srcLayer);
            blitInfo.setDstSubresource(dstLayer);
            blitInfo.setSrcOffsets(
                vk::Offset3D(0, 0, 0),
                vk::Offset3D((int32_t)srcWidth, (int32_t)srcHeight, 1)
            );
            blitInfo.setDstOffsets(
                vk::Offset3D(0, 0, 0),
                vk::Offset3D((int32_t)dstWidth, (int32_t)dstHeight, 1)
            );

            mCmdBuffer.blitImage(
                image.GetNativeImage(),
                vk::ImageLayout::eTransferSrcOptimal,
                image.GetNativeImage(),
                vk::ImageLayout::eTransferDstOptimal,
                blitInfo,
                BlitFilterToNative(filter)
            );
        }

        auto mipLevelSubRange = GetDefaultImageSubresourceRange(image);
        mipLevelSubRange.setLevelCount(mipLevelSubRange.levelCount - 1);
        vk::ImageMemoryBarrier mipTransferBarrier {};
        mipTransferBarrier.setSrcAccessMask(vk::AccessFlagBits::eTransferRead);
        mipTransferBarrier.setDstAccessMask(vk::AccessFlagBits::eTransferWrite);
        mipTransferBarrier.setOldLayout(vk::ImageLayout::eTransferSrcOptimal);
        mipTransferBarrier.setNewLayout(vk::ImageLayout::eTransferDstOptimal);
        mipTransferBarrier.setSrcQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED);
        mipTransferBarrier.setDstQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED);
        mipTransferBarrier.setImage(image.GetNativeImage());
        mipTransferBarrier.setSubresourceRange(mipLevelSubRange);

        mCmdBuffer.pipelineBarrier(
            vk::PipelineStageFlagBits::eTransfer,
            vk::PipelineStageFlagBits::eTransfer,
            {},
            {},
            {},
            mipTransferBarrier
        );
    }

    void CommandBufferVK::TransferLayout(const ImageVK &image, ImageUsage::Bits oldLayout, ImageUsage::Bits newLayout)
    {
        auto barrier = GetImageMemoryBarrier(image, oldLayout, newLayout);

        mCmdBuffer.pipelineBarrier(
            ImageUsageToPipelineStage(oldLayout),
            ImageUsageToPipelineStage(newLayout),
            vk::DependencyFlags {},
            {},
            {},
            barrier
        );
    }

    void CommandBufferVK::TransferLayout(ArrayView<ImageVKReference> images, ImageUsage::Bits oldLayout, ImageUsage::Bits newLayout)
    {
        std::vector<vk::ImageMemoryBarrier> barriers {};
        barriers.reserve(images.size());

        for (const auto& image : images)
        {
            barriers.push_back(GetImageMemoryBarrier(image.get(), oldLayout, newLayout));
        }
        mCmdBuffer.pipelineBarrier(
            ImageUsageToPipelineStage(oldLayout),
            ImageUsageToPipelineStage(newLayout),
            vk::DependencyFlags {},
            {},
            {},
            barriers
        );
    }

    void CommandBufferVK::TransferLayout(ArrayView<ImageVK> images, ImageUsage::Bits oldLayout, ImageUsage::Bits newLayout)
    {
        std::vector<vk::ImageMemoryBarrier> barriers {};
        barriers.reserve(images.size());

        for (const auto& image : images)
        {
            barriers.push_back(GetImageMemoryBarrier(image, oldLayout, newLayout));
        }
        mCmdBuffer.pipelineBarrier(
            ImageUsageToPipelineStage(oldLayout),
            ImageUsageToPipelineStage(newLayout),
            vk::DependencyFlags {},
            {},
            {},
            barriers
        );
    }
}