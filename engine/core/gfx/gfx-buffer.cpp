#include "gfx-buffer.h"
#include "gfx.h"
#include "gfx-context.h"

GfxBuffer::GfxBuffer()
{
}
void GfxBuffer::create(VkDeviceSize bufferSize)
{
    this->_uniformBuffers.resize(MAX_FRAMES_IN_FLIGHT);
    this->_uniformBuffersMemory.resize(MAX_FRAMES_IN_FLIGHT);
    this->_uniformBuffersMapped.resize(MAX_FRAMES_IN_FLIGHT);
    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    {
        // 创建缓冲区
        VkBufferCreateInfo bufferInfo{};
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferInfo.size = bufferSize;
        bufferInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
        bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
        if (vkCreateBuffer(Gfx::context->vkDevice(), &bufferInfo, nullptr, &this->_uniformBuffers[i]) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to create uniform buffer!");
        }
        // 获取内存需求
        VkMemoryRequirements memRequirements;
        vkGetBufferMemoryRequirements(Gfx::context->vkDevice(), this->_uniformBuffers[i], &memRequirements);
        // 分配内存
        VkMemoryAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = this->_findMemoryType(
            memRequirements.memoryTypeBits,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

        if (vkAllocateMemory(Gfx::context->vkDevice(), &allocInfo, nullptr, &this->_uniformBuffersMemory[i]) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to allocate uniform buffer memory!");
        }
        // 绑定内存
        vkBindBufferMemory(Gfx::context->vkDevice(), this->_uniformBuffers[i], this->_uniformBuffersMemory[i], 0);
        // 映射内存
        vkMapMemory(Gfx::context->vkDevice(), this->_uniformBuffersMemory[i], 0, bufferSize, 0, &this->_uniformBuffersMapped[i]);
    }
}
uint32_t GfxBuffer::_findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
{
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(Gfx::context->physicalDevice(), &memProperties);

    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
    {
        /* // 检查内存类型是否满足类型过滤器要求
        // 并且具有我们需要的属性标志 */
        if ((typeFilter & (1 << i)) &&
            (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
        {
            return i;
        }
    }

    throw std::runtime_error("Failed to find suitable memory type!");
}
GfxBuffer::~GfxBuffer()
{
}
