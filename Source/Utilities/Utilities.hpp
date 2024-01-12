#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <cassert>
#include <functional>

#include <tcb/span.hpp>

#define NOCOPY(ClassName) \
    ClassName(ClassName&) = delete; \
    ClassName& operator=(ClassName&) = delete;\
    ClassName(const ClassName&) = delete;\
    ClassName& operator=(const ClassName&) = delete;

template <typename T>
using ArrayView = tcb::span<T, tcb::dynamic_extent>;

template <typename T>
constexpr auto MakeView(T&& v)
{
	using ValueType = typename std::decay_t<T>::value_type;
	using Ret = std::conditional_t<std::is_const_v<std::remove_reference_t<T>>, const ValueType, ValueType>;
	return ArrayView<Ret>{ v.data(), v.size() };
}

inline void GDebugInfoCallbackFunc(const std::string& type, const std::string& message)
{
    std::cout << "[INFO " << type <<"] : " << message << std::endl;
}

static std::function<void(const std::string&, const std::string&)> GDebugInfoCallback = GDebugInfoCallbackFunc;

namespace Utilities
{
    class FileUtils
    {
    public:
        static std::string ReadTextFile(const std::string& fileName)
        {
            std::string result;
            {
                std::ifstream file(fileName, std::ios::ate | std::ios::binary);
                assert(file.is_open());
                size_t fileSize = file.tellg();
                result.resize(fileSize);
                file.seekg(0);
                file.read(result.data(), static_cast<std::streamsize>(fileSize));
                file.close();
            }
            return result;
        }
    };
}