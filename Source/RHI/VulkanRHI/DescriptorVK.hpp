#pragma once

#include "Utilities/Utilities.hpp"
#include "RHI/RHICommon.hpp"
#include "BufferVK.hpp"
#include "ImageVK.hpp"
#include "SamplerVK.hpp"
#include "ShaderReflection.hpp"

#include <vector>
#include <string>
#include <unordered_map>

namespace RHI::Vulkan
{
    class ResolveInfo
    {
    public:
        void Resolve(const std::string &name, const BufferVK &buffer);
        void Resolve(const std::string &name, ArrayView<const BufferVK> buffers);
        void Resolve(const std::string &name, ArrayView<const BufferVKReference> buffers);

        void Resolve(const std::string &name, const ImageVK &image);
        void Resolve(const std::string &name, ArrayView<const ImageVK> images);
        void Resolve(const std::string &name, ArrayView<const ImageVKReference> images);

        const auto & GetBuffers() const { return this->mBufferResolves; }
        const auto & GetImages() const { return this->mImageResolves; }

    private:
        std::unordered_map<std::string, std::vector<BufferVKReference>> mBufferResolves;
        std::unordered_map<std::string, std::vector<ImageVKReference>> mImageResolves;
    };

    class DescriptorBinding
    {
    public:
        // 绑定Buffer
        DescriptorBinding& Bind(uint32_t binding, const std::string &name, UniformType type);
        // 绑定Image
        DescriptorBinding& Bind(uint32_t binding, const std::string &name, UniformType type, ImageView view);
        DescriptorBinding& Bind(uint32_t binding, const std::string &name, const SamplerVK &sampler, UniformType type);
        DescriptorBinding& Bind(uint32_t binding, const std::string &name, const SamplerVK &sampler, UniformType type, ImageView view);

        DescriptorBinding& Bind(uint32_t binding, const SamplerVK &sampler, UniformType type);

        void SetOptions(ResolveOptions options) { this->mOptions = options; }

        // 根据Resolve信息生成WriteDesc
        void Resolve(const ResolveInfo &resolveInfo);
        // 上传已经写入WriteDesc的信息
        void Write(const vk::DescriptorSet &descriptorSet);
        const auto& GetBoundBuffers() const { return this->mBufferToResolve; }
        const auto& GetBoundImages() const { return this->mImageToResolve; }

    private:
        // 向对应的Resolve信息数组中加入信息，并返回索引
        size_t AllocateBinding(const BufferVK &buffer, UniformType type);
        size_t AllocateBinding(const ImageVK &image, ImageView view, UniformType type);
        size_t AllocateBinding(const ImageVK &image, const SamplerVK &sampler, ImageView view, UniformType type);
        size_t AllocateBinding(const SamplerVK &sampler);

    private:
        struct DescriptorWriteInfo
        {
            UniformType Type;
            uint32_t Binding;
            uint32_t FirstIndex;
            uint32_t Count;
        };

        struct BufferWriteInfo
        {
            const BufferVK *Handle;
            BufferUsage::Bits Usage;
        };

        struct ImageWriteInfo
        {
            const ImageVK *Handle;
            ImageUsage::Bits Usage;
            ImageView View;
            const SamplerVK *SamplerHandle;
        };

        struct ImageToResolve
        {
            std::string Name;
            uint32_t Binding;
            UniformType Type;
            ImageUsage::Bits Usage;
            ImageView View;
            const SamplerVK *SamplerHandle;
        };

        struct BufferToResolve
        {
            std::string Name;
            uint32_t Binding;
            UniformType Type;
            BufferUsage::Bits Usage;
        };

        struct SamplerToResolve
        {
            const SamplerVK *SamplerHandle;
            uint32_t Binding;
            UniformType Type;
        };

        std::vector<DescriptorWriteInfo> mDescWrites;
        std::vector<BufferWriteInfo> mBufferWirteInfos;
        std::vector<ImageWriteInfo> mImageWriteInfos;
        std::vector<BufferToResolve> mBufferToResolve;
        std::vector<ImageToResolve> mImageToResolve;
        std::vector<SamplerToResolve> mSamplerToResolve;

        ResolveOptions mOptions = ResolveOptions::RESOLVE_EACH_FRAME;
    };

    struct Descriptor
    {
        vk::DescriptorSetLayout DescSetLayout;
        vk::DescriptorSet DescSet;
    };

    class DescriptorCacheVK
    {
    public:
        void Init();
        void Destroy();
        const auto& GetDescriptorPool() const { return mDescPool; }
        Descriptor GetDescriptor(ArrayView<const ShaderUniforms> specification);

    private:
        vk::DescriptorSetLayout CreateDescriptorSetLayout(ArrayView<const ShaderUniforms> specification);
        vk::DescriptorSet AllocateDescriptorSet(vk::DescriptorSetLayout layout);
        void DestroyDescriptorSetLayout(vk::DescriptorSetLayout layout);
        void FreeDescriptorSet(vk::DescriptorSet set);

    private:
        vk::DescriptorPool mDescPool;
        std::vector<Descriptor> mDescriptor;
    };
}