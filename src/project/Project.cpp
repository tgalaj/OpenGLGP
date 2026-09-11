#include "project/Project.h"

#include <glad/glad.h>
#include <imgui.h>

namespace openglgp::project
{

void Project::update(const framework::FrameContext&)
{
}

void Project::render(const framework::FrameContext& frame)
{
    // A small non-uniform smoke-test image keeps the unmodified template
    // useful in headless CI without providing assignment geometry.
    const GLsizei halfWidth = static_cast<GLsizei>(frame.framebufferWidth / 2);

    glEnable(GL_SCISSOR_TEST);
    glScissor(0, 0, halfWidth, frame.framebufferHeight);
    glClearColor(0.07f, 0.12f, 0.20f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glScissor(halfWidth, 0, static_cast<GLsizei>(frame.framebufferWidth - halfWidth), frame.framebufferHeight);
    glClearColor(0.16f, 0.08f, 0.18f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glDisable(GL_SCISSOR_TEST);
}

void Project::drawGui()
{
    ImGui::SetNextWindowPos(ImVec2(16.0f, 16.0f), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(360.0f, 110.0f), ImGuiCond_FirstUseEver);
    ImGui::Begin("OpenGLGP");
    ImGui::TextUnformatted("Project hook is ready.");
    ImGui::TextUnformatted("Add assignment code under src/project.");
    ImGui::End();
}

} // namespace openglgp::project
