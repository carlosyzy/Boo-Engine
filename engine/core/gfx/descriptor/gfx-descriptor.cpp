
#include "gfx-descriptor.h"
#include "../gfx.h"
#include "../gfx-context.h"
#include "../gfx-struct.h"
GfxDescriptor::GfxDescriptor()
{
    this->_createDescriptorPool();
    this->_createDescriptorSetLayout();
    this->_createDescriptorSets();
}
void GfxDescriptor::_createDescriptorPool()
{
    std::vector<VkDescriptorPoolSize> poolSizes(1);
    poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    poolSizes[1].descriptorCount = 3; // 单个描述符集3

    VkDescriptorPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
    poolInfo.pPoolSizes = poolSizes.data();
    poolInfo.maxSets = MAX_FRAMES_IN_FLIGHT * 10; // 支持更多描述符集 暂时10个
    if (vkCreateDescriptorPool(Gfx::context->getVkDevice(), &poolInfo, nullptr, &this->_descriptorPool) != VK_SUCCESS)
    {
        std::cout << "Gfx : Descriptor ::create descriptor pool failed " << std::endl;
        return;
    }
    std::cout << "Gfx : Descriptor ::create descriptor pool success " << std::endl;
}
void GfxDescriptor::_createDescriptorSetLayout()
{
    std::vector<VkDescriptorSetLayoutBinding> bindings;
    // 采样器绑定
    VkDescriptorSetLayoutBinding samplerLayoutBinding{};
    samplerLayoutBinding.binding = 0;
    samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    samplerLayoutBinding.descriptorCount = 3;
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
void GfxDescriptor::_createDescriptorSets()
{
    std::vector<VkImageView> &swapChainImageViews = Gfx::context->getSwapChainImageViews();
    uint32_t swapChainImageCount = swapChainImageViews.size();

    std::vector<VkDescriptorSetLayout> layouts(swapChainImageCount, this->_descriptorSetLayout);
    VkDescriptorSetAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = this->_descriptorPool;
    allocInfo.descriptorSetCount = swapChainImageCount;
    allocInfo.pSetLayouts = layouts.data();
    this->_descriptorSets.resize(swapChainImageCount);
    if (vkAllocateDescriptorSets(Gfx::context->getVkDevice(), &allocInfo, this->_descriptorSets.data()) != VK_SUCCESS)
    {
        std::cout << "Gfx : Descriptor ::create descriptor sets failed " << std::endl;
        return;
    }
    std::cout << "Gfx : Descriptor ::create descriptor sets success " << std::endl;
}
GfxDescriptor::~GfxDescriptor()
{
}
