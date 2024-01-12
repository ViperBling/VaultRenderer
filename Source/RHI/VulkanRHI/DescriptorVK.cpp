#include "DescriptorVK.hpp"
#include "CommonVK.hpp"

#include "Renderer/RendererBase.hpp"

namespace RHI::Vulkan
{
    SamplerVK EmptySampler;
    
    void ResolveInfo::Resolve(const std::string &name, const BufferVK &buffer)
    {
        assert(mBufferResolves.find(name) == mBufferResolves.end());
        mBufferResolves[name] = { buffer };
    }
    
    void ResolveInfo::Resolve(const std::string &name, ArrayView<const BufferVK> buffers)
    {
        assert(mBufferResolves.find(name) == mBufferResolves.end());
        for (const auto& buffer : buffers) 
        {
            mBufferResolves[name].push_back(buffer);
        }
    }

    void ResolveInfo::Resolve(const std::string &name, ArrayView<const BufferVKReference> buffers)
    {
        assert(mBufferResolves.find(name) == mBufferResolves.end());
        for (const auto& buffer : buffers)
        {
            mBufferResolves[name].push_back(buffer);
        }
    }

    void ResolveInfo::Resolve(const std::string &name, const ImageVK &image)
    {
        assert(mImageResolves.find(name) == mImageResolves.end());
        mImageResolves[name] = { image };
    }

    void ResolveInfo::Resolve(const std::string &name, ArrayView<const ImageVK> images)
    {
        assert(mImageResolves.find(name) == mImageResolves.end());
        for (const auto& image : images)
        {
            mImageResolves[name].push_back(image);
        }
    }

    void ResolveInfo::Resolve(const std::string &name, ArrayView<const ImageVKReference> images)
    {
        assert(mImageResolves.find(name) == mImageResolves.end());
        for (const auto& image : images)
        {
            mImageResolves[name].push_back(image);
        }
    }

    DescriptorBinding &DescriptorBinding::Bind(uint32_t binding, const std::string &name, UniformType type)
    {
        if (UniformTypeToBufferUsage(type) == BufferUsage::UNKNOWN)
        {
            return Bind(binding, name, EmptySampler, type, ImageView::NATIVE);
        }
        mBufferToResolve.push_back(BufferToResolve(
            name,
            binding,
            type,
            UniformTypeToBufferUsage(type)
        ));
        return *this;
    }

    DescriptorBinding &DescriptorBinding::Bind(uint32_t binding, const std::string &name, UniformType type, ImageView view)
    {
        return Bind(binding, name, EmptySampler, type, view);
    }

    DescriptorBinding &DescriptorBinding::Bind(uint32_t binding, const std::string &name, const SamplerVK &sampler, UniformType type)
    {
        return Bind(binding, name, sampler, type, ImageView::NATIVE);
    }

    DescriptorBinding &DescriptorBinding::Bind(uint32_t binding, const std::string &name, const SamplerVK &sampler, UniformType type, ImageView view)
    {
        mImageToResolve.push_back(ImageToResolve(
            name,
            binding,
            type,
            UniformTypeToImageUsage(type),
            view,
            std::addressof(sampler)
        ));
        return *this;
    }

    DescriptorBinding &DescriptorBinding::Bind(uint32_t binding, const SamplerVK &sampler, UniformType type)
    {
        mSamplerToResolve.push_back(SamplerToResolve(
            std::addressof(sampler),
            binding,
            type
        ));
        return *this;
    }

    void DescriptorBinding::Resolve(const ResolveInfo &resolveInfo)
    {
        mImageWriteInfos.clear();
        mBufferWirteInfos.clear();
        mDescWrites.clear();

        for (const auto& imageToResolve : mImageToResolve)
        {
            auto & images = resolveInfo.GetImages().at(imageToResolve.Name);
            size_t index = 0;
            if (imageToResolve.SamplerHandle->GetNativeSampler())
            {
                for (const auto& image : images)
                {
                    index = AllocateBinding(image.get(), *imageToResolve.SamplerHandle, imageToResolve.View, imageToResolve.Type);
                }
            }
            else
            {
                for (const auto& image : images)
                {
                    index = AllocateBinding(image.get(), imageToResolve.View, imageToResolve.Type);
                }
            }
            mDescWrites.push_back(DescriptorWriteInfo{
                imageToResolve.Type,
                imageToResolve.Binding,
                uint32_t(index + 1 - images.size()),
                uint32_t(images.size())
            });
        }

        for (const auto& bufferToResolve : mBufferToResolve)
        {
            auto& buffers = resolveInfo.GetBuffers().at(bufferToResolve.Name);
            size_t index = 0;
            for (const auto& buffer : buffers)
            {
                index = this->AllocateBinding(buffer.get(), bufferToResolve.Type);
            }
            mDescWrites.push_back({
                bufferToResolve.Type,
                bufferToResolve.Binding,
                uint32_t(index + 1 - buffers.size()),
                uint32_t(buffers.size())
            });
        }

        for (const auto& samplerToResolve : mSamplerToResolve)
        {
            size_t index = this->AllocateBinding(*samplerToResolve.SamplerHandle);

            mDescWrites.push_back({
                samplerToResolve.Type,
                samplerToResolve.Binding,
                uint32_t(index),
                1
            });
        }
    }

    void DescriptorBinding::Write(const vk::DescriptorSet &descriptorSet)
    {
        if (mOptions == ResolveOptions::ALREADY_RESOLVED) { return; }
        if (mOptions == ResolveOptions::RESOLVE_ONCE) { mOptions = ResolveOptions::ALREADY_RESOLVED; }

        std::vector<vk::WriteDescriptorSet> writeDescSets;
        std::vector<vk::DescriptorBufferInfo> descBufferInfos;
        std::vector<vk::DescriptorImageInfo> descImageInfos;
        writeDescSets.reserve(mDescWrites.size());
        descBufferInfos.reserve(mBufferWirteInfos.size());
        descImageInfos.reserve(mImageWriteInfos.size());

        for (const auto& bufferInfo : mBufferWirteInfos)
        {
            descBufferInfos.push_back(vk::DescriptorBufferInfo{
                bufferInfo.Handle->GetNativeBuffer(),
                0,
                bufferInfo.Handle->GetSize()
            });
        }
        for (const auto& imageInfo : mImageWriteInfos)
        {
            descImageInfos.push_back(vk::DescriptorImageInfo{
                imageInfo.SamplerHandle ? imageInfo.SamplerHandle->GetNativeSampler() : vk::Sampler{ },
                imageInfo.Handle ? imageInfo.Handle->GetNativeView(imageInfo.View) : vk::ImageView{ },
                ImageUsageToImageLayout(imageInfo.Usage)
            });
        }
        for (const auto& descWrite : mDescWrites)
        {
            auto& writeDescSet = writeDescSets.emplace_back();
            writeDescSet.setDstSet(descriptorSet);
            writeDescSet.setDstBinding(descWrite.Binding);
            writeDescSet.setDescriptorType(ToNative(descWrite.Type));
            writeDescSet.setDescriptorCount(descWrite.Count);

            if (IsBufferType(descWrite.Type))
            {
                writeDescSet.setPBufferInfo(descBufferInfos.data() + descWrite.FirstIndex);
            }
            else
            {
                writeDescSet.setPImageInfo(descImageInfos.data() + descWrite.FirstIndex);
            }
        }
        GetCurrentRenderer().GetDevice().updateDescriptorSets(writeDescSets, { });
    }

    size_t DescriptorBinding::AllocateBinding(const BufferVK &buffer, UniformType type)
    {
        mBufferWirteInfos.push_back(BufferWriteInfo(
            std::addressof(buffer),
            UniformTypeToBufferUsage(type)
        ));
        return mBufferWirteInfos.size() - 1;
    }

    size_t DescriptorBinding::AllocateBinding(const ImageVK &image, ImageView view, UniformType type)
    {
        mImageWriteInfos.push_back(ImageWriteInfo(
            std::addressof(image),
            UniformTypeToImageUsage(type),
            view,
            {}
        ));
        return mImageWriteInfos.size() - 1;
    }

    size_t DescriptorBinding::AllocateBinding(const ImageVK &image, const SamplerVK &sampler, ImageView view, UniformType type)
    {
        mImageWriteInfos.push_back(ImageWriteInfo(
            std::addressof(image),
            UniformTypeToImageUsage(type),
            view,
            std::addressof(sampler)
        ));
        return mImageWriteInfos.size() - 1;
    }

    size_t DescriptorBinding::AllocateBinding(const SamplerVK &sampler)
    {
        mImageWriteInfos.push_back(ImageWriteInfo(
            {},
            ImageUsage::UNKNOWN,
            {},
            std::addressof(sampler)
        ));
        return mImageWriteInfos.size() - 1;
    }

    void DescriptorCacheVK::Init()
    {
    }

    void DescriptorCacheVK::Destroy()
    {
    }

    Descriptor DescriptorCacheVK::GetDescriptor(ArrayView<const ShaderUniforms> specification)
    {
        return Descriptor();
    }

    vk::DescriptorSetLayout DescriptorCacheVK::CreateDescriptorSetLayout(ArrayView<const ShaderUniforms> specification)
    {
        return vk::DescriptorSetLayout();
    }

    vk::DescriptorSet DescriptorCacheVK::AllocateDescriptorSet(vk::DescriptorSetLayout layout)
    {
        return vk::DescriptorSet();
    }

    void DescriptorCacheVK::DestroyDescriptorSetLayout(vk::DescriptorSetLayout layout)
    {
    }

    void DescriptorCacheVK::FreeDescriptorSet(vk::DescriptorSet set)
    {
    }
}