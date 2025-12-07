
#include "gfx-descriptor.h"
#include "../gfx.h"
#include "../gfx-context.h"
#include "../gfx-struct.h"
GfxDescriptor::GfxDescriptor(uint32_t samplerCount)
{
    this->_samplerCount = samplerCount;
    std::vector<VkImageView> &swapChainImageViews = Gfx::context->getSwapChainImageViews();
    uint32_t swapChainImageCount = swapChainImageViews.size();
    this->_descriptorCount = std::max(MAX_FRAMES_IN_FLIGHT, swapChainImageCount);
    this->_createDescriptorPool();
    this->_createDescriptorSetLayout();
    this->_createDescriptorSets();
}
void GfxDescriptor::_createDescriptorPool()
{
    std::vector<VkDescriptorPoolSize> poolSizes(1);
    poolSizes[0].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    // 总采样器数量 = 每个集的采样器数 × 集的数量
    uint32_t totalSamplersWithMargin = static_cast<uint32_t>(this->_samplerCount * this->_descriptorCount * 1.5);
    poolSizes[0].descriptorCount = totalSamplersWithMargin; // 单个描述符集3

    VkDescriptorPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
    poolInfo.pPoolSizes = poolSizes.data();
    poolInfo.maxSets = this->_descriptorCount * 10; // 支持更多描述符集 暂时10个
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
void GfxDescriptor::_createDescriptorSets()
{
    std::vector<VkDescriptorSetLayout> layouts(this->_descriptorCount, this->_descriptorSetLayout);
    VkDescriptorSetAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = this->_descriptorPool;
    allocInfo.descriptorSetCount = this->_descriptorCount;
    allocInfo.pSetLayouts = layouts.data();
    this->_descriptorSets.resize(this->_descriptorCount);
    if (vkAllocateDescriptorSets(Gfx::context->getVkDevice(), &allocInfo, this->_descriptorSets.data()) != VK_SUCCESS)
    {
        std::cout << "Gfx : Descriptor ::create descriptor sets failed " << std::endl;
        return;
    }
    std::cout << "Gfx : Descriptor ::create descriptor sets success " << std::endl;
}
void GfxDescriptor::reset()
{
    std::vector<VkImageView> &swapChainImageViews = Gfx::context->getSwapChainImageViews();
    uint32_t swapChainImageCount = swapChainImageViews.size();
    uint32_t descriptorCount = std::max(MAX_FRAMES_IN_FLIGHT, swapChainImageCount);
    if (descriptorCount == this->_descriptorCount)
    {
        return;
    }
    // 销毁描述符集（Descriptor Sets）
    for (auto &descriptorSet : this->_descriptorSets)
    {
        vkFreeDescriptorSets(Gfx::context->getVkDevice(), this->_descriptorPool, static_cast<uint32_t>(this->_descriptorSets.size()), &descriptorSet);
        descriptorSet = VK_NULL_HANDLE;
    }
    this->_descriptorSets.clear();
    // 销毁描述符布局（Descriptor Set Layout）
    vkDestroyDescriptorSetLayout(Gfx::context->getVkDevice(), this->_descriptorSetLayout, nullptr);
    this->_descriptorSetLayout = VK_NULL_HANDLE;
    // 销毁描述符池（Descriptor Pool）
    vkDestroyDescriptorPool(Gfx::context->getVkDevice(), this->_descriptorPool, nullptr);
    this->_descriptorPool = VK_NULL_HANDLE;

    this->_descriptorCount = descriptorCount;
    this->_createDescriptorPool();
    this->_createDescriptorSetLayout();
    this->_createDescriptorSets();
}
GfxDescriptor::~GfxDescriptor()
{
}
