#pragma once

#include "RHI/RHICommon.hpp"
#include "ImageVK.hpp"
#include "BufferVK.hpp"

namespace RHI::Vulkan
{
    vk::VertexInputRate VertexBindingRateToVertexInputRate(VertexBinding::Rate rate);
    vk::ShaderStageFlags PipelineTypeToShaderStages(vk::PipelineBindPoint type);
    vk::AttachmentLoadOp AttachmentStateToLoadOp(AttachmentState state);
    ImageUsage::Bits AttachmentStateToImageUsage(AttachmentState state);
    vk::PipelineStageFlags BufferUsageToPipelineStage(BufferUsage::Bits layout);
    vk::AccessFlags BufferUsageToAccessFlags(BufferUsage::Bits layout);
    bool HasImageWriteDependency(ImageUsage::Bits usage);
    bool HasBufferWriteDependency(BufferUsage::Bits usage);
    vk::Filter BlitFilterToNative(BlitFilter filter);

    vk::ImageAspectFlags ImageFormatToImageAspect(Format format);
    vk::ImageLayout ImageUsageToImageLayout(ImageUsage::Bits usage);
    vk::AccessFlags ImageUsageToAccessFlags(ImageUsage::Bits usage);
    vk::PipelineStageFlags ImageUsageToPipelineStage(ImageUsage::Bits usage);

    vk::ImageSubresourceLayers GetDefaultImageSubresourceLayers(const ImageVK& image);
    vk::ImageSubresourceLayers GetDefaultImageSubresourceLayers(const ImageVK& image, uint32_t mipLevel, uint32_t layer);
    vk::ImageSubresourceRange GetDefaultImageSubresourceRange(const ImageVK& image);

    uint32_t CalculateImageMipLevelCount(ImageOptions::Value options, uint32_t width, uint32_t height);
    uint32_t CalculateImageLayerCount(ImageOptions::Value options);

    vk::ImageViewType GetImageViewType(const ImageVK& image);
    vk::ImageMemoryBarrier CreateImageMemoryBarrier(vk::Image image, ImageUsage::Bits oldUsage, ImageUsage::Bits newUsage, Format format, uint32_t mipLevelCount, uint32_t layerCount);

    ImageUsage::Bits UniformTypeToImageUsage(UniformType type);
    BufferUsage::Bits UniformTypeToBufferUsage(UniformType type);

    bool IsBufferType(UniformType type);
}