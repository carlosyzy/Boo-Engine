#include "gfx-builtin-pipeline.h"
#include "../../gfx.h"
#include "../../gfx-context.h"
#include "../../gfx-renderer.h"
#include "../../base/gfx-pipeline.h"
#include "../../base/gfx-shader.h"
#include "../../base/gfx-render-pass.h"
#include "../../../log.h"

GfxBuiltinPipeline::GfxBuiltinPipeline(const std::string &name) : GfxPipeline(name)
{
}

void GfxBuiltinPipeline::_createPipeline()
{
    GfxPipeline::_createPipeline();
}

void GfxBuiltinPipeline::_initShaderState()
{
    GfxPipeline::_initShaderState();
}
/**
 * @brief 初始化顶点输入状态
 * 基础的顶点输入状态，只包含位置和纹理坐标
 */
void GfxBuiltinPipeline::_initVertexInputState()
{
}
void GfxBuiltinPipeline::_initInputAssemblyState()
{
    GfxPipeline::_initInputAssemblyState();
}
void GfxBuiltinPipeline::_initDynamicState()
{
    this->_dynamicStateInfo = {};
    this->_dynamicStates = {
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_SCISSOR,
        VK_DYNAMIC_STATE_STENCIL_REFERENCE,
		VK_DYNAMIC_STATE_STENCIL_COMPARE_MASK,
		VK_DYNAMIC_STATE_STENCIL_WRITE_MASK
    };
    this->_dynamicStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    this->_dynamicStateInfo.dynamicStateCount = static_cast<uint32_t>(this->_dynamicStates.size());
    this->_dynamicStateInfo.pDynamicStates = this->_dynamicStates.data();
}
void GfxBuiltinPipeline::_initViewportState()
{
    this->_viewport = {};
    this->_viewport.x = 0.0f;
    this->_viewport.y = 0.0f;
    this->_viewport.width = (float)Gfx::_context->getSwapChainExtent().width;
    this->_viewport.height = (float)Gfx::_context->getSwapChainExtent().height;
    this->_viewport.minDepth = 0.0f;
    this->_viewport.maxDepth = 1.0f;

    // 裁剪定义哪一块区域的像素实际被存储在帧缓存中。任何位于裁剪范围之外都会被光栅化丢弃
    this->_scissor = {};
    this->_scissor.offset = {0, 0};
    this->_scissor.extent = Gfx::_context->getSwapChainExtent();

    this->_viewportInfo = {};
    this->_viewportInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    this->_viewportInfo.viewportCount = 1;
    // this->_viewportInfo.pViewports = &this->_viewport;
    this->_viewportInfo.scissorCount = 1;
    // this->_viewportInfo.pScissors = &this->_scissor;
}
void GfxBuiltinPipeline::_initRasterizationState()
{
    GfxPipeline::_initRasterizationState();
}

void GfxBuiltinPipeline::_initMultisampleState()
{
    this->_multisampleInfo = {};
    this->_multisampleInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    this->_multisampleInfo.sampleShadingEnable = VK_FALSE;               // MsaaSamples == VK_SAMPLE_COUNT_1_BIT ? VK_FALSE : VK_TRUE;
    this->_multisampleInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT; // MsaaSamples
    this->_multisampleInfo.minSampleShading = 1.0f;
    this->_multisampleInfo.pSampleMask = nullptr;
    this->_multisampleInfo.alphaToCoverageEnable = VK_FALSE;
    this->_multisampleInfo.alphaToOneEnable = VK_FALSE;
}
void GfxBuiltinPipeline::_initDepthStencilState()
{
    this->_depthStencilInfo = {};
    this->_depthStencilInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;

    // 根据配置启用深度测试
    this->_depthStencilInfo.depthTestEnable = this->_rendererState.depthTest != 0 ? VK_TRUE : VK_FALSE;
    this->_depthStencilInfo.depthWriteEnable = this->_rendererState.depthWrite != 0 ? VK_TRUE : VK_FALSE;
    this->_depthStencilInfo.depthCompareOp = this->_getCompareOp(this->_rendererState.depthCompareOp);

    // 根据配置启用Stencil测试
    this->_depthStencilInfo.stencilTestEnable = this->_rendererState.stencilTest != 0 ? VK_TRUE : VK_FALSE;

    if (this->_rendererState.stencilTest != 0)
    {
        // 配置正面Stencil操作
        this->_depthStencilInfo.front.failOp = this->_getStencilOp(this->_rendererState.stencilFrontFailOp);
        this->_depthStencilInfo.front.depthFailOp = this->_getStencilOp(this->_rendererState.stencilFrontDepthFailOp);
        this->_depthStencilInfo.front.passOp = this->_getStencilOp(this->_rendererState.stencilFrontPassOp);
        this->_depthStencilInfo.front.compareOp = this->_getCompareOp(this->_rendererState.stencilFrontCompareOp);
        this->_depthStencilInfo.front.compareMask = this->_rendererState.stencilFrontCompareMask;
        this->_depthStencilInfo.front.writeMask = this->_rendererState.stencilFrontWriteMask;
        this->_depthStencilInfo.front.reference = this->_rendererState.stencilFrontRreference;

        // 配置背面Stencil操作
        this->_depthStencilInfo.back.failOp = this->_getStencilOp(this->_rendererState.stencilBackFailOp);
        this->_depthStencilInfo.back.depthFailOp = this->_getStencilOp(this->_rendererState.stencilBackDepthFailOp);
        this->_depthStencilInfo.back.passOp = this->_getStencilOp(this->_rendererState.stencilBackPassOp);
        this->_depthStencilInfo.back.compareOp = this->_getCompareOp(this->_rendererState.stencilBackCompareOp);
        this->_depthStencilInfo.back.compareMask = this->_rendererState.stencilBackCompareMask;
        this->_depthStencilInfo.back.writeMask = this->_rendererState.stencilBackWriteMask;
        this->_depthStencilInfo.back.reference = this->_rendererState.stencilBackRreference;
    }
}
void GfxBuiltinPipeline::_initColorBlendState()
{
    this->_colorBlendAttachment = {};
    this->_colorBlendAttachment.blendEnable = this->_rendererState.colorBlend != 0 ? VK_TRUE : VK_FALSE;
    this->_colorBlendAttachment.srcColorBlendFactor = this->_getBlendFactor(this->_rendererState.srcColorBlendFactor); // VK_BLEND_FACTOR_ONE;
    this->_colorBlendAttachment.dstColorBlendFactor = this->_getBlendFactor(this->_rendererState.dstColorBlendFactor); // VK_BLEND_FACTOR_ZERO;
    this->_colorBlendAttachment.colorBlendOp = this->_getBlendOp(this->_rendererState.colorBlendOp);                   // VK_BLEND_OP_ADD;
    this->_colorBlendAttachment.srcAlphaBlendFactor = this->_getBlendFactor(this->_rendererState.srcAlphaBlendFactor); // VK_BLEND_FACTOR_ONE;
    this->_colorBlendAttachment.dstAlphaBlendFactor = this->_getBlendFactor(this->_rendererState.dstAlphaBlendFactor); // VK_BLEND_FACTOR_ZERO;
    this->_colorBlendAttachment.alphaBlendOp = this->_getBlendOp(this->_rendererState.alphaBlendOp);                   // VK_BLEND_OP_ADD;
    this->_colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;

    this->_colorBlendInfo = {};
    this->_colorBlendInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    this->_colorBlendInfo.logicOpEnable = VK_FALSE;
    this->_colorBlendInfo.attachmentCount = 1;
    this->_colorBlendInfo.pAttachments = &this->_colorBlendAttachment;
    this->_colorBlendInfo.logicOp = VK_LOGIC_OP_COPY;
    this->_colorBlendInfo.blendConstants[0] = 0.0f;
    this->_colorBlendInfo.blendConstants[1] = 0.0f;
    this->_colorBlendInfo.blendConstants[2] = 0.0f;
    this->_colorBlendInfo.blendConstants[3] = 0.0f;
}
void GfxBuiltinPipeline::_initPipelineLayout()
{
    this->_pipelineLayoutInfo = {};
    this->_pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    // push constant
    this->_pipelineLayoutInfo.pushConstantRangeCount = 0;
    // 绑定描述符集布局
    std::vector<VkDescriptorSetLayout> setLayouts;
    setLayouts.push_back(this->_descriptorSetLayout);
    this->_pipelineLayoutInfo.setLayoutCount = setLayouts.size();
    this->_pipelineLayoutInfo.pSetLayouts = setLayouts.data();
    // 第八步：管线布局
    if (vkCreatePipelineLayout(Gfx::_context->getVkDevice(), &this->_pipelineLayoutInfo, nullptr, &this->_vkPipelineLayout) != VK_SUCCESS)
    {
        LOGI("[Gfx : PipelineBuiltin] :: create pipeline layout failed %s", this->_name.c_str());
        return;
    }
}
void GfxBuiltinPipeline::_initPipeline()
{
    this->_pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    this->_pipelineInfo.stageCount = this->_shaderStages.size();
    this->_pipelineInfo.pStages = this->_shaderStages.data();
    this->_pipelineInfo.pVertexInputState = &this->_vertexInputInfo;
    this->_pipelineInfo.pInputAssemblyState = &this->_inputAssemblyInfo;
    this->_pipelineInfo.pViewportState = &this->_viewportInfo;
    this->_pipelineInfo.pDynamicState = &this->_dynamicStateInfo; // 关键：设置动态状态
    this->_pipelineInfo.pRasterizationState = &this->_rasterizationInfo;
    this->_pipelineInfo.pMultisampleState = &this->_multisampleInfo;
    this->_pipelineInfo.pColorBlendState = &this->_colorBlendInfo;
    this->_pipelineInfo.pDepthStencilState = &this->_depthStencilInfo;
    this->_pipelineInfo.layout = this->_vkPipelineLayout;
    this->_pipelineInfo.renderPass = this->_pass->getVKRenderPass();
    this->_pipelineInfo.subpass = 0;
    this->_pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
    if (vkCreateGraphicsPipelines(Gfx::_context->getVkDevice(), VK_NULL_HANDLE, 1, &this->_pipelineInfo, nullptr, &this->_vkPipeline) != VK_SUCCESS)
    {
        LOGI("[Gfx : PipelineBuiltin] :: create pipeline failed %s", this->_name.c_str());
        return;
    }
    LOGI("[Gfx : PipelineBuiltin] :: create pipeline success %s", this->_name.c_str());
}

GfxBuiltinPipeline::~GfxBuiltinPipeline()
{
}