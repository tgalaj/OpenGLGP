#pragma once

#include <cstdint>

struct GLFWwindow;

namespace openglgp::framework
{

struct FrameContext
{
    double deltaSeconds = 0.0;
    double elapsedSeconds = 0.0;
    std::uint64_t frameIndex = 0;
    int framebufferWidth = 0;
    int framebufferHeight = 0;
    GLFWwindow* window = nullptr;
    bool uiWantsMouse = false;
    bool uiWantsKeyboard = false;
};

} // namespace openglgp::framework
