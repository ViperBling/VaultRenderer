#pragma once

#include "Utilities/Utilities.hpp"
#include "RHI/RHICommon.hpp"

namespace RHI::Vulkan
{
    class ShaderVK
    {
    public:
        virtual ~ShaderVK() = default;

        virtual ArrayView<const TypeSPIRV> GetInputAttributes() const = 0;
        virtual ArrayView<const ShaderUniforms> GetShaderUniforms() const = 0;
        virtual const vk::ShaderModule& GetNativeShaderModule() const = 0;
    };

    class GraphicShaderVK : public ShaderVK
    {
    public:
        GraphicShaderVK() = default;
        GraphicShaderVK(const std::string& vertexShader, const std::string& fragmentShader);
        virtual ~GraphicShaderVK();

        void Init(const ShaderData& vertexShader, const ShaderData& fragmentShader);

        GraphicShaderVK(const GraphicShaderVK& other) = delete;
        GraphicShaderVK& operator=(const GraphicShaderVK& other) = delete;
        GraphicShaderVK(GraphicShaderVK&& other) noexcept;
        GraphicShaderVK& operator=(GraphicShaderVK&& other) noexcept;
        

        virtual ArrayView<const TypeSPIRV> GetInputAttributes() const override { return mInputAttributes; }
        virtual ArrayView<const ShaderUniforms> GetShaderUniforms() const override { return mShaderUniforms; }
        virtual const vk::ShaderModule& GetNativeShaderModule() const override;

    private:
        void Destroy();

    private:
        vk::ShaderModule mVertexShader;
        vk::ShaderModule mPixelShader;
        std::vector<ShaderUniforms> mShaderUniforms;
        std::vector<TypeSPIRV> mInputAttributes;
    };

    class ComputeShaderVK : public ShaderVK
    {
    public:
        ComputeShaderVK() = default;
        ComputeShaderVK(const std::string& computeShader);
        virtual ~ComputeShaderVK();

        void Init(const ShaderData& computeShader);

        ComputeShaderVK(const ComputeShaderVK& other) = delete;
        ComputeShaderVK& operator=(const ComputeShaderVK& other) = delete;
        ComputeShaderVK(ComputeShaderVK&& other) noexcept;
        ComputeShaderVK& operator=(ComputeShaderVK&& other) noexcept;

        virtual ArrayView<const TypeSPIRV> GetInputAttributes() const override { return {}; }
        virtual ArrayView<const ShaderUniforms> GetShaderUniforms() const override { return mShaderUniforms; }
        virtual const vk::ShaderModule& GetNativeShaderModule() const override;

    private:
        void Destroy();

    private:
        vk::ShaderModule mComputeShader;
        std::vector<ShaderUniforms> mShaderUniforms;
    };
}