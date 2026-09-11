#include "framework/Window.h"

#include <glad/glad.h>

#include <GLFW/glfw3.h>
#if defined(_WIN32)
#define GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WGL
#include <GLFW/glfw3native.h>
#endif
#include <spdlog/spdlog.h>

#include <memory>
#include <string>

namespace openglgp::framework
{
namespace
{

void glfwErrorCallback(const int code, const char* description)
{
    spdlog::error("GLFW error {}: {}", code, description != nullptr ? description : "unknown");
}

bool debugContextWanted() noexcept
{
#ifdef OPENGLGP_DEBUG_BUILD
    return true;
#else
    return false;
#endif
}

bool defaultFramebufferIsSrgb(GLFWwindow* window)
{
#if defined(_WIN32)
    using WglGetPixelFormatAttribiv = BOOL(WINAPI*)(HDC, int, int, UINT, const int*, int*);
    constexpr int wglFramebufferSrgbCapable = 0x20A9;

    const HWND nativeWindow = glfwGetWin32Window(window);
    const HDC deviceContext = GetDC(nativeWindow);
    if (deviceContext != nullptr)
    {
        const int pixelFormat = GetPixelFormat(deviceContext);
        const auto queryPixelFormat = reinterpret_cast<WglGetPixelFormatAttribiv>(wglGetProcAddress("wglGetPixelFormatAttribivARB"));
        bool queried = false;
        bool srgbCapable = false;
        if (queryPixelFormat != nullptr)
        {
            int value = 0;
            if (queryPixelFormat(
                    deviceContext,
                    pixelFormat,
                    0,
                    1,
                    &wglFramebufferSrgbCapable,
                    &value) == TRUE)
            {
                queried = true;
                srgbCapable = value != 0;
            }
        }
        ReleaseDC(nativeWindow, deviceContext);
        if (queried)
        {
            return srgbCapable;
        }
    }
#else
    static_cast<void>(window);
#endif

    GLint colorEncoding = GL_LINEAR;
    while (glGetError() != GL_NO_ERROR)
    {
    }
    glGetFramebufferAttachmentParameteriv(
        GL_FRAMEBUFFER,
        GL_BACK_LEFT,
        GL_FRAMEBUFFER_ATTACHMENT_COLOR_ENCODING,
        &colorEncoding);
    return glGetError() == GL_NO_ERROR && colorEncoding == static_cast<GLint>(GL_SRGB);
}

} // namespace

std::unique_ptr<Window> Window::create(const RunConfig& config, std::string& error)
{
    auto result = std::unique_ptr<Window>(new Window);
    result->debugContextRequested_ = debugContextWanted();

    glfwSetErrorCallback(glfwErrorCallback);
    if (glfwInit() != GLFW_TRUE)
    {
        error = "GLFW initialization failed.";
        return nullptr;
    }
    result->glfwInitialized_ = true;

    glfwDefaultWindowHints();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    glfwWindowHint(GLFW_SRGB_CAPABLE, GLFW_TRUE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, result->debugContextRequested_ ? GLFW_TRUE : GLFW_FALSE);
    if (config.diagnostics)
    {
        glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
    }

    result->handle_ = glfwCreateWindow(RunConfig::defaultWidth, RunConfig::defaultHeight, "OpenGLGP", nullptr, nullptr);
    if (result->handle_ == nullptr)
    {
        error = "Could not create an OpenGL 4.5 Core window. "
                "A desktop OpenGL 4.5-capable GPU and current driver are required.";
        return nullptr;
    }

    glfwMakeContextCurrent(result->handle_);
    if (gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)) == 0)
    {
        error = "OpenGL function loading failed.";
        return nullptr;
    }

    GLint major = 0;
    GLint minor = 0;
    GLint profile = 0;
    GLint contextFlags = 0;
    glGetIntegerv(GL_MAJOR_VERSION, &major);
    glGetIntegerv(GL_MINOR_VERSION, &minor);
    glGetIntegerv(GL_CONTEXT_PROFILE_MASK, &profile);
    glGetIntegerv(GL_CONTEXT_FLAGS, &contextFlags);

    if (major < 4 || (major == 4 && minor < 5))
    {
        error = "OpenGL 4.5 Core is required, but the created context reports " +
                std::to_string(major) + '.' + std::to_string(minor) + '.';
        return nullptr;
    }
    if ((profile & GL_CONTEXT_CORE_PROFILE_BIT) == 0)
    {
        error = "The created OpenGL context is not a Core profile context.";
        return nullptr;
    }
    if (result->debugContextRequested_ && (contextFlags & GL_CONTEXT_FLAG_DEBUG_BIT) == 0)
    {
        error = "A debug OpenGL context was requested but not provided by the driver.";
        return nullptr;
    }

    result->srgbCapable_ = defaultFramebufferIsSrgb(result->handle_);
    glfwSwapInterval(1);

    return result;
}

Window::~Window()
{
    if (handle_ != nullptr)
    {
        glfwDestroyWindow(handle_);
    }
    if (glfwInitialized_)
    {
        glfwTerminate();
    }
}

GLFWwindow* Window::nativeHandle() const noexcept
{
    return handle_;
}

bool Window::shouldClose() const noexcept
{
    return glfwWindowShouldClose(handle_) == GLFW_TRUE;
}

bool Window::srgbCapable() const noexcept
{
    return srgbCapable_;
}

bool Window::debugContextRequested() const noexcept
{
    return debugContextRequested_;
}

std::pair<int, int> Window::framebufferSize() const noexcept
{
    int width = 0;
    int height = 0;
    glfwGetFramebufferSize(handle_, &width, &height);
    return {width, height};
}

void Window::pollEvents() const
{
    glfwPollEvents();
}

void Window::waitForFramebuffer() const
{
    glfwWaitEventsTimeout(0.05);
}

void Window::swapBuffers() const
{
    glfwSwapBuffers(handle_);
}

} // namespace openglgp::framework
