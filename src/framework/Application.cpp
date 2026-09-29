#include "framework/Application.h"

#include "framework/AssetLocator.h"
#include "framework/FrameContext.h"
#include "framework/GlDiagnostics.h"
#include "framework/Window.h"
#include "project/Project.h"

#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <spdlog/spdlog.h>

#include <algorithm>
#include <chrono>
#include <cstdint>
#include <iostream>
#include <memory>
#include <string>
#include <utility>

namespace openglgp::framework
{
namespace
{

class ImGuiSession
{
public:
    ~ImGuiSession()
    {
        if (initialized_)
        {
            ImGui_ImplOpenGL3_Shutdown();
            ImGui_ImplGlfw_Shutdown();
            ImGui::DestroyContext();
        }
    }

    [[nodiscard]] bool initialize(GLFWwindow* window, std::string& error)
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui::StyleColorsDark();

        if (!ImGui_ImplGlfw_InitForOpenGL(window, true))
        {
            error = "ImGui GLFW backend initialization failed.";
            ImGui::DestroyContext();
            return false;
        }
        if (!ImGui_ImplOpenGL3_Init("#version 450 core"))
        {
            error = "ImGui OpenGL backend initialization failed.";
            ImGui_ImplGlfw_Shutdown();
            ImGui::DestroyContext();
            return false;
        }

        initialized_ = true;
        return true;
    }

private:
    bool initialized_ = false;
};

} // namespace

Application::Application(RunConfig config)
    : config_(std::move(config))
{
}

ExitCode Application::run()
{
    std::string error;
    auto window = Window::create(config_, error);
    if (!window)
    {
        spdlog::error("{}", error);
        return ExitCode::initializationFailure;
    }

    const GlReport report = collectGlReport(window->srgbCapable());
    logGlInfo(report);
    if (report.debugContext)
    {
        installGlDebugCallback();
    }

    if (config_.diagnostics)
    {
        std::cout << report.toText();
        if (config_.diagnosticsJson &&
            !writeGlReportJson(report, *config_.diagnosticsJson, error))
        {
            spdlog::error("{}", error);
            return ExitCode::outputFailure;
        }
        return report.compliant() ? ExitCode::success : ExitCode::diagnosticsNonCompliant;
    }

    ImGuiSession imgui;
    if (!imgui.initialize(window->nativeHandle(), error))
    {
        spdlog::error("{}", error);
        return ExitCode::initializationFailure;
    }

    AssetLocator assets(config_.executablePath);
    for (const auto& root : assets.searchRoots())
    {
        spdlog::debug("Asset search root: {}", root.string());
    }

    std::uint64_t completedFrames = 0;
    double elapsedSeconds = 0.0;
    auto previousTime = std::chrono::steady_clock::now();

    {
        auto project = std::make_unique<project::Project>();

        while (!window->shouldClose())
        {
            window->pollEvents();

            const auto [framebufferWidth, framebufferHeight] = window->framebufferSize();
            if (framebufferWidth <= 0 || framebufferHeight <= 0)
            {
                window->waitForFramebuffer();
                previousTime = std::chrono::steady_clock::now();
                continue;
            }

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();
            const ImGuiIO& io = ImGui::GetIO();
            const bool uiWantsMouse = io.WantCaptureMouse;
            const bool uiWantsKeyboard = io.WantCaptureKeyboard;

            const auto currentTime = std::chrono::steady_clock::now();
            const double measuredDelta = std::chrono::duration<double>(currentTime - previousTime).count();
            previousTime = currentTime;
            const double deltaSeconds = std::clamp(measuredDelta, 0.0, 0.25);
            elapsedSeconds += deltaSeconds;

            const FrameContext frame{
                .deltaSeconds = deltaSeconds,
                .elapsedSeconds = elapsedSeconds,
                .frameIndex = completedFrames,
                .framebufferWidth = framebufferWidth,
                .framebufferHeight = framebufferHeight,
                .window = window->nativeHandle(),
                .uiWantsMouse = uiWantsMouse,
                .uiWantsKeyboard = uiWantsKeyboard};
            project->update(frame);

            glViewport(0, 0, framebufferWidth, framebufferHeight);
            glClearColor(0.045f, 0.055f, 0.075f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            project->render(frame);

            project->drawGui();
            ImGui::Render();
            // Project code may enable framebuffer sRGB for its final pass.
            // ImGui colors are already display-encoded and must not be
            // converted a second time.
            glDisable(GL_FRAMEBUFFER_SRGB);
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            window->swapBuffers();
            ++completedFrames;
        }

        // Project-owned GL objects are released while the context is still current.
        project.reset();
    }

    return ExitCode::success;
}

} // namespace openglgp::framework
