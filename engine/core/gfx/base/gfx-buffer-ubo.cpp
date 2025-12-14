#include "gfx-buffer-ubo.h"
#include "../gfx.h"
#include "../gfx-context.h"
#include "gfx-buffer.h"

GfxBufferUBO::GfxBufferUBO()
{
}
GfxBuffer *GfxBufferUBO::getBuffer(int size)
{
    // 检查是否存在该大小的池
    if (this->pools.find(size) == this->pools.end())
    {
        this->pools[size] = new GfxBufferUBOPool();
    }
    GfxBufferUBOPool *pool = this->pools[size];
    if (pool->buffers.empty())
    {
        GfxBuffer *buffer = new GfxBuffer();
        buffer->create(size, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT);
        buffer->setIsOccupied(true);
        pool->buffers.push_back(buffer);
        return buffer;
    }
    else
    {
        for (auto buffer : pool->buffers)
        {
            if (!buffer->getIsOccupied())
            {
                buffer->setIsOccupied(true);
                return buffer;
            }
        }
        GfxBuffer *buffer = new GfxBuffer();
        buffer->create(size, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT);
        buffer->setIsOccupied(true);
        pool->buffers.push_back(buffer);
        return buffer;
    }
}

void GfxBufferUBO::clear()
{
    for (auto pool : this->pools)
    {
        for (auto buffer : pool.second->buffers)
        {
            buffer->setIsOccupied(false);
        }
    }
}
// void GfxBufferUBO::create(VkDeviceSize bufferSize)
// {
//     this->_buffer = VK_NULL_HANDLE;
//     this->_bufferMemory = VK_NULL_HANDLE;
//     this->_bufferMapped = VK_NULL_HANDLE;

//     // 创建缓冲区
//     VkBufferCreateInfo bufferInfo{};
//     bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
//     bufferInfo.size = bufferSize;
//     bufferInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
//     bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
//     if (vkCreateBuffer(Gfx::context->getVkDevice(), &bufferInfo, nullptr, &this->_buffer) != VK_SUCCESS)
//     {
//         throw std::runtime_error("Failed to create buffer!");
//     }
//     // 获取内存需求
//     VkMemoryRequirements memRequirements;
//     vkGetBufferMemoryRequirements(Gfx::context->getVkDevice(), this->_buffer, &memRequirements);
//     // 分配内存
//     VkMemoryAllocateInfo allocInfo{};
//     allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
//     allocInfo.allocationSize = memRequirements.size;
//     allocInfo.memoryTypeIndex = this->_findMemoryType(
//         memRequirements.memoryTypeBits,
//         VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
//     if (vkAllocateMemory(Gfx::context->getVkDevice(), &allocInfo, nullptr, &this->_bufferMemory) != VK_SUCCESS)
//     {
//         throw std::runtime_error("Failed to allocate uniform buffer memory!");
//     }
//     // 绑定内存
//     vkBindBufferMemory(Gfx::context->getVkDevice(), this->_buffer, this->_bufferMemory, 0);
//     // 映射内存
//     vkMapMemory(Gfx::context->getVkDevice(), this->_bufferMemory, 0, bufferSize, 0, &this->_bufferMapped);
// }
// uint32_t GfxBufferUBO::_findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
// {
//     VkPhysicalDeviceMemoryProperties memProperties;
//     vkGetPhysicalDeviceMemoryProperties(Gfx::context->getPhysicalDevice(), &memProperties);

//     for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
//     {
//         /* // 检查内存类型是否满足类型过滤器要求
//         // 并且具有我们需要的属性标志 */
//         if ((typeFilter & (1 << i)) &&
//             (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
//         {
//             return i;
//         }
//     }

//     throw std::runtime_error("Failed to find suitable memory type!");
// }
// void GfxBufferUBO::setIsOccupied(bool isOccupied)
// {
//     this->_isOccupied = isOccupied;
// }
// bool GfxBufferUBO::getIsOccupied() const
// {
//     return this->_isOccupied;
// }
// void *GfxBufferUBO::getMappedData()
// {
//     return this->_bufferMapped;
// }
// VkBuffer GfxBufferUBO::getBuffer() const
// {
//     return this->_buffer;
// }

GfxBufferUBO::~GfxBufferUBO()
{
}
