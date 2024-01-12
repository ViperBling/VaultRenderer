#pragma once

#include "RHI/RHICommon.hpp"
#include "MemoryAllocatorVK.hpp"

#include <cstdint>
#include <string>
#include <vector>
#include <array>

namespace RHI::Vulkan
{
    class ImageVK
    {
    public:
        ImageVK() = default;
        ImageVK(uint32_t width, uint32_t height, Format format, ImageUsage::Value usage, MemoryUsage memoryUsage, ImageOptions::Value options);
        ImageVK(vk::Image image, uint32_t width, uint32_t height, Format format);
        ImageVK(ImageVK&& other) noexcept;
        ImageVK& operator=(ImageVK&& other) noexcept;
        virtual ~ImageVK();

        void Init(uint32_t width, uint32_t height, Format format, ImageUsage::Value usage, MemoryUsage memoryUsage, ImageOptions::Value options);

        vk::ImageView GetNativeView(ImageView view) const;
        vk::ImageView GetNativeView(ImageView view, uint32_t layer) const;
        uint32_t GetMipLevelWidth(uint32_t mipLevel) const;
        uint32_t GetMipLevelHeight(uint32_t mipLevel) const;

        vk::Image GetNativeImage() const { return mImage; }
        Format GetFormat() const { return mFormat; }
        uint32_t GetWidth() const { return mExtent.width; }
        uint32_t GetHeight() const { return mExtent.height; }
        uint32_t GetMipLevelCount() const { return mMipLevelCount; }
        uint32_t GetLayerCount() const { return mLayerCount; }

    private:
        void Destroy();
        void InitViews(const vk::Image& image, Format format);

    private:
        struct ImageViews
        {
            vk::ImageView NativeView;
            vk::ImageView DepthOnlyView;
            vk::ImageView StencilOnlyView;
        };

        vk::Image mImage;
        ImageViews mImageViews;
        std::vector<ImageViews> mCubeImageViews;
        vk::Extent2D mExtent = {0u, 0u};
        uint32_t mMipLevelCount = 1;
        uint32_t mLayerCount = 1;
        Format mFormat = Format::UNDEFINED;
        VmaAllocation mAllocation = VK_NULL_HANDLE;
    };

    using ImageVKReference = std::reference_wrapper<const ImageVK>;
}