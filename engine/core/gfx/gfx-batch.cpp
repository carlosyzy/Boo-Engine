#include "gfx-batch.h"
#include "gfx.h"
#include "gfx-context.h"

GfxBatch::GfxBatch()
{
}
void GfxBatch::_createStorageBuffers()
{
    VkDeviceSize bufferSize = sizeof(StorageBufferObject) * maxObjects;

    // 创建buffer
    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = bufferSize;
    bufferInfo.usage = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateBuffer(Gfx::context->vkDevice(), &bufferInfo, nullptr, &_storageBuffers) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create storage buffer!");
    }
    // 获取内存需求
    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(Gfx::context->vkDevice(), this->_storageBuffers, &memRequirements);
    // 分配内存
    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = this->_findMemoryType(
        memRequirements.memoryTypeBits,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

    if (vkAllocateMemory(Gfx::context->vkDevice(), &allocInfo, nullptr, &this->_storageBuffersMemory) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to allocate storage buffer memory!");
    }
    // 绑定内存
    vkBindBufferMemory(Gfx::context->vkDevice(), this->_storageBuffers, this->_storageBuffersMemory, 0);
    // 映射内存
    vkMapMemory(Gfx::context->vkDevice(), this->_storageBuffersMemory, 0, bufferSize, 0, &this->_storageBuffersMapped);
}
uint32_t GfxBatch::_findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
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
void GfxBatch::addObject(StorageBufferObject &object)
{
    // 检查是否超过最大对象数量
    if (this->_objectCount >= this->maxObjects)
    {
        throw std::runtime_error("Cannot add more objects: batch is full!");
    }
    auto *storageData = static_cast<StorageBufferObject *>(this->_storageBuffersMapped);
    // 将对象数据复制到映射的内存中
    // storageData 指向数组的第一个元素
    // storageData + 1 指向数组的第二个元素（偏移一个 StorageBufferObject 的大小）
    // storageData + n 指向数组的第 n+1 个元素
    memcpy(storageData + this->_objectCount, &object, sizeof(StorageBufferObject));
    // 增加对象计数
    this->_objectCount++;
}

void GfxBatch::_cleanStorageBuffers()
{
    if (this->_storageBuffersMapped)
    {
        vkUnmapMemory(Gfx::context->vkDevice(), this->_storageBuffersMemory);
        this->_storageBuffersMapped = nullptr;
    }
    if (this->_storageBuffers != VK_NULL_HANDLE)
    {
        vkDestroyBuffer(Gfx::context->vkDevice(), this->_storageBuffers, nullptr);
        this->_storageBuffers = VK_NULL_HANDLE;
    }
    if (this->_storageBuffersMemory != VK_NULL_HANDLE)
    {
        vkFreeMemory(Gfx::context->vkDevice(), this->_storageBuffersMemory, nullptr);
        this->_storageBuffersMemory = VK_NULL_HANDLE;
    }
}
GfxBatch::~GfxBatch()
{
}
