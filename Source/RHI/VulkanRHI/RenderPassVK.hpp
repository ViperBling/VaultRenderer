#pragma once

#include "RHI/RHICommon.hpp"
#include "CommandBufferVK.hpp"
#include "PipelineVK.hpp"
#include "DescriptorVK.hpp"

namespace RHI::Vulkan
{
    class RenderGraph;

    struct NativeRenderPass
    {
        vk::RenderPass              RenderPassHandle;
        vk::DescriptorSet           DescriptorSet;
        vk::Framebuffer             Framebuffer;
        vk::Pipeline                Pipeline;
        vk::PipelineLayout          PipelineLayout;
        vk::PipelineBindPoint       PipelineType = { };
        vk::Rect2D                  RenderArea = { };
        std::vector<vk::ClearValue> ClearValues;
    };

    struct RenderPassState
    {
        RenderGraph& Graph;
        CommandBufferVK& CommandBuffer;
        const NativeRenderPass& RenderPass;
        const ImageVK& GetAttachment(const std::string& name);
    };

    using PipelineState = PipelineVK&;
    using ResolveState = ResolveInfo&;
}