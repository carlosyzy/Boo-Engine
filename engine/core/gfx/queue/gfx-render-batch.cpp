#include "gfx-render-batch.h"
#include "../gfx.h"
#include "../gfx-context.h"

GfxRenderBatch::GfxRenderBatch(GfxMaterial &material, GfxMesh &mesh):_material(material),_mesh(mesh),_objectCount(0)
{
}
void GfxRenderBatch::addObject()
{
    if (this->_objectCount >= maxObjects)
    {
        throw std::runtime_error("GfxRenderBatch::addObject() object count exceeds maxObjects!");
    }
}
GfxRenderBatch::~GfxRenderBatch()
{
}

// void GfxRenderBatch::_createStorageBuffers()
// {
//     VkDeviceSize bufferSize = sizeof(StorageBufferObject) * maxObjects;
//     this->_storageBuffers.resize(MAX_FRAMES_IN_FLIGHT);
//     this->_storageBuffersMemory.resize(MAX_FRAMES_IN_FLIGHT);
//     this->_storageBuffersMapped.resize(MAX_FRAMES_IN_FLIGHT);
//     for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
//     {
//         // 创建缓冲区
//         VkBufferCreateInfo bufferInfo{};
//         bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
//         bufferInfo.size = bufferSize;
//         bufferInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
//         bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
//         if (vkCreateBuffer(Gfx::context->vkDevice(), &bufferInfo, nullptr, &this->_storageBuffers[i]) != VK_SUCCESS)
//         {
//             throw std::runtime_error("Failed to create uniform buffer!");
//         }
//         // 获取内存需求
//         VkMemoryRequirements memRequirements;
//         vkGetBufferMemoryRequirements(Gfx::context->vkDevice(), this->_storageBuffers[i], &memRequirements);
//         // 分配内存
//         VkMemoryAllocateInfo allocInfo{};
//         allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
//         allocInfo.allocationSize = memRequirements.size;
//         allocInfo.memoryTypeIndex = this->_findMemoryType(
//             memRequirements.memoryTypeBits,
//             VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

//         if (vkAllocateMemory(Gfx::context->vkDevice(), &allocInfo, nullptr, &this->_storageBuffersMemory[i]) != VK_SUCCESS)
//         {
//             throw std::runtime_error("Failed to allocate uniform buffer memory!");
//         }
//         // 绑定内存
//         vkBindBufferMemory(Gfx::context->vkDevice(), this->_storageBuffers[i], this->_storageBuffersMemory[i], 0);
//         // 映射内存
//         vkMapMemory(Gfx::context->vkDevice(), this->_storageBuffersMemory[i], 0, bufferSize, 0, &this->_storageBuffersMapped[i]);
//     }
// }
// uint32_t GfxRenderBatch::_findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
// {
//     VkPhysicalDeviceMemoryProperties memProperties;
//     vkGetPhysicalDeviceMemoryProperties(Gfx::context->physicalDevice(), &memProperties);

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
// void GfxBatch::addObject(StorageBufferObject &object)
// {
//     // 检查是否超过最大对象数量
//     if (this->_objectCount >= this->maxObjects)
//     {
//         throw std::runtime_error("Cannot add more objects: batch is full!");
//     }
//     auto *storageData = static_cast<StorageBufferObject *>(this->_storageBuffersMapped);
//     // 将对象数据复制到映射的内存中
//     // storageData 指向数组的第一个元素
//     // storageData + 1 指向数组的第二个元素（偏移一个 StorageBufferObject 的大小）
//     // storageData + n 指向数组的第 n+1 个元素
//     memcpy(storageData + this->_objectCount, &object, sizeof(StorageBufferObject));
//     // 增加对象计数
//     this->_objectCount++;
// }

// void GfxRenderBatch::_cleanStorageBuffers()
// {
// }

