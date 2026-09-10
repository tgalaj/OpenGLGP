#include "framework/GlDiagnostics.h"

#include <glad/glad.h>

#include <spdlog/spdlog.h>

#include <charconv>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string_view>
#include <system_error>

namespace openglgp::framework
{
namespace
{

std::string glString(const GLenum name)
{
    const auto* value = glGetString(name);
    return value != nullptr ? reinterpret_cast<const char*>(value) : "unavailable";
}

bool glslAtLeast450(const std::string& version)
{
    const auto separator = version.find('.');
    if (separator == std::string::npos)
    {
        return false;
    }

    int major = 0;
    int minor = 0;
    const auto majorResult =
        std::from_chars(version.data(), version.data() + separator, major);
    const auto minorStart = version.data() + separator + 1;
    const auto minorResult =
        std::from_chars(minorStart, version.data() + version.size(), minor);
    return majorResult.ec == std::errc{} &&
           majorResult.ptr == version.data() + separator &&
           minorResult.ec == std::errc{} &&
           (major > 4 || (major == 4 && minor >= 50));
}

std::string jsonEscape(const std::string_view value)
{
    std::string escaped;
    escaped.reserve(value.size());
    for (const char character : value)
    {
        switch (character)
        {
        case '"':
            escaped += "\\\"";
            break;
        case '\\':
            escaped += "\\\\";
            break;
        case '\b':
            escaped += "\\b";
            break;
        case '\f':
            escaped += "\\f";
            break;
        case '\n':
            escaped += "\\n";
            break;
        case '\r':
            escaped += "\\r";
            break;
        case '\t':
            escaped += "\\t";
            break;
        default:
            if (static_cast<unsigned char>(character) >= 0x20)
            {
                escaped += character;
            }
            break;
        }
    }
    return escaped;
}

const char* jsonBoolean(const bool value) noexcept
{
    return value ? "true" : "false";
}

void APIENTRY debugMessageCallback(
    GLenum,
    const GLenum type,
    GLuint,
    const GLenum severity,
    GLsizei,
    const GLchar* message,
    const void*)
{
    const char* text = message != nullptr ? message : "OpenGL debug message without text";
    if (type == GL_DEBUG_TYPE_ERROR || severity == GL_DEBUG_SEVERITY_HIGH)
    {
        spdlog::error("OpenGL: {}", text);
    }
    else if (severity == GL_DEBUG_SEVERITY_MEDIUM)
    {
        spdlog::warn("OpenGL: {}", text);
    }
    else
    {
        spdlog::debug("OpenGL: {}", text);
    }
}

} // namespace

std::vector<std::string> GlReport::complianceFailures() const
{
    std::vector<std::string> failures;
    if (majorVersion < 4 || (majorVersion == 4 && minorVersion < 5))
    {
        failures.emplace_back("OpenGL 4.5 or newer is required");
    }
    if (!glslAtLeast450(glslVersion))
    {
        failures.emplace_back("GLSL 4.50 or newer is required");
    }
    if (!coreProfile)
    {
        failures.emplace_back("an OpenGL Core profile is required");
    }
    if (!timerQuerySupport)
    {
        failures.emplace_back("timer query entry points are unavailable");
    }
    if (!dsaFunctionsAvailable)
    {
        failures.emplace_back("required OpenGL 4.5 DSA entry points are unavailable");
    }
    if (maxUniformBufferBindings < 84)
    {
        failures.emplace_back("fewer than 84 uniform-buffer bindings are available");
    }
    if (maxShaderStorageBufferBindings < 8)
    {
        failures.emplace_back("fewer than 8 shader-storage-buffer bindings are available");
    }
    if (maxTextureSize < 16384)
    {
        failures.emplace_back("maximum 2D texture size is below 16384");
    }
    if (maxCombinedTextureImageUnits < 96)
    {
        failures.emplace_back("fewer than 96 combined texture image units are available");
    }
    if (maxVertexAttribs < 16)
    {
        failures.emplace_back("fewer than 16 vertex attributes are available");
    }
    return failures;
}

bool GlReport::compliant() const
{
    return complianceFailures().empty();
}

std::string GlReport::toText() const
{
    std::ostringstream output;
    output << "OpenGL diagnostics\n"
           << "  Vendor: " << vendor << '\n'
           << "  Renderer: " << renderer << '\n'
           << "  OpenGL: " << glVersion << '\n'
           << "  GLSL: " << glslVersion << '\n'
           << "  Core profile: " << (coreProfile ? "yes" : "no") << '\n'
           << "  Debug context: " << (debugContext ? "yes" : "no") << '\n'
           << "  sRGB framebuffer: " << (srgbFramebuffer ? "yes" : "no") << '\n'
           << "  UBO bindings: " << maxUniformBufferBindings << '\n'
           << "  SSBO bindings: " << maxShaderStorageBufferBindings << '\n'
           << "  Max texture size: " << maxTextureSize << '\n'
           << "  Combined texture units: " << maxCombinedTextureImageUnits << '\n'
           << "  Vertex attributes: " << maxVertexAttribs << '\n'
           << "  Timer queries: " << (timerQuerySupport ? "yes" : "no") << '\n'
           << "  OpenGL 4.5 DSA functions: " << (dsaFunctionsAvailable ? "yes" : "no")
           << '\n'
           << "  Compliant: " << (compliant() ? "yes" : "no") << '\n';

    for (const auto& failure : complianceFailures())
    {
        output << "  Failure: " << failure << '\n';
    }
    return output.str();
}

std::string GlReport::toJson() const
{
    std::ostringstream output;
    output << "{\n"
           << "  \"vendor\": \"" << jsonEscape(vendor) << "\",\n"
           << "  \"renderer\": \"" << jsonEscape(renderer) << "\",\n"
           << "  \"openglVersion\": \"" << jsonEscape(glVersion) << "\",\n"
           << "  \"glslVersion\": \"" << jsonEscape(glslVersion) << "\",\n"
           << "  \"coreProfile\": " << jsonBoolean(coreProfile) << ",\n"
           << "  \"debugContext\": " << jsonBoolean(debugContext) << ",\n"
           << "  \"srgbFramebuffer\": " << jsonBoolean(srgbFramebuffer) << ",\n"
           << "  \"limits\": {\n"
           << "    \"uniformBufferBindings\": " << maxUniformBufferBindings << ",\n"
           << "    \"shaderStorageBufferBindings\": " << maxShaderStorageBufferBindings << ",\n"
           << "    \"maxTextureSize\": " << maxTextureSize << ",\n"
           << "    \"combinedTextureImageUnits\": " << maxCombinedTextureImageUnits << ",\n"
           << "    \"vertexAttribs\": " << maxVertexAttribs << "\n"
           << "  },\n"
           << "  \"timerQuerySupport\": " << jsonBoolean(timerQuerySupport) << ",\n"
           << "  \"dsaFunctionsAvailable\": " << jsonBoolean(dsaFunctionsAvailable) << ",\n"
           << "  \"compliant\": " << jsonBoolean(compliant()) << ",\n"
           << "  \"failures\": [";

    const auto failures = complianceFailures();
    for (std::size_t index = 0; index < failures.size(); ++index)
    {
        output << (index == 0 ? "\n" : ",\n")
               << "    \"" << jsonEscape(failures[index]) << '"';
    }
    if (!failures.empty())
    {
        output << '\n';
    }
    output << "  ]\n}\n";
    return output.str();
}

GlReport collectGlReport(const bool srgbFramebuffer)
{
    GlReport report;
    report.vendor = glString(GL_VENDOR);
    report.renderer = glString(GL_RENDERER);
    report.glVersion = glString(GL_VERSION);
    report.glslVersion = glString(GL_SHADING_LANGUAGE_VERSION);
    report.srgbFramebuffer = srgbFramebuffer;

    GLint profile = 0;
    GLint flags = 0;
    glGetIntegerv(GL_MAJOR_VERSION, &report.majorVersion);
    glGetIntegerv(GL_MINOR_VERSION, &report.minorVersion);
    glGetIntegerv(GL_CONTEXT_PROFILE_MASK, &profile);
    glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
    report.coreProfile = (profile & GL_CONTEXT_CORE_PROFILE_BIT) != 0;
    report.debugContext = (flags & GL_CONTEXT_FLAG_DEBUG_BIT) != 0;

    glGetIntegerv(GL_MAX_UNIFORM_BUFFER_BINDINGS, &report.maxUniformBufferBindings);
    glGetIntegerv(
        GL_MAX_SHADER_STORAGE_BUFFER_BINDINGS,
        &report.maxShaderStorageBufferBindings);
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &report.maxTextureSize);
    glGetIntegerv(
        GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS,
        &report.maxCombinedTextureImageUnits);
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &report.maxVertexAttribs);

    report.timerQuerySupport =
        glad_glQueryCounter != nullptr && glad_glGetQueryObjectui64v != nullptr;
    report.dsaFunctionsAvailable =
        glad_glCreateBuffers != nullptr &&
        glad_glNamedBufferStorage != nullptr &&
        glad_glCreateVertexArrays != nullptr &&
        glad_glVertexArrayVertexBuffer != nullptr &&
        glad_glCreateTextures != nullptr &&
        glad_glTextureStorage2D != nullptr &&
        glad_glCreateFramebuffers != nullptr &&
        glad_glNamedFramebufferTexture != nullptr;
    return report;
}

void installGlDebugCallback()
{
    if (glad_glDebugMessageCallback == nullptr)
    {
        return;
    }

    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(debugMessageCallback, nullptr);
    glDebugMessageControl(
        GL_DONT_CARE,
        GL_DONT_CARE,
        GL_DEBUG_SEVERITY_NOTIFICATION,
        0,
        nullptr,
        GL_FALSE);
}

void logGlInfo(const GlReport& report)
{
    spdlog::info("OpenGL vendor: {}", report.vendor);
    spdlog::info("OpenGL renderer: {}", report.renderer);
    spdlog::info("OpenGL version: {}", report.glVersion);
    spdlog::info("GLSL version: {}", report.glslVersion);
    spdlog::info(
        "Context: Core={}, Debug={}, sRGB={}",
        report.coreProfile,
        report.debugContext,
        report.srgbFramebuffer);
}

bool writeGlReportJson(
    const GlReport& report,
    const std::filesystem::path& path,
    std::string& error)
{
    std::error_code filesystemError;
    if (!path.parent_path().empty())
    {
        std::filesystem::create_directories(path.parent_path(), filesystemError);
        if (filesystemError)
        {
            error = "Could not create diagnostics directory: " + filesystemError.message();
            return false;
        }
    }

    std::ofstream output(path, std::ios::binary);
    if (!output)
    {
        error = "Could not open diagnostics JSON for writing: " + path.string();
        return false;
    }
    output << report.toJson();
    if (!output)
    {
        error = "Could not write diagnostics JSON: " + path.string();
        return false;
    }
    return true;
}

} // namespace openglgp::framework
