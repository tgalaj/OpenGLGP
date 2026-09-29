#pragma once

#include <filesystem>
#include <optional>
#include <span>
#include <string>
#include <string_view>

namespace openglgp::framework
{

struct RunConfig
{
    static constexpr int defaultWidth = 1280;
    static constexpr int defaultHeight = 720;

    bool diagnostics = false;
    std::optional<std::filesystem::path> diagnosticsJson;
    bool showHelp = false;
    std::filesystem::path executablePath;
};

struct ParseResult
{
    RunConfig config;
    std::string error;

    [[nodiscard]] explicit operator bool() const noexcept
    {
        return error.empty();
    }
};

[[nodiscard]] ParseResult parseRunConfig(std::span<const std::string_view> arguments, std::filesystem::path executablePath = {});
[[nodiscard]] ParseResult parseRunConfig(int argc, char* argv[]);
[[nodiscard]] std::string_view commandLineHelp() noexcept;

} // namespace openglgp::framework
