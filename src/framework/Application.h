#pragma once

#include "framework/RunConfig.h"

namespace openglgp::framework
{

enum class ExitCode : int
{
    success = 0,
    commandLineError = 2,
    initializationFailure = 3,
    diagnosticsNonCompliant = 4,
    outputFailure = 5,
    runtimeFailure = 6
};

class Application
{
public:
    explicit Application(RunConfig config);

    [[nodiscard]] ExitCode run();

private:
    RunConfig config_;
};

} // namespace openglgp::framework
