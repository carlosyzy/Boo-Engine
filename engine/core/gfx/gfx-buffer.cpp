#include "gfx-buffer.h"
#include "gfx.h"
#include "gfx-context.h"

GfxBuffer::GfxBuffer()
{
}
void GfxBuffer::create(int size, VkBufferUsageFlags usageFlags)
{
    this->_buffer = VK_NULL_HANDLE;
    this->_bufferMemory = VK_NULL_HANDLE;
    this->_bufferMapped = VK_NULL_HANDLE;
    VkDeviceSize bufferSize = sizeof(float) * size;
    // 创建缓冲区
    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = bufferSize;
    bufferInfo.usage = usageFlags; // VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;  VK_BUFFER_USAGE_VERTEX_BUFFER_BIT
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    if (vkCreateBuffer(Gfx::context->getVkDevice(), &bufferInfo, nullptr, &this->_buffer) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create buffer!");
    }
    // 获取内存需求
    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(Gfx::context->getVkDevice(), this->_buffer, &memRequirements);
    // 分配内存
    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = this->_findMemoryType(
        memRequirements.memoryTypeBits,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
    if (vkAllocateMemory(Gfx::context->getVkDevice(), &allocInfo, nullptr, &this->_bufferMemory) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to allocate uniform buffer memory!");
    }
    // 绑定内存
    vkBindBufferMemory(Gfx::context->getVkDevice(), this->_buffer, this->_bufferMemory, 0);
    // 映射内存
    vkMapMemory(Gfx::context->getVkDevice(), this->_bufferMemory, 0, bufferSize, 0, &this->_bufferMapped);
}
uint32_t GfxBuffer::_findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
{
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(Gfx::context->getPhysicalDevice(), &memProperties);

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
void GfxBuffer::setIsOccupied(bool isOccupied)
{
    this->_isOccupied = isOccupied;
}
bool GfxBuffer::getIsOccupied() const
{
    return this->_isOccupied;
}
void *GfxBuffer::getMappedData()
{
    return this->_bufferMapped;
}
VkBuffer GfxBuffer::getBuffer() const
{
    return this->_buffer;
}
void GfxBuffer::destroy()
{
    if (this->_buffer != VK_NULL_HANDLE)
    {
        vkDestroyBuffer(Gfx::context->getVkDevice(), this->_buffer, nullptr);
        this->_buffer = VK_NULL_HANDLE;
    }
    if (this->_bufferMemory != VK_NULL_HANDLE)
    {
        vkFreeMemory(Gfx::context->getVkDevice(), this->_bufferMemory, nullptr);
        this->_bufferMemory = VK_NULL_HANDLE;
    }
    if (this->_bufferMapped != VK_NULL_HANDLE)
    {
        vkUnmapMemory(Gfx::context->getVkDevice(), this->_bufferMemory);
        this->_bufferMapped = VK_NULL_HANDLE;
    }
}

GfxBuffer::~GfxBuffer()
{
}
