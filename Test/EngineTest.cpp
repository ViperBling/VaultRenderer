#include <gtest/gtest.h>

#include "Core/VaultEngine.hpp"
#include "Windows/GLFWindow.hpp"

TEST(EngineTest, BasicTest)
{
    Windows::WindowCreateInfo windowCI;
    windowCI.Position = { 100, 100,};
    windowCI.Size = {1280, 720};
    windowCI.Title = "Vultana";

    Windows::GLFWindow window(windowCI);

    Core::VaultEngine::GetInstance()->Init(&window, windowCI.Size.x, windowCI.Size.y);

    while (!window.ShouldClose())
    {
        window.PollEvents();
        Core::VaultEngine::GetInstance()->Tick();
    }
    Core::VaultEngine::GetInstance()->Shutdown();
}