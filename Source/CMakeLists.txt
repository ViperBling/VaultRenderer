find_package(assimp CONFIG REQUIRED)
find_package(glfw3 3.3 REQUIRED)
find_package(Vulkan REQUIRED)
find_package(spirv_cross_core CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(glslang CONFIG REQUIRED)
# find_package(cityhash CONFIG REQUIRED)

set(GLSLANG_LIB glslang::OSDependent glslang::glslang glslang::MachineIndependent glslang::GenericCodeGen glslang::glslang-default-resource-limits glslang::OGLCompiler glslang::SPVRemapper glslang::SPIRV glslang::HLSL)

find_path(TCB_SPAN_INCLUDE_DIRS "tcb/span.hpp")

file(GLOB_RECURSE Core_Head         ${SourcePath}/Core/*.hpp)
file(GLOB_RECURSE Core_Src          ${SourcePath}/Core/*.cpp)
file(GLOB_RECURSE Renderer_Head     ${SourcePath}/Renderer/*.hpp)
file(GLOB_RECURSE Renderer_Src      ${SourcePath}/Renderer/*.cpp)
file(GLOB_RECURSE RHI_Head          ${SourcePath}/RHI/*.hpp)
file(GLOB_RECURSE RHI_Src           ${SourcePath}/RHI/*.cpp)
file(GLOB_RECURSE Scene_Head        ${SourcePath}/Scene/*.hpp)
file(GLOB_RECURSE Scene_Src         ${SourcePath}/Scene/*.cpp)
file(GLOB_RECURSE Window_Head       ${SourcePath}/Windows/*.hpp)
file(GLOB_RECURSE Window_Src        ${SourcePath}/Windows/*.cpp)
file(GLOB_RECURSE Utilities_Head       ${SourcePath}/Utilities/*.hpp)
file(GLOB_RECURSE Utilities_Src        ${SourcePath}/Utilities/*.cpp)

add_library(FrameworkLib STATIC 
    ${Core_Head}        ${Core_Src}
    ${Renderer_Head}    ${Renderer_Src}
    ${RHI_Head}         ${RHI_Src}
    ${Scene_Head}       ${Scene_Src}
    ${Window_Head}      ${Window_Src}
    ${Utilities_Head}   ${Utilities_Src}
)

target_include_directories(FrameworkLib PUBLIC ${SourcePath} ${ExternalPath} ${TCB_SPAN_INCLUDE_DIRS})
target_link_libraries(FrameworkLib PUBLIC
    assimp::assimp
    glfw
    glm::glm
    Vulkan::Vulkan
    spirv-cross-core
    ${GLSLANG_LIB}
    # fmt::fmt-header-only
    # Microsoft::DirectXShaderCompiler
    # cityhash
)