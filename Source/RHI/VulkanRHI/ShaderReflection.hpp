#pragma once

#include "RHI/RHICommon.hpp"

namespace RHI::Vulkan
{
    const vk::ShaderStageFlagBits& ToNative(ShaderType type);
    ShaderType FromNative(const vk::ShaderStageFlagBits& type);

    const vk::Format& ToNative(Format format);
    Format FromNative(const vk::Format& format);

    const vk::DescriptorType& ToNative(UniformType type);
    UniformType FromNative(const vk::DescriptorType& type);

    inline bool operator==(const TypeSPIRV& t1, const TypeSPIRV& t2) { return t1.LayoutFormat == t2.LayoutFormat && t1.ComponentCount == t2.ComponentCount && t1.ByteSize == t2.ByteSize; }
    inline bool operator!=(const TypeSPIRV& t1, const TypeSPIRV& t2) { return !(t1 == t2); }

    inline bool operator==(const Uniform& u1, const Uniform& u2) { return u1.Layout == u2.Layout && u1.Type == u2.Type && u1.Binding == u2.Binding && u1.Count == u2.Count; }
    inline bool operator!=(const Uniform& u1, const Uniform& u2) { return !(u1 == u2); }

    inline bool operator==(const ShaderUniforms& u1, const ShaderUniforms& u2) { return u1.ShaderStage == u2.ShaderStage && u1.Uniforms == u2.Uniforms; }
    inline bool operator!=(const ShaderUniforms& u1, const ShaderUniforms& u2) { return !(u1 == u2); }
}