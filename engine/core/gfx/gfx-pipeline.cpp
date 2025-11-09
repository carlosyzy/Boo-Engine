#include "gfx-pipeline.h"
#include "gfx-pass.h"
#include "gfx-shader.h"
#include "gfx-mgr.h"
#include "gfx-context.h"

GfxPipeline::GfxPipeline(std::string name, GfxContext* context)
{
	this->_name = name;
	this->_context = context;
}
void GfxPipeline::create(GfxPass* pass, GfxShader* vertexShader, GfxShader* fragmentShader, std::unordered_map<std::string, std::string> pipelineState)
{

	this->_pass = pass;
	this->_vertexShader = vertexShader;
	this->_fragmentShader = fragmentShader;
	this->_pipelineState = pipelineState;
	this->_createDescriptorSetLayout();
	this->_createPipeline();
}
bool GfxPipeline::isTransparent()
{
	return this->_pipelineState["Blend"] == "1";
}

void GfxPipeline::_createPipeline()
{
	this->_Log("create graphics pipeline start...");
	// 第一步：初始化着色器模块 */
	std::vector<VkPipelineShaderStageCreateInfo> shaderStages;

	this->_initShaderStage(shaderStages);

	/*  // 第二步：初始化顶点输入 */
	VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
	/*  // 第一步：顶点输入环节
	 // Vertex bindings an attributes
	 // Binding description */
	VkVertexInputBindingDescription vInputBindDescription{};
	vInputBindDescription.binding = 0;
	vInputBindDescription.stride = (3 + 4 + 3 + 2) * sizeof(float);
	vInputBindDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
	std::vector<VkVertexInputBindingDescription> vertexInputBindings = { vInputBindDescription };
	/*  // 顶点坐标属性 3个float */
	VkVertexInputAttributeDescription vInputAttribDescriptionPos{};
	vInputAttribDescriptionPos.location = 0;
	vInputAttribDescriptionPos.binding = 0;
	vInputAttribDescriptionPos.format = VK_FORMAT_R32G32B32_SFLOAT;
	vInputAttribDescriptionPos.offset = 0;
	/*  // 顶点颜色属性 4个float */
	VkVertexInputAttributeDescription vInputAttribDescriptionColor{};
	vInputAttribDescriptionColor.location = 1;
	vInputAttribDescriptionColor.binding = 0;
	vInputAttribDescriptionColor.format = VK_FORMAT_R32G32B32_SFLOAT;
	vInputAttribDescriptionColor.offset = 3 * sizeof(float);
	/*  // 顶点法线属性 3个float */
	VkVertexInputAttributeDescription vInputAttribDescriptionNormal{};
	vInputAttribDescriptionNormal.location = 2;
	vInputAttribDescriptionNormal.binding = 0;
	vInputAttribDescriptionNormal.format = VK_FORMAT_R32G32B32_SFLOAT;
	vInputAttribDescriptionNormal.offset = (3 + 4) * sizeof(float);

	/*  // 顶点纹理坐标属性 2个float */
	VkVertexInputAttributeDescription vInputAttribDescriptionTexCoord{};
	vInputAttribDescriptionTexCoord.location = 3;
	vInputAttribDescriptionTexCoord.binding = 0;
	vInputAttribDescriptionTexCoord.format = VK_FORMAT_R32G32_SFLOAT;
	vInputAttribDescriptionTexCoord.offset = sizeof(float) * (3 + 4 + 3);

	std::vector<VkVertexInputAttributeDescription> vertexInputAttributes = {
		vInputAttribDescriptionPos,      /* // Position */
		vInputAttribDescriptionColor,   /*  // Color */
		vInputAttribDescriptionNormal,   /* // Normal */
		vInputAttribDescriptionTexCoord, /* // TexCoord */
	};

	vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertexInputInfo.vertexBindingDescriptionCount = static_cast<uint32_t>(vertexInputBindings.size());
	vertexInputInfo.pVertexBindingDescriptions = vertexInputBindings.data();
	vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(vertexInputAttributes.size());
	vertexInputInfo.pVertexAttributeDescriptions = vertexInputAttributes.data();

	/* // 第二步：顶点数据使用的几何图元 */
	VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
	inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	inputAssembly.primitiveRestartEnable = VK_FALSE;

	/*  // 第三步：视口和裁剪
	 // 视口是输出渲染结果的帧缓冲区域，一般情况下是(0,0)到(width,height)
	 // 视口将要显示的是交换链中的图像，所以尺寸应该与交换链中图像代销保持一致 */

	VkViewport viewport{};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = (float)this->_context->getSwapChainExtent().width;
	viewport.height = (float)this->_context->getSwapChainExtent().height;
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;

	/* // 裁剪定义哪一块区域的像素实际被存储在帧缓存中。任何位于裁剪范围之外都会被光栅化丢弃 */
	VkRect2D scissor{};
	scissor.offset = { 0, 0 };
	scissor.extent = this->_context->getSwapChainExtent();

	VkPipelineViewportStateCreateInfo viewportState{};
	viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportState.viewportCount = 1;
	viewportState.pViewports = &viewport;
	viewportState.scissorCount = 1;
	viewportState.pScissors = &scissor;

	/* // // ==================== 关键修改：启用动态视口和裁剪 ====================
	// // 定义动态状态（视口和裁剪区域将在命令缓冲区中动态设置）
	// std::vector<VkDynamicState> dynamicStates = {
	//     VK_DYNAMIC_STATE_VIEWPORT,
	//     VK_DYNAMIC_STATE_SCISSOR // 启用动态裁剪
	// };
	// VkPipelineDynamicStateCreateInfo dynamicState{};
	// dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	// dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
	// dynamicState.pDynamicStates = dynamicStates.data();

	// // 视口和裁剪状态（现在只设置数量，具体值将在命令缓冲区中动态设置）
	// VkPipelineViewportStateCreateInfo viewportState{};
	// viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	// viewportState.viewportCount = 1; // 使用动态状态，这里只需要指定数量
	// viewportState.scissorCount = 1;  // 使用动态状态，这里只需要指定数量
	// // 注意：pViewports 和 pScissors 设置为 nullptr，因为使用动态状态
	// // ==================== 关键修改结束 ==================== */

	/*  // 第四步：光栅化，将顶点构成的几何图元转换为片段，交由片段着色器着色 */
	VkPipelineRasterizationStateCreateInfo rasterizer{};
	rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterizer.depthClampEnable = VK_FALSE;
	rasterizer.rasterizerDiscardEnable = VK_FALSE;
	rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
	rasterizer.lineWidth = 1.0f;
	rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
	rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
	rasterizer.depthBiasEnable = VK_FALSE;

	/* // 第五步：多重采样，减少边缘锯齿 */
	VkPipelineMultisampleStateCreateInfo multisampling{};
	multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisampling.sampleShadingEnable = VK_FALSE;    /*  // 可选：启用样本着色，用于更高质量的抗锯齿 */
	multisampling.rasterizationSamples = MsaaSamples;/*  // <-- 使用之前获取的样本数 */
	multisampling.minSampleShading = 1.0f;
	multisampling.pSampleMask = nullptr;
	multisampling.alphaToCoverageEnable = VK_FALSE;
	multisampling.alphaToOneEnable = VK_FALSE;

	/* // 第六步：深度测试---本程序不涉及
	// 禁用深度测试（2D UI 不需要）
	// 2d 暂时这么处理，后边需要动态的设置 */
	VkPipelineDepthStencilStateCreateInfo depthStencil{};
	this->_initPipelineDepth(depthStencil);

	/* // 第七步：颜色缓和，片段着色器返回的颜色需要和帧缓冲中对应像素的颜色进行混合 */
	VkPipelineColorBlendAttachmentState colorBlendAttachment{};
	this->_initPipelineColorBlend(colorBlendAttachment);

	VkPipelineColorBlendStateCreateInfo colorBlending{};
	colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	colorBlending.logicOpEnable = VK_FALSE;
	colorBlending.attachmentCount = 1;
	colorBlending.pAttachments = &colorBlendAttachment;
	/*   // colorBlending.logicOp = VK_LOGIC_OP_COPY;
	  // colorBlending.blendConstants[0] = 0.0f;
	  // colorBlending.blendConstants[1] = 0.0f;
	  // colorBlending.blendConstants[2] = 0.0f;
	  // colorBlending.blendConstants[3] = 0.0f; */

	VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;

	std::vector<VkDescriptorSetLayout> setLayouts = {
		this->_descriptorSetLayout
	};
	/* // 绑定描述符集
	// 先获取描述符集布局 */
	pipelineLayoutInfo.setLayoutCount = setLayouts.size();
	pipelineLayoutInfo.pSetLayouts = setLayouts.data();

	/* // 第七步：推送常量，用于在着色器中传递数据 */
	VkPushConstantRange pushConstantRange{};
	pushConstantRange.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
	pushConstantRange.offset = 0;
	pushConstantRange.size = sizeof(PushConstants);

	pipelineLayoutInfo.pushConstantRangeCount = 1;
	pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;

	this->_Log("create graphics pipeline : start bind 1111...");
	/*  // 第八步：管线布局， */
	if (vkCreatePipelineLayout(this->_context->getVkDevice(), &pipelineLayoutInfo, nullptr, &this->_vkPipelineLayout) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create pipeline layout!");
	}
	this->_Log("create graphics pipeline : pipelineLayout ok...");

	VkGraphicsPipelineCreateInfo pipelineInfo{};
	pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipelineInfo.stageCount = shaderStages.size();
	pipelineInfo.pStages = shaderStages.data();
	pipelineInfo.pVertexInputState = &vertexInputInfo;
	pipelineInfo.pInputAssemblyState = &inputAssembly;
	pipelineInfo.pViewportState = &viewportState;
	/* // pipelineInfo.pDynamicState = &dynamicState;  // 关键：设置动态状态 */
	pipelineInfo.pRasterizationState = &rasterizer;
	pipelineInfo.pMultisampleState = &multisampling;
	pipelineInfo.pColorBlendState = &colorBlending;
	pipelineInfo.pDepthStencilState = &depthStencil;
	pipelineInfo.layout = this->_vkPipelineLayout;
	/*  // this->_Log("create graphics pipeline : start bind pass..."+std::to_string(this->_pass->getVkRenderPass())); */
	pipelineInfo.renderPass = this->_pass->getVkRenderPass();

	/* this->_Log("create graphics pipeline : start bind pass1...");*/

	pipelineInfo.subpass = 0;
	pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

	this->_Log("create graphics pipeline : start bind pass2...");
	if (vkCreateGraphicsPipelines(this->_context->getVkDevice(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &this->_vkPipeline) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create graphics pipeline!");
	}
	this->_Log("create graphics pipeline success...");
}
/**
 * 初始化着色器阶段
 */
void GfxPipeline::_initShaderStage(std::vector<VkPipelineShaderStageCreateInfo>& shaderStages)
{
	/**
	 * 要实际使用Shader，我们需要通过 VkPipelineShaderStageCreateInfo 结构将它们分配给特定的管道阶段，作为实际管道创建过程的一部分。
	 *  我们将首先在 createGraphicsPipeline 函数中填充顶点着色器的结构。
	 */
	VkPipelineShaderStageCreateInfo vertShaderStageInfo = {};
	vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	/*  // VK_SHADER_STAGE_VERTEX_BIT代表是顶点着色器 */
	vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
	vertShaderStageInfo.module = this->_vertexShader->getShaderModule();
	/*   // 指定想调用什么函数。这意味着可以将多个片段着色器组合到一个着色器模块中，并使用不同的入口点来区分它们的行为。 */
	vertShaderStageInfo.pName = "main";
	shaderStages.push_back(vertShaderStageInfo);

	VkPipelineShaderStageCreateInfo fragShaderStageInfo = {};
	fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	/*    // VK_SHADER_STAGE_FRAGMENT_BIT代表是像素着色器 */
	fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	fragShaderStageInfo.module = this->_fragmentShader->getShaderModule();
	fragShaderStageInfo.pName = "main";
	shaderStages.push_back(fragShaderStageInfo);

	/*  // VkPipelineShaderStageCreateInfo geomShaderStageInfo{};
	 // geomShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	 // geomShaderStageInfo.stage = VK_SHADER_STAGE_GEOMETRY_BIT;
	 // geomShaderStageInfo.module = shader->getGeomShaderModule();
	 // geomShaderStageInfo.pName = "main";
	 // std::cout << "create graphics pipeline : shader ok..." << std::endl; */

	shaderStages = { vertShaderStageInfo, fragShaderStageInfo };
}

/**
 * 初始化深度测试
 */
void GfxPipeline::_initPipelineDepth(VkPipelineDepthStencilStateCreateInfo& depthStencil)
{
	/* // this->_Log("initPipelineDepth"); */

	depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	if (this->_pipelineState.find("DepthTest") != this->_pipelineState.end() && this->_pipelineState["DepthTest"] == "1")
	{
		depthStencil.depthTestEnable = VK_TRUE; /* // 禁用深度测试 */
	}
	else
	{
		depthStencil.depthTestEnable = VK_FALSE; /* // 禁止写入深度 */
	}
	if (this->_pipelineState.find("DepthWrite") != this->_pipelineState.end() && this->_pipelineState["DepthWrite"] == "1")
	{
		depthStencil.depthWriteEnable = VK_TRUE; /* // 禁用深度测试 */
	}
	else
	{
		depthStencil.depthWriteEnable = VK_FALSE; /* // 禁止写入深度 */
	}
}
/**
 * 初始化颜色混合
 */
void GfxPipeline::_initPipelineColorBlend(VkPipelineColorBlendAttachmentState& colorBlendAttachment)
{
	this->_Log("initPipelineColorBlend");

	if (this->_pipelineState.find("Blend") != this->_pipelineState.end())
	{
		if (this->_pipelineState["Blend"] == "0")
		{
			colorBlendAttachment.blendEnable = VK_FALSE;
			colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
			colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
			colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
			colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
			colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
			colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;
			colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
		}
		else if (this->_pipelineState["Blend"] == "1")
		{
			colorBlendAttachment.blendEnable = VK_TRUE;
			colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
			colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
			colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
			colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
			colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
			colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;
			colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
		}
	}
}

void GfxPipeline::_createDescriptorSetLayout()
{
	std::vector<VkDescriptorSetLayoutBinding> bindings;
	/* // ubo缓冲区绑定 */
	VkDescriptorSetLayoutBinding uboLayoutBinding{};
	uboLayoutBinding.binding = 0;
	uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	uboLayoutBinding.descriptorCount = 1;
	uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
	bindings.push_back(uboLayoutBinding);

	/*  // 纹理采样器绑定 */
	VkDescriptorSetLayoutBinding samplerLayoutBinding{};
	samplerLayoutBinding.binding = 1;
	samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	samplerLayoutBinding.descriptorCount = 1;
	samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
	bindings.push_back(samplerLayoutBinding);

	VkDescriptorSetLayoutCreateInfo layoutInfo{};
	layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
	layoutInfo.pBindings = bindings.data();

	if (vkCreateDescriptorSetLayout(this->_context->getVkDevice(),
		&layoutInfo, nullptr, &this->_descriptorSetLayout) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create object descriptor set layout!");
	}
	this->_Log("create graphics pipeline : descriptorSetLayout ok...");
}

void GfxPipeline::clear()
{
	/* // 销毁图形管线（Pipeline） */
	if (this->_vkPipeline != VK_NULL_HANDLE)
	{
		vkDestroyPipeline(this->_context->getVkDevice(), this->_vkPipeline, nullptr);
		this->_vkPipeline = VK_NULL_HANDLE;
	}
	/* // 销毁管线布局（Pipeline Layout） */
	if (this->_vkPipelineLayout != VK_NULL_HANDLE)
	{
		vkDestroyPipelineLayout(this->_context->getVkDevice(), this->_vkPipelineLayout, nullptr);
		this->_vkPipelineLayout = VK_NULL_HANDLE;
	}
	/*  // // 销毁描述符集布局（Descriptor Set Layout）
	 // if (this->_descriptorSetLayout != VK_NULL_HANDLE)
	 // {
	 //     vkDestroyDescriptorSetLayout(this->_context->getVkDevice(), this->_descriptorSetLayout, nullptr);
	 //     this->_descriptorSetLayout = VK_NULL_HANDLE;
	 // } */
}
void GfxPipeline::reset()
{
	this->_createPipeline();
}

void GfxPipeline::_Log(std::string msg)
{
	std::cout << "GfxPipeline: " << msg << std::endl;
}
GfxPipeline::~GfxPipeline()
{
}
