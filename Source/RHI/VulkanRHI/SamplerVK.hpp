#pragma once

#include "RHI/RHICommon.hpp"

namespace RHI::Vulkan
{
    class SamplerVK
    {
    public:
        enum class Filter : uint8_t
        {
            NEAREST = 0,
            LINEAR
        };

        using MinFilter = Filter;
        using MagFilter = Filter;
        using MipFilter = Filter;

        enum class AddressMode : uint8_t
        {
            REPEAT = 0,
            MIRRORED_REPEAT,
            CLAMP_TO_EDGE,
            CLAMP_TO_BORDER,
        };

        SamplerVK() = default;
        ~SamplerVK();
        SamplerVK(SamplerVK&& other) noexcept;
        SamplerVK& operator=(SamplerVK&& other) noexcept;
        SamplerVK(MinFilter minFilter, MagFilter magFilter, AddressMode uvwAddress, MipFilter mipFilter);

        void Init(MinFilter minFilter, MagFilter magFilter, AddressMode uvwAddress, MipFilter mipFilter);

        const vk::Sampler& GetNativeSampler() const { return mSampler; }
    
    private:
        void Destroy();

    private:
        vk::Sampler mSampler;
    };

    using SamplerVKReference = std::reference_wrapper<const SamplerVK>;
}