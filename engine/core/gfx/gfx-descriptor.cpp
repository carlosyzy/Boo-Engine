#include "gfx-descriptor.h"
#include "gfx.h"
#include "gfx-context.h"
#include "gfx-struct.h"
GfxDescriptor::GfxDescriptor()
{
	this->_createDescriptorPool();
	this->_createDescriptorSetLayout();
	this->_createDescriptorSets();
}
void GfxDescriptor::_createDescriptorPool()
{
	std::vector<VkDescriptorPoolSize> poolSizes(3);
	// 1. UBO数量计算
	// 每个帧：2个UBO（3D UBO + UI UBO）
	// 可能需要额外：阴影UBO、后处理UBO
	poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	poolSizes[0].descriptorCount = 200; // 支持更多
	// 2. 存储缓冲区数量计算
	// 每个帧可能需要的存储缓冲区：
	// - 模型数据buffer（所有物体的变换）
	// - 材质数据buffer
	// - 灯光数据buffer
	// - 实例数据buffer
	poolSizes[1].type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
	poolSizes[1].descriptorCount = 200; // 支持更多
	// 3. Bindless纹理：关键是这个要足够大！
	poolSizes[2].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	// ⚠️ 临时修复：增加容量（但这不是最佳方案，见下方建议）
	// 如果每帧 500 个 × MAX_FRAMES_IN_FLIGHT 帧 = 需要更大容量
	poolSizes[2].descriptorCount = 500 * MAX_FRAMES_IN_FLIGHT;  // 1000 或更多

	// 4. maxSets计算：需要多少个描述符集？
	// 每个帧需要：
	// - 3D渲染描述符集
	// - UI渲染描述符集
	// - 可能还有：阴影Pass描述符集、后处理描述符集
	int descriptorSetsPerFrame = 400;          // 3D + UI（最基本）
	uint32_t maxSets = descriptorSetsPerFrame; // 6个描述符集

	VkDescriptorPoolCreateInfo poolInfo{};
	poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	// ✓ 关键：添加 UPDATE_AFTER_BIND 标志（Bindless 必需）
	poolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_UPDATE_AFTER_BIND_BIT;
	poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
	poolInfo.pPoolSizes = poolSizes.data();
	poolInfo.maxSets = maxSets; // 支持更多描述符集

	if (vkCreateDescriptorPool(Gfx::context->vkDevice(), &poolInfo, nullptr, &this->_descriptorPool) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create descriptor pool!");
	}

	std::cout << "✓ Created descriptor pool with UPDATE_AFTER_BIND support" << std::endl;
	std::cout << "  Max sets: " << maxSets << std::endl;
	std::cout << "  UBO descriptors: " << poolSizes[0].descriptorCount << std::endl;
	std::cout << "  Storage Buffer descriptors: " << poolSizes[1].descriptorCount << std::endl;
	std::cout << "  Texture descriptors: " << poolSizes[2].descriptorCount << std::endl;
}
void GfxDescriptor::_createDescriptorSetLayout()
{
	// 动态 UBO 研究
	std::vector<VkDescriptorSetLayoutBinding> bindings;
	// ubo缓冲区绑定  -全局统一属性
	VkDescriptorSetLayoutBinding uboLayoutBinding{};
	uboLayoutBinding.binding = 0;
	uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	uboLayoutBinding.descriptorCount = 1;
	uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
	bindings.push_back(uboLayoutBinding);
	// 使用存储缓冲区绑定 - 物体单独属性
	VkDescriptorSetLayoutBinding ssboBinding = {};
	ssboBinding.binding = 1; // 对应shader中的 binding = 0
	ssboBinding.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
	ssboBinding.descriptorCount = 1;
	ssboBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT |   // 顶点着色器可以访问
		VK_SHADER_STAGE_FRAGMENT_BIT | // 片段着色器可以访问
		VK_SHADER_STAGE_COMPUTE_BIT;   // 计算着色器可以访问
	bindings.push_back(ssboBinding);
	// 绑定纹理数组采样器
	VkDescriptorSetLayoutBinding textureArrayBinding{};
	textureArrayBinding.binding = 2;
	textureArrayBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	textureArrayBinding.descriptorCount = 500; // 你的池子大小
	textureArrayBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
	bindings.push_back(textureArrayBinding);

	// ==================== 启用Bindless扩展 ====================
	// 需要这些扩展标志
	std::vector<VkDescriptorBindingFlags> bindingFlags(bindings.size(), 0);
	// 为纹理数组启用部分绑定和更新后绑定
	bindingFlags[2] = VK_DESCRIPTOR_BINDING_PARTIALLY_BOUND_BIT_EXT |
		VK_DESCRIPTOR_BINDING_UPDATE_AFTER_BIND_BIT_EXT |
		VK_DESCRIPTOR_BINDING_VARIABLE_DESCRIPTOR_COUNT_BIT_EXT; // 新增

	VkDescriptorSetLayoutBindingFlagsCreateInfoEXT bindingFlagsInfo;
	bindingFlagsInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_BINDING_FLAGS_CREATE_INFO_EXT;
	bindingFlagsInfo.bindingCount = static_cast<uint32_t>(bindingFlags.size());
	bindingFlagsInfo.pBindingFlags = bindingFlags.data();

	VkDescriptorSetLayoutCreateInfo layoutInfo{};
	layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
	layoutInfo.pBindings = bindings.data();
	layoutInfo.pNext = &bindingFlagsInfo;
	// ✓ 修正：使用正确的标志（LAYOUT 的标志，不是 POOL 的标志）
	layoutInfo.flags = VK_DESCRIPTOR_SET_LAYOUT_CREATE_UPDATE_AFTER_BIND_POOL_BIT;

	if (vkCreateDescriptorSetLayout(Gfx::context->vkDevice(),
		&layoutInfo, nullptr, &this->_descriptorSetLayout) != VK_SUCCESS)
	{
		std::cout << "GfxPipeline :create descriptor set layout failed " << std::endl;
		return;
	}
	std::cout << "GfxPipeline :create descriptor set layout success " << std::endl;
}
void GfxDescriptor::_createDescriptorSets()
{
	// 每个帧需要一个描述符集
	std::vector<VkDescriptorSetLayout> layouts(MAX_FRAMES_IN_FLIGHT, this->_descriptorSetLayout);

	// 关键：设置可变描述符数量信息
	VkDescriptorSetVariableDescriptorCountAllocateInfoEXT countInfo = {};
	countInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_VARIABLE_DESCRIPTOR_COUNT_ALLOCATE_INFO_EXT;
	countInfo.descriptorSetCount = MAX_FRAMES_IN_FLIGHT;
	// 每个描述符集实际使用的纹理数量（可以动态设置）
	std::vector<uint32_t> counts(MAX_FRAMES_IN_FLIGHT, 500); // 每个集最多500个纹理
	countInfo.pDescriptorCounts = counts.data();


	VkDescriptorSetAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.pNext = &countInfo; // 链接可变数量信息
	allocInfo.descriptorPool = this->_descriptorPool;
	allocInfo.descriptorSetCount = MAX_FRAMES_IN_FLIGHT;
	allocInfo.pSetLayouts = layouts.data();
	this->_descriptorSets.resize(MAX_FRAMES_IN_FLIGHT);
	if (vkAllocateDescriptorSets(Gfx::context->vkDevice(), &allocInfo, this->_descriptorSets.data()) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to allocate descriptor sets!");
	}
	std::cout << "Allocated " << MAX_FRAMES_IN_FLIGHT << " descriptor sets" << std::endl;
}
GfxDescriptor::~GfxDescriptor()
{
}
