#pragma once
#include <vulkan/vulkan_core.h>
#include <algorithm>
#include <vector>

class GfxDescriptor
{
protected:
    /**
     * @brief 采样器数量
     */
    uint32_t _samplerCount;
    /**
     * @brief 最大对象数量
     */
    uint32_t _maxObjectCount;

    uint32_t _currentObjectCount;

    VkDescriptorPool _descriptorPool;
    /**
     * @brief 描述符集布局
     * 绑定ubo和采样器
     */
    VkDescriptorSetLayout _descriptorSetLayout;

    virtual void _createDescriptorPool();
    virtual void _createDescriptorSetLayout();

public:
    GfxDescriptor(uint32_t samplerCount, uint32_t maxObject);

    VkDescriptorSetLayout getDescriptorSetLayout() const
    {
        return this->_descriptorSetLayout;
    }
    std::vector<VkDescriptorSet> getDescriptorSets();
    void reset();

    ~GfxDescriptor();
};