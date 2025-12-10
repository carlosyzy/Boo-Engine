
// #include "gfx-descriptor.h"
// #include "../gfx.h"
// #include "../gfx-context.h"
// #include "../gfx-struct.h"
// GfxDescriptor::GfxDescriptor(uint32_t samplerCount, uint32_t maxObject)
// {
//     this->_samplerCount = samplerCount;
//     this->_currentObjectCount = 0;
//     this->_maxObjectCount = maxObject;

//     this->_createDescriptorPool();
//     this->_createDescriptorSetLayout();
// }
// void GfxDescriptor::_createDescriptorPool()
// {
//     std::vector<VkImageView> &swapChainImageViews = Gfx::context->getSwapChainImageViews();
//     uint32_t swapChainImageCount = swapChainImageViews.size();

//     std::vector<VkDescriptorPoolSize> poolSizes(1);
//     poolSizes[0].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
//     // 总采样器数量 = 每个集的采样器数 × 集的数量
//     poolSizes[0].descriptorCount = swapChainImageCount * (this->_maxObjectCount + 3) * this->_samplerCount; // 单个描述符集3

//     VkDescriptorPoolCreateInfo poolInfo{};
//     poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
//     poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
//     poolInfo.pPoolSizes = poolSizes.data();
//     poolInfo.maxSets = swapChainImageCount * (this->_maxObjectCount + 3); // 描述符集的最大数量
//     if (vkCreateDescriptorPool(Gfx::context->getVkDevice(), &poolInfo, nullptr, &this->_descriptorPool) != VK_SUCCESS)
//     {
//         std::cout << "Gfx : Descriptor ::create descriptor pool failed " << std::endl;
//         return;
//     }
//     std::cout << "Gfx : Descriptor ::create descriptor pool success " << std::endl;
// }
// void GfxDescriptor::_createDescriptorSetLayout()
// {
//     std::vector<VkDescriptorSetLayoutBinding> bindings;
//     // 采样器绑定
//     VkDescriptorSetLayoutBinding samplerLayoutBinding{};
//     samplerLayoutBinding.binding = 0;
//     samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
//     samplerLayoutBinding.descriptorCount = this->_samplerCount;
//     samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
//     bindings.push_back(samplerLayoutBinding);

//     VkDescriptorSetLayoutCreateInfo layoutInfo{};
//     layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
//     layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
//     layoutInfo.pBindings = bindings.data();

//     if (vkCreateDescriptorSetLayout(Gfx::context->getVkDevice(),
//                                     &layoutInfo, nullptr, &this->_descriptorSetLayout) != VK_SUCCESS)
//     {
//         std::cout << "Gfx : Descriptor ::create descriptor set layout failed " << std::endl;
//         return;
//     }
//     std::cout << "Gfx : Descriptor ::create descriptor set layout success " << std::endl;
// }
// VkDescriptorSetLayout GfxDescriptor::getDescriptorSetLayout()
// {
//     return this->_descriptorSetLayout;
// }
// std::vector<VkDescriptorSet> GfxDescriptor::getDescriptorSets()
// {
//     if (this->_currentObjectCount >= this->_maxObjectCount)
//     {
//         std::cout << "Gfx : Descriptor ::get descriptor sets failed, max object count reached " << std::endl;
//         return {VK_NULL_HANDLE, VK_NULL_HANDLE};
//     }
//     this->_currentObjectCount++;
//     std::vector<VkDescriptorSet> descriptorSets;
//     std::vector<VkImageView> &swapChainImageViews = Gfx::context->getSwapChainImageViews();
//     uint32_t swapChainImageCount = swapChainImageViews.size();
//     std::vector<VkDescriptorSetLayout> layouts(swapChainImageCount, this->_descriptorSetLayout);
//     VkDescriptorSetAllocateInfo allocInfo{};
//     allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
//     allocInfo.descriptorPool = this->_descriptorPool;
//     allocInfo.descriptorSetCount = swapChainImageCount;
//     allocInfo.pSetLayouts = layouts.data();
//     descriptorSets.resize(swapChainImageCount);
//     if (vkAllocateDescriptorSets(Gfx::context->getVkDevice(), &allocInfo, descriptorSets.data()) != VK_SUCCESS)
//     {
//         std::cout << "Gfx : Descriptor ::create descriptor sets failed " << std::endl;
//         return {VK_NULL_HANDLE, VK_NULL_HANDLE};
//     }
//     std::cout << "Gfx : Descriptor ::create descriptor sets success " << std::endl;
//     return descriptorSets;
// }

// void GfxDescriptor::reset()
// {
//     this->_currentObjectCount = 0;
//     vkDestroyDescriptorSetLayout(Gfx::context->getVkDevice(), this->_descriptorSetLayout, nullptr);
//     vkDestroyDescriptorPool(Gfx::context->getVkDevice(), this->_descriptorPool, nullptr);
//     this->_createDescriptorPool();
//     this->_createDescriptorSetLayout();
// }
// GfxDescriptor::~GfxDescriptor()
// {
// }

// // void GfxDescriptor::_createDescriptorSets()
// // {
// //     // std::vector<VkImageView> &swapChainImageViews = Gfx::context->getSwapChainImageViews();
// //     // uint32_t swapChainImageCount = swapChainImageViews.size();

// //     // std::vector<VkDescriptorSetLayout> layouts(swapChainImageCount, this->_descriptorSetLayout);
// //     // VkDescriptorSetAllocateInfo allocInfo{};
// //     // allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
// //     // allocInfo.descriptorPool = this->_descriptorPool;
// //     // allocInfo.descriptorSetCount = swapChainImageCount;
// //     // allocInfo.pSetLayouts = layouts.data();
// //     // this->_descriptorSets.resize(swapChainImageCount);
// //     // if (vkAllocateDescriptorSets(Gfx::context->getVkDevice(), &allocInfo, this->_descriptorSets.data()) != VK_SUCCESS)
// //     // {
// //     //     std::cout << "Gfx : Descriptor ::create descriptor sets failed " << std::endl;
// //     //     return;
// //     // }
// //     // std::cout << "Gfx : Descriptor ::create descriptor sets success " << std::endl;

// //     // for (uint32_t i = 0; i < this->_maxObjectCount - 1; i++)
// //     // {
// //     //     std::vector<VkDescriptorSet> descriptorSets;

// //     //     std::vector<VkImageView> &swapChainImageViews = Gfx::context->getSwapChainImageViews();
// //     //     uint32_t swapChainImageCount = swapChainImageViews.size();

// //     //     std::vector<VkDescriptorSetLayout> layouts(swapChainImageCount, this->_descriptorSetLayout);
// //     //     VkDescriptorSetAllocateInfo allocInfo{};
// //     //     allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
// //     //     allocInfo.descriptorPool = this->_descriptorPool;
// //     //     allocInfo.descriptorSetCount = swapChainImageCount;
// //     //     allocInfo.pSetLayouts = layouts.data();
// //     //     descriptorSets.resize(swapChainImageCount);
// //     //     if (vkAllocateDescriptorSets(Gfx::context->getVkDevice(), &allocInfo, descriptorSets.data()) != VK_SUCCESS)
// //     //     {
// //     //         std::cout << "Gfx : Descriptor ::create descriptor sets failed " << std::endl;
// //     //         return;
// //     //     }
// //     //     std::cout << "Gfx : Descriptor ::create descriptor sets success " << std::endl;
// //     // }
// // }