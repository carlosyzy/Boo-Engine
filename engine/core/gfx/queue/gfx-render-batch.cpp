#include "gfx-render-batch.h"
#include "../gfx.h"
#include "../gfx-mgr.h"
#include "../gfx-context.h"
#include "../gfx-renderer.h"
#include "../pass/gfx-pass.h"
#include "../pipeline/gfx-pipeline.h"

GfxRenderBatch::GfxRenderBatch(const GfxMaterial &material, const GfxMesh &mesh) : _material(material), _mesh(mesh), _objectCount(0)
{
    std::cout << "material uuid: " << this->_material.uuid << std::endl;
    std::cout << "material pass : " << "pass-built" << std::endl;
    std::cout << "material pipeline : " << "pipeline-built" << std::endl;
    std::cout << "mesh uuid: " << this->_mesh.uuid << std::endl;

    std::vector<float> vertices{
        -0.5f,0.5f, 0.0f,0.0f,   0.0f, 0.0f,
        -0.5f, -0.5f, 0.0f,0.0f,    0.0f,1.0f,
        0.5f, -0.5f,0.0f,0.0f,    1.0f, 1.0f
    };

    std::vector<float> indices{
        0,
        1,
        2,
    };

    // 清理旧缓冲区
    if (this->_vertexBuffer != VK_NULL_HANDLE)
    {
        vkDestroyBuffer(Gfx::context->vkDevice(), this->_vertexBuffer, nullptr);
        this->_vertexBuffer = VK_NULL_HANDLE;
    }
    if (_vertexMemory != VK_NULL_HANDLE)
    {
        vkFreeMemory(Gfx::context->vkDevice(), this->_vertexMemory, nullptr);
        this->_vertexMemory = VK_NULL_HANDLE;
    }
    if (this->_indexBuffer != VK_NULL_HANDLE)
    {
        vkDestroyBuffer(Gfx::context->vkDevice(), this->_indexBuffer, nullptr);
        this->_indexBuffer = VK_NULL_HANDLE;
    }
    if (this->_indexMemory != VK_NULL_HANDLE)
    {
        vkFreeMemory(Gfx::context->vkDevice(), this->_indexMemory, nullptr);
        this->_indexMemory = VK_NULL_HANDLE;
    }

    // 使用传入的mesh数据创建缓冲区
    // 顶点缓冲区
    GfxMgr::getInstance()->createBuffer(
        VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
        &this->_vertexBuffer,
        &this->_vertexMemory,
        vertices.size() * sizeof(float),
        vertices.data());

    // 索引缓冲区（使用正确的uint32_t类型）
    GfxMgr::getInstance()->createBuffer(
        VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
        &this->_indexBuffer,
        &this->_indexMemory,
        indices.size() * sizeof(uint32_t),
        indices.data());

    this->_indexSize = indices.size();

    this->_createBuffers();
}
void GfxRenderBatch::addObject()
{
    if (this->_objectCount >= maxObjects)
    {
        throw std::runtime_error("GfxRenderBatch::addObject() object count exceeds maxObjects!");
    }
    this->_objectCount++;
}
void GfxRenderBatch::render(uint32_t imageIndex, std::vector<VkCommandBuffer> &commandBuffers)
{
    // if (this->_objectCount == 0)
    // {
    //     return;
    // }
    // std::cout << "GfxRenderBatch::render() imageIndex:" << imageIndex << std::endl;

    vkResetCommandBuffer(this->_commandBuffers[imageIndex], 0);

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    if (vkBeginCommandBuffer(this->_commandBuffers[imageIndex], &beginInfo) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to begin recording command buffer!");
    }

    this->_beginBindRenderPass(imageIndex);

    // this->_createVertexBuffers();

    GfxPipeline *pipeline = Gfx::renderer->getPipeline("pipeline-built");
    if (pipeline == nullptr)
    {
        throw std::runtime_error("GfxRenderBatch::render() pipeline not found!");
    }
    vkCmdBindPipeline(this->_commandBuffers[imageIndex], VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->vkPipeline());

    VkDeviceSize offsets[1] = {0};
    vkCmdBindVertexBuffers(this->_commandBuffers[imageIndex], 0, 1, &this->_vertexBuffer, offsets);
    vkCmdBindIndexBuffer(this->_commandBuffers[imageIndex], this->_indexBuffer, 0, VK_INDEX_TYPE_UINT32);

    // VkDescriptorSet descriptorSet = Gfx::renderer->getDescriptorSet(imageIndex);
    // vkCmdBindDescriptorSets(this->_commandBuffers[imageIndex], VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->vkPipelineLayout(), 0, 1, &descriptorSet, 0, nullptr);

    // // 设置正确的视口尺寸，使用交换链的实际尺寸
    // VkViewport viewport{};
    // viewport.x = 0.0f;
    // viewport.y = 0.0f;
    // viewport.width = static_cast<float>(Gfx::context->getSwapChainExtent().width);
    // viewport.height = static_cast<float>(Gfx::context->getSwapChainExtent().height);
    // viewport.minDepth = 0.0f;
    // viewport.maxDepth = 1.0f;
    // vkCmdSetViewport(this->_commandBuffers[imageIndex], 0, 1, &viewport);

    // // 使用相同的区域进行裁剪
    // VkRect2D scissor{};
    // scissor.offset = {0, 0};
    // scissor.extent = Gfx::context->getSwapChainExtent();
    // vkCmdSetScissor(this->_commandBuffers[imageIndex], 0, 1, &scissor);

    vkCmdDrawIndexed(
        this->_commandBuffers[imageIndex],
        this->_indexSize, // 只绘制3个索引（第一个三角形）
        1,                // 实例数 （2的话代表绘制2个实例，也就是绘制两次）
        0,                // 第一个顶点的索引 每个 UI 元素占用 6 个顶点
        0,                // 第一个实例的索引 从第 0 个实例开始绘制
        0                 // 实例偏移
    );
    // 渲染结束
    vkCmdEndRenderPass(this->_commandBuffers[imageIndex]);
    // 结束渲染pass
    if (vkEndCommandBuffer(this->_commandBuffers[imageIndex]) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to record command buffer!");
    }
    commandBuffers.push_back(this->_commandBuffers[imageIndex]);
}
void GfxRenderBatch::_createBuffers()
{
    this->_createFramebuffers();
    this->_createCommandBuffers();
}
void GfxRenderBatch::_createFramebuffers()
{
    GfxPass *pass = Gfx::renderer->getPass("pass-built");
    if (pass == nullptr)
    {
        throw std::runtime_error("GfxRenderBatch::_createFramebuffers() pass not found!");
    }

    this->_framebuffers.clear();
    std::vector<VkImageView> &swapChainImageViews = Gfx::context->getSwapChainImageViews();

    this->_framebuffers.resize(swapChainImageViews.size());
    for (size_t i = 0; i < swapChainImageViews.size(); i++)
    {
        VkImageView attachments[] = {swapChainImageViews[i]};

        VkFramebufferCreateInfo framebufferInfo = {};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = pass->vkRenderPass();

        framebufferInfo.attachmentCount = 1;
        framebufferInfo.pAttachments = attachments;                         // 渲染流程对象用于描述附着信息的pAttachment数组
        framebufferInfo.width = Gfx::context->getSwapChainExtent().width;   // width和height用于指定帧缓冲的大小
        framebufferInfo.height = Gfx::context->getSwapChainExtent().height; // 交换链图像都是单层，layers设置为1
        framebufferInfo.layers = 1;

        if (vkCreateFramebuffer(Gfx::context->vkDevice(), &framebufferInfo, nullptr, &this->_framebuffers[i]) != VK_SUCCESS)

        {
            throw std::runtime_error("Failed to create framebuffer!");
        }
    }
}
void GfxRenderBatch::_createCommandBuffers()
{

    this->_commandBuffers.resize(this->_framebuffers.size());
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = Gfx::context->getCommandPool();
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = (uint32_t)this->_commandBuffers.size();

    if (vkAllocateCommandBuffers(Gfx::context->vkDevice(), &allocInfo, this->_commandBuffers.data()) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to allocate command buffers!");
    }
    // this->_Log("create commandBuffers success...");
}
void GfxRenderBatch::_createVertexBuffers()
{
}

void GfxRenderBatch::_beginBindRenderPass(uint32_t imageIndex)
{

    GfxPass *pass = Gfx::renderer->getPass("pass-built");
    if (pass == nullptr)
    {
        std::cout << "GfxRenderBatch::_beginBindRenderPass() pass not found!" << std::endl;
        return;
    }

    // 绑定render pass
    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = pass->vkRenderPass();
    renderPassInfo.framebuffer = this->_framebuffers[imageIndex];
    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = Gfx::context->getSwapChainExtent();

    // 设置清除颜色为黑色
    VkClearValue clearColor{};
    clearColor.color = {{1.0f, 1.0f, 1.0f, 1.0f}};
    renderPassInfo.clearValueCount = 1;
    renderPassInfo.pClearValues = &clearColor;

    std::cout << "GfxRenderBatch::_beginBindRenderPass() clearColor:" << std::endl;

    vkCmdBeginRenderPass(this->_commandBuffers[imageIndex], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
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
