#include "framework/RunConfig.h"

#include <string>
#include <utility>
#include <vector>

namespace openglgp::framework
{

ParseResult parseRunConfig(
    const std::span<const std::string_view> arguments,
    std::filesystem::path executablePath)
{
    ParseResult result;
    result.config.executablePath = std::move(executablePath);

    const auto fail = [&result](std::string message) {
        result.error = std::move(message);
    };

    for (std::size_t index = 0; index < arguments.size(); ++index)
    {
        const std::string_view option = arguments[index];
        const auto valueAfter = [&](const std::string_view name) -> std::optional<std::string_view> {
            if (index + 1 >= arguments.size())
            {
                fail("Missing value after " + std::string(name) + '.');
                return std::nullopt;
            }
            return arguments[++index];
        };

        if (option == "--help" || option == "-h")
        {
            result.config.showHelp = true;
        }
        else if (option == "--diagnostics")
        {
            result.config.diagnostics = true;
        }
        else if (option == "--diagnostics-json")
        {
            const auto value = valueAfter(option);
            if (!value)
            {
                return result;
            }
            if (value->empty())
            {
                fail("The diagnostics JSON path cannot be empty.");
                return result;
            }
            result.config.diagnostics = true;
            result.config.diagnosticsJson = std::filesystem::path(*value);
        }
        else
        {
            fail("Unknown option: " + std::string(option));
            return result;
        }
    }

    return result;
}

ParseResult parseRunConfig(const int argc, char* argv[])
{
    std::vector<std::string_view> arguments;
    arguments.reserve(argc > 1 ? static_cast<std::size_t>(argc - 1) : 0);
    for (int index = 1; index < argc; ++index)
    {
        arguments.emplace_back(argv[index]);
    }

    const std::filesystem::path executablePath =
        argc > 0 && argv[0] != nullptr ? std::filesystem::path(argv[0]) : std::filesystem::path{};
    return parseRunConfig(arguments, executablePath);
}

std::string_view commandLineHelp() noexcept
{
    return
        "Usage: OpenGLGP [options]\n"
        "  --diagnostics                 Print OpenGL 4.5 capability diagnostics\n"
        "  --diagnostics-json PATH       Also write diagnostics as JSON\n"
        "  --help, -h                    Show this help\n";
}

} // namespace openglgp::framework
