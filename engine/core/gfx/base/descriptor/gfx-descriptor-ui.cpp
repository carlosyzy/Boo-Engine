
#include "gfx-descriptor-ui.h"
#include "../gfx.h"
#include "../gfx-context.h"
#include "../gfx-struct.h"
GfxDescriptorUI::GfxDescriptorUI(uint32_t samplerCount, uint32_t maxObject)
    : GfxDescriptor(samplerCount, maxObject)
{
}
void GfxDescriptorUI::_createDescriptorPool()
{
    std::vector<VkDescriptorPoolSize> poolSizes(2);

    poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    poolSizes[0].descriptorCount = MAX_FRAMES_IN_FLIGHT * (this->_maxObjectCount + 3); // 支持更多

    poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    // 总采样器数量 = 每个集的采样器数 × 集的数量
    poolSizes[1].descriptorCount = (this->_maxObjectCount + 3) * this->_samplerCount; // 单个描述符集3

    VkDescriptorPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
    poolInfo.pPoolSizes = poolSizes.data();
    poolInfo.maxSets = (this->_maxObjectCount + 3); // 描述符集的最大数量
    if (vkCreateDescriptorPool(Gfx::context->getVkDevice(), &poolInfo, nullptr, &this->_descriptorPool) != VK_SUCCESS)
    {
        std::cout << "Gfx : Descriptor ::create descriptor pool failed " << std::endl;
        return;
    }
    std::cout << "Gfx : Descriptor ::create descriptor pool success " << std::endl;
}
void GfxDescriptorUI::_createDescriptorSetLayout()
{
    std::vector<VkDescriptorSetLayoutBinding> bindings;
    VkDescriptorSetLayoutBinding uboLayoutBinding{};
    uboLayoutBinding.binding = 0;
    uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    uboLayoutBinding.descriptorCount = 1;
    uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
    bindings.push_back(uboLayoutBinding);

    // 纹理采样器绑定
    VkDescriptorSetLayoutBinding samplerLayoutBinding{};
    samplerLayoutBinding.binding = 1;
    samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    samplerLayoutBinding.descriptorCount = this->_samplerCount;
    samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
    bindings.push_back(samplerLayoutBinding);

    VkDescriptorSetLayoutCreateInfo layoutInfo{};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
    layoutInfo.pBindings = bindings.data();

    if (vkCreateDescriptorSetLayout(Gfx::context->getVkDevice(),
                                    &layoutInfo, nullptr, &this->_descriptorSetLayout) != VK_SUCCESS)
    {
        std::cout << "Gfx : Descriptor ::create descriptor set layout failed " << std::endl;
        return;
    }
    std::cout << "Gfx : Descriptor ::create descriptor set layout success " << std::endl;
}
std::vector<VkDescriptorSet> GfxDescriptorUI::getDescriptorSets()
{
    if (this->_currentObjectCount >= this->_maxObjectCount)
    {
        std::cout << "Gfx : Descriptor ::get descriptor sets failed, max object count reached " << std::endl;
        return {VK_NULL_HANDLE, VK_NULL_HANDLE};
    }
    this->_currentObjectCount++;
    std::vector<VkDescriptorSet> descriptorSets;
    std::vector<VkDescriptorSetLayout> layouts(1, this->_descriptorSetLayout);
    VkDescriptorSetAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = this->_descriptorPool;
    allocInfo.descriptorSetCount = 1;
    allocInfo.pSetLayouts = layouts.data();
    descriptorSets.resize(1);
    if (vkAllocateDescriptorSets(Gfx::context->getVkDevice(), &allocInfo, descriptorSets.data()) != VK_SUCCESS)
    {
        std::cout << "Gfx : Descriptor ::create descriptor sets failed " << std::endl;
        return {VK_NULL_HANDLE, VK_NULL_HANDLE};
    }
    std::cout << "Gfx : Descriptor ::create descriptor sets success " << std::endl;
    return descriptorSets;
}


GfxDescriptorUI::~GfxDescriptorUI()
{
}

// void GfxDescriptor::_createDescriptorSets()
// {
//     // std::vector<VkImageView> &swapChainImageViews = Gfx::context->getSwapChainImageViews();
//     // uint32_t swapChainImageCount = swapChainImageViews.size();

//     // std::vector<VkDescriptorSetLayout> layouts(swapChainImageCount, this->_descriptorSetLayout);
//     // VkDescriptorSetAllocateInfo allocInfo{};
//     // allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
//     // allocInfo.descriptorPool = this->_descriptorPool;
//     // allocInfo.descriptorSetCount = swapChainImageCount;
//     // allocInfo.pSetLayouts = layouts.data();
//     // this->_descriptorSets.resize(swapChainImageCount);
//     // if (vkAllocateDescriptorSets(Gfx::context->getVkDevice(), &allocInfo, this->_descriptorSets.data()) != VK_SUCCESS)
//     // {
//     //     std::cout << "Gfx : Descriptor ::create descriptor sets failed " << std::endl;
//     //     return;
//     // }
//     // std::cout << "Gfx : Descriptor ::create descriptor sets success " << std::endl;

//     // for (uint32_t i = 0; i < this->_maxObjectCount - 1; i++)
//     // {
//     //     std::vector<VkDescriptorSet> descriptorSets;

//     //     std::vector<VkImageView> &swapChainImageViews = Gfx::context->getSwapChainImageViews();
//     //     uint32_t swapChainImageCount = swapChainImageViews.size();

//     //     std::vector<VkDescriptorSetLayout> layouts(swapChainImageCount, this->_descriptorSetLayout);
//     //     VkDescriptorSetAllocateInfo allocInfo{};
//     //     allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
//     //     allocInfo.descriptorPool = this->_descriptorPool;
//     //     allocInfo.descriptorSetCount = swapChainImageCount;
//     //     allocInfo.pSetLayouts = layouts.data();
//     //     descriptorSets.resize(swapChainImageCount);
//     //     if (vkAllocateDescriptorSets(Gfx::context->getVkDevice(), &allocInfo, descriptorSets.data()) != VK_SUCCESS)
//     //     {
//     //         std::cout << "Gfx : Descriptor ::create descriptor sets failed " << std::endl;
//     //         return;
//     //     }
//     //     std::cout << "Gfx : Descriptor ::create descriptor sets success " << std::endl;
//     // }
// }