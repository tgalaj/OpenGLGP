#include "framework/Application.h"
#include "framework/RunConfig.h"

#include <spdlog/spdlog.h>

#include <exception>
#include <iostream>

int main(const int argc, char* argv[])
{
    using openglgp::framework::Application;
    using openglgp::framework::ExitCode;
    using openglgp::framework::commandLineHelp;
    using openglgp::framework::parseRunConfig;

    const auto parsed = parseRunConfig(argc, argv);
    if (!parsed)
    {
        spdlog::error("{}", parsed.error);
        std::cerr << commandLineHelp();
        return static_cast<int>(ExitCode::commandLineError);
    }
    if (parsed.config.showHelp)
    {
        std::cout << commandLineHelp();
        return static_cast<int>(ExitCode::success);
    }

    try
    {
        Application application(parsed.config);
        return static_cast<int>(application.run());
    }
    catch (const std::exception& exception)
    {
        spdlog::critical("Unhandled exception: {}", exception.what());
        return static_cast<int>(ExitCode::runtimeFailure);
    }
    catch (...)
    {
        spdlog::critical("Unhandled non-standard exception.");
        return static_cast<int>(ExitCode::runtimeFailure);
    }
}
