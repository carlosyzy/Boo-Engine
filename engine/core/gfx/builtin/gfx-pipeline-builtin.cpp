#include "gfx-pipeline-builtin.h"
#include "../gfx.h"
#include "../gfx-context.h"
#include "../gfx-renderer.h"
#include "../base/gfx-pipeline.h"
#include "../base/gfx-pipeline-struct.h"
#include "../base/gfx-shader.h"
#include "../base/gfx-render-pass.h"

GfxPipelineBuiltin::GfxPipelineBuiltin(const std::string &name) : GfxPipeline(name)
{
}

void GfxPipelineBuiltin::_createPipeline()
{
    std::cout << "Gfx : Pipeline :: create pipeline " << this->_name << " start..." << std::endl;
    GfxPipeline::_createPipeline();
}

void GfxPipelineBuiltin::_initShaderState()
{
    GfxPipeline::_initShaderState();
}
/**
 * @brief 初始化顶点输入状态
 * 基础的顶点输入状态，只包含位置和纹理坐标
 */
void GfxPipelineBuiltin::_initVertexInputState()
{

    // 顶点 位置输入属性描述
    // this->_vInputAttribDescriptionPos = {};
    // this->_vInputAttribDescriptionPos.location = 0;
    // this->_vInputAttribDescriptionPos.binding = 0;
    // this->_vInputAttribDescriptionPos.format = VK_FORMAT_R32G32B32_SFLOAT;
    // this->_vInputAttribDescriptionPos.offset = 0;
    // // 顶点 纹理坐标输入属性描述
    // this->_vInputAttribDescriptionTexCoord = {};
    // this->_vInputAttribDescriptionTexCoord.location = 1;
    // this->_vInputAttribDescriptionTexCoord.binding = 0;
    // this->_vInputAttribDescriptionTexCoord.format = VK_FORMAT_R32G32_SFLOAT;
    // this->_vInputAttribDescriptionTexCoord.offset = sizeof(float) * 3;

    // this->_vertexInputAttributes.push_back(this->_vInputAttribDescriptionPos);
    // this->_vertexInputAttributes.push_back(this->_vInputAttribDescriptionTexCoord);
    // 顶点 输入绑定描述
    // this->_vertexInputBindings.push_back(this->_vInputBindDescription);

    // // 初始化实例数据-模型矩阵-输入属性描述
    // this->_vInputAttribDescriptionInstanceModel = {};
    // this->_vInputAttribDescriptionInstanceModel.location = 2;
    // this->_vInputAttribDescriptionInstanceModel.binding = 1;
    // this->_vInputAttribDescriptionInstanceModel.format = VK_FORMAT_R32G32B32A32_SFLOAT;
    // this->_vInputAttribDescriptionInstanceModel.offset = 0;
    // this->_vertexInputAttributesInstanceModel.push_back(this->_vInputAttribDescriptionInstanceModel);

    // // 初始化实例数据-模型矩阵-输入绑定描述
    // this->_vInputBindDescriptionInstanceModel = {};
    // this->_vInputBindDescriptionInstanceModel.binding = 1;
    // this->_vInputBindDescriptionInstanceModel.stride = sizeof(float) * 4 * 4;
    // this->_vInputBindDescriptionInstanceModel.inputRate = VK_VERTEX_INPUT_RATE_INSTANCE;
    // this->_vertexInputBindings.push_back(this->_vInputBindDescriptionInstanceModel);

    // this->_vertexInputInfo = {};
    // this->_vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    // this->_vertexInputInfo.vertexBindingDescriptionCount = (uint32_t)this->_vertexInputBindings.size();
    // this->_vertexInputInfo.pVertexBindingDescriptions = this->_vertexInputBindings.data();
    // this->_vertexInputInfo.vertexAttributeDescriptionCount = (uint32_t)this->_vertexInputAttributes.size();
    // this->_vertexInputInfo.pVertexAttributeDescriptions = this->_vertexInputAttributes.data();
    // 顶点输入（来自绑定0）
    // layout(location = 0) in vec3 inPosition;
    // layout(location = 1) in vec2 inTexCoord;

    // // 实例输入（来自绑定1）
    // layout(location = 2) in vec4 inModelMatrix0;
    // layout(location = 3) in vec4 inModelMatrix1;
    // layout(location = 4) in vec4 inModelMatrix2;
    // layout(location = 5) in vec4 inModelMatrix3;

    // // 重建矩阵
    // mat4 modelMatrix = mat4(
    //     inModelMatrix0,
    //     inModelMatrix1,
    //     inModelMatrix2,
    //     inModelMatrix3
    // );

    if (this->_pipelineStruct.render == uint32_t(GfxPipelineRender::_UI))
    {
        this->_initUIVertexInputState();
    }
    else if (this->_pipelineStruct.render == uint32_t(GfxPipelineRender::_3D))
    {
        this->_init3DVertexInputState();
    }
}
void GfxPipelineBuiltin::_initUIVertexInputState()
{
    // 初始化顶点输入属性描述
    this->_vInputBindDescription = {};
    this->_vInputBindDescription.binding = 0;
    this->_vInputBindDescription.stride = sizeof(float) * (3 + 2);
    this->_vInputBindDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
    // 初始化实例输入属性描述
    this->_vInputBindInstanceDescription = {};
    this->_vInputBindInstanceDescription.binding = 1;
    this->_vInputBindInstanceDescription.stride = sizeof(float) * (16 + 4); // 16个float(模型变换矩阵) + 4个float（颜色）
    this->_vInputBindInstanceDescription.inputRate = VK_VERTEX_INPUT_RATE_INSTANCE;

    this->_vertexInputAttributes = {
        {
            // 顶点坐标
            .location = 0,
            .binding = 0,
            .format = VK_FORMAT_R32G32B32_SFLOAT,
            .offset = 0,
        },
        {
            // 顶点纹理坐标
            .location = 1,
            .binding = 0,
            .format = VK_FORMAT_R32G32_SFLOAT,
            .offset = sizeof(float) * 3,
        },
        //-----实例
        // 矩阵第一行 (location 2)
        {
            .location = 2,
            .binding = 1, // 从绑定1读取！
            .format = VK_FORMAT_R32G32B32A32_SFLOAT,
            .offset = 0,
        },
        // 矩阵第二行 (location 3)
        {
            .location = 3,
            .binding = 1,
            .format = VK_FORMAT_R32G32B32A32_SFLOAT,
            .offset = sizeof(float) * 4,
        },
        // 矩阵第三行 (location 4)
        {
            .location = 4,
            .binding = 1,
            .format = VK_FORMAT_R32G32B32A32_SFLOAT,
            .offset = sizeof(float) * 8,
        },
        // 矩阵第四行 (location 5)
        {
            .location = 5,
            .binding = 1,
            .format = VK_FORMAT_R32G32B32A32_SFLOAT,
            .offset = sizeof(float) * 12,
        },
        // 颜色 (location 6)
        {
            .location = 6,
            .binding = 1,
            .format = VK_FORMAT_R32G32B32A32_SFLOAT,
            .offset = sizeof(float) * 16,
        },
    };

    this->_vertexInputBindings.push_back(this->_vInputBindDescription);
    this->_vertexInputBindings.push_back(this->_vInputBindInstanceDescription);

    this->_vertexInputInfo = {};
    this->_vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    this->_vertexInputInfo.vertexBindingDescriptionCount = (uint32_t)this->_vertexInputBindings.size();
    this->_vertexInputInfo.pVertexBindingDescriptions = this->_vertexInputBindings.data();
    this->_vertexInputInfo.vertexAttributeDescriptionCount = (uint32_t)this->_vertexInputAttributes.size();
    this->_vertexInputInfo.pVertexAttributeDescriptions = this->_vertexInputAttributes.data();
}
void GfxPipelineBuiltin::_init3DVertexInputState()
{
    
}

void GfxPipelineBuiltin::_initInputAssemblyState()
{
    GfxPipeline::_initInputAssemblyState();
}
void GfxPipelineBuiltin::_initDynamicState()
{
    this->_dynamicStates = {
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_SCISSOR,
    };
    this->_dynamicStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    this->_dynamicStateInfo.dynamicStateCount = static_cast<uint32_t>(this->_dynamicStates.size());
    this->_dynamicStateInfo.pDynamicStates = this->_dynamicStates.data();
}
void GfxPipelineBuiltin::_initViewportState()
{
    this->_viewport = {};
    this->_viewport.x = 0.0f;
    this->_viewport.y = 0.0f;
    this->_viewport.width = (float)Gfx::context->getSwapChainExtent().width;
    this->_viewport.height = (float)Gfx::context->getSwapChainExtent().height;
    this->_viewport.minDepth = 0.0f;
    this->_viewport.maxDepth = 1.0f;

    // 裁剪定义哪一块区域的像素实际被存储在帧缓存中。任何位于裁剪范围之外都会被光栅化丢弃
    this->_scissor = {};
    this->_scissor.offset = {0, 0};
    this->_scissor.extent = Gfx::context->getSwapChainExtent();

    this->_viewportInfo = {};
    this->_viewportInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    this->_viewportInfo.viewportCount = 1;
    // this->_viewportInfo.pViewports = &this->_viewport;
    this->_viewportInfo.scissorCount = 1;
    // this->_viewportInfo.pScissors = &this->_scissor;
}
void GfxPipelineBuiltin::_initRasterizationState()
{
    GfxPipeline::_initRasterizationState();
}

void GfxPipelineBuiltin::_initMultisampleState()
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
void GfxPipelineBuiltin::_initDepthStencilState()
{
    GfxPipeline::_initDepthStencilState();
}
void GfxPipelineBuiltin::_initColorBlendState()
{
    this->_colorBlendAttachment = {};
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
        this->_colorBlendAttachment.colorWriteMask = 0; // 不写入任何颜色通道
    }
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
void GfxPipelineBuiltin::_initPipelineLayout()
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
    if (vkCreatePipelineLayout(Gfx::context->getVkDevice(), &this->_pipelineLayoutInfo, nullptr, &this->_vkPipelineLayout) != VK_SUCCESS)
    {
        std::cout << "Gfx : Pipeline :: create pipeline layout failed " << std::endl;
        return;
    }
    std::cout << "Gfx : Pipeline :: create pipeline layout success " << std::endl;
}
void GfxPipelineBuiltin::_initPipeline()
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
    std::cout << "Gfx : Pipeline :: create pipeline info success " << this->_pass << "   " << this->_pass->getVKRenderPass() << std::endl;
    this->_pipelineInfo.renderPass = this->_pass->getVKRenderPass();
    this->_pipelineInfo.subpass = 0;
    this->_pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
    if (vkCreateGraphicsPipelines(Gfx::context->getVkDevice(), VK_NULL_HANDLE, 1, &this->_pipelineInfo, nullptr, &this->_vkPipeline) != VK_SUCCESS)
    {
        std::cout << "Gfx : Pipeline :: create pipeline failed " << std::endl;
        return;
    }
    std::cout << "Gfx : Pipeline :: create pipeline success " << std::endl;
}

GfxPipelineBuiltin::~GfxPipelineBuiltin()
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
// void GfxPipelineBuiltin::_initDescriptorSetLayout()
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
//         std::cout << "GfxPipelineBuiltin :create descriptor set layout failed " << this->_name << std::endl;
//         return;
//     }
//     std::cout << "GfxPipelineBuiltin :create descriptor set layout success " << this->_name << std::endl;
// }