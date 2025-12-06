#include "gfx-pipeline.h"
#include "../gfx.h"
#include "../gfx-context.h"
#include "../gfx-renderer.h"
#include "../gfx-descriptor.h"
#include "../pass/gfx-pass.h"
#include "../gfx-shader.h"

GfxPipeline::GfxPipeline(const std::string &name)
{
    this->_name = name;
}

void GfxPipeline::create(GfxPass *pass, GfxShader *vertexShader, GfxShader *fragmentShader, GfxPipelineStruct pipelineStruct)
{
    this->_pass = pass;
    this->_vertexShader = vertexShader;
    this->_fragmentShader = fragmentShader;
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
    // 初始化顶点着色器状态
    this->_vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    // VK_SHADER_STAGE_VERTEX_BIT代表是顶点着色器
    this->_vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
    this->_vertShaderStageInfo.module = this->_vertexShader->getShaderModule();
    // 指定想调用什么函数。这意味着可以将多个片段着色器组合到一个着色器模块中，并使用不同的入口点来区分它们的行为。
    this->_vertShaderStageInfo.pName = "main";
    // 初始化片段着色器状态
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
    // 初始化顶点位置输入属性描述
    this->_vInputAttribDescriptionPos.location = 0;
    this->_vInputAttribDescriptionPos.binding = 0;
    this->_vInputAttribDescriptionPos.format = VK_FORMAT_R32G32B32_SFLOAT;
    this->_vInputAttribDescriptionPos.offset = 0;
    // // 初始化顶点纹理坐标输入属性描述
    // this->_vInputAttribDescriptionTexCoord.location = 1;
    // this->_vInputAttribDescriptionTexCoord.binding = 0;
    // this->_vInputAttribDescriptionTexCoord.format = VK_FORMAT_R32G32_SFLOAT;
    // this->_vInputAttribDescriptionTexCoord.offset = sizeof(float) * (3);

    this->_vertexInputAttributes.push_back(this->_vInputAttribDescriptionPos);
    // this->_vertexInputAttributes.push_back(this->_vInputAttribDescriptionTexCoord);

    this->_vInputBindDescription.binding = 0;
    this->_vInputBindDescription.stride = sizeof(float) * (3);
    this->_vInputBindDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
    this->_vertexInputBindings.push_back(this->_vInputBindDescription);

    this->_vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    this->_vertexInputInfo.vertexBindingDescriptionCount = (uint32_t)this->_vertexInputBindings.size();
    this->_vertexInputInfo.pVertexBindingDescriptions = this->_vertexInputBindings.data();
    this->_vertexInputInfo.vertexAttributeDescriptionCount = (uint32_t)this->_vertexInputAttributes.size();
    this->_vertexInputInfo.pVertexAttributeDescriptions = this->_vertexInputAttributes.data();
}
void GfxPipeline::_initInputAssemblyState()
{
    this->_inputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    this->_inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    this->_inputAssemblyInfo.primitiveRestartEnable = VK_FALSE;
}
void GfxPipeline::_initDynamicState()
{
    this->_dynamicStates = {
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_SCISSOR,
        VK_DYNAMIC_STATE_STENCIL_REFERENCE,
        VK_DYNAMIC_STATE_STENCIL_COMPARE_MASK,
        VK_DYNAMIC_STATE_STENCIL_WRITE_MASK};
    this->_dynamicStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    this->_dynamicStateInfo.dynamicStateCount = static_cast<uint32_t>(this->_dynamicStates.size());
    this->_dynamicStateInfo.pDynamicStates = this->_dynamicStates.data();
}
void GfxPipeline::_initViewportState()
{
    this->_viewportInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    this->_viewportInfo.viewportCount = 1;
    this->_viewportInfo.scissorCount = 1;
}
void GfxPipeline::_initRasterizationState()
{
    this->_rasterizationInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    this->_rasterizationInfo.depthClampEnable = VK_FALSE;
    this->_rasterizationInfo.rasterizerDiscardEnable = VK_FALSE;
    this->_rasterizationInfo.polygonMode = this->_getPolygonMode(this->_pipelineStruct.polygonMode);
    this->_rasterizationInfo.lineWidth = 1.0f;
    this->_rasterizationInfo.cullMode = this->_getCullMode(this->_pipelineStruct.cullMode); // 背面剔除
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
    this->_depthStencilInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    this->_depthStencilInfo.depthTestEnable = this->_pipelineStruct.depthTest != 0 ? VK_TRUE : VK_FALSE;   /* // 禁用深度测试 */
    this->_depthStencilInfo.depthWriteEnable = this->_pipelineStruct.depthWrite != 0 ? VK_TRUE : VK_FALSE; /* // 禁止写入深度 */
    this->_depthStencilInfo.depthCompareOp = this->_getCompareOp(this->_pipelineStruct.depthCompareOp);

    this->_depthStencilInfo.stencilTestEnable = this->_pipelineStruct.stencilTest != 0 ? VK_TRUE : VK_FALSE;
    if (this->_pipelineStruct.stencilTest != 0)
    {
        this->_depthStencilInfo.front.failOp = this->_getStencilOp(this->_pipelineStruct.stencilFrontFailOp);
        this->_depthStencilInfo.front.depthFailOp = this->_getStencilOp(this->_pipelineStruct.stencilFrontDepthFailOp);
        this->_depthStencilInfo.front.passOp = this->_getStencilOp(this->_pipelineStruct.stencilFrontPassOp);
        this->_depthStencilInfo.front.compareOp = this->_getCompareOp(this->_pipelineStruct.stencilFrontCompareOp);
        this->_depthStencilInfo.front.compareMask = 0xFF; // 比较所有位
        this->_depthStencilInfo.front.writeMask = 0xFF;   // 写入所有位
        this->_depthStencilInfo.front.reference = 1;      // 参考值（会被动态覆盖）
        this->_depthStencilInfo.back.failOp = this->_getStencilOp(this->_pipelineStruct.stencilBackFailOp);
        this->_depthStencilInfo.back.depthFailOp = this->_getStencilOp(this->_pipelineStruct.stencilBackDepthFailOp);
        this->_depthStencilInfo.back.passOp = this->_getStencilOp(this->_pipelineStruct.stencilBackPassOp);
        this->_depthStencilInfo.back.compareOp = this->_getCompareOp(this->_pipelineStruct.stencilBackCompareOp);
        this->_depthStencilInfo.back.compareMask = 0xFF;
        this->_depthStencilInfo.back.writeMask = 0xFF;
        this->_depthStencilInfo.back.reference = 1;
    }
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
    this->_colorBlendAttachment.blendEnable = this->_pipelineStruct.colorBlend != 0 ? VK_TRUE : VK_FALSE;
    if (this->_pipelineStruct.colorBlend != 0)
    {
        this->_colorBlendAttachment.srcColorBlendFactor = this->_getBlendFactor(this->_pipelineStruct.srcColorBlendFactor); // VK_BLEND_FACTOR_ONE;
        this->_colorBlendAttachment.dstColorBlendFactor = this->_getBlendFactor(this->_pipelineStruct.dstColorBlendFactor); // VK_BLEND_FACTOR_ZERO;
        this->_colorBlendAttachment.colorBlendOp = this->_getBlendOp(this->_pipelineStruct.colorBlendOp);                   // VK_BLEND_OP_ADD;
        this->_colorBlendAttachment.srcAlphaBlendFactor = this->_getBlendFactor(this->_pipelineStruct.srcAlphaBlendFactor); // VK_BLEND_FACTOR_ONE;
        this->_colorBlendAttachment.dstAlphaBlendFactor = this->_getBlendFactor(this->_pipelineStruct.dstAlphaBlendFactor); // VK_BLEND_FACTOR_ZERO;
        this->_colorBlendAttachment.alphaBlendOp = this->_getBlendOp(this->_pipelineStruct.alphaBlendOp);                   // VK_BLEND_OP_ADD;
        this->_colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    }
    else
    {
        // 禁用颜色写入（用于只写Stencil的遮罩管线）
        this->_colorBlendAttachment.colorWriteMask = 0; // 不写入任何颜色通道
    }
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
    this->_pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    this->_setLayouts.push_back(Gfx::renderer->descriptorSetLayout());
    this->_pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(this->_setLayouts.size());
    this->_pipelineLayoutInfo.pSetLayouts = this->_setLayouts.data();
    // 绑定推送常量 默认没有推送常量
    this->_pipelineLayoutInfo.pushConstantRangeCount = 0;
    // 第八步：管线布局
    if (vkCreatePipelineLayout(Gfx::context->vkDevice(), &this->_pipelineLayoutInfo, nullptr, &this->_vkPipelineLayout) != VK_SUCCESS)
    {
        std::cout << "GfxPipeline :create pipeline layout failed " << this->_name << std::endl;
        return;
    }
}
void GfxPipeline::_initPipeline()
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
    this->_pipelineInfo.renderPass = this->_pass->vkRenderPass();

    this->_pipelineInfo.subpass = 0;
    this->_pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
    if (vkCreateGraphicsPipelines(Gfx::context->vkDevice(), VK_NULL_HANDLE, 1, &this->_pipelineInfo, nullptr, &this->_vkPipeline) != VK_SUCCESS)
    {
        std::cout << "GfxPipeline :create pipeline failed " << this->_name << std::endl;
        return;
    }
    std::cout << "GfxPipeline :create pipeline success " << this->_name << std::endl;
}
void GfxPipeline::clear()
{
    /* // 销毁图形管线（Pipeline） */
    if (this->_vkPipeline != VK_NULL_HANDLE)
    {
        vkDestroyPipeline(Gfx::context->vkDevice(), this->_vkPipeline, nullptr);
        this->_vkPipeline = VK_NULL_HANDLE;
    }
    /* // 销毁管线布局（Pipeline Layout） */
    if (this->_vkPipelineLayout != VK_NULL_HANDLE)
    {
        vkDestroyPipelineLayout(Gfx::context->vkDevice(), this->_vkPipelineLayout, nullptr);
        this->_vkPipelineLayout = VK_NULL_HANDLE;
    }
}
void GfxPipeline::reset()
{
    this->_initPipelineLayout();
    this->_initPipeline();
}

GfxPipeline::~GfxPipeline()
{
}





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