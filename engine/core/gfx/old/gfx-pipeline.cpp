// #include "gfx-pipeline.h"

// #include "gfx-pass.h"
// #include "gfx-shader.h"
// #include "gfx-mgr.h"
// #include "gfx-context.h"

// GfxPipeline::GfxPipeline(std::string name, GfxContext *context)
// {
// 	this->_name = name;
// 	this->_context = context;
// }
// void GfxPipeline::create(GfxRenderPass *pass, GfxShader *vertexShader, GfxShader *fragmentShader, std::unordered_map<std::string, std::string> pipelineState)
// {

// 	// this->_pass = pass;
// 	// this->_vertexShader = vertexShader;
// 	// this->_fragmentShader = fragmentShader;
// 	// this->_pipelineState = pipelineState;
// 	// this->_createDescriptorSetLayout();
// 	// this->_createPipeline();
// }
// void GfxPipeline::create(GfxRenderPass *pass, GfxShader *vertexShader, GfxShader *fragmentShader, GfxPipelineStruct pipelineStruct)
// {
// 	this->_pass = pass;
// 	this->_vertexShader = vertexShader;
// 	this->_fragmentShader = fragmentShader;
// 	this->_pipelineStruct = pipelineStruct;
// 	this->_createDescriptorSetLayout();
// 	this->_createPipeline();
// }

// // bool GfxPipeline::isTransparent()
// // {
// // 	return this->_pipelineState["Blend"] == "1";
// // }

// void GfxPipeline::_createPipeline()
// {
// 	// 第一步：初始化着色器模块 */
// 	std::vector<VkPipelineShaderStageCreateInfo> shaderStages;
// 	/**
// 	 * 要实际使用Shader，我们需要通过 VkPipelineShaderStageCreateInfo 结构将它们分配给特定的管道阶段，作为实际管道创建过程的一部分。
// 	 *  我们将首先在 createGraphicsPipeline 函数中填充顶点着色器的结构。
// 	 */
// 	VkPipelineShaderStageCreateInfo vertShaderStageInfo = {};
// 	vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
// 	// VK_SHADER_STAGE_VERTEX_BIT代表是顶点着色器
// 	vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
// 	vertShaderStageInfo.module = this->_vertexShader->getShaderModule();
// 	// 指定想调用什么函数。这意味着可以将多个片段着色器组合到一个着色器模块中，并使用不同的入口点来区分它们的行为。
// 	vertShaderStageInfo.pName = "main";

// 	VkPipelineShaderStageCreateInfo fragShaderStageInfo = {};
// 	fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
// 	// VK_SHADER_STAGE_FRAGMENT_BIT代表是像素着色器
// 	fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
// 	fragShaderStageInfo.module = this->_fragmentShader->getShaderModule();
// 	fragShaderStageInfo.pName = "main";

// 	// VkPipelineShaderStageCreateInfo geomShaderStageInfo{};
// 	// geomShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
// 	// geomShaderStageInfo.stage = VK_SHADER_STAGE_GEOMETRY_BIT;
// 	// geomShaderStageInfo.module = shader->getGeomShaderModule();
// 	// geomShaderStageInfo.pName = "main";
// 	shaderStages = {vertShaderStageInfo, fragShaderStageInfo};

// 	// 第二步：初始化顶点输入
// 	VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
// 	// 顶点输入描述 绑定描述
// 	VkVertexInputBindingDescription vInputBindDescription{};
// 	vInputBindDescription.binding = 0;
// 	vInputBindDescription.stride = (3 + 4 + 3 + 2) * sizeof(float);
// 	vInputBindDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
// 	std::vector<VkVertexInputBindingDescription> vertexInputBindings = {vInputBindDescription};
// 	// 顶点属性描述
// 	VkVertexInputAttributeDescription vInputAttribDescriptionPos{};
// 	vInputAttribDescriptionPos.location = 0;
// 	vInputAttribDescriptionPos.binding = 0;
// 	vInputAttribDescriptionPos.format = VK_FORMAT_R32G32B32_SFLOAT;
// 	vInputAttribDescriptionPos.offset = 0;
// 	// 顶点颜色属性 4个float
// 	VkVertexInputAttributeDescription vInputAttribDescriptionColor{};
// 	vInputAttribDescriptionColor.location = 1;
// 	vInputAttribDescriptionColor.binding = 0;
// 	vInputAttribDescriptionColor.format = VK_FORMAT_R32G32B32_SFLOAT;
// 	vInputAttribDescriptionColor.offset = 3 * sizeof(float);
// 	// 顶点法线属性 3个float
// 	VkVertexInputAttributeDescription vInputAttribDescriptionNormal{};
// 	vInputAttribDescriptionNormal.location = 2;
// 	vInputAttribDescriptionNormal.binding = 0;
// 	vInputAttribDescriptionNormal.format = VK_FORMAT_R32G32B32_SFLOAT;
// 	vInputAttribDescriptionNormal.offset = (3 + 4) * sizeof(float);
// 	// 顶点纹理坐标属性 2个float
// 	VkVertexInputAttributeDescription vInputAttribDescriptionTexCoord{};
// 	vInputAttribDescriptionTexCoord.location = 3;
// 	vInputAttribDescriptionTexCoord.binding = 0;
// 	vInputAttribDescriptionTexCoord.format = VK_FORMAT_R32G32_SFLOAT;
// 	vInputAttribDescriptionTexCoord.offset = sizeof(float) * (3 + 4 + 3);

// 	std::vector<VkVertexInputAttributeDescription> vertexInputAttributes = {
// 		vInputAttribDescriptionPos,		 /* // Position */
// 		vInputAttribDescriptionColor,	 /*  // Color */
// 		vInputAttribDescriptionNormal,	 /* // Normal */
// 		vInputAttribDescriptionTexCoord, /* // TexCoord */
// 	};

// 	vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
// 	vertexInputInfo.vertexBindingDescriptionCount = static_cast<uint32_t>(vertexInputBindings.size());
// 	vertexInputInfo.pVertexBindingDescriptions = vertexInputBindings.data();
// 	vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(vertexInputAttributes.size());
// 	vertexInputInfo.pVertexAttributeDescriptions = vertexInputAttributes.data();

// 	// 第二步：顶点数据使用的几何图元
// 	VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
// 	inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
// 	inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
// 	inputAssembly.primitiveRestartEnable = VK_FALSE;

// 	// 第三步：视口和裁剪
// 	// 视口是输出渲染结果的帧缓冲区域，一般情况下是(0,0)到(width,height)
// 	// 视口将要显示的是交换链中的图像，所以尺寸应该与交换链中图像代销保持一致
// 	VkViewport viewport{};
// 	viewport.x = 0.0f;
// 	viewport.y = 0.0f;
// 	viewport.width = (float)this->_context->getSwapChainExtent().width;
// 	viewport.height = (float)this->_context->getSwapChainExtent().height;
// 	viewport.minDepth = 0.0f;
// 	viewport.maxDepth = 1.0f;

// 	// 裁剪定义哪一块区域的像素实际被存储在帧缓存中。任何位于裁剪范围之外都会被光栅化丢弃
// 	VkRect2D scissor{};
// 	scissor.offset = {0, 0};
// 	scissor.extent = this->_context->getSwapChainExtent();

// 	VkPipelineViewportStateCreateInfo viewportState{};
// 	viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
// 	viewportState.viewportCount = 1;
// 	viewportState.pViewports = &viewport;
// 	viewportState.scissorCount = 1;
// 	viewportState.pScissors = &scissor;

// 	// ==================== 关键修改：启用动态视口和裁剪 ====================
// 	// 定义动态状态（视口和裁剪区域将在命令缓冲区中动态设置）
// 	std::vector<VkDynamicState> dynamicStates = {
// 		// VK_DYNAMIC_STATE_VIEWPORT,
// 		// VK_DYNAMIC_STATE_SCISSOR // 启用动态裁剪
// 		VK_DYNAMIC_STATE_STENCIL_REFERENCE,
// 		VK_DYNAMIC_STATE_STENCIL_COMPARE_MASK,
// 		VK_DYNAMIC_STATE_STENCIL_WRITE_MASK};
// 	VkPipelineDynamicStateCreateInfo dynamicState{};
// 	dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
// 	dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
// 	dynamicState.pDynamicStates = dynamicStates.data();

// 	// 视口和裁剪状态（现在只设置数量，具体值将在命令缓冲区中动态设置）
// 	// VkPipelineViewportStateCreateInfo viewportState{};
// 	// viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
// 	// viewportState.viewportCount = 1; // 使用动态状态，这里只需要指定数量
// 	// viewportState.scissorCount = 1;  // 使用动态状态，这里只需要指定数量
// 	// 注意：pViewports 和 pScissors 设置为 nullptr，因为使用动态状态
// 	// // ==================== 关键修改结束 ====================

// 	/*  // 第四步：光栅化，将顶点构成的几何图元转换为片段，交由片段着色器着色 */
// 	VkPipelineRasterizationStateCreateInfo rasterizer{};
// 	rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
// 	rasterizer.depthClampEnable = VK_FALSE;
// 	rasterizer.rasterizerDiscardEnable = VK_FALSE;
// 	rasterizer.polygonMode = this->getPolygonMode(this->_pipelineStruct.polygonMode);
// 	rasterizer.lineWidth = 1.0f;
// 	rasterizer.cullMode = this->getCullMode(this->_pipelineStruct.cullMode); // 背面剔除
// 	rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
// 	rasterizer.depthBiasEnable = VK_FALSE; // 禁用深度偏移

// 	/* // 第五步：多重采样，减少边缘锯齿 */
// 	VkPipelineMultisampleStateCreateInfo multisampling{};
// 	multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
// 	multisampling.sampleShadingEnable = VK_FALSE;				// MsaaSamples == VK_SAMPLE_COUNT_1_BIT ? VK_FALSE : VK_TRUE; /*  // 可选：启用样本着色，用于更高质量的抗锯齿 */
// 	multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT; // MsaaSamples;											   /*  // <-- 使用之前获取的样本数 */
// 	multisampling.minSampleShading = 1.0f;
// 	multisampling.pSampleMask = nullptr;
// 	multisampling.alphaToCoverageEnable = VK_FALSE;
// 	multisampling.alphaToOneEnable = VK_FALSE;

// 	/* // 第六步：深度测试---本程序不涉及
// 	// 禁用深度测试（2D UI 不需要）
// 	// 2d 暂时这么处理，后边需要动态的设置 */
// 	VkPipelineDepthStencilStateCreateInfo depthStencil{};
// 	depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
// 	depthStencil.depthTestEnable = this->_pipelineStruct.depthTest != 0 ? VK_TRUE : VK_FALSE;	/* // 禁用深度测试 */
// 	depthStencil.depthWriteEnable = this->_pipelineStruct.depthWrite != 0 ? VK_TRUE : VK_FALSE; /* // 禁止写入深度 */
// 	depthStencil.depthCompareOp = this->getCompareOp(this->_pipelineStruct.depthCompareOp);

// 	depthStencil.stencilTestEnable = this->_pipelineStruct.stencilTest != 0 ? VK_TRUE : VK_FALSE;
// 	if (this->_pipelineStruct.stencilTest != 0)
// 	{
// 		depthStencil.front.failOp = this->getStencilOp(this->_pipelineStruct.stencilFrontFailOp);
// 		depthStencil.front.depthFailOp = this->getStencilOp(this->_pipelineStruct.stencilFrontDepthFailOp);
// 		depthStencil.front.passOp = this->getStencilOp(this->_pipelineStruct.stencilFrontPassOp);
// 		depthStencil.front.compareOp = this->getCompareOp(this->_pipelineStruct.stencilFrontCompareOp);
// 		depthStencil.front.compareMask = 0xFF; // 比较所有位
// 		depthStencil.front.writeMask = 0xFF;   // 写入所有位
// 		depthStencil.front.reference = 1;	   // 参考值（会被动态覆盖）
// 		depthStencil.back.failOp = this->getStencilOp(this->_pipelineStruct.stencilBackFailOp);
// 		depthStencil.back.depthFailOp = this->getStencilOp(this->_pipelineStruct.stencilBackDepthFailOp);
// 		depthStencil.back.passOp = this->getStencilOp(this->_pipelineStruct.stencilBackPassOp);
// 		depthStencil.back.compareOp = this->getCompareOp(this->_pipelineStruct.stencilBackCompareOp);
// 		depthStencil.back.compareMask = 0xFF;
// 		depthStencil.back.writeMask = 0xFF;
// 		depthStencil.back.reference = 1;
// 	}

// 	/* // 第七步：颜色缓和，片段着色器返回的颜色需要和帧缓冲中对应像素的颜色进行混合 */
// 	VkPipelineColorBlendAttachmentState colorBlendAttachment{};
// 	colorBlendAttachment.blendEnable = this->_pipelineStruct.colorBlend != 0 ? VK_TRUE : VK_FALSE;
// 	if (this->_pipelineStruct.colorBlend != 0)
// 	{
// 		colorBlendAttachment.srcColorBlendFactor = this->getBlendFactor(this->_pipelineStruct.srcColorBlendFactor); // VK_BLEND_FACTOR_ONE;
// 		colorBlendAttachment.dstColorBlendFactor = this->getBlendFactor(this->_pipelineStruct.dstColorBlendFactor); // VK_BLEND_FACTOR_ZERO;
// 		colorBlendAttachment.colorBlendOp = this->getBlendOp(this->_pipelineStruct.colorBlendOp);					// VK_BLEND_OP_ADD;
// 		colorBlendAttachment.srcAlphaBlendFactor = this->getBlendFactor(this->_pipelineStruct.srcAlphaBlendFactor); // VK_BLEND_FACTOR_ONE;
// 		colorBlendAttachment.dstAlphaBlendFactor = this->getBlendFactor(this->_pipelineStruct.dstAlphaBlendFactor); // VK_BLEND_FACTOR_ZERO;
// 		colorBlendAttachment.alphaBlendOp = this->getBlendOp(this->_pipelineStruct.alphaBlendOp);					// VK_BLEND_OP_ADD;
// 		colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
// 	}
// 	else
// 	{
// 		// 禁用颜色写入（用于只写Stencil的遮罩管线）
// 		colorBlendAttachment.colorWriteMask = 0;  // 不写入任何颜色通道
// 	}

// 	VkPipelineColorBlendStateCreateInfo colorBlending{};
// 	colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
// 	colorBlending.logicOpEnable = VK_FALSE;
// 	colorBlending.attachmentCount = 1;
// 	colorBlending.pAttachments = &colorBlendAttachment;
// 	colorBlending.logicOp = VK_LOGIC_OP_COPY;
// 	colorBlending.blendConstants[0] = 0.0f;
// 	colorBlending.blendConstants[1] = 0.0f;
// 	colorBlending.blendConstants[2] = 0.0f;
// 	colorBlending.blendConstants[3] = 0.0f;

// 	VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
// 	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;

// 	std::vector<VkDescriptorSetLayout> setLayouts = {
// 		this->_descriptorSetLayout};
// 	/* // 绑定描述符集
// 	// 先获取描述符集布局 */
// 	pipelineLayoutInfo.setLayoutCount = setLayouts.size();
// 	pipelineLayoutInfo.pSetLayouts = setLayouts.data();

// 	/* // 第七步：推送常量，用于在着色器中传递数据 */
// 	VkPushConstantRange pushConstantRange{};
// 	pushConstantRange.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
// 	pushConstantRange.offset = 0;
// 	pushConstantRange.size = sizeof(PushConstants);

// 	pipelineLayoutInfo.pushConstantRangeCount = 1;
// 	pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;

// 	/*  // 第八步：管线布局， */
// 	if (vkCreatePipelineLayout(this->_context->getVkDevice(), &pipelineLayoutInfo, nullptr, &this->_vkPipelineLayout) != VK_SUCCESS)
// 	{
// 		// throw std::runtime_error("failed to create pipeline layout!");
// 		std::cout << "GfxPipeline :create pipeline layout failed " << this->_name << std::endl;
		
// 		return;
// 	}

// 	VkGraphicsPipelineCreateInfo pipelineInfo{};
// 	pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
// 	pipelineInfo.stageCount = shaderStages.size();
// 	pipelineInfo.pStages = shaderStages.data();
// 	pipelineInfo.pVertexInputState = &vertexInputInfo;
// 	pipelineInfo.pInputAssemblyState = &inputAssembly;
// 	pipelineInfo.pViewportState = &viewportState;
// 	pipelineInfo.pDynamicState = &dynamicState; // 关键：设置动态状态
// 	pipelineInfo.pRasterizationState = &rasterizer;
// 	pipelineInfo.pMultisampleState = &multisampling;
// 	pipelineInfo.pColorBlendState = &colorBlending;
// 	pipelineInfo.pDepthStencilState = &depthStencil;
// 	pipelineInfo.layout = this->_vkPipelineLayout;
// 	pipelineInfo.renderPass = this->_pass->getVkRenderPass();

// 	pipelineInfo.subpass = 0;
// 	pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

// 	if (vkCreateGraphicsPipelines(this->_context->getVkDevice(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &this->_vkPipeline) != VK_SUCCESS)
// 	{
// 		std::cout << "GfxPipeline :create pipeline failed " << this->_name << std::endl;
// 		return;
// 	}
// 	std::cout << "GfxPipeline :create pipeline success " << this->_name << std::endl;
// }
// void GfxPipeline::_createDescriptorSetLayout()
// {
// 	// 动态 UBO 研究
// 	std::vector<VkDescriptorSetLayoutBinding> bindings;
// 	/* // ubo缓冲区绑定 */
// 	VkDescriptorSetLayoutBinding uboLayoutBinding{};
// 	uboLayoutBinding.binding = 0;
// 	uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
// 	uboLayoutBinding.descriptorCount = 1;
// 	uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
// 	bindings.push_back(uboLayoutBinding);

// 	/*  // 纹理采样器绑定 */
// 	VkDescriptorSetLayoutBinding samplerLayoutBinding{};
// 	samplerLayoutBinding.binding = 1;
// 	samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
// 	samplerLayoutBinding.descriptorCount = 1;
// 	samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
// 	bindings.push_back(samplerLayoutBinding);

// 	VkDescriptorSetLayoutCreateInfo layoutInfo{};
// 	layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
// 	layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
// 	layoutInfo.pBindings = bindings.data();

// 	if (vkCreateDescriptorSetLayout(this->_context->getVkDevice(),
// 									&layoutInfo, nullptr, &this->_descriptorSetLayout) != VK_SUCCESS)
// 	{
// 		std::cout << "GfxPipeline :create descriptor set layout failed " << this->_name << std::endl;
// 		return;
// 	}
// 	std::cout << "GfxPipeline :create descriptor set layout success " << this->_name << std::endl;
// }
// VkPolygonMode GfxPipeline::getPolygonMode(GfxPipelinePolygonMode polygonMode)
// {
// 	if (polygonMode == GfxPipelinePolygonMode::Fill)
// 	{
// 		return VK_POLYGON_MODE_FILL;
// 	}
// 	else if (polygonMode == GfxPipelinePolygonMode::Line)
// 	{
// 		return VK_POLYGON_MODE_LINE;
// 	}
// 	else
// 	{
// 		return VK_POLYGON_MODE_POINT;
// 	}
// }
// VkCullModeFlags GfxPipeline::getCullMode(GfxPipelineCullMode cullMode)
// {
// 	if (cullMode == GfxPipelineCullMode::Back)
// 	{
// 		return VK_CULL_MODE_BACK_BIT;
// 	}
// 	else if (cullMode == GfxPipelineCullMode::Front)
// 	{
// 		return VK_CULL_MODE_FRONT_BIT;
// 	}
// 	else
// 	{
// 		return VK_CULL_MODE_NONE;
// 	}
// }
// VkCompareOp GfxPipeline::getCompareOp(GfxPipelineCompareOp compareOp)
// {
// 	if (compareOp == GfxPipelineCompareOp::Never)
// 	{
// 		return VK_COMPARE_OP_NEVER;
// 	}
// 	else if (compareOp == GfxPipelineCompareOp::Less)
// 	{
// 		return VK_COMPARE_OP_LESS;
// 	}
// 	else if (compareOp == GfxPipelineCompareOp::LessOrEqual)
// 	{
// 		return VK_COMPARE_OP_LESS_OR_EQUAL;
// 	}
// 	else if (compareOp == GfxPipelineCompareOp::Greater)
// 	{
// 		return VK_COMPARE_OP_GREATER;
// 	}
// 	else if (compareOp == GfxPipelineCompareOp::GreaterOrEqual)
// 	{
// 		return VK_COMPARE_OP_GREATER_OR_EQUAL;
// 	}
// 	else if (compareOp == GfxPipelineCompareOp::Equal)
// 	{
// 		return VK_COMPARE_OP_EQUAL;
// 	}
// 	else if (compareOp == GfxPipelineCompareOp::NotEqual)
// 	{
// 		return VK_COMPARE_OP_NOT_EQUAL;
// 	}
// 	else
// 	{
// 		return VK_COMPARE_OP_ALWAYS;
// 	}
// }
// VkStencilOp GfxPipeline::getStencilOp(GfxPipelineStencilOp stencilOp)
// {
// 	if (stencilOp == GfxPipelineStencilOp::Keep)
// 	{
// 		return VK_STENCIL_OP_KEEP;
// 	}
// 	else if (stencilOp == GfxPipelineStencilOp::Increment_Add)
// 	{
// 		return VK_STENCIL_OP_INCREMENT_AND_WRAP;
// 	}
// 	else if (stencilOp == GfxPipelineStencilOp::Decrement_Subtract)
// 	{
// 		return VK_STENCIL_OP_DECREMENT_AND_WRAP;
// 	}
// 	else if (stencilOp == GfxPipelineStencilOp::Replace)
// 	{
// 		return VK_STENCIL_OP_REPLACE;
// 	}
// 	return VK_STENCIL_OP_INVERT;
// }

// VkBlendFactor GfxPipeline::getBlendFactor(GfxPipelineColorBlendFactor blendFactor)
// {
// 	if (blendFactor == GfxPipelineColorBlendFactor::Zero)
// 	{
// 		return VK_BLEND_FACTOR_ZERO;
// 	}
// 	else if (blendFactor == GfxPipelineColorBlendFactor::One)
// 	{
// 		return VK_BLEND_FACTOR_ONE;
// 	}
// 	else if (blendFactor == GfxPipelineColorBlendFactor::SrcColor)
// 	{
// 		return VK_BLEND_FACTOR_SRC_COLOR;
// 	}
// 	else if (blendFactor == GfxPipelineColorBlendFactor::DstColor)
// 	{
// 		return VK_BLEND_FACTOR_DST_COLOR;
// 	}
// 	else if (blendFactor == GfxPipelineColorBlendFactor::SrcAlpha)
// 	{
// 		return VK_BLEND_FACTOR_SRC_ALPHA;
// 	}
// 	else if (blendFactor == GfxPipelineColorBlendFactor::DstAlpha)
// 	{
// 		return VK_BLEND_FACTOR_DST_ALPHA;
// 	}
// 	else if (blendFactor == GfxPipelineColorBlendFactor::OneMinusSrcAlpha)
// 	{
// 		return VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
// 	}
// 	else if (blendFactor == GfxPipelineColorBlendFactor::OneMinusDstAlpha)
// 	{
// 		return VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA;
// 	}
// 	return VK_BLEND_FACTOR_ONE_MINUS_SRC_COLOR;
// }
// VkBlendOp GfxPipeline::getBlendOp(GfxPipelineColorBlendOp blendOp)
// {
// 	if (blendOp == GfxPipelineColorBlendOp::Add)
// 	{
// 		return VK_BLEND_OP_ADD;
// 	}
// 	else if (blendOp == GfxPipelineColorBlendOp::Subtract)
// 	{
// 		return VK_BLEND_OP_SUBTRACT;
// 	}
// 	else
// 	{
// 		return VK_BLEND_OP_MIN;
// 	}
// }

// void GfxPipeline::clear()
// {
// 	/* // 销毁图形管线（Pipeline） */
// 	if (this->_vkPipeline != VK_NULL_HANDLE)
// 	{
// 		vkDestroyPipeline(this->_context->getVkDevice(), this->_vkPipeline, nullptr);
// 		this->_vkPipeline = VK_NULL_HANDLE;
// 	}
// 	/* // 销毁管线布局（Pipeline Layout） */
// 	if (this->_vkPipelineLayout != VK_NULL_HANDLE)
// 	{
// 		vkDestroyPipelineLayout(this->_context->getVkDevice(), this->_vkPipelineLayout, nullptr);
// 		this->_vkPipelineLayout = VK_NULL_HANDLE;
// 	}
// }
// void GfxPipeline::reset()
// {
// 	this->_createPipeline();
// }

// GfxPipeline::~GfxPipeline()
// {
// }
