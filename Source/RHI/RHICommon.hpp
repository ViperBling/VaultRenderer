#pragma once

#include <iostream>

#include <vulkan/vulkan.hpp>

namespace RHI
{
    enum class MemoryUsage
    {
        GPUOnly = 0,
        CPUOnly,
        CPUToGPU,
        GPUToCPU,
        CPUCopy,
        GPULazyAllocated,
        Count,
    };

    struct BufferUsage
    {
        using Value = uint32_t;

        enum Bits : Value
        {
            UNKNOWN = (Value)vk::BufferUsageFlags{},
            TRANSFER_SOURCE = (Value)vk::BufferUsageFlagBits::eTransferSrc,
            TRANSFER_DESTINATION = (Value)vk::BufferUsageFlagBits::eTransferDst,
            UNIFORM_TEXEL_BUFFER = (Value)vk::BufferUsageFlagBits::eUniformTexelBuffer,
            STORAGE_TEXEL_BUFFER = (Value)vk::BufferUsageFlagBits::eStorageTexelBuffer,
            UNIFORM_BUFFER = (Value)vk::BufferUsageFlagBits::eUniformBuffer,
            STORAGE_BUFFER = (Value)vk::BufferUsageFlagBits::eStorageBuffer,
            INDEX_BUFFER = (Value)vk::BufferUsageFlagBits::eIndexBuffer,
            VERTEX_BUFFER = (Value)vk::BufferUsageFlagBits::eVertexBuffer,
            INDIRECT_BUFFER = (Value)vk::BufferUsageFlagBits::eIndirectBuffer,
            SHADER_DEVICE_ADDRESS = (Value)vk::BufferUsageFlagBits::eShaderDeviceAddress,
            TRANSFORM_FEEDBACK_BUFFER = (Value)vk::BufferUsageFlagBits::eTransformFeedbackBufferEXT,
            TRANSFORM_FEEDBACK_COUNTER_BUFFER = (Value)vk::BufferUsageFlagBits::eTransformFeedbackCounterBufferEXT,
            CONDITIONAL_RENDERING = (Value)vk::BufferUsageFlagBits::eConditionalRenderingEXT,
            ACCELERATION_STRUCTURE_BUILD_INPUT_READONLY = (Value)vk::BufferUsageFlagBits::eAccelerationStructureBuildInputReadOnlyKHR,
            ACCELERATION_STRUCTURE_STORAGE = (Value)vk::BufferUsageFlagBits::eAccelerationStructureStorageKHR,
            SHADER_BINDING_TABLE = (Value)vk::BufferUsageFlagBits::eShaderBindingTableKHR,
        };
    };

    struct ImageUsage
    {
        using Value = uint32_t;

        enum Bits : Value
        {
            UNKNOWN = (Value)vk::ImageUsageFlagBits{},
            TRANSFER_SOURCE = (Value)vk::ImageUsageFlagBits::eTransferSrc,
            TRANSFER_DESTINATION = (Value)vk::ImageUsageFlagBits::eTransferDst,
            SHADER_READ = (Value)vk::ImageUsageFlagBits::eSampled,
            STORAGE = (Value)vk::ImageUsageFlagBits::eStorage,
            COLOR_ATTACHMENT = (Value)vk::ImageUsageFlagBits::eColorAttachment,
            DEPTH_STENCIL_ATTACHMENT = (Value)vk::ImageUsageFlagBits::eDepthStencilAttachment,
            INPUT_ATTACHMENT = (Value)vk::ImageUsageFlagBits::eInputAttachment,
            FRAGMENT_SHADING_RATE_ATTACHMENT = (Value)vk::ImageUsageFlagBits::eFragmentShadingRateAttachmentKHR,
        };
    };

    enum class ImageView
    {
        NATIVE = 0,
        DEPTH_ONLY,
        STENCIL_ONLY,
    };

    struct ImageOptions
    {
        using Value = uint32_t;

        enum Bits : Value
        {
            DEFAULT = 0,
            MIPMAPS = 1 << 0,
            CUBEMAP = 1 << 1,
        };
    };

    struct ImageData
    {
        std::vector<uint8_t> ByteData;
        Format ImageFormat = Format::UNDEFINED;
        uint32_t Width = 0;
        uint32_t Height = 0;
        std::vector<std::vector<uint8_t>> MipLevels;
    };

    struct CubemapData
    {
        std::array<std::vector<uint8_t>, 6> Faces;
        Format FaceFormat = Format::UNDEFINED;
        uint32_t FaceWidth = 0;
        uint32_t FaceHeight = 0;
    };

    enum class ShaderType : uint32_t
    {
        VERTEX = 0,
        TESS_CONTROL,
        TESS_EVALUATION,
        GEOMETRY,
        FRAGMENT,
        COMPUTE,
        RAY_GEN,
        INTERSECT,
        ANY_HIT,
        CLOSEST_HIT,
        MISS,
        CALLABLE,
        TASK_NV,
        MESH_NV,
    };

    enum class ShaderLanguage
    {
        GLSL = 0,
        HLSL,
    };

    enum class Format : uint32_t
    {
        UNDEFINED = 0,
        R4G4_UNORM_PACK_8,
        R4G4B4A4_UNORM_PACK_16,
        B4G4R4A4_UNORM_PACK_16,
        R5G6B5_UNORM_PACK_16,
        B5G6R5_UNORM_PACK_16,
        R5G5B5A1_UNORM_PACK_16,
        B5G5R5A1_UNORM_PACK_16,
        A1R5G5B5_UNORM_PACK_16,
        R8_UNORM,
        R8_SNORM,
        R8_USCALED,
        R8_SSCALED,
        R8_UINT,
        R8_SINT,
        R8_SRGB,
        R8G8_UNORM,
        R8G8_SNORM,
        R8G8_USCALED,
        R8G8_SSCALED,
        R8G8_UINT,
        R8G8_SINT,
        R8G8_SRGB,
        R8G8B8_UNORM,
        R8G8B8_SNORM,
        R8G8B8_USCALED,
        R8G8B8_SSCALED,
        R8G8B8_UINT,
        R8G8B8_SINT,
        R8G8B8_SRGB,
        B8G8R8_UNORM,
        B8G8R8_SNORM,
        B8G8R8_USCALED,
        B8G8R8_SSCALED,
        B8G8R8_UINT,
        B8G8R8_SINT,
        B8G8R8_SRGB,
        R8G8B8A8_UNORM,
        R8G8B8A8_SNORM,
        R8G8B8A8_USCALED,
        R8G8B8A8_SSCALED,
        R8G8B8A8_UINT,
        R8G8B8A8_SINT,
        R8G8B8A8_SRGB,
        B8G8R8A8_UNORM,
        B8G8R8A8_SNORM,
        B8G8R8A8_USCALED,
        B8G8R8A8_SSCALED,
        B8G8R8A8_UINT,
        B8G8R8A8_SINT,
        B8G8R8A8_SRGB,
        A8B8G8R8_UNORM_PACK_32,
        A8B8G8R8_SNORM_PACK_32,
        A8B8G8R8_USCALED_PACK_32,
        A8B8G8R8_SSCALED_PACK_32,
        A8B8G8R8_UINT_PACK_32,
        A8B8G8R8_SINT_PACK_32,
        A8B8G8R8_SRGB_PACK_32,
        A2R10G10B10_UNORM_PACK_32,
        A2R10G10B10_SNORM_PACK_32,
        A2R10G10B10_USCALED_PACK_32,
        A2R10G10B10_SSCALED_PACK_32,
        A2R10G10B10_UINT_PACK_32,
        A2R10G10B10_SINT_PACK_32,
        A2B10G10R10_UNORM_PACK_32,
        A2B10G10R10_SNORM_PACK_32,
        A2B10G10R10_USCALED_PACK_32,
        A2B10G10R10_SSCALED_PACK_32,
        A2B10G10R10_UINT_PACK_32,
        A2B10G10R10_SINT_PACK_32,
        R16_UNORM,
        R16_SNORM,
        R16_USCALED,
        R16_SSCALED,
        R16_UINT,
        R16_SINT,
        R16_SFLOAT,
        R16G16_UNORM,
        R16G16_SNORM,
        R16G16_USCALED,
        R16G16_SSCALED,
        R16G16_UINT,
        R16G16_SINT,
        R16G16_SFLOAT,
        R16G16B16_UNORM,
        R16G16B16_SNORM,
        R16G16B16_USCALED,
        R16G16B16_SSCALED,
        R16G16B16_UINT,
        R16G16B16_SINT,
        R16G16B16_SFLOAT,
        R16G16B16A16_UNORM,
        R16G16B16A16_SNORM,
        R16G16B16A16_USCALED,
        R16G16B16A16_SSCALED,
        R16G16B16A16_UINT,
        R16G16B16A16_SINT,
        R16G16B16A16_SFLOAT,
        R32_UINT,
        R32_SINT,
        R32_SFLOAT,
        R32G32_UINT,
        R32G32_SINT,
        R32G32_SFLOAT,
        R32G32B32_UINT,
        R32G32B32_SINT,
        R32G32B32_SFLOAT,
        R32G32B32A32_UINT,
        R32G32B32A32_SINT,
        R32G32B32A32_SFLOAT,
        R64_UINT,
        R64_SINT,
        R64_SFLOAT,
        R64G64_UINT,
        R64G64_SINT,
        R64G64_SFLOAT,
        R64G64B64_UINT,
        R64G64B64_SINT,
        R64G64B64_SFLOAT,
        R64G64B64A64_UINT,
        R64G64B64A64_SINT,
        R64G64B64A64_SFLOAT,
        B10G11R11_UFLOAT_PACK_32,
        E5B9G9R9_UFLOAT_PACK_32,
        D16_UNORM,
        X8D24_UNORM_PACK_32,
        D32_SFLOAT,
        S8_UINT,
        D16_UNORM_S8_UINT,
        D24_UNORM_S8_UINT,
        D32_SFLOAT_S8_UINT,
    };

    enum class UniformType : uint32_t
    {
        SAMPLER = 0,
        COMBINED_IMAGE_SAMPLER,
        SAMPLED_IMAGE,
        STORAGE_IMAGE,
        UNIFORM_TEXEL_BUFFER,
        STORAGE_TEXEL_BUFFER,
        UNIFORM_BUFFER,
        STORAGE_BUFFER,
        UNIFORM_BUFFER_DYNAMIC,
        STORAGE_BUFFER_DYNAMIC,
        INPUT_ATTACHMENT,
        INLINE_UNIFORM_BLOCK_EXT,
        ACCELERATION_STRUCTURE_KHR,
    };

    struct TypeSPIRV
    {
        Format LayoutFormat;
        int32_t ComponentCount;
        int32_t ByteSize;

        template <typename T>
        static TypeSPIRV As();
    };

    struct VertexBinding
    {
        enum class Rate : uint8_t
        {
            PER_VERTEX = 0,
            PER_INSTANCE
        } InputRate;

        uint32_t BindingRange;

        constexpr static uint32_t BindingRangeAll = uint32_t(-1);
    };

    struct Uniform
    {
        std::vector<TypeSPIRV> Layout;
        UniformType Type;
        uint32_t Binding;
        uint32_t Count;
    };

    struct ShaderUniforms
    {
        std::vector<Uniform> Uniforms;
        ShaderType ShaderStage;
    };

    struct ShaderData
    {
        using ByteCodeSPIRV = std::vector<uint32_t>;
        using Attributes = std::vector<TypeSPIRV>;
        using UniformBlock = std::vector<Uniform>;
        using Uniforms = std::vector<UniformBlock>;

        ByteCodeSPIRV ByteCode;
        Attributes InputAttributes;
        Uniforms UniformDescSets;
    };

    struct Rect2D
    {
        int32_t OffsetWidth = 0;
        int32_t OffSetHeight = 0;
        uint32_t Width = 0;
        uint32_t Height = 0;
    };

    struct Viewport
    {
        float OffsetWidth = 0.0f;
        float OffsetHeight = 0.0f;
        float Width = 0.0f;
        float Height = 0.0f;
        float MinDepth = 0.0f;
        float MaxDepth = 0.0f;
    };

    struct ClearColor
    {
        float R = 0.0f;
        float G = 0.0f;
        float B = 0.0f;
        float A = 1.0f;
    };

    struct ClearDepthStencil
    {
        float Depth = 1.0f;
        uint32_t Stencil = 0;
    };

    enum class BlitFilter
    {
        NEAREST = 0,
        LINEAR,
        CUBIC,
    };

    enum class AttachmentState
    {
        DISCARD_COLOR = 0,
        DISCARD_DEPTH_SPENCIL,
        LOAD_COLOR,
        LOAD_DEPTH_STENCIL,
        CLEAR_COLOR,
        CLEAR_DEPTH_SPENCIL,
    };

    enum class ResolveOptions
    {
        RESOLVE_EACH_FRAME,
        RESOLVE_ONCE,
        ALREADY_RESOLVED,
    };
}