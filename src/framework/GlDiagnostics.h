#pragma once

#include <filesystem>
#include <string>
#include <vector>

namespace openglgp::framework
{

struct GlReport
{
    std::string vendor;
    std::string renderer;
    std::string glVersion;
    std::string glslVersion;
    int majorVersion = 0;
    int minorVersion = 0;
    bool coreProfile = false;
    bool debugContext = false;
    bool srgbFramebuffer = false;
    int maxUniformBufferBindings = 0;
    int maxShaderStorageBufferBindings = 0;
    int maxTextureSize = 0;
    int maxCombinedTextureImageUnits = 0;
    int maxVertexAttribs = 0;
    bool timerQuerySupport = false;
    bool dsaFunctionsAvailable = false;

    [[nodiscard]] std::vector<std::string> complianceFailures() const;
    [[nodiscard]] bool compliant() const;
    [[nodiscard]] std::string toText() const;
    [[nodiscard]] std::string toJson() const;
};

[[nodiscard]] GlReport collectGlReport(bool srgbFramebuffer);
void installGlDebugCallback();
void logGlInfo(const GlReport& report);
[[nodiscard]] bool writeGlReportJson(
    const GlReport& report,
    const std::filesystem::path& path,
    std::string& error);

} // namespace openglgp::framework
