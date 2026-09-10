#pragma once

#include "framework/RunConfig.h"

#include <memory>
#include <string>
#include <utility>

struct GLFWwindow;

namespace openglgp::framework
{

class Window
{
public:
    static std::unique_ptr<Window> create(const RunConfig& config, std::string& error);

    ~Window();

    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

    [[nodiscard]] GLFWwindow* nativeHandle() const noexcept;
    [[nodiscard]] bool shouldClose() const noexcept;
    [[nodiscard]] bool srgbCapable() const noexcept;
    [[nodiscard]] bool debugContextRequested() const noexcept;
    [[nodiscard]] std::pair<int, int> framebufferSize() const noexcept;

    void pollEvents() const;
    void waitForFramebuffer() const;
    void swapBuffers() const;

private:
    Window() = default;

    GLFWwindow* handle_ = nullptr;
    bool glfwInitialized_ = false;
    bool srgbCapable_ = false;
    bool debugContextRequested_ = false;
};

} // namespace openglgp::framework
