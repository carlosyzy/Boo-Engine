#pragma once
#include <vulkan/vulkan_core.h>
#include <algorithm>
#include <vector>

#include "gfx-descriptor.h"

class GfxDescriptorUI : public GfxDescriptor
{
protected:
     void _createDescriptorPool() override;
     void _createDescriptorSetLayout() override;
public:
    GfxDescriptorUI(uint32_t samplerCount, uint32_t maxObject);

    VkDescriptorSetLayout getDescriptorSetLayout() const
    {
        return this->_descriptorSetLayout;
    }
    std::vector<VkDescriptorSet> getDescriptorSets();
    void reset();

     ~GfxDescriptorUI();
};