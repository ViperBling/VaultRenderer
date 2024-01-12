#include "CommonVK.hpp"

namespace RHI::Vulkan
{
    vk::VertexInputRate VertexBindingRateToVertexInputRate(VertexBinding::Rate rate)
    {
        switch (rate)
        {
        case VertexBinding::Rate::PER_VERTEX:
            return vk::VertexInputRate::eVertex;
        case VertexBinding::Rate::PER_INSTANCE:
            return vk::VertexInputRate::eInstance;
        default:
            assert(false);
            return vk::VertexInputRate::eVertex;
        }
    }

    vk::ShaderStageFlags PipelineTypeToShaderStages(vk::PipelineBindPoint type)
    {
        switch (type)
        {
        case vk::PipelineBindPoint::eGraphics:
            return vk::ShaderStageFlagBits::eVertex | vk::ShaderStageFlagBits::eFragment;
        case vk::PipelineBindPoint::eCompute:
            return vk::ShaderStageFlagBits::eCompute;
        default:
            assert(false);
            return vk::ShaderStageFlags{};
        }
    }

    vk::AttachmentLoadOp AttachmentStateToLoadOp(AttachmentState state)
    {
        switch (state)
        {
        case AttachmentState::DISCARD_COLOR:
            return vk::AttachmentLoadOp::eDontCare;
        case AttachmentState::DISCARD_DEPTH_SPENCIL:
            return vk::AttachmentLoadOp::eDontCare;
        case AttachmentState::LOAD_COLOR:
            return vk::AttachmentLoadOp::eLoad;
        case AttachmentState::LOAD_DEPTH_STENCIL:
            return vk::AttachmentLoadOp::eLoad;
        case AttachmentState::CLEAR_COLOR:
            return vk::AttachmentLoadOp::eClear;
        case AttachmentState::CLEAR_DEPTH_SPENCIL:
            return vk::AttachmentLoadOp::eClear;
        default:
            assert(false);
            return vk::AttachmentLoadOp::eDontCare;
        }
    }

    ImageUsage::Bits AttachmentStateToImageUsage(AttachmentState state)
    {
        switch (state)
        {
        case AttachmentState::DISCARD_COLOR:
            return ImageUsage::COLOR_ATTACHMENT;
        case AttachmentState::DISCARD_DEPTH_SPENCIL:
            return ImageUsage::DEPTH_STENCIL_ATTACHMENT;
        case AttachmentState::LOAD_COLOR:
            return ImageUsage::COLOR_ATTACHMENT;
        case AttachmentState::LOAD_DEPTH_STENCIL:
            return ImageUsage::DEPTH_STENCIL_ATTACHMENT;
        case AttachmentState::CLEAR_COLOR:
            return ImageUsage::COLOR_ATTACHMENT;
        case AttachmentState::CLEAR_DEPTH_SPENCIL:
            return ImageUsage::DEPTH_STENCIL_ATTACHMENT;
        default:
            assert(false);
            return ImageUsage::COLOR_ATTACHMENT;
        }
    }

    vk::PipelineStageFlags BufferUsageToPipelineStage(BufferUsage::Bits layout)
    {
        switch (layout)
        {
        case BufferUsage::UNKNOWN:
            return vk::PipelineStageFlagBits::eTopOfPipe;
        case BufferUsage::TRANSFER_SOURCE:
            return vk::PipelineStageFlagBits::eTransfer;
        case BufferUsage::TRANSFER_DESTINATION:
            return vk::PipelineStageFlagBits::eTransfer;
        case BufferUsage::UNIFORM_TEXEL_BUFFER:
            return vk::PipelineStageFlagBits::eVertexShader; // TODO: from other shader stages?
        case BufferUsage::STORAGE_TEXEL_BUFFER:
            return vk::PipelineStageFlagBits::eComputeShader;
        case BufferUsage::UNIFORM_BUFFER:
            return vk::PipelineStageFlagBits::eVertexShader;
        case BufferUsage::STORAGE_BUFFER:
            return vk::PipelineStageFlagBits::eComputeShader;
        case BufferUsage::INDEX_BUFFER:
            return vk::PipelineStageFlagBits::eVertexInput;
        case BufferUsage::VERTEX_BUFFER:
            return vk::PipelineStageFlagBits::eVertexInput;
        case BufferUsage::INDIRECT_BUFFER:
            return vk::PipelineStageFlagBits::eDrawIndirect;
        case BufferUsage::SHADER_DEVICE_ADDRESS:
            return vk::PipelineStageFlagBits::eFragmentShader; // TODO: what should be here?
        case BufferUsage::TRANSFORM_FEEDBACK_BUFFER:
            return vk::PipelineStageFlagBits::eTransformFeedbackEXT;
        case BufferUsage::TRANSFORM_FEEDBACK_COUNTER_BUFFER:
            return vk::PipelineStageFlagBits::eTransformFeedbackEXT;
        case BufferUsage::CONDITIONAL_RENDERING:
            return vk::PipelineStageFlagBits::eConditionalRenderingEXT;
        case BufferUsage::ACCELERATION_STRUCTURE_BUILD_INPUT_READONLY:
            return vk::PipelineStageFlagBits::eAccelerationStructureBuildKHR;
        case BufferUsage::ACCELERATION_STRUCTURE_STORAGE:
            return vk::PipelineStageFlagBits::eAccelerationStructureBuildKHR; // TODO: what should be here?
        case BufferUsage::SHADER_BINDING_TABLE:
            return vk::PipelineStageFlagBits::eFragmentShader; // TODO: what should be here?
        default:
            assert(false);
            return vk::PipelineStageFlags{};
        }
    }

    vk::AccessFlags BufferUsageToAccessFlags(BufferUsage::Bits layout)
    {
        switch (layout)
        {
        case BufferUsage::UNKNOWN:
            return vk::AccessFlags{};
        case BufferUsage::TRANSFER_SOURCE:
            return vk::AccessFlagBits::eTransferRead;
        case BufferUsage::TRANSFER_DESTINATION:
            return vk::AccessFlagBits::eTransferWrite;
        case BufferUsage::UNIFORM_TEXEL_BUFFER:
            return vk::AccessFlagBits::eShaderRead;
        case BufferUsage::STORAGE_TEXEL_BUFFER:
            return vk::AccessFlagBits::eShaderWrite | vk::AccessFlagBits::eShaderRead;
        case BufferUsage::UNIFORM_BUFFER:
            return vk::AccessFlagBits::eShaderRead;
        case BufferUsage::STORAGE_BUFFER:
            return vk::AccessFlagBits::eShaderWrite | vk::AccessFlagBits::eShaderRead;
        case BufferUsage::INDEX_BUFFER:
            return vk::AccessFlagBits::eIndexRead;
        case BufferUsage::VERTEX_BUFFER:
            return vk::AccessFlagBits::eVertexAttributeRead;
        case BufferUsage::INDIRECT_BUFFER:
            return vk::AccessFlagBits::eIndirectCommandRead;
        case BufferUsage::SHADER_DEVICE_ADDRESS:
            return vk::AccessFlagBits::eShaderRead;
        case BufferUsage::TRANSFORM_FEEDBACK_BUFFER:
            return vk::AccessFlagBits::eTransformFeedbackWriteEXT;
        case BufferUsage::TRANSFORM_FEEDBACK_COUNTER_BUFFER:
            return vk::AccessFlagBits::eTransformFeedbackCounterWriteEXT;
        case BufferUsage::CONDITIONAL_RENDERING:
            return vk::AccessFlagBits::eConditionalRenderingReadEXT;
        case BufferUsage::ACCELERATION_STRUCTURE_BUILD_INPUT_READONLY:
            return vk::AccessFlagBits::eAccelerationStructureReadKHR;
        case BufferUsage::ACCELERATION_STRUCTURE_STORAGE:
            return vk::AccessFlagBits::eAccelerationStructureReadKHR;
        case BufferUsage::SHADER_BINDING_TABLE:
            return vk::AccessFlagBits::eShaderRead;
        default:
            assert(false);
            return vk::AccessFlags{};
        }
    }

    bool HasImageWriteDependency(ImageUsage::Bits usage)
    {
        switch (usage)
        {
        case ImageUsage::TRANSFER_DESTINATION:
        case ImageUsage::STORAGE:
        case ImageUsage::COLOR_ATTACHMENT:
        case ImageUsage::DEPTH_STENCIL_ATTACHMENT:
        case ImageUsage::FRAGMENT_SHADING_RATE_ATTACHMENT:
            return true;
        default:
            return false;
        }
    }

    bool HasBufferWriteDependency(BufferUsage::Bits usage)
    {
        switch (usage)
        {
        case BufferUsage::TRANSFER_DESTINATION:
        case BufferUsage::UNIFORM_TEXEL_BUFFER:
        case BufferUsage::STORAGE_TEXEL_BUFFER:
        case BufferUsage::STORAGE_BUFFER:
        case BufferUsage::TRANSFORM_FEEDBACK_BUFFER:
        case BufferUsage::TRANSFORM_FEEDBACK_COUNTER_BUFFER:
        case BufferUsage::ACCELERATION_STRUCTURE_STORAGE:
            return true;
        default:
            return false;
        }
    }

    vk::Filter BlitFilterToNative(BlitFilter filter)
    {
        switch (filter)
        {
        case BlitFilter::NEAREST:
            return vk::Filter::eNearest;
        case BlitFilter::LINEAR:
            return vk::Filter::eLinear;
        case BlitFilter::CUBIC:
            return vk::Filter::eCubicEXT;
        default:
            assert(false);
            return vk::Filter::eNearest;
        }
    }

    vk::ImageAspectFlags ImageFormatToImageAspect(Format format)
    {
        switch (format)
        {
        case Format::D16_UNORM:
            return vk::ImageAspectFlagBits::eDepth;
        case Format::X8D24_UNORM_PACK_32:
            return vk::ImageAspectFlagBits::eDepth;
        case Format::D32_SFLOAT:
            return vk::ImageAspectFlagBits::eDepth;
        case Format::D16_UNORM_S8_UINT:
            return vk::ImageAspectFlagBits::eDepth | vk::ImageAspectFlagBits::eStencil;
        case Format::D24_UNORM_S8_UINT:
            return vk::ImageAspectFlagBits::eDepth | vk::ImageAspectFlagBits::eStencil;
        case Format::D32_SFLOAT_S8_UINT:
            return vk::ImageAspectFlagBits::eDepth | vk::ImageAspectFlagBits::eStencil;
        default:
            return vk::ImageAspectFlagBits::eColor;
        }
    }

    vk::ImageLayout ImageUsageToImageLayout(ImageUsage::Bits usage)
    {
        switch (usage)
        {
        case ImageUsage::UNKNOWN:
            return vk::ImageLayout::eUndefined;
        case ImageUsage::TRANSFER_SOURCE:
            return vk::ImageLayout::eTransferSrcOptimal;
        case ImageUsage::TRANSFER_DESTINATION:
            return vk::ImageLayout::eTransferDstOptimal;
        case ImageUsage::SHADER_READ:
            return vk::ImageLayout::eShaderReadOnlyOptimal;
        case ImageUsage::STORAGE:
            return vk::ImageLayout::eGeneral;
        case ImageUsage::COLOR_ATTACHMENT:
            return vk::ImageLayout::eColorAttachmentOptimal;
        case ImageUsage::DEPTH_STENCIL_ATTACHMENT:
            return vk::ImageLayout::eDepthStencilAttachmentOptimal;
        case ImageUsage::INPUT_ATTACHMENT:
            return vk::ImageLayout::eAttachmentOptimalKHR; // TODO: is it ok?
        case ImageUsage::FRAGMENT_SHADING_RATE_ATTACHMENT:
            return vk::ImageLayout::eFragmentShadingRateAttachmentOptimalKHR;
        default:
            assert(false);
            return vk::ImageLayout::eUndefined;
        }
    }

    vk::AccessFlags ImageUsageToAccessFlags(ImageUsage::Bits usage)
    {
        switch (usage)
        {
        case ImageUsage::UNKNOWN:
            return vk::AccessFlags{};
        case ImageUsage::TRANSFER_SOURCE:
            return vk::AccessFlagBits::eTransferRead;
        case ImageUsage::TRANSFER_DESTINATION:
            return vk::AccessFlagBits::eTransferWrite;
        case ImageUsage::SHADER_READ:
            return vk::AccessFlagBits::eShaderRead;
        case ImageUsage::STORAGE:
            return vk::AccessFlagBits::eShaderRead | vk::AccessFlagBits::eShaderWrite; // TODO: what if storage is not read or write?
        case ImageUsage::COLOR_ATTACHMENT:
            return vk::AccessFlagBits::eColorAttachmentWrite;
        case ImageUsage::DEPTH_STENCIL_ATTACHMENT:
            return vk::AccessFlagBits::eDepthStencilAttachmentWrite;
        case ImageUsage::INPUT_ATTACHMENT:
            return vk::AccessFlagBits::eInputAttachmentRead;
        case ImageUsage::FRAGMENT_SHADING_RATE_ATTACHMENT:
            return vk::AccessFlagBits::eFragmentShadingRateAttachmentReadKHR;
        default:
            assert(false);
            return vk::AccessFlags{};
        }
    }

    vk::PipelineStageFlags ImageUsageToPipelineStage(ImageUsage::Bits usage)
    {
        switch (usage)
        {
        case ImageUsage::UNKNOWN:
            return vk::PipelineStageFlagBits::eTopOfPipe;
        case ImageUsage::TRANSFER_SOURCE:
            return vk::PipelineStageFlagBits::eTransfer;
        case ImageUsage::TRANSFER_DESTINATION:
            return vk::PipelineStageFlagBits::eTransfer;
        case ImageUsage::SHADER_READ:
            return vk::PipelineStageFlagBits::eFragmentShader; // TODO: whats for vertex shader reads?
        case ImageUsage::STORAGE:
            return vk::PipelineStageFlagBits::eFragmentShader; // TODO: whats for vertex shader reads?
        case ImageUsage::COLOR_ATTACHMENT:
            return vk::PipelineStageFlagBits::eColorAttachmentOutput;
        case ImageUsage::DEPTH_STENCIL_ATTACHMENT:
            return vk::PipelineStageFlagBits::eEarlyFragmentTests; // TODO: whats for late fragment test?
        case ImageUsage::INPUT_ATTACHMENT:
            return vk::PipelineStageFlagBits::eFragmentShader; // TODO: check if at least works
        case ImageUsage::FRAGMENT_SHADING_RATE_ATTACHMENT:
            return vk::PipelineStageFlagBits::eFragmentShadingRateAttachmentKHR;
        default:
            assert(false);
            return vk::PipelineStageFlags{};
        }
    }

    vk::ImageSubresourceLayers GetDefaultImageSubresourceLayers(const ImageVK &image)
    {
        auto subresourceRange = GetDefaultImageSubresourceRange(image);
        return vk::ImageSubresourceLayers{
            subresourceRange.aspectMask,
            subresourceRange.baseMipLevel,
            subresourceRange.baseArrayLayer,
            subresourceRange.layerCount};
    }

    vk::ImageSubresourceLayers GetDefaultImageSubresourceLayers(const ImageVK &image, uint32_t mipLevel, uint32_t layer)
    {
        return vk::ImageSubresourceLayers{
            ImageFormatToImageAspect(image.GetFormat()),
            mipLevel,
            layer,
            1};
    }

    vk::ImageSubresourceRange GetDefaultImageSubresourceRange(const ImageVK &image)
    {
        return vk::ImageSubresourceRange{
            ImageFormatToImageAspect(image.GetFormat()),
            0, // base mip level
            image.GetMipLevelCount(),
            0, // base layer
            image.GetLayerCount()};
    }

    uint32_t CalculateImageMipLevelCount(ImageOptions::Value options, uint32_t width, uint32_t height)
    {
        if (options & ImageOptions::MIPMAPS)
        {
            return (uint32_t)std::floor(std::log2(std::max(width, height))) + 1;
        }
        else
        {
            return 1;
        }
    }

    uint32_t CalculateImageLayerCount(ImageOptions::Value options)
    {
        if (options & ImageOptions::CUBEMAP)
        {
            return 6;
        }
        else
        {
            return 1;
        }
    }

    vk::ImageViewType GetImageViewType(const ImageVK &image)
    {
        if (image.GetLayerCount() == 1)
        {
            return vk::ImageViewType::e2D;
        }
        else
        {
            return vk::ImageViewType::eCube;
        }
    }

    vk::ImageMemoryBarrier CreateImageMemoryBarrier(vk::Image image, ImageUsage::Bits oldUsage, ImageUsage::Bits newUsage, Format format, uint32_t mipLevelCount, uint32_t layerCount)
    {
        vk::ImageSubresourceRange subresourceRange;
        subresourceRange
            .setAspectMask(ImageFormatToImageAspect(format))
            .setBaseArrayLayer(0)
            .setBaseMipLevel(0)
            .setLayerCount(layerCount)
            .setLevelCount(mipLevelCount);

        vk::ImageMemoryBarrier imageBarrier;
        imageBarrier
            .setImage(image)
            .setOldLayout(ImageUsageToImageLayout(oldUsage))
            .setNewLayout(ImageUsageToImageLayout(newUsage))
            .setSrcAccessMask(ImageUsageToAccessFlags(oldUsage))
            .setDstAccessMask(ImageUsageToAccessFlags(newUsage))
            .setSrcQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED)
            .setDstQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED)
            .setSubresourceRange(subresourceRange);

        return imageBarrier;
    }

    ImageUsage::Bits UniformTypeToImageUsage(UniformType type)
    {
        switch (type)
        {
        case UniformType::SAMPLER:
            return ImageUsage::UNKNOWN;
        case UniformType::COMBINED_IMAGE_SAMPLER:
            return ImageUsage::SHADER_READ;
        case UniformType::SAMPLED_IMAGE:
            return ImageUsage::SHADER_READ;
        case UniformType::STORAGE_IMAGE:
            return ImageUsage::STORAGE;
        case UniformType::UNIFORM_TEXEL_BUFFER:
            return ImageUsage::UNKNOWN;
        case UniformType::STORAGE_TEXEL_BUFFER:
            return ImageUsage::UNKNOWN;
        case UniformType::UNIFORM_BUFFER:
            return ImageUsage::UNKNOWN;
        case UniformType::STORAGE_BUFFER:
            return ImageUsage::UNKNOWN;
        case UniformType::UNIFORM_BUFFER_DYNAMIC:
            return ImageUsage::UNKNOWN;
        case UniformType::STORAGE_BUFFER_DYNAMIC:
            return ImageUsage::UNKNOWN;
        case UniformType::INPUT_ATTACHMENT:
            return ImageUsage::INPUT_ATTACHMENT;
        case UniformType::INLINE_UNIFORM_BLOCK_EXT:
            return ImageUsage::UNKNOWN;
        case UniformType::ACCELERATION_STRUCTURE_KHR:
            return ImageUsage::UNKNOWN;
        default:
            return ImageUsage::UNKNOWN;
        }
    }

    BufferUsage::Bits UniformTypeToBufferUsage(UniformType type)
    {
        switch (type)
        {
        case UniformType::SAMPLER:
            return BufferUsage::UNKNOWN;
        case UniformType::COMBINED_IMAGE_SAMPLER:
            return BufferUsage::UNKNOWN;
        case UniformType::SAMPLED_IMAGE:
            return BufferUsage::UNKNOWN;
        case UniformType::STORAGE_IMAGE:
            return BufferUsage::UNKNOWN;
        case UniformType::UNIFORM_TEXEL_BUFFER:
            return BufferUsage::UNIFORM_TEXEL_BUFFER;
        case UniformType::STORAGE_TEXEL_BUFFER:
            return BufferUsage::STORAGE_TEXEL_BUFFER;
        case UniformType::UNIFORM_BUFFER:
            return BufferUsage::UNIFORM_BUFFER;
        case UniformType::STORAGE_BUFFER:
            return BufferUsage::STORAGE_BUFFER;
        case UniformType::UNIFORM_BUFFER_DYNAMIC:
            return BufferUsage::UNIFORM_BUFFER;
        case UniformType::STORAGE_BUFFER_DYNAMIC:
            return BufferUsage::STORAGE_BUFFER;
        case UniformType::INPUT_ATTACHMENT:
            return BufferUsage::UNKNOWN;
        case UniformType::INLINE_UNIFORM_BLOCK_EXT:
            return BufferUsage::UNIFORM_BUFFER;
        case UniformType::ACCELERATION_STRUCTURE_KHR:
            return BufferUsage::ACCELERATION_STRUCTURE_STORAGE;
        default:
            return BufferUsage::UNKNOWN;
        }
    }

    bool IsBufferType(UniformType type)
    {
        switch (type)
        {
        case UniformType::UNIFORM_TEXEL_BUFFER:
        case UniformType::STORAGE_TEXEL_BUFFER:
        case UniformType::UNIFORM_BUFFER:
        case UniformType::STORAGE_BUFFER:
        case UniformType::UNIFORM_BUFFER_DYNAMIC:
        case UniformType::STORAGE_BUFFER_DYNAMIC:
        case UniformType::INLINE_UNIFORM_BLOCK_EXT:
            return true;
        default:
            return false;
        }
    }
}