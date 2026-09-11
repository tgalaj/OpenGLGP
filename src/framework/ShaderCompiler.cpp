#include "framework/ShaderCompiler.h"

#include <glad/glad.h>

#include <fstream>
#include <sstream>
#include <string>
#include <vector>

namespace openglgp::framework
{
namespace
{

std::string shaderLog(const GLuint shader)
{
    GLint length = 0;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
    if (length <= 1)
    {
        return {};
    }

    std::string log(static_cast<std::size_t>(length), '\0');
    GLsizei written = 0;
    glGetShaderInfoLog(shader, length, &written, log.data());
    log.resize(static_cast<std::size_t>(written));
    return log;
}

std::string programLog(const GLuint program)
{
    GLint length = 0;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
    if (length <= 1)
    {
        return {};
    }

    std::string log(static_cast<std::size_t>(length), '\0');
    GLsizei written = 0;
    glGetProgramInfoLog(program, length, &written, log.data());
    log.resize(static_cast<std::size_t>(written));
    return log;
}

} // namespace

ShaderBuildResult ShaderCompiler::compileFile(
    const unsigned int shaderType,
    const std::filesystem::path& path)
{
    std::ifstream input(path, std::ios::binary);
    if (!input)
    {
        return {0, "Could not open shader file: " + path.string()};
    }

    std::ostringstream source;
    source << input.rdbuf();
    if (!input.good() && !input.eof())
    {
        return {0, "Could not read shader file: " + path.string()};
    }
    return compileSource(shaderType, source.str(), path);
}

ShaderBuildResult ShaderCompiler::compileSource(
    const unsigned int shaderType,
    const std::string& source,
    const std::filesystem::path& sourcePath)
{
    const GLuint shader = glCreateShader(shaderType);
    if (shader == 0)
    {
        return {0, "glCreateShader failed for: " + sourcePath.string()};
    }

    const char* sourcePointer = source.c_str();
    const auto sourceLength = static_cast<GLint>(source.size());
    glShaderSource(shader, 1, &sourcePointer, &sourceLength);
    glCompileShader(shader);

    GLint compiled = GL_FALSE;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    const std::string log = shaderLog(shader);
    if (compiled != GL_TRUE)
    {
        glDeleteShader(shader);
        return { 0, "Shader compilation failed for " + sourcePath.string() + (log.empty() ? std::string{} : ":\n" + log) };
    }

    return { shader, log.empty() ? std::string{} : "Shader compiler output for " + sourcePath.string() + ":\n" + log };
}

ShaderBuildResult ShaderCompiler::linkProgram(const std::span<const unsigned int> shaders)
{
    if (shaders.empty())
    {
        return {0, "Cannot link a program without shaders."};
    }

    const GLuint program = glCreateProgram();
    if (program == 0)
    {
        return {0, "glCreateProgram failed."};
    }

    for (const GLuint shader : shaders)
    {
        glAttachShader(program, shader);
    }
    
    glLinkProgram(program);

    for (const GLuint shader : shaders)
    {
        glDetachShader(program, shader);
    }

    GLint linked = GL_FALSE;
    glGetProgramiv(program, GL_LINK_STATUS, &linked);
    const std::string log = programLog(program);
    if (linked != GL_TRUE)
    {
        glDeleteProgram(program);
        return { 0, "Shader program link failed" + (log.empty() ? std::string(".") : ":\n" + log) };
    }

    return { program, log.empty() ? std::string{} : "Shader linker output:\n" + log };
}

} // namespace openglgp::framework
