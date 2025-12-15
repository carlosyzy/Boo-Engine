#include "gfx-pipeline.h"
#include "../gfx.h"
#include "../gfx-context.h"
#include "gfx-shader.h"

GfxPipeline::GfxPipeline(const std::string &name)
{
    this->_name = name;
}
const std::string &GfxPipeline::getName()
{
    return this->_name;
}
void GfxPipeline::create(GfxRenderPass *pass, GfxShader *vertexShader, GfxShader *fragmentShader, VkDescriptorSetLayout descriptorSetLayout, GfxPipelineStruct pipelineStruct)
{
    if (pass == nullptr)
    {
        std::cout << "[Gfx : GfxPipeline]::create: pass  is nullptr" << std::endl;
        return;
    }
    if (vertexShader == nullptr)
    {
        std::cout << "[Gfx : GfxPipeline]::create: vertexShader  is nullptr" << std::endl;
        return;
    }
    if (fragmentShader == nullptr)
    {
        std::cout << "[Gfx : GfxPipeline]::create: fragmentShader  is nullptr" << std::endl;
        return;
    }
    if (descriptorSetLayout == nullptr)
    {
        std::cout << "[Gfx : GfxPipeline]::create: descriptorSetLayout  is nullptr" << std::endl;
        return;
    }

    this->_pass = pass;
    this->_vertexShader = vertexShader;
    this->_fragmentShader = fragmentShader;
    this->_descriptorSetLayout = descriptorSetLayout;
    this->_pipelineStruct = pipelineStruct;
    this->_createPipeline();
}
void GfxPipeline::_createPipeline()
{
    this->_initShaderState();
    this->_initVertexInputState();
    this->_initInputAssemblyState();
    this->_initDynamicState();
    this->_initViewportState();
    this->_initRasterizationState();
    this->_initMultisampleState();
    this->_initDepthStencilState();
    this->_initColorBlendState();
    this->_initPipelineLayout();
    this->_initPipeline();
}

void GfxPipeline::_initShaderState()
{
    this->_vertShaderStageInfo = {};
    // 初始化顶点着色器状态
    this->_vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    // VK_SHADER_STAGE_VERTEX_BIT代表是顶点着色器
    this->_vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
    this->_vertShaderStageInfo.module = this->_vertexShader->getShaderModule();
    // 指定想调用什么函数。这意味着可以将多个片段着色器组合到一个着色器模块中，并使用不同的入口点来区分它们的行为。
    this->_vertShaderStageInfo.pName = "main";
    // 初始化片段着色器状态
    this->_fragShaderStageInfo = {};
    this->_fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    // VK_SHADER_STAGE_FRAGMENT_BIT代表是片段着色器
    this->_fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    this->_fragShaderStageInfo.module = this->_fragmentShader->getShaderModule();
    // 指定想调用什么函数。这意味着可以将多个片段着色器组合到一个着色器模块中，并使用不同的入口点来区分它们的行为。
    this->_fragShaderStageInfo.pName = "main";
    // 将顶点着色器和片段着色器添加到着色器阶段列表中
    this->_shaderStages.push_back(this->_vertShaderStageInfo);
    this->_shaderStages.push_back(this->_fragShaderStageInfo);
}
/**
 * @brief 初始化顶点输入状态
 * 基础的顶点输入状态，只包含位置和纹理坐标
 */
void GfxPipeline::_initVertexInputState()
{
}
void GfxPipeline::_initInputAssemblyState()
{
    this->_inputAssemblyInfo = {};
    this->_inputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    this->_inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    this->_inputAssemblyInfo.primitiveRestartEnable = VK_FALSE;
}
void GfxPipeline::_initDynamicState()
{
    this->_dynamicStateInfo = {};
    this->_dynamicStates = {
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_SCISSOR,
    };
    this->_dynamicStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    this->_dynamicStateInfo.dynamicStateCount = static_cast<uint32_t>(this->_dynamicStates.size());
    this->_dynamicStateInfo.pDynamicStates = this->_dynamicStates.data();
}
void GfxPipeline::_initViewportState()
{
}
void GfxPipeline::_initRasterizationState()
{
    this->_rasterizationInfo = {};
    this->_rasterizationInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    this->_rasterizationInfo.depthClampEnable = VK_FALSE;
    this->_rasterizationInfo.rasterizerDiscardEnable = VK_FALSE;
    this->_rasterizationInfo.polygonMode = this->_getPolygonMode(this->_pipelineStruct.polygonMode);
    // this->_rasterizationInfo.polygonMode = VK_POLYGON_MODE_FILL;
    this->_rasterizationInfo.lineWidth = 1.0f;
    this->_rasterizationInfo.cullMode = this->_getCullMode(this->_pipelineStruct.cullMode); // 背面剔除
    // this->_rasterizationInfo.cullMode = VK_CULL_MODE_BACK_BIT; // 背面剔除
    this->_rasterizationInfo.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
    this->_rasterizationInfo.depthBiasEnable = VK_FALSE; // 禁用深度偏移
}
VkPolygonMode GfxPipeline::_getPolygonMode(GfxPipelinePolygonMode polygonMode)
{
    if (polygonMode == GfxPipelinePolygonMode::Fill)
    {
        return VK_POLYGON_MODE_FILL;
    }
    else if (polygonMode == GfxPipelinePolygonMode::Line)
    {
        return VK_POLYGON_MODE_LINE;
    }
    else
    {
        return VK_POLYGON_MODE_POINT;
    }
}
VkCullModeFlags GfxPipeline::_getCullMode(GfxPipelineCullMode cullMode)
{
    if (cullMode == GfxPipelineCullMode::Back)
    {
        return VK_CULL_MODE_BACK_BIT;
    }
    else if (cullMode == GfxPipelineCullMode::Front)
    {
        return VK_CULL_MODE_FRONT_BIT;
    }
    else
    {
        return VK_CULL_MODE_NONE;
    }
}

void GfxPipeline::_initMultisampleState()
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
void GfxPipeline::_initDepthStencilState()
{
    /* // 第六步：深度测试---本程序不涉及
    // 禁用深度测试（2D UI 不需要）
    // 2d 暂时这么处理，后边需要动态的设置 */
    this->_depthStencilInfo = {};
    this->_depthStencilInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    this->_depthStencilInfo.depthTestEnable = VK_FALSE;   // 禁用深度测试
    this->_depthStencilInfo.depthWriteEnable = VK_FALSE;  // 禁止写入深度
    this->_depthStencilInfo.stencilTestEnable = VK_FALSE; // 禁止写入深度
}
VkCompareOp GfxPipeline::_getCompareOp(GfxPipelineCompareOp compareOp)
{
    if (compareOp == GfxPipelineCompareOp::Never)
    {
        return VK_COMPARE_OP_NEVER;
    }
    else if (compareOp == GfxPipelineCompareOp::Less)
    {
        return VK_COMPARE_OP_LESS;
    }
    else if (compareOp == GfxPipelineCompareOp::LessOrEqual)
    {
        return VK_COMPARE_OP_LESS_OR_EQUAL;
    }
    else if (compareOp == GfxPipelineCompareOp::Greater)
    {
        return VK_COMPARE_OP_GREATER;
    }
    else if (compareOp == GfxPipelineCompareOp::GreaterOrEqual)
    {
        return VK_COMPARE_OP_GREATER_OR_EQUAL;
    }
    else if (compareOp == GfxPipelineCompareOp::Equal)
    {
        return VK_COMPARE_OP_EQUAL;
    }
    else if (compareOp == GfxPipelineCompareOp::NotEqual)
    {
        return VK_COMPARE_OP_NOT_EQUAL;
    }
    else
    {
        return VK_COMPARE_OP_ALWAYS;
    }
}
VkStencilOp GfxPipeline::_getStencilOp(GfxPipelineStencilOp stencilOp)
{
    if (stencilOp == GfxPipelineStencilOp::Keep)
    {
        return VK_STENCIL_OP_KEEP;
    }
    else if (stencilOp == GfxPipelineStencilOp::Increment_Add)
    {
        return VK_STENCIL_OP_INCREMENT_AND_WRAP;
    }
    else if (stencilOp == GfxPipelineStencilOp::Decrement_Subtract)
    {
        return VK_STENCIL_OP_DECREMENT_AND_WRAP;
    }
    else if (stencilOp == GfxPipelineStencilOp::Replace)
    {
        return VK_STENCIL_OP_REPLACE;
    }
    return VK_STENCIL_OP_INVERT;
}
void GfxPipeline::_initColorBlendState()
{
    this->_colorBlendAttachment = {};
    this->_colorBlendAttachment.blendEnable = VK_TRUE;
    this->_colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;

    this->_colorBlendInfo = {};
    this->_colorBlendInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    this->_colorBlendInfo.logicOpEnable = VK_FALSE;
    this->_colorBlendInfo.attachmentCount = 1;
    this->_colorBlendInfo.pAttachments = &this->_colorBlendAttachment;
}
VkBlendFactor GfxPipeline::_getBlendFactor(GfxPipelineColorBlendFactor blendFactor)
{
    if (blendFactor == GfxPipelineColorBlendFactor::Zero)
    {
        return VK_BLEND_FACTOR_ZERO;
    }
    else if (blendFactor == GfxPipelineColorBlendFactor::One)
    {
        return VK_BLEND_FACTOR_ONE;
    }
    else if (blendFactor == GfxPipelineColorBlendFactor::SrcColor)
    {
        return VK_BLEND_FACTOR_SRC_COLOR;
    }
    else if (blendFactor == GfxPipelineColorBlendFactor::DstColor)
    {
        return VK_BLEND_FACTOR_DST_COLOR;
    }
    else if (blendFactor == GfxPipelineColorBlendFactor::SrcAlpha)
    {
        return VK_BLEND_FACTOR_SRC_ALPHA;
    }
    else if (blendFactor == GfxPipelineColorBlendFactor::DstAlpha)
    {
        return VK_BLEND_FACTOR_DST_ALPHA;
    }
    else if (blendFactor == GfxPipelineColorBlendFactor::OneMinusSrcAlpha)
    {
        return VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
    }
    else if (blendFactor == GfxPipelineColorBlendFactor::OneMinusDstAlpha)
    {
        return VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA;
    }
    return VK_BLEND_FACTOR_ONE_MINUS_SRC_COLOR;
}
VkBlendOp GfxPipeline::_getBlendOp(GfxPipelineColorBlendOp blendOp)
{
    if (blendOp == GfxPipelineColorBlendOp::Add)
    {
        return VK_BLEND_OP_ADD;
    }
    else if (blendOp == GfxPipelineColorBlendOp::Subtract)
    {
        return VK_BLEND_OP_SUBTRACT;
    }
    else
    {
        return VK_BLEND_OP_MIN;
    }
}
void GfxPipeline::_initPipelineLayout()
{
}
VkPipelineLayout GfxPipeline::getVKPipelineLayout()
{
    return this->_vkPipelineLayout;
}

void GfxPipeline::_initPipeline()
{
}

VkPipeline GfxPipeline::getVKPipeline()
{
    return this->_vkPipeline;
}

void GfxPipeline::_clear()
{
    /* // 销毁图形管线（Pipeline） */
    if (this->_vkPipeline != VK_NULL_HANDLE)
    {
        vkDestroyPipeline(Gfx::context->getVkDevice(), this->_vkPipeline, nullptr);
        this->_vkPipeline = VK_NULL_HANDLE;
    }
    /* // 销毁管线布局（Pipeline Layout） */
    if (this->_vkPipelineLayout != VK_NULL_HANDLE)
    {
        vkDestroyPipelineLayout(Gfx::context->getVkDevice(), this->_vkPipelineLayout, nullptr);
        this->_vkPipelineLayout = VK_NULL_HANDLE;
    }
}
void GfxPipeline::_reset()
{
    this->_initPipelineLayout();
    this->_initPipeline();
}

GfxPipeline::~GfxPipeline()
{
}

/**
 * 要实际使用Shader，我们需要通过 VkPipelineShaderStageCreateInfo 结构将它们分配给特定的管道阶段，作为实际管道创建过程的一部分。
 *  我们将首先在 createGraphicsPipeline 函数中填充顶点着色器的结构。
 */
// VkPipelineShaderStageCreateInfo vertShaderStageInfo = {};
// vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
// // VK_SHADER_STAGE_VERTEX_BIT代表是顶点着色器
// vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
// vertShaderStageInfo.module = this->_vertexShader->getShaderModule();
// // 指定想调用什么函数。这意味着可以将多个片段着色器组合到一个着色器模块中，并使用不同的入口点来区分它们的行为。
// vertShaderStageInfo.pName = "main";

// VkPipelineShaderStageCreateInfo fragShaderStageInfo = {};
// fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
// // VK_SHADER_STAGE_FRAGMENT_BIT代表是像素着色器
// fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
// fragShaderStageInfo.module = this->_fragmentShader->getShaderModule();
// fragShaderStageInfo.pName = "main";

// // VkPipelineShaderStageCreateInfo geomShaderStageInfo{};
// // geomShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
// // geomShaderStageInfo.stage = VK_SHADER_STAGE_GEOMETRY_BIT;
// // geomShaderStageInfo.module = shader->getGeomShaderModule();
// // geomShaderStageInfo.pName = "main";
// std::cout << "create graphics pipeline : shader ok..." << std::endl;
// VkPipelineShaderStageCreateInfo shaderStages[] = {vertShaderStageInfo, fragShaderStageInfo}; // getGeomShaderModule

// // 第一步：顶点输入环节
// // Vertex bindings an attributes
// // Binding description
// VkVertexInputBindingDescription vInputBindDescription{};
// vInputBindDescription.binding = 0;

// vInputBindDescription.stride = (4 + 4 + 3 + 2) * sizeof(float);
// vInputBindDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
// std::vector<VkVertexInputBindingDescription> vertexInputBindings = {vInputBindDescription};
// // 顶点坐标属性 4个float
// VkVertexInputAttributeDescription vInputAttribDescriptionPos{};
// vInputAttribDescriptionPos.location = 0;
// vInputAttribDescriptionPos.binding = 0;
// vInputAttribDescriptionPos.format = VK_FORMAT_R32G32B32_SFLOAT;
// vInputAttribDescriptionPos.offset = 0;
// // 顶点颜色属性 4个float
// VkVertexInputAttributeDescription vInputAttribDescriptionColor{};
// vInputAttribDescriptionColor.location = 1;
// vInputAttribDescriptionColor.binding = 0;
// vInputAttribDescriptionColor.format = VK_FORMAT_R32G32B32_SFLOAT;
// vInputAttribDescriptionColor.offset = 4 * sizeof(float);
// // 顶点法线属性 3个float
// VkVertexInputAttributeDescription vInputAttribDescriptionNormal{};
// vInputAttribDescriptionNormal.location = 2;
// vInputAttribDescriptionNormal.binding = 0;
// vInputAttribDescriptionNormal.format = VK_FORMAT_R32G32B32_SFLOAT;
// vInputAttribDescriptionNormal.offset = (4 + 4) * sizeof(float);

// // 顶点纹理坐标属性 2个float
// VkVertexInputAttributeDescription vInputAttribDescriptionTexCoord{};
// vInputAttribDescriptionTexCoord.location = 3;
// vInputAttribDescriptionTexCoord.binding = 0;
// vInputAttribDescriptionTexCoord.format = VK_FORMAT_R32G32_SFLOAT;
// vInputAttribDescriptionTexCoord.offset = sizeof(float) * (4 + 4 + 3);

// std::vector<VkVertexInputAttributeDescription> vertexInputAttributes = {
//     vInputAttribDescriptionPos,      // Position
//     vInputAttribDescriptionColor,    // Color
//     vInputAttribDescriptionNormal,   // Normal
//     vInputAttribDescriptionTexCoord, // TexCoord
// };

// VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
// vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
// vertexInputInfo.vertexBindingDescriptionCount = static_cast<uint32_t>(vertexInputBindings.size());
// vertexInputInfo.pVertexBindingDescriptions = vertexInputBindings.data();
// vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(.size());
// vertexInputInfo.pVertexAttributeDescriptions = vertexInputAttributes.data();

// // 第二步：顶点数据使用的几何图元
// VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
// inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
// inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
// inputAssembly.primitiveRestartEnable = VK_FALSE;

// // 第三步：视口和裁剪
// // 视口是输出渲染结果的帧缓冲区域，一般情况下是(0,0)到(width,height)
// // 视口将要显示的是交换链中的图像，所以尺寸应该与交换链中图像代销保持一致
// VkViewport viewport{};
// viewport.x = 0.0f;
// viewport.y = 0.0f;
// viewport.width = (float)Gfx::context->getSwapChainExtent().width;
// viewport.height = (float)Gfx::context->getSwapChainExtent().height;
// viewport.minDepth = 0.0f;
// viewport.maxDepth = 1.0f;
// // 裁剪定义哪一块区域的像素实际被存储在帧缓存中。任何位于裁剪范围之外都会被光栅化丢弃
// VkRect2D scissor{};
// scissor.offset = {0, 0};
// scissor.extent = Gfx::context->getSwapChainExtent();

// VkPipelineViewportStateCreateInfo viewportState{};
// viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
// viewportState.viewportCount = 1;
// viewportState.pViewports = &viewport;
// viewportState.scissorCount = 1;
// viewportState.pScissors = &scissor;

// // 第四步：光栅化，将顶点构成的几何图元转换为片段，交由片段着色器着色
// VkPipelineRasterizationStateCreateInfo rasterizer{};
// rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
// rasterizer.depthClampEnable = VK_FALSE;
// rasterizer.rasterizerDiscardEnable = VK_FALSE;
// rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
// rasterizer.lineWidth = 1.0f;
// rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
// rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
// rasterizer.depthBiasEnable = VK_FALSE;

// // 第五步：多重采样，减少边缘锯齿
// VkPipelineMultisampleStateCreateInfo multisampling{};
// multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
// multisampling.sampleShadingEnable = VK_FALSE;
// multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

// // 第六步：深度测试---本程序不涉及
// // 禁用深度测试（2D UI 不需要）
// // 2d 暂时这么处理，后边需要动态的设置
// VkPipelineDepthStencilStateCreateInfo depthStencil{};
// depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
// depthStencil.depthTestEnable = VK_FALSE;  // 禁用深度测试
// depthStencil.depthWriteEnable = VK_FALSE; // 禁止写入深度

// // 第七步：颜色缓和，片段着色器返回的颜色需要和帧缓冲中对应像素的颜色进行混合
// VkPipelineColorBlendAttachmentState colorBlendAttachment{};
// colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
// colorBlendAttachment.blendEnable = VK_TRUE;
// colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
// colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
// colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
// colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
// colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
// colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

// VkPipelineColorBlendStateCreateInfo colorBlending{};
// colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
// colorBlending.logicOpEnable = VK_FALSE;
// colorBlending.attachmentCount = 1;
// colorBlending.pAttachments = &colorBlendAttachment;

// colorBlending.logicOp = VK_LOGIC_OP_COPY;
// colorBlending.blendConstants[0] = 0.0f;
// colorBlending.blendConstants[1] = 0.0f;
// colorBlending.blendConstants[2] = 0.0f;
// colorBlending.blendConstants[3] = 0.0f;

// // VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
// // pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
// // pipelineLayoutInfo.pNext = nullptr;  // ✅ 如果没有扩展结构，设为nullptr
// // pipelineLayoutInfo.flags = 0;  // ✅ 通常为0
// // pipelineLayoutInfo.setLayoutCount = 0;
// // pipelineLayoutInfo.pushConstantRangeCount = 0;
// VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
// pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
// pipelineLayoutInfo.setLayoutCount = 0;
// pipelineLayoutInfo.pushConstantRangeCount = 0;

// // // 第七步：推送常量，用于在着色器中传递数据
// // VkPushConstantRange pushConstantRange{};
// // pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
// // pushConstantRange.offset = 0;
// // pushConstantRange.size = sizeof(PushConstants);
// // pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;

// // 第八步：管线布局，
// if (vkCreatePipelineLayout(Gfx::context->getVkDevice(), &pipelineLayoutInfo, nullptr, &this->_vkPipelineLayout) != VK_SUCCESS)
// {
//     throw std::runtime_error("failed to create pipeline layout!");
// }
// std::cout << "create graphics pipeline : pipelineLayout ok..." << std::endl;

// VkGraphicsPipelineCreateInfo pipelineInfo{};
// pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
// // pipelineInfo.stageCount = sizeof(shaderStages) / sizeof(shaderStages[0]);
// // pipelineInfo.pStages = shaderStages;
// pipelineInfo.stageCount = this->_shaderStages.size();
// pipelineInfo.pStages = this->_shaderStages.data();

// pipelineInfo.pVertexInputState = &this->_vertexInputInfo;
// pipelineInfo.pInputAssemblyState = &this->_inputAssemblyInfo;
// pipelineInfo.pViewportState = &this->_viewportInfo;
// pipelineInfo.pRasterizationState = &this->_rasterizationInfo;
// pipelineInfo.pMultisampleState = &this->_multisampleInfo;
// pipelineInfo.pColorBlendState = &this->_colorBlendInfo;
// pipelineInfo.pDepthStencilState = &this->_depthStencilInfo;
// pipelineInfo.layout = this->_vkPipelineLayout;
// std::cout << "create graphics pipeline : start bind pass..." << std::endl;
// pipelineInfo.renderPass = this->_pass->vkRenderPass();
// std::cout << "create graphics pipeline : start bind 1111..." << std::endl;
// pipelineInfo.subpass = 0;
// pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

// if (vkCreateGraphicsPipelines(Gfx::context->getVkDevice(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &this->_vkPipeline) != VK_SUCCESS)
// {
//     throw std::runtime_error("Failed to create graphics pipeline!");
// }
// std::cout << "create graphics pipeline success..." << std::endl;

// std::cout << "create graphics pipeline : start 1..." << std::endl;

// /**
//  * @brief 初始化描述符集布局
//  * 绑定ubo和采样器
//  * 基础类不包含采样器
//  */
// void GfxPipeline::_initDescriptorSetLayout()
// {
//     // 动态 UBO 研究
//     std::vector<VkDescriptorSetLayoutBinding> bindings;
//     // ubo缓冲区绑定  -全局统一属性
//     VkDescriptorSetLayoutBinding uboLayoutBinding{};
//     uboLayoutBinding.binding = 0;
//     uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
//     uboLayoutBinding.descriptorCount = 1;
//     uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
//     bindings.push_back(uboLayoutBinding);
//     // 使用存储缓冲区绑定 - 物体单独属性
//     VkDescriptorSetLayoutBinding ssboBinding = {};
//     ssboBinding.binding = 1; // 对应shader中的 binding = 0
//     ssboBinding.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
//     ssboBinding.descriptorCount = 1;
//     ssboBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT |   // 顶点着色器可以访问
//                              VK_SHADER_STAGE_FRAGMENT_BIT | // 片段着色器可以访问
//                              VK_SHADER_STAGE_COMPUTE_BIT;   // 计算着色器可以访问
//     bindings.push_back(ssboBinding);
//     // 绑定纹理数组采样器
//     VkDescriptorSetLayoutBinding textureArrayBinding{};
//     textureArrayBinding.binding = 3;
//     textureArrayBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
//     textureArrayBinding.descriptorCount = 500;  // 你的池子大小
//     textureArrayBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
//     bindings.push_back(textureArrayBinding);

//     // ==================== 启用Bindless扩展 ====================
//     // 需要这些扩展标志
//     std::vector<VkDescriptorBindingFlags> bindingFlags(bindings.size(), 0);
//     // 为纹理数组启用部分绑定和更新后绑定
//     bindingFlags[2] = VK_DESCRIPTOR_BINDING_PARTIALLY_BOUND_BIT_EXT |
//                      VK_DESCRIPTOR_BINDING_UPDATE_AFTER_BIND_BIT_EXT;

//     VkDescriptorSetLayoutBindingFlagsCreateInfoEXT bindingFlagsInfo = {
//         .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_BINDING_FLAGS_CREATE_INFO_EXT,
//         .bindingCount = static_cast<uint32_t>(bindingFlags.size()),
//         .pBindingFlags = bindingFlags.data()
//     };

//     VkDescriptorSetLayoutCreateInfo layoutInfo{};
//     layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
//     layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
//     layoutInfo.pBindings = bindings.data();
//     layoutInfo.pNext = &bindingFlagsInfo;
//     layoutInfo.flags = VK_DESCRIPTOR_SET_LAYOUT_CREATE_UPDATE_AFTER_BIND_POOL_BIT_EXT;

//     if (vkCreateDescriptorSetLayout(this->_context->getVkDevice(),
//                                     &layoutInfo, nullptr, &this->_descriptorSetLayout) != VK_SUCCESS)
//     {
//         std::cout << "GfxPipeline :create descriptor set layout failed " << this->_name << std::endl;
//         return;
//     }
//     std::cout << "GfxPipeline :create descriptor set layout success " << this->_name << std::endl;
// }