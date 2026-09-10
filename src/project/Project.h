#pragma once

#include "framework/FrameContext.h"

namespace openglgp::project
{

class Project
{
public:
    void update(const framework::FrameContext& frame);
    void render(const framework::FrameContext& frame);
    void drawGui();
};

} // namespace openglgp::project
