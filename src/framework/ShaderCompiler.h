#pragma once

#include <filesystem>
#include <span>
#include <string>

namespace openglgp::framework
{

struct ShaderBuildResult
{
    unsigned int object = 0;
    std::string diagnostics;

    [[nodiscard]] explicit operator bool() const noexcept
    {
        return object != 0;
    }
};

class ShaderCompiler
{
public:
    [[nodiscard]] static ShaderBuildResult compileFile(
        unsigned int shaderType,
        const std::filesystem::path& path);
    [[nodiscard]] static ShaderBuildResult linkProgram(
        std::span<const unsigned int> shaders);

private:
    [[nodiscard]] static ShaderBuildResult compileSource(
        unsigned int shaderType,
        const std::string& source,
        const std::filesystem::path& sourcePath);
};

} // namespace openglgp::framework
