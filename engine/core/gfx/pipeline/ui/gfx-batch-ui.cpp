#include "gfx-batch-ui.h"
#include "../../gfx.h"
#include "../../gfx-mgr.h"
#include "../../gfx-context.h"
#include "../../gfx-mesh.h"
#include "../../gfx-material.h"
#include "../../gfx-pipeline-struct.h"
#include "../../gfx-render-texture.h"
#include "gfx-renderer-ui.h"
#include "gfx-pipeline-ui.h"

GfxBatchUI::GfxBatchUI(GfxRendererUI *renderer, GfxRenderTexture *renderTexture, GfxMaterial *material, GfxMesh *mesh)
{
    this->_renderer = renderer;
    this->_renderTexture = renderTexture;
    this->_material = material;
    this->_mesh = mesh;
}
// 后续带上每个物体独有的数据
void GfxBatchUI::addObject(std::vector<float> &instanceData)
{
    if (instanceData.size() == 0)
    {
        throw std::runtime_error("GfxBatchUI::addObject() instanceData size is 0!");
    }
    else if (instanceData.size() != (16 + 4))
    {
        throw std::runtime_error("GfxBatchUI::addObject() instanceData size is not multiple of vertex size!");
    }
    this->_instanceDatas.insert(this->_instanceDatas.end(), instanceData.begin(), instanceData.end());
}
void GfxBatchUI::render(VkCommandBuffer &queueCommandBuffer)
{
    const GfxPipelineStruct &pipelineStruct = this->_material->getPipelineStruct();
    GfxPipelineUI *pipeline = this->_renderer->getPipeline(pipelineStruct.generateKey());
    if (pipeline == nullptr)
    {
        throw std::runtime_error("GfxRenderBatch::render() pipeline not found!");
    }
    this->_bindPipeline(queueCommandBuffer, pipeline);

    VkDescriptorSet &descriptor = this->_renderer->getDescriptorSet();
    std::array<VkWriteDescriptorSet, 5> descriptorWrites{};
    // 绑定ubo
    VkDescriptorBufferInfo bufferInfo{};
    // bufferInfo.buffer = VK_NULL_HANDLE;
    // bufferInfo.offset = 0;
    // bufferInfo.range = 0;
    descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWrites[0].dstSet = descriptor;
    descriptorWrites[0].dstBinding = 0;
    descriptorWrites[0].dstArrayElement = 0;
    descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    descriptorWrites[0].descriptorCount = 1;
    descriptorWrites[0].pBufferInfo = &bufferInfo;
    // 绑定采样器
    for (size_t i = 0; i < 4; i++)
    {
        VkDescriptorImageInfo imageInfo{};
        imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        if (i < this->_material->getTextures().size())
        {
            std::string textureUuid = this->_material->getTextures()[i];
            GfxTexture *texture = Gfx::renderer->getTexture(textureUuid);
            std::cout << "GfxBatchUI::render() textureUuid: " << textureUuid << " index: " << i << std::endl;
            if (texture != nullptr)
            {
                imageInfo.imageView = texture->getImageView();
                imageInfo.sampler = texture->getSampler();
            }
        }
        else
        {
            std::cout << "GfxBatchUI::render() texture not found! index: " << i << std::endl;
        }
        descriptorWrites[i + 1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrites[i + 1].dstSet = descriptor;
        descriptorWrites[i + 1].dstBinding = i + 1;
        descriptorWrites[i + 1].dstArrayElement = 0;
        descriptorWrites[i + 1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        descriptorWrites[i + 1].descriptorCount = 1;
        descriptorWrites[i + 1].pImageInfo = &imageInfo;
    }

    vkUpdateDescriptorSets(Gfx::context->getVkDevice(), static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
    vkCmdBindDescriptorSets(queueCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->getVKPipelineLayout(), 0, 1, &descriptor, 0, nullptr);

    VkDeviceSize offsets[1] = {0};
    VkBuffer vertexBuffer = this->_mesh->getVertexBuffer();
    VkBuffer indexBuffer = this->_mesh->getIndexBuffer();
    vkCmdBindVertexBuffers(queueCommandBuffer, 0, 1, &vertexBuffer, offsets);
    vkCmdBindIndexBuffer(queueCommandBuffer, indexBuffer, 0, VK_INDEX_TYPE_UINT32);

    // 设置视口
    VkViewport _viewport{};
    // 裁剪区域 原点左上角为(0,0)
    VkRect2D _scissor{};
    _viewport.x = 0.0f;
    _viewport.y = 0.0f;
    _viewport.width = (float)this->_renderTexture->getWidth();
    _viewport.height = (float)this->_renderTexture->getHeight();
    _viewport.minDepth = 0.0f;
    _viewport.maxDepth = 1.0f;
    _scissor.offset = {0, 0};
    _scissor.extent = {(uint32_t)(this->_renderTexture->getWidth()), (uint32_t)(this->_renderTexture->getHeight())};
    vkCmdSetViewport(queueCommandBuffer, 0, 1, &_viewport);
    // 设置裁剪区域
    vkCmdSetScissor(queueCommandBuffer, 0, 1, &_scissor);

    vkCmdDrawIndexed(
        queueCommandBuffer,
        3, // 只绘制3个索引（第一个三角形）
        1, // 实例数 （2的话代表绘制2个实例，也就是绘制两次）
        0, // 第一个顶点的索引 每个 UI 元素占用 6 个顶点
        0, // 第一个实例的索引 从第 0 个实例开始绘制
        0  // 实例偏移
    );
}
void GfxBatchUI::_bindPipeline(VkCommandBuffer &queueCommandBuffer, GfxPipelineUI *pipeline)
{
    vkCmdBindPipeline(queueCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->getVKPipeline());
}
void GfxBatchUI::destroy()
{
    this->_renderer = nullptr;
    this->_renderTexture = nullptr;
    this->_material = nullptr;
    this->_mesh = nullptr;
}
GfxBatchUI::~GfxBatchUI()
{
}

// GfxPipeline *pipeline = Gfx::renderer->getPipeline(this->_material->_pipelineStruct.generateKey());
// if (pipeline == nullptr)
// {
//     throw std::runtime_error("GfxRenderBatch::render() pipeline not found!");
// }
// GfxDescriptor *descriptor = pipeline->getDescriptor();
// if (descriptor == nullptr)
// {
//     throw std::runtime_error("GfxRenderBatch::render() descriptor not found!");
// }
// std::array<VkWriteDescriptorSet, 1> descriptorWrites{};
// // 纹理绑定
// std::vector<VkDescriptorImageInfo> imageInfos{};
// uint32_t samplerCount = descriptor->samplerCount();
// for (uint32_t i = 0; i < samplerCount; i++)
// {
//     GfxTexture *texture = Gfx::renderer->getTexture(this->_material.textures[i]);
//     VkDescriptorImageInfo imageInfo{};
//     imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
//     if (texture != nullptr)
//     {
//         imageInfo.imageView = texture->getImageView();
//         imageInfo.sampler = texture->getSampler();
//     }
//     imageInfos.push_back(imageInfo);
// }
// descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
// descriptorWrites[0].dstSet = descriptorSet;
// descriptorWrites[0].dstBinding = 0;
// descriptorWrites[0].dstArrayElement = 0;
// descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
// descriptorWrites[0].descriptorCount = (uint32_t)imageInfos.size();
// descriptorWrites[0].pImageInfo = imageInfos.data();
// vkUpdateDescriptorSets(Gfx::context->getVkDevice(), static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
// vkCmdBindDescriptorSets(queueCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->getVKPipelineLayout(), 0, 1, &descriptorSet, 0, nullptr);

// VkDeviceSize offsets[1] = {0};
// vkCmdBindVertexBuffers(queueCommandBuffer, 0, 1, &this->_vertexBuffer, offsets);
// vkCmdBindIndexBuffer(queueCommandBuffer, this->_indexBuffer, 0, VK_INDEX_TYPE_UINT32);

// vkCmdDrawIndexed(
//     queueCommandBuffer,
//     3, // 只绘制3个索引（第一个三角形）
//     1, // 实例数 （2的话代表绘制2个实例，也就是绘制两次）
//     0, // 第一个顶点的索引 每个 UI 元素占用 6 个顶点
//     0, // 第一个实例的索引 从第 0 个实例开始绘制
//     0  // 实例偏移
// );
// // 渲染结束
// vkCmdEndRenderPass(queueCommandBuffer);
// // 结束渲染pass
// if (vkEndCommandBuffer(queueCommandBuffer) != VK_SUCCESS)
// {
//     throw std::runtime_error("Failed to record command buffer!");
// }

// commandBuffers.push_back(renderTexture->getCommandBuffer());
// std::cout << "GfxRenderBatch::_beginBindRenderPass() commandBuffers.size():" << std::endl;

// VkDescriptorImageInfo imageInfo{};
// imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
// imageInfo.imageView = texture->getImageView();
// imageInfo.sampler = texture->getSampler();

// GfxDescriptor *descriptor = pipeline->getDescriptor();
// if (descriptor != nullptr)
// {
//     std::vector<VkDescriptorSet> descriptorSets = pipeline->getDescriptorSets();
//     GfxTexture *texture = Gfx::renderer->getTexture("default-texture");
//     VkDescriptorImageInfo imageInfo{};
//     imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
//     imageInfo.imageView = texture->getImageView();
//     imageInfo.sampler = texture->getSampler();

//     std::array<VkWriteDescriptorSet, 1> descriptorWrites{};
//     descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
//     descriptorWrites[0].dstSet = descriptorSets[imageIndex];
//     descriptorWrites[0].dstBinding = 0;
//     descriptorWrites[0].dstArrayElement = 0;
//     descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
//     descriptorWrites[0].descriptorCount = 1;
//     descriptorWrites[0].pImageInfo = &imageInfo;
//     vkUpdateDescriptorSets(Gfx::context->getVkDevice(), static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);

//     vkCmdBindDescriptorSets(renderTexture->getCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->getVKPipelineLayout(), 0, 1, &(descriptorSets[imageIndex]), 0, nullptr);
// }
//     vkCmdBindDescriptorSets(commandBuffers[imageIndex], VK_PIPELINE_BIND_POINT_GRAPHICS, this->_pipeline->getVKPipelineLayout(), 0, 1, &this->_descriptorSets[imageIndex], 0, nullptr);

// VkDeviceSize offsets[1] = {0};
// vkCmdBindVertexBuffers(renderTexture->getCommandBuffer(), 0, 1, &this->_vertexBuffer, offsets);
// vkCmdBindIndexBuffer(renderTexture->getCommandBuffer(), this->_indexBuffer, 0, VK_INDEX_TYPE_UINT32);

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

// PushConstants pushConstants{};
// pushConstants.defaultColor[0] = 1.0f;
// pushConstants.defaultColor[1] = 0.0f;
// pushConstants.defaultColor[2] = 0.0f;
// pushConstants.defaultColor[3] = 1.0f;
// vkCmdPushConstants(commandBuffers[imageIndex], pipeline->vkPipelineLayout(), VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(PushConstants), &pushConstants);

// vkCmdDrawIndexed(
//     renderTexture->getCommandBuffer(),
//     3, // 只绘制3个索引（第一个三角形）
//     1, // 实例数 （2的话代表绘制2个实例，也就是绘制两次）
//     0, // 第一个顶点的索引 每个 UI 元素占用 6 个顶点
//     0, // 第一个实例的索引 从第 0 个实例开始绘制
//     0  // 实例偏移
// );
// // 渲染结束
// vkCmdEndRenderPass(renderTexture->getCommandBuffer());
// // 结束渲染pass
// if (vkEndCommandBuffer(renderTexture->getCommandBuffer()) != VK_SUCCESS)
// {
//     throw std::runtime_error("Failed to record command buffer!");
// }
// commandBuffers.push_back(renderTexture->getCommandBuffer());
// std::cout << "GfxRenderBatch::_beginBindRenderPass() commandBuffers.size():" << std::endl;

// if (this->_objectCount == 0)
// {
//     return;
// }
// std::cout << "GfxRenderBatch::render() imageIndex:" << imageIndex << std::endl;
// if (renderTexture == nullptr)
// {
//     // throw std::runtime_error("GfxRenderBatch::render() renderTexture is nullptr!");
//     if (imageIndex >= 2)
//     {
//         return;
//     }
// }

// GfxPipeline *pipeline = Gfx::renderer->getPipeline("default");
// if (pipeline == nullptr)
// {
//     throw std::runtime_error("GfxRenderBatch::render() pipeline not found!");
// }
// GfxPass *pass = Gfx::renderer->getPass("default");
// if (pass == nullptr)
// {
//     throw std::runtime_error("GfxRenderBatch::_createFramebuffers() pass not found!");
// }

// vkResetCommandBuffer(renderTexture->getCommandBuffer(), 0);

// VkCommandBufferBeginInfo beginInfo{};
// beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
// if (vkBeginCommandBuffer(renderTexture->getCommandBuffer(), &beginInfo) != VK_SUCCESS)
// {
//     throw std::runtime_error("Failed to begin recording command buffer!");
// }

// VkRenderPassBeginInfo renderPassInfo{};
// renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
// renderPassInfo.renderPass = pass->vkRenderPass();
// renderPassInfo.framebuffer = renderTexture->getFramebuffer();

// renderPassInfo.renderArea.offset = {0, 0};
// renderPassInfo.renderArea.extent = Gfx::context->getSwapChainExtent();

// VkClearValue clearColor{};
// renderPassInfo.clearValueCount = 1;
// clearColor.color = {{0.0f, 0.0f, 0.0f, 0.0f}};
// renderPassInfo.pClearValues = &clearColor;

// vkCmdBeginRenderPass(renderTexture->getCommandBuffer(), &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

// this->_createVertexBuffers();

// vkCmdBindPipeline(renderTexture->getCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->vkPipeline());
// }

// void GfxRenderBatch::_beginBindRenderPass(GfxRenderTexture *renderTexture)
// {

//     GfxPass *pass = Gfx::renderer->getPass("default");
//     if (pass == nullptr)
//     {
//         std::cout << "GfxRenderBatch::_beginBindRenderPass() pass not found!" << std::endl;
//         return;
//     }

//     // 绑定render pass
//     VkRenderPassBeginInfo renderPassInfo{};
//     renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
//     renderPassInfo.renderPass = pass->vkRenderPass();
//     renderPassInfo.framebuffer = renderTexture->getFramebuffer();
//     renderPassInfo.renderArea.offset = {0, 0};
//     renderPassInfo.renderArea.extent = Gfx::context->getSwapChainExtent();

//     // 设置清除颜色为黑色
//     VkClearValue clearColor{};
//     clearColor.color = {{1.0f, 1.0f, 1.0f, 1.0f}};
//     renderPassInfo.clearValueCount = 1;
//     renderPassInfo.pClearValues = &clearColor;

//     std::cout << "GfxRenderBatch::_beginBindRenderPass() clearColor:" << std::endl;

//     vkCmdBeginRenderPass(renderTexture->getCommandBuffer(), &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
// }

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
//         if (vkCreateBuffer(Gfx::context->getVkDevice(), &bufferInfo, nullptr, &this->_storageBuffers[i]) != VK_SUCCESS)
//         {
//             throw std::runtime_error("Failed to create uniform buffer!");
//         }
//         // 获取内存需求
//         VkMemoryRequirements memRequirements;
//         vkGetBufferMemoryRequirements(Gfx::context->getVkDevice(), this->_storageBuffers[i], &memRequirements);
//         // 分配内存
//         VkMemoryAllocateInfo allocInfo{};
//         allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
//         allocInfo.allocationSize = memRequirements.size;
//         allocInfo.memoryTypeIndex = this->_findMemoryType(
//             memRequirements.memoryTypeBits,
//             VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

//         if (vkAllocateMemory(Gfx::context->getVkDevice(), &allocInfo, nullptr, &this->_storageBuffersMemory[i]) != VK_SUCCESS)
//         {
//             throw std::runtime_error("Failed to allocate uniform buffer memory!");
//         }
//         // 绑定内存
//         vkBindBufferMemory(Gfx::context->getVkDevice(), this->_storageBuffers[i], this->_storageBuffersMemory[i], 0);
//         // 映射内存
//         vkMapMemory(Gfx::context->getVkDevice(), this->_storageBuffersMemory[i], 0, bufferSize, 0, &this->_storageBuffersMapped[i]);
//     }
// }
// uint32_t GfxRenderBatch::_findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
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
