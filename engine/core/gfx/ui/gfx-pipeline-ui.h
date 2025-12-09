#pragma once
#include "gfx-pipeline.h"
class GfxPipelineUI : public GfxPipeline
{
protected:
    void _initVertexInputState() override;

public:
    GfxPipelineUI(const std::string &name);
    ~GfxPipelineUI() {}
};