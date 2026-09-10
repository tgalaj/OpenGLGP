#include "framework/RunConfig.h"

#include <iostream>
#include <stdexcept>
#include <string_view>
#include <vector>

namespace
{

using openglgp::framework::RunConfig;
using openglgp::framework::parseRunConfig;

void expect(const bool condition, const std::string_view message)
{
    if (!condition)
    {
        throw std::runtime_error(std::string(message));
    }
}

void testDefaults()
{
    const std::vector<std::string_view> arguments;
    const auto result = parseRunConfig(arguments);
    expect(static_cast<bool>(result), "default command line should parse");
    expect(!result.config.diagnostics, "diagnostics should be disabled by default");
    expect(!result.config.showHelp, "help should be disabled by default");
    expect(RunConfig::defaultWidth == 1280, "default width should be 1280");
    expect(RunConfig::defaultHeight == 720, "default height should be 720");
}

void testDiagnosticsFlag()
{
    const std::vector<std::string_view> arguments{"--diagnostics"};
    const auto result = parseRunConfig(arguments);
    expect(static_cast<bool>(result), "diagnostics flag should parse");
    expect(result.config.diagnostics, "diagnostics flag should be preserved");
}

void testInvalidArguments()
{
    {
        const std::vector<std::string_view> arguments{"--diagnostics-json"};
        expect(!parseRunConfig(arguments), "missing diagnostics path should be rejected");
    }
    {
        const std::vector<std::string_view> arguments{"--unknown"};
        expect(!parseRunConfig(arguments), "unknown option should be rejected");
    }
}

void testDiagnosticsJsonImpliesDiagnostics()
{
    const std::vector<std::string_view> arguments{
        "--diagnostics-json",
        "diagnostics/report.json"};
    const auto result = parseRunConfig(arguments);
    expect(static_cast<bool>(result), "diagnostics JSON command line should parse");
    expect(result.config.diagnostics, "diagnostics JSON should enable diagnostics mode");
    expect(result.config.diagnosticsJson.has_value(), "diagnostics JSON path should be set");
}

} // namespace

int main()
{
    try
    {
        testDefaults();
        testDiagnosticsFlag();
        testInvalidArguments();
        testDiagnosticsJsonImpliesDiagnostics();
        std::cout << "All framework contract tests passed.\n";
        return 0;
    }
    catch (const std::exception& exception)
    {
        std::cerr << "Framework test failure: " << exception.what() << '\n';
        return 1;
    }
}
