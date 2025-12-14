
#pragma once
#include <string>
#include <vector>
#include <vulkan/vulkan_core.h>
#include "../base/gfx-pipeline.h"

class GfxPipelineBuiltin : public GfxPipeline
{
protected:
    void _createPipeline() override;
    void _initShaderState() override;
    void _initVertexInputState() override;
    void _initUIVertexInputState();
    void _init3DVertexInputState();
    void _initInputAssemblyState() override;
    void _initDynamicState() override;
    void _initViewportState() override;
    void _initRasterizationState() override;
    void _initMultisampleState() override;
    void _initDepthStencilState() override;
    void _initColorBlendState() override;
    void _initPipelineLayout() override;
    void _initPipeline() override;

public:
    GfxPipelineBuiltin(const std::string &name);
    virtual ~GfxPipelineBuiltin();
};