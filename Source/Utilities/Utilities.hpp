#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <cassert>
#include <functional>

#define NOCOPY(ClassName) \
    ClassName(ClassName&) = delete; \
    ClassName& operator=(ClassName&) = delete;\
    ClassName(const ClassName&) = delete;\
    ClassName& operator=(const ClassName&) = delete;

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