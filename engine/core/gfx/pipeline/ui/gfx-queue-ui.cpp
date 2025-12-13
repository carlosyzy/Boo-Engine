#include "gfx-queue-ui.h"
#include "../../gfx.h"
#include "../../gfx-struct.h"
#include "../../gfx-context.h"
#include "../../gfx-pass.h"
#include "../../gfx-mgr.h"
#include "../../gfx-render-texture.h"
#include "../../gfx-texture.h"
#include "../../gfx-material.h"
#include "../../gfx-mesh.h"
#include "gfx-renderer-ui.h"
#include "gfx-pass-ui.h"
#include "gfx-batch-ui.h"
#include "gfx-pipeline-ui.h"

GfxQueueUI::GfxQueueUI(GfxRendererUI *renderer, GfxRenderTexture *renderTexture)
{
    this->_renderer = renderer;
    this->_renderTexture = renderTexture;
}
void GfxQueueUI::init()
{
}
void GfxQueueUI::submitMat(const std::array<float, 16> &viewMatrix, const std::array<float, 16> &projMatrix)
{
    this->_viewMatrix = viewMatrix;
    this->_projMatrix = projMatrix;
}
void GfxQueueUI::submitObject(GfxMaterial *material, GfxMesh *mesh)
{
    if (material == nullptr || mesh == nullptr)
    {
        std::cout << "submitObject: material or mesh is nullptr" << std::endl;
        return;
    }
    if (this->_batches.empty())
    {
        GfxBatchUI *batch = new GfxBatchUI(this->_renderer, this->_renderTexture, material, mesh);
        this->_batches.push_back(batch);
    }
    else
    {
        GfxBatchUI *batch = this->_batches.back();
        if (material->equals(batch->getMaterial()) && mesh->equals(batch->getMesh()))
        {
            batch->addObject();
        }
        else
        {
            GfxBatchUI *batch = new GfxBatchUI(this->_renderer, this->_renderTexture, material, mesh);
            this->_batches.push_back(batch);
        }
    }
}
void GfxQueueUI::render(std::vector<VkCommandBuffer> &commandBuffers,std::vector<std::string> &pipelineOutds)
{
    this->_resetCommandBuffer();
    this->_beginCommandBuffer();
    this->_beginRenderPass();
    for (size_t i = 0; i < this->_batches.size(); i++)
    {
        GfxBatchUI *batch = this->_batches[i];
        batch->render(this->_renderTexture->getCommandBuffer());
    }
    // 渲染结束
    vkCmdEndRenderPass(this->_renderTexture->getCommandBuffer());
    // 结束渲染pass
    if (vkEndCommandBuffer(this->_renderTexture->getCommandBuffer()) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to record command buffer!");
    }
    commandBuffers.push_back(this->_renderTexture->getCommandBuffer());

    pipelineOutds.push_back(this->_renderTexture->getColorTextureUuid());

    // 清空批次
    for (size_t i = 0; i < this->_batches.size(); i++)
    {
        GfxBatchUI *batch = this->_batches[i];
        batch->destroy();
        delete batch;
        batch = nullptr;
    }
    this->_batches.clear();
    // this->_renderTexture->saveToFile1("ui.png");

    // this->_bindPipeline(pipeline);

    // // 进行一次顶点绑定
    // VkDeviceSize offsets[1] = {0};
    // vkCmdBindVertexBuffers(this->_commandBuffers[imageIndex], 0, 1, &this->_vertexBuffer, offsets);
    // vkCmdBindIndexBuffer(this->_commandBuffers[imageIndex], this->_indexBuffer, 0, VK_INDEX_TYPE_UINT32);

    // for (size_t i = 0; i < this->_renderTextures.size(); i++)
    // {
    //     std::string textureUuid = this->_renderTextures[i];
    //     GfxTexture *texture = Gfx::renderer->getTexture(textureUuid);
    //     if (texture == nullptr)
    //     {
    //         std::cout << "render: texture not found:" << textureUuid << std::endl;
    //         continue;
    //     }

    //     std::vector<VkDescriptorSet> descriptorSets = this->_renderer->getDescriptorSets();
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
    //     vkCmdBindDescriptorSets(this->_commandBuffers[imageIndex], VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->getVKPipelineLayout(), 0, 1, &descriptorSets[imageIndex], 0, nullptr);

    //     // 设置视口
    //     VkViewport _viewport{};
    //     // 裁剪区域 原点左上角为(0,0)
    //     VkRect2D _scissor{};
    //     _viewport.x = 0.0f;
    //     _viewport.y = 0.0f;
    //     _viewport.width = (float)Gfx::context->getSwapChainExtent().width;
    //     _viewport.height = (float)Gfx::context->getSwapChainExtent().height;
    //     _viewport.minDepth = 0.0f;
    //     _viewport.maxDepth = 1.0f;
    //     _scissor.offset = {0, 0};
    //     _scissor.extent = {(uint32_t)(Gfx::context->getSwapChainExtent().width), (uint32_t)(Gfx::context->getSwapChainExtent().height)};
    //     vkCmdSetViewport(this->_commandBuffers[imageIndex], 0, 1, &_viewport);
    //     // 设置裁剪区域
    //     vkCmdSetScissor(this->_commandBuffers[imageIndex], 0, 1, &_scissor);

    //     vkCmdDrawIndexed(
    //         this->_commandBuffers[imageIndex],
    //         3, // 只绘制3个索引（第一个三角形）
    //         1, // 实例数 （2的话代表绘制2个实例，也就是绘制两次）
    //         0, // 第一个顶点的索引 每个 UI 元素占用 6 个顶点
    //         0, // 第一个实例的索引 从第 0 个实例开始绘制
    //         0  // 实例偏移
    //     );
    // }
    // // 渲染结束
    // vkCmdEndRenderPass(this->_commandBuffers[imageIndex]);
    // // 结束渲染pass
    // if (vkEndCommandBuffer(this->_commandBuffers[imageIndex]) != VK_SUCCESS)
    // {
    //     throw std::runtime_error("Failed to record command buffer!");
    // }
    // commandBuffers.push_back(this->_commandBuffers[imageIndex]);
    // this->_renderTextures.clear();
}
void GfxQueueUI::_resetCommandBuffer()
{
    vkResetCommandBuffer(this->_renderTexture->getCommandBuffer(), 0);
}
void GfxQueueUI::_beginCommandBuffer()
{
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    if (vkBeginCommandBuffer(this->_renderTexture->getCommandBuffer(), &beginInfo) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to begin recording command buffer!");
    }
}
void GfxQueueUI::_beginRenderPass()
{
    GfxPassUI *pass = this->_renderer->getRenderPass();
    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.framebuffer = this->_renderTexture->getFramebuffer();
    renderPassInfo.renderArea.extent = {this->_renderTexture->getWidth(), this->_renderTexture->getHeight()};
    renderPassInfo.renderPass = pass->getVKRenderPass();
    renderPassInfo.renderArea.offset = {0, 0};

    // VkClearValue clearColor{};
    // clearColor.color = {{0.0f, 0.0f, 0.0f, 1.0f}};
    // renderPassInfo.pClearValues = &clearColor;
    // renderPassInfo.clearValueCount = 1;

    std::array<VkClearValue, 2> clearValues = {}; /*  // 至少4个，因为最高索引是3 */
    clearValues[0].color = {{0.0f, 0.0f, 0.0f, 1.0f}};
    clearValues[1].depthStencil = {1.0f, 0}; /* // 深度=1.0f，模板=0 */
    renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
    renderPassInfo.pClearValues = clearValues.data();

    vkCmdBeginRenderPass(this->_renderTexture->getCommandBuffer(), &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
}
GfxRenderTexture *GfxQueueUI::getRenderTexture()
{
    return this->_renderTexture;
}

void GfxQueueUI::_clean()
{
}
void GfxQueueUI::_reset()
{
}
void GfxQueueUI::destroy()
{
    this->_renderer=nullptr;
    this->_renderTexture=nullptr;
    // 清空批次
    for (size_t i = 0; i < this->_batches.size(); i++)
    {
        GfxBatchUI *batch = this->_batches[i];
        batch->destroy();
        delete batch;
        batch = nullptr;
    }
    this->_batches.clear();
}

GfxQueueUI::~GfxQueueUI()
{
}

// void GfxQueueUI::_createVertexBuffers()
// {
//     std::vector<float> interleavedVertices = {
//         // 位置                uv
//         -1.0f, 1.0f, 0.0f, 0.0f, 0.0f,  /** @brief 左上 */
//         -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, /** @brief 坐下 */
//         1.0f, -1.0f, 0.0f, 1.0f, 1.0f,  /** @brief 右下 */
//         1.0f, 1.0f, 1.0f, 1.0f, 0.0f    /** @brief 右上 */
//     };
//     std::vector<uint32_t> indices = {
//         0, 1, 2,
//         0, 2, 3};
//     // 顶点缓冲区
//     GfxMgr::getInstance()->createBuffer(
//         VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
//         VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
//         &this->_vertexBuffer,
//         &this->_vertexMemory,
//         interleavedVertices.size() * sizeof(float), // 总字节数
//         interleavedVertices.data()                  // 数据指针
//     );

//     // 索引缓冲区（不变）
//     GfxMgr::getInstance()->createBuffer(
//         VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
//         VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
//         &this->_indexBuffer,
//         &this->_indexMemory,
//         indices.size() * sizeof(uint32_t),
//         indices.data());
//     std::cout << "Gfx : RenderQueue :: Created vertex buffer with " << interleavedVertices.size() << " vertices." << std::endl;
// }
// void GfxQueue::updateUniformBuffer(uint32_t frame, std::array<float, 16> &viewMat, std::array<float, 16> &projMat, float time)
// {
//     UniformBufferObject *ubo = (UniformBufferObject *)this->_uniformBuffersMapped[frame];
//     memcpy(ubo->viewMat, viewMat.data(), sizeof(float) * 16);
//     memcpy(ubo->projMat, projMat.data(), sizeof(float) * 16);
//     ubo->time = time;
// }

// void GfxQueue::_cleanUniformBuffers()
// {
//     for (size_t i = 0; i < this->_uniformBuffers.size(); i++)
//     {
//         if (this->_uniformBuffersMapped[i])
//         {
//             vkUnmapMemory(Gfx::context->getVkDevice(), this->_uniformBuffersMemory[i]);
//             this->_uniformBuffersMapped[i] = nullptr;
//         }
//         if (this->_uniformBuffers[i] != VK_NULL_HANDLE)
//         {
//             vkDestroyBuffer(Gfx::context->getVkDevice(), this->_uniformBuffers[i], nullptr);
//             this->_uniformBuffers[i] = VK_NULL_HANDLE;
//         }
//         if (this->_uniformBuffersMemory[i] != VK_NULL_HANDLE)
//         {
//             vkFreeMemory(Gfx::context->getVkDevice(), this->_uniformBuffersMemory[i], nullptr);
//             this->_uniformBuffersMemory[i] = VK_NULL_HANDLE;
//         }
//     }
//     this->_uniformBuffers.clear();
//     this->_uniformBuffersMemory.clear();
//     this->_uniformBuffersMapped.clear();
// }

// GfxQueue::GfxQueue(std::string name, GfxContext *context)
// {
//     this->_name = name;
//     this->_context = context;
// }
// void GfxQueue::create(GfxPass *pass)
// {
//     this->_pass = pass;
//     this->_createBuffers();
// }
// /**
//  * 创建帧缓冲区和命令缓冲区
//  */
// void GfxQueue::_createBuffers()
// {
//     this->_createTextures();
//     this->_createFramebuffers();
//     this->_createCommandBuffers();
// }

// void GfxQueue::_createTextures()
// {
//     float width = (float)this->_context->getSwapChainExtent().width;
//     float height = (float)this->_context->getSwapChainExtent().height;
//     GfxPassStruct &passStruct = this->_pass->getGfxPassStruct();

//     // 创建深度纹理
//     if (passStruct.depthAttachment.enable)
//     {

//         VkFormat depthFormat = this->_pass->getAttachmentFormat(passStruct.depthAttachment.format);
//         this->_depthTexture = new GfxTexture(this->_context);
//         this->_depthTexture->createImage(width, height, depthFormat, VK_IMAGE_TILING_OPTIMAL,
//                                          VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
//                                          VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, MsaaSamples);
//         this->_depthTexture->createImageView(depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT);
//     }

//     // if (passStruct.msaa && passStruct.offscreen)
//     // {
//     //     VkFormat colorFormat = this->_pass->getAttachmentFormat(passStruct.colorAttachment.format);
//     //     this->_colorTexture = new GfxTexture(this->_context);
//     //     this->_colorTexture->createImage(width, height, colorFormat, VK_IMAGE_TILING_OPTIMAL,
//     //                                      VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
//     //                                      VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, MsaaSamples);
//     //     this->_colorTexture->createImageView(colorFormat, VK_IMAGE_ASPECT_COLOR_BIT);
//     // }
//     // else if (passStruct.offscreen)
//     // {
//     //     // VkFormat colorFormat = this->_pass->getAttachmentFormat(passStruct.colorAttachment.format);
//     //     // this->_colorTexture = new GfxTexture(this->_context);
//     //     // this->_colorTexture->createImage(width, height, colorFormat, VK_IMAGE_TILING_OPTIMAL,
//     //     //                                  VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
//     //     //                                  VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, MsaaSamples);
//     //     // std::cout << "this->_colorMsaaTexture createImage end" << std::endl;
//     //     // this->_colorTexture->createImageView(colorFormat, VK_IMAGE_ASPECT_COLOR_BIT);
//     // }
//     // // if(passStruct.offscreen){
//     // // }
//     // // viewport.width = (float)this->_context->getSwapChainExtent().width;
//     // // viewport.height = (float)this->_context->getSwapChainExtent().height;
//     // // this->_colorTexture = new GfxTexture(this->_context);
//     // // this->_depthTexture = new GfxTexture(this->_context);
// }

// void GfxQueue::_createFramebuffers()
// {
//     this->_queueFramebuffers.clear();
//     std::vector<VkImageView> &swapChainImageViews = this->_context->getSwapChainImageViews();
//     this->_queueFramebuffers.resize(swapChainImageViews.size());
//     GfxPassStruct &passStruct = this->_pass->getGfxPassStruct();

//     for (size_t i = 0; i < swapChainImageViews.size(); i++)
//     {
//         std::vector<VkImageView> attachments = {};
//         attachments.push_back(swapChainImageViews[i]);
//         attachments.push_back(this->_depthTexture->getImageView());
//         // 多重采样
//         // std::array<VkImageView, 3> attachments = {
//         //     this->_context->getColorMsaaTexture()->getImageView(),/*  // 多重采样颜色附件 */
//         //     swapChainImageViews[i],                                /* // 解析附件（单采样，指向交换链图像） */
//         //     this->_context->getDepthMsaaTexture()->getImageView() /*  // 多重采样深度附件 */
//         // };
//         VkFramebufferCreateInfo framebufferInfo = {};
//         framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
//         framebufferInfo.renderPass = this->_pass->getVkRenderPass();

//         framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size()); /* // 指定附着的个数 */
//         framebufferInfo.pAttachments = attachments.data();                           /*  // 渲染流程对象用于描述附着信息的pAttachment数组 */
//         // std::cout << "framebufferInfo.attachmentCount = " << framebufferInfo.attachmentCount << std::endl;
//         framebufferInfo.width = this->_context->getSwapChainExtent().width;   /*  // width和height用于指定帧缓冲的大小 */
//         framebufferInfo.height = this->_context->getSwapChainExtent().height; /* // 交换链图像都是单层，layers设置为1 */
//         framebufferInfo.layers = 1;
//         if (vkCreateFramebuffer(Gfx::context->getVkDevice(), &framebufferInfo, nullptr, &this->_queueFramebuffers[i]) != VK_SUCCESS)
//         {
//             throw std::runtime_error("Failed to create framebuffer!");
//         }
//     }
// }

// void GfxQueue::_createCommandBuffers()
// {
//     this->_queueCommandBuffers.resize(this->_queueFramebuffers.size());
//     VkCommandBufferAllocateInfo allocInfo{};
//     allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
//     allocInfo.commandPool = this->_context->getCommandPool();
//     allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
//     allocInfo.commandBufferCount = (uint32_t)this->_queueCommandBuffers.size();

//     if (vkAllocateCommandBuffers(Gfx::context->getVkDevice(), &allocInfo, this->_queueCommandBuffers.data()) != VK_SUCCESS)
//     {
//         throw std::runtime_error("Failed to allocate command buffers!");
//     }
// }

// void GfxQueue::submit(GfxObject *object)
// {
//     GfxPipeline *pipeline = object->getPipeline();
//     if (pipeline == nullptr)
//         return;
//     if (pipeline->isTransparent())
//     {
//         this->_queueTransparentQueue.push_back(object);
//     }
//     else
//     {
//         this->_queueOpaqueQueue.push_back(object);
//     }
// }
// void GfxQueue::render(uint32_t imageIndex, std::vector<VkCommandBuffer> &commandBuffers)
// {
//     // std::cout << "render:queue" << std::endl;
//     if (this->_queueCommandBuffers.empty())
//     {
//         return;
//     }
//     if (imageIndex >= this->_queueCommandBuffers.size())
//         return;

//     VkCommandBufferBeginInfo beginInfo{};
//     beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
//     if (vkBeginCommandBuffer(this->_queueCommandBuffers[imageIndex], &beginInfo) != VK_SUCCESS)
//     {
//         throw std::runtime_error("Failed to begin recording command buffer!");
//     }
//     this->_stencilRef = 0;
//     /*  // 绑定render pass */
//     this->_beginBindRenderPass(imageIndex);
//     // // std::cout << "render:queue pass" << std::endl;
//     // /* // 更新渲染物体 */
//     // for (auto object : this->_opaqueQueue)
//     // {
//     //     // std::cout << "render:opaque" << std::endl;
//     //     object->render(imageIndex, this->_commandBuffers);
//     // }s
//     // VkCommandBuffer &commandBuffer = this->_commandBuffers[imageIndex];
//     for (auto *object : this->_queueTransparentQueue)
//     {
//         if (object != nullptr)
//         {
//             this->_renderObject(imageIndex, object);
//         }
//     }
//     // for (auto *object : this->_transparentQueue)
//     // {
//     //     if (object->getType() == GfxObjectType::UI_MASK)
//     //     {
//     //         if (object->getUIMaskBehavior() == 1)
//     //         {
//     //             this->_stencilRef++;
//     //         }
//     //         else
//     //         {
//     //             this->_stencilRef--;
//     //         }
//     //         vkCmdSetStencilReference(commandBuffer, VK_STENCIL_FACE_FRONT_AND_BACK, this->_stencilRef);  //增加和减少每次都是固定值1
//     //         vkCmdSetStencilCompareMask(commandBuffer, VK_STENCIL_FACE_FRONT_AND_BACK, 0xFF); // 比较所有位
//     //         vkCmdSetStencilWriteMask(commandBuffer, VK_STENCIL_FACE_FRONT_AND_BACK, 0xFF);   // 写入所有位
//     //     }
//     //     else
//     //     {
//     //         vkCmdSetStencilReference(commandBuffer, VK_STENCIL_FACE_FRONT_AND_BACK, this->_stencilRef);
//     //         vkCmdSetStencilCompareMask(commandBuffer, VK_STENCIL_FACE_FRONT_AND_BACK, 0xFF); // 比较所有位
//     //         vkCmdSetStencilWriteMask(commandBuffer, VK_STENCIL_FACE_FRONT_AND_BACK, 0x00);   // 不写入模板（保持遮罩）
//     //     }
//     //     object->render(imageIndex, this->_commandBuffers);
//     // }

//     /*  // 渲染结束 */
//     vkCmdEndRenderPass(this->_queueCommandBuffers[imageIndex]);
//     /*  // 结束渲染pass */
//     if (vkEndCommandBuffer(this->_queueCommandBuffers[imageIndex]) == VK_SUCCESS)
//     {
//         commandBuffers.push_back(this->_queueCommandBuffers[imageIndex]);
//     }
//     else
//     {
//         // throw std::runtime_error("Failed to end recording command buffer!");
//         std::cout << "GfxQueue render:Failed to end recording command buffer!" << std::endl;
//     }

//     this->_queueOpaqueQueue.clear();
//     this->_queueTransparentQueue.clear();
// }
// /**
//  * 绑定render pass
//  */
// void GfxQueue::_beginBindRenderPass(uint32_t imageIndex)
// {
//     /*  // 绑定render pass */
//     VkRenderPassBeginInfo renderPassInfo{};
//     renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
//     renderPassInfo.renderPass = this->_pass->getVkRenderPass();
//     renderPassInfo.framebuffer = this->_queueFramebuffers[imageIndex];
//     renderPassInfo.renderArea.offset = {0, 0};
//     renderPassInfo.renderArea.extent = this->_context->getSwapChainExtent();

//     std::array<VkClearValue, 2> clearValues = {}; /*  // 至少4个，因为最高索引是3 */
//     clearValues[0].color = {{0.0f, 0.0f, 0.0f, 1.0f}};
//     clearValues[1].depthStencil = {1.0f, 0}; /* // 深度=1.0f，模板=0 */
//     renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
//     renderPassInfo.pClearValues = clearValues.data();
//     vkCmdBeginRenderPass(this->_queueCommandBuffers[imageIndex], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
// }
// void GfxQueue::_renderObject(uint32_t imageIndex, GfxObject *object)
// {
//     // std::cout << "GfxQueue : _renderObject object " << object << std::endl;
//     // 检测渲染管线是否发生了变换
//     GfxPipeline *pipeline = object->getPipeline();
//     GfxTexture *texture = object->getTexture();
//     // std::cout << "GfxQueue : _renderObject object " << object->getUuid() << " type " << static_cast<int>(object->getType()) << static_cast<int>(GfxObjectType::UI_MASK) << " UIMaskBehavior " << object->getUIMaskBehavior() << std::endl;
//     if (pipeline == nullptr)
//     {
//         return;
//     }
//     // 更新新的渲染管线
//     // if (this->_rendererStatus.pipelineName != pipeline->getName())
//     // {
//     //     this->_rendererStatus.pipelineName = pipeline->getName();
//     //     std::cout << "GfxQueue render:bind pipeline " << pipeline->getName() << std::endl;
//     //     vkCmdBindPipeline(this->_queueCommandBuffers[imageIndex], VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->getVKPipeline());
//     // }else{
//     //     std::cout << "GfxQueue render:use pipeline " << pipeline->getName() << std::endl;
//     // }
//     vkCmdBindPipeline(this->_queueCommandBuffers[imageIndex], VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->getVKPipeline());
//     if (object->getType() == GfxObjectType::UI_MASK)
//     {
//         if (object->getUIMaskBehavior() == 1)
//         {
//             this->_stencilRef++;
//         }
//         else
//         {
//             this->_stencilRef--;
//         }
//         // std::cout << "GfxQueue : _renderObject UIMask " << object->getUuid() << " _stencilRef1  " << this->_stencilRef << std::endl;
//         vkCmdSetStencilReference(this->_queueCommandBuffers[imageIndex], VK_STENCIL_FACE_FRONT_AND_BACK, 1);      // 增加和减少每次都是固定值1
//         vkCmdSetStencilCompareMask(this->_queueCommandBuffers[imageIndex], VK_STENCIL_FACE_FRONT_AND_BACK, 0xFF); // 比较所有位
//         vkCmdSetStencilWriteMask(this->_queueCommandBuffers[imageIndex], VK_STENCIL_FACE_FRONT_AND_BACK, 0xFF);   // 写入所有位
//         // std::cout << "GfxQueue : _renderObject UIMask " << object->getUuid() << " _stencilRef2   " << this->_stencilRef << std::endl;
//     }
//     else
//     {
//         // std::cout << "GfxQueue : _renderObject UI  _stencilRef" << this->_stencilRef << std::endl;
//         vkCmdSetStencilReference(this->_queueCommandBuffers[imageIndex], VK_STENCIL_FACE_FRONT_AND_BACK, this->_stencilRef);
//         vkCmdSetStencilCompareMask(this->_queueCommandBuffers[imageIndex], VK_STENCIL_FACE_FRONT_AND_BACK, 0xFF); // 比较所有位
//         vkCmdSetStencilWriteMask(this->_queueCommandBuffers[imageIndex], VK_STENCIL_FACE_FRONT_AND_BACK, 0x00);   // 不写入模板（保持遮罩）
//     }
//     object->render(imageIndex, this->_queueCommandBuffers);
//     // std::cout << "GfxQueue : _renderObject object end " << object->getUuid() << std::endl;

//     // object->render(imageIndex, this->_queueCommandBuffers);
//     //     if (object->getType() == GfxObjectType::UI_MASK)
//     //     {
//     //         if (object->getUIMaskBehavior() == 1)
//     //         {
//     //             this->_stencilRef++;
//     //         }
//     //         else
//     //         {
//     //             this->_stencilRef--;
//     //         }
//     //         vkCmdSetStencilReference(commandBuffer, VK_STENCIL_FACE_FRONT_AND_BACK, this->_stencilRef);  //增加和减少每次都是固定值1
//     //         vkCmdSetStencilCompareMask(commandBuffer, VK_STENCIL_FACE_FRONT_AND_BACK, 0xFF); // 比较所有位
//     //         vkCmdSetStencilWriteMask(commandBuffer, VK_STENCIL_FACE_FRONT_AND_BACK, 0xFF);   // 写入所有位
//     //     }
//     //     else
//     //     {
//     //         vkCmdSetStencilReference(commandBuffer, VK_STENCIL_FACE_FRONT_AND_BACK, this->_stencilRef);
//     //         vkCmdSetStencilCompareMask(commandBuffer, VK_STENCIL_FACE_FRONT_AND_BACK, 0xFF); // 比较所有位
//     //         vkCmdSetStencilWriteMask(commandBuffer, VK_STENCIL_FACE_FRONT_AND_BACK, 0x00);   // 不写入模板（保持遮罩）
//     //     }
//     //     object->render(imageIndex, this->_commandBuffers);
// }

// void GfxQueue::clear()
// {
//     this->_clearTextures();
//     this->_cleanFramebuffers();
//     this->_cleanCommandBuffers();
// }
// void GfxQueue::_clearTextures()
// {
//     if (this->_depthTexture != nullptr)
//     {
//         delete this->_depthTexture;
//         this->_depthTexture = nullptr;
//     }
// }
// void GfxQueue::_cleanFramebuffers()
// {
//     /*  // 销毁帧缓冲（Framebuffers） */
//     for (auto framebuffer : this->_queueFramebuffers)
//     {
//         vkDestroyFramebuffer(Gfx::context->getVkDevice(), framebuffer, nullptr);
//     }
//     this->_queueFramebuffers.clear();
// }
// void GfxQueue::_cleanCommandBuffers()
// {
//     /* // 销毁命令缓冲（Command Buffers） */
//     if (!this->_queueCommandBuffers.empty())

//     {
//         vkFreeCommandBuffers(Gfx::context->getVkDevice(), this->_context->getCommandPool(), static_cast<uint32_t>(_queueCommandBuffers.size()), _queueCommandBuffers.data());
//         this->_queueCommandBuffers.clear();
//     }
// }
// void GfxQueue::reset()
// {
//     this->_createBuffers();
// }

// GfxQueue::~GfxQueue()
// {
// }

// #include "gfx-render-queue.h"
// #include "../../gfx.h"
// #include "../../gfx-context.h"
// #include "../../gfx-shader.h"
// #include "../../gfx-shader-struct.h"
// #include "../../gfx-texture.h"
// #include "../../../math/mat4.h"
// #include "gfx-render-batch.h"

// GfxRenderQueue::GfxRenderQueue()
// {
//     this->_renderTexture = nullptr;
//     this->_viewMat = {};
//     this->_projMat = {};
// }
// void GfxRenderQueue::init(GfxRenderTexture *renderTexture)
// {
//     this->_renderTexture = renderTexture;
//     if (this->_renderTexture == nullptr)
//     {
//         this->_createBuffers();
//     }
// }
// void GfxRenderQueue::_createBuffers()
// {
//     this->_createFramebuffers();
//     this->_createCommandBuffers();
// }
// void GfxRenderQueue::_createFramebuffers()
// {
//     GfxPass *pass = Gfx::renderer->getPass("scene");
//     if (pass == nullptr)
//     {
//         throw std::runtime_error("GfxRenderBatch::_createFramebuffers() pass not found!");
//     }
//     this->_framebuffers.clear();
//     std::vector<VkImageView> &swapChainImageViews = Gfx::context->getSwapChainImageViews();

//     this->_framebuffers.resize(swapChainImageViews.size());
//     for (size_t i = 0; i < swapChainImageViews.size(); i++)
//     {
//         VkImageView attachments[] = {swapChainImageViews[i]};
//         VkFramebufferCreateInfo framebufferInfo = {};
//         framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
//         framebufferInfo.renderPass = pass->vkRenderPass();
//         framebufferInfo.attachmentCount = 1;                                // 指定附着的个数
//         framebufferInfo.pAttachments = attachments;                         // 渲染流程对象用于描述附着信息的pAttachment数组
//         framebufferInfo.width = Gfx::context->getSwapChainExtent().width;   // width和height用于指定帧缓冲的大小
//         framebufferInfo.height = Gfx::context->getSwapChainExtent().height; // 交换链图像都是单层，layers设置为1
//         framebufferInfo.layers = 1;
//         if (vkCreateFramebuffer(Gfx::context->getVkDevice(), &framebufferInfo, nullptr, &this->_framebuffers[i]) != VK_SUCCESS)
//         {
//             throw std::runtime_error("Gfx : RenderQueue :: Failed to create framebuffer!");
//         }
//     }
// }
// void GfxRenderQueue::_createCommandBuffers()
// {
//     this->_commandBuffers.resize(this->_framebuffers.size());
//     VkCommandBufferAllocateInfo allocInfo{};
//     allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
//     allocInfo.commandPool = Gfx::context->getCommandPool();
//     allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
//     allocInfo.commandBufferCount = (uint32_t)this->_commandBuffers.size();

//     if (vkAllocateCommandBuffers(Gfx::context->getVkDevice(), &allocInfo, this->_commandBuffers.data()) != VK_SUCCESS)
//     {
//         throw std::runtime_error("Gfx : RenderQueue :: Failed to allocate command buffers!");
//     }
// }

// void GfxRenderQueue::submitObject(const GfxMaterial &material, const GfxMesh &mesh)
// {
//     if (this->_batches.empty())
//     {
//         GfxRenderBatch *batch = new GfxRenderBatch(material, mesh);
//         this->_batches.push_back(batch);
//         batch->addObject();
//     }
//     else
//     {
//         GfxRenderBatch *batch = this->_batches.back();
//         if (batch->material().uuid == material.uuid && batch->mesh().uuid == mesh.uuid)
//         {
//             // 后续还要判断材质中的push constant是否一致和纹理是否一致
//             batch->addObject();
//         }
//         else
//         {
//             GfxRenderBatch *batch = new GfxRenderBatch(material, mesh);
//             this->_batches.push_back(batch);
//             batch->addObject();
//         }
//     }
// }
// void GfxRenderQueue::render(uint32_t imageIndex, std::vector<VkCommandBuffer> &commandBuffers)
// {
//     GfxPipeline *pipeline = nullptr;
//     VkCommandBuffer queueCommandBuffer = VK_NULL_HANDLE;

//     this->_resetCommandBuffer(imageIndex);
//     this->_beginCommandBuffer(imageIndex);
//     this->_beginRenderPass(imageIndex);
//     if (this->_renderTexture == nullptr)
//     {
//         queueCommandBuffer = this->_commandBuffers[imageIndex];
//     }
//     else
//     {
//         queueCommandBuffer = this->_renderTexture->getCommandBuffer();
//     }
//     for (auto &batch : this->_batches)
//     {
//         if (pipeline == nullptr || pipeline->getName() != batch->material().pipeline)
//         {
//             pipeline = Gfx::renderer->getPipeline(batch->material().pipeline);
//             this->_bindPipeline(imageIndex, pipeline);
//         }
//         VkDescriptorSet descriptorSet = VK_NULL_HANDLE;
//         if (this->_renderTexture == nullptr)
//         {
//             descriptorSet = pipeline->getDescriptorSets()[0];
//         }
//         else
//         {
//             descriptorSet = pipeline->getDescriptorSets()[imageIndex];
//         }
//         batch->render(queueCommandBuffer, descriptorSet);
//     }
//     commandBuffers.push_back(queueCommandBuffer);

//     // this->_testBatch->render(this->_renderTexture, imageIndex, commandBuffers);
//     // this->_renderTexture->saveToFile1("test.png");
//     // std::cout << "GfxRenderQueue::render() imageIndex:" << imageIndex << " batches.size:" << this->_batches.size() << std::endl;
//     // for (auto &batch : this->_batches)
//     // {
//     //     batch->render(imageIndex, commandBuffers);
//     // }

//     // //// 渲染完成后清空批次，避免每次渲染都累积批次
//     // //for (auto batch : this->_batches)
//     // //{
//     // //    delete batch;
//     // //}
//     // this->_batches.clear();
// }
// void GfxRenderQueue::_resetCommandBuffer(uint32_t imageIndex)
// {
//     if (this->_renderTexture == nullptr)
//     {
//         vkResetCommandBuffer(this->_commandBuffers[imageIndex], 0);
//     }
//     else
//     {
//         vkResetCommandBuffer(this->_renderTexture->getCommandBuffer(), 0);
//     }
// }
// void GfxRenderQueue::_beginCommandBuffer(uint32_t imageIndex)
// {
//     VkCommandBufferBeginInfo beginInfo{};
//     beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
//     if (this->_renderTexture == nullptr)
//     {
//         if (vkBeginCommandBuffer(this->_commandBuffers[imageIndex], &beginInfo) != VK_SUCCESS)
//         {
//             throw std::runtime_error("Failed to begin recording command buffer!");
//         }
//     }
//     else
//     {
//         if (vkBeginCommandBuffer(this->_renderTexture->getCommandBuffer(), &beginInfo) != VK_SUCCESS)
//         {
//             throw std::runtime_error("Failed to begin recording command buffer!");
//         }
//     }
// }
// void GfxRenderQueue::_beginRenderPass(uint32_t imageIndex)
// {
//     VkRenderPassBeginInfo renderPassInfo{};
//     renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
//     GfxPass *pass = nullptr;
//     if (this->_renderTexture == nullptr)
//     {
//         pass = Gfx::renderer->getPass("scene");
//         renderPassInfo.framebuffer = this->_framebuffers[imageIndex];
//         renderPassInfo.renderArea.extent = Gfx::context->getSwapChainExtent();
//     }
//     else
//     {
//         pass = Gfx::renderer->getPass("default");
//         renderPassInfo.framebuffer = this->_renderTexture->getFramebuffer();
//         renderPassInfo.renderArea.extent = {this->_renderTexture->getWidth(), this->_renderTexture->getHeight()};
//     }
//     renderPassInfo.renderPass = pass->vkRenderPass();
//     renderPassInfo.renderArea.offset = {0, 0};
//     VkClearValue clearColor{};
//     renderPassInfo.clearValueCount = 1;
//     clearColor.color = {{0.0f, 0.0f, 0.0f, 0.0f}};
//     renderPassInfo.pClearValues = &clearColor;
//     if (this->_renderTexture == nullptr)
//     {
//         vkCmdBeginRenderPass(this->_commandBuffers[imageIndex], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
//     }
//     else
//     {
//         vkCmdBeginRenderPass(this->_renderTexture->getCommandBuffer(), &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
//     }
// }
// void GfxRenderQueue::_bindPipeline(uint32_t imageIndex, GfxPipeline *pipeline)
// {
//     if (this->_renderTexture == nullptr)
//     {
//         vkCmdBindPipeline(this->_commandBuffers[imageIndex], VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->vkPipeline());
//     }
//     else
//     {
//         vkCmdBindPipeline(this->_renderTexture->getCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->vkPipeline());
//     }
// }

// void GfxRenderQueue::_clear()
// {
//     if (this->_renderTexture != nullptr)
//     {
//         this->_renderTexture->_reset();
//     }
//     else
//     {
//         // 销毁帧缓冲（Framebuffers）
//         for (auto framebuffer : this->_framebuffers)
//         {
//             vkDestroyFramebuffer(Gfx::context->getVkDevice(), framebuffer, nullptr);
//         }
//         this->_framebuffers.clear();
//         // 销毁命令缓冲（Command Buffers）
//         if (!this->_commandBuffers.empty())
//         {
//             vkFreeCommandBuffers(Gfx::context->getVkDevice(), Gfx::context->getCommandPool(), static_cast<uint32_t>(_commandBuffers.size()), _commandBuffers.data());
//             this->_commandBuffers.clear();
//         }
//     }
// }
// void GfxRenderQueue::_reset()
// {
//     if (this->_renderTexture != nullptr)
//     {
//         this->_renderTexture->_reset();
//     }
//     else
//     {
//         this->_createBuffers();
//     }
// }

// GfxRenderQueue::~GfxRenderQueue()
// {
// }

// // void GfxQueue::updateUniformBuffer(uint32_t frame, std::array<float, 16> &viewMat, std::array<float, 16> &projMat, float time)
// // {
// //     UniformBufferObject *ubo = (UniformBufferObject *)this->_uniformBuffersMapped[frame];
// //     memcpy(ubo->viewMat, viewMat.data(), sizeof(float) * 16);
// //     memcpy(ubo->projMat, projMat.data(), sizeof(float) * 16);
// //     ubo->time = time;
// // }

// // void GfxQueue::_cleanUniformBuffers()
// // {
// //     for (size_t i = 0; i < this->_uniformBuffers.size(); i++)
// //     {
// //         if (this->_uniformBuffersMapped[i])
// //         {
// //             vkUnmapMemory(Gfx::context->getVkDevice(), this->_uniformBuffersMemory[i]);
// //             this->_uniformBuffersMapped[i] = nullptr;
// //         }
// //         if (this->_uniformBuffers[i] != VK_NULL_HANDLE)
// //         {
// //             vkDestroyBuffer(Gfx::context->getVkDevice(), this->_uniformBuffers[i], nullptr);
// //             this->_uniformBuffers[i] = VK_NULL_HANDLE;
// //         }
// //         if (this->_uniformBuffersMemory[i] != VK_NULL_HANDLE)
// //         {
// //             vkFreeMemory(Gfx::context->getVkDevice(), this->_uniformBuffersMemory[i], nullptr);
// //             this->_uniformBuffersMemory[i] = VK_NULL_HANDLE;
// //         }
// //     }
// //     this->_uniformBuffers.clear();
// //     this->_uniformBuffersMemory.clear();
// //     this->_uniformBuffersMapped.clear();
// // }

// // GfxQueue::GfxQueue(std::string name, GfxContext *context)
// // {
// //     this->_name = name;
// //     this->_context = context;
// // }
// // void GfxQueue::create(GfxPass *pass)
// // {
// //     this->_pass = pass;
// //     this->_createBuffers();
// // }
// // /**
// //  * 创建帧缓冲区和命令缓冲区
// //  */
// // void GfxQueue::_createBuffers()
// // {
// //     this->_createTextures();
// //     this->_createFramebuffers();
// //     this->_createCommandBuffers();
// // }

// // void GfxQueue::_createTextures()
// // {
// //     float width = (float)this->_context->getSwapChainExtent().width;
// //     float height = (float)this->_context->getSwapChainExtent().height;
// //     GfxPassStruct &passStruct = this->_pass->getGfxPassStruct();

// //     // 创建深度纹理
// //     if (passStruct.depthAttachment.enable)
// //     {

// //         VkFormat depthFormat = this->_pass->getAttachmentFormat(passStruct.depthAttachment.format);
// //         this->_depthTexture = new GfxTexture(this->_context);
// //         this->_depthTexture->createImage(width, height, depthFormat, VK_IMAGE_TILING_OPTIMAL,
// //                                          VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
// //                                          VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, MsaaSamples);
// //         this->_depthTexture->createImageView(depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT);
// //     }

// //     // if (passStruct.msaa && passStruct.offscreen)
// //     // {
// //     //     VkFormat colorFormat = this->_pass->getAttachmentFormat(passStruct.colorAttachment.format);
// //     //     this->_colorTexture = new GfxTexture(this->_context);
// //     //     this->_colorTexture->createImage(width, height, colorFormat, VK_IMAGE_TILING_OPTIMAL,
// //     //                                      VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
// //     //                                      VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, MsaaSamples);
// //     //     this->_colorTexture->createImageView(colorFormat, VK_IMAGE_ASPECT_COLOR_BIT);
// //     // }
// //     // else if (passStruct.offscreen)
// //     // {
// //     //     // VkFormat colorFormat = this->_pass->getAttachmentFormat(passStruct.colorAttachment.format);
// //     //     // this->_colorTexture = new GfxTexture(this->_context);
// //     //     // this->_colorTexture->createImage(width, height, colorFormat, VK_IMAGE_TILING_OPTIMAL,
// //     //     //                                  VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
// //     //     //                                  VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, MsaaSamples);
// //     //     // std::cout << "this->_colorMsaaTexture createImage end" << std::endl;
// //     //     // this->_colorTexture->createImageView(colorFormat, VK_IMAGE_ASPECT_COLOR_BIT);
// //     // }
// //     // // if(passStruct.offscreen){
// //     // // }
// //     // // viewport.width = (float)this->_context->getSwapChainExtent().width;
// //     // // viewport.height = (float)this->_context->getSwapChainExtent().height;
// //     // // this->_colorTexture = new GfxTexture(this->_context);
// //     // // this->_depthTexture = new GfxTexture(this->_context);
// // }

// // void GfxQueue::_createFramebuffers()
// // {
// //     this->_queueFramebuffers.clear();
// //     std::vector<VkImageView> &swapChainImageViews = this->_context->getSwapChainImageViews();
// //     this->_queueFramebuffers.resize(swapChainImageViews.size());
// //     GfxPassStruct &passStruct = this->_pass->getGfxPassStruct();

// //     for (size_t i = 0; i < swapChainImageViews.size(); i++)
// //     {
// //         std::vector<VkImageView> attachments = {};
// //         attachments.push_back(swapChainImageViews[i]);
// //         attachments.push_back(this->_depthTexture->getImageView());
// //         // 多重采样
// //         // std::array<VkImageView, 3> attachments = {
// //         //     this->_context->getColorMsaaTexture()->getImageView(),/*  // 多重采样颜色附件 */
// //         //     swapChainImageViews[i],                                /* // 解析附件（单采样，指向交换链图像） */
// //         //     this->_context->getDepthMsaaTexture()->getImageView() /*  // 多重采样深度附件 */
// //         // };
// //         VkFramebufferCreateInfo framebufferInfo = {};
// //         framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
// //         framebufferInfo.renderPass = this->_pass->getVkRenderPass();

// //         framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size()); /* // 指定附着的个数 */
// //         framebufferInfo.pAttachments = attachments.data();                           /*  // 渲染流程对象用于描述附着信息的pAttachment数组 */
// //         // std::cout << "framebufferInfo.attachmentCount = " << framebufferInfo.attachmentCount << std::endl;
// //         framebufferInfo.width = this->_context->getSwapChainExtent().width;   /*  // width和height用于指定帧缓冲的大小 */
// //         framebufferInfo.height = this->_context->getSwapChainExtent().height; /* // 交换链图像都是单层，layers设置为1 */
// //         framebufferInfo.layers = 1;
// //         if (vkCreateFramebuffer(Gfx::context->getVkDevice(), &framebufferInfo, nullptr, &this->_queueFramebuffers[i]) != VK_SUCCESS)
// //         {
// //             throw std::runtime_error("Failed to create framebuffer!");
// //         }
// //     }
// // }

// // void GfxQueue::_createCommandBuffers()
// // {
// //     this->_queueCommandBuffers.resize(this->_queueFramebuffers.size());
// //     VkCommandBufferAllocateInfo allocInfo{};
// //     allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
// //     allocInfo.commandPool = this->_context->getCommandPool();
// //     allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
// //     allocInfo.commandBufferCount = (uint32_t)this->_queueCommandBuffers.size();

// //     if (vkAllocateCommandBuffers(Gfx::context->getVkDevice(), &allocInfo, this->_queueCommandBuffers.data()) != VK_SUCCESS)
// //     {
// //         throw std::runtime_error("Failed to allocate command buffers!");
// //     }
// // }

// // void GfxQueue::submit(GfxObject *object)
// // {
// //     GfxPipeline *pipeline = object->getPipeline();
// //     if (pipeline == nullptr)
// //         return;
// //     if (pipeline->isTransparent())
// //     {
// //         this->_queueTransparentQueue.push_back(object);
// //     }
// //     else
// //     {
// //         this->_queueOpaqueQueue.push_back(object);
// //     }
// // }
// // void GfxQueue::render(uint32_t imageIndex, std::vector<VkCommandBuffer> &commandBuffers)
// // {
// //     // std::cout << "render:queue" << std::endl;
// //     if (this->_queueCommandBuffers.empty())
// //     {
// //         return;
// //     }
// //     if (imageIndex >= this->_queueCommandBuffers.size())
// //         return;

// //     VkCommandBufferBeginInfo beginInfo{};
// //     beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
// //     if (vkBeginCommandBuffer(this->_queueCommandBuffers[imageIndex], &beginInfo) != VK_SUCCESS)
// //     {
// //         throw std::runtime_error("Failed to begin recording command buffer!");
// //     }
// //     this->_stencilRef = 0;
// //     /*  // 绑定render pass */
// //     this->_beginBindRenderPass(imageIndex);
// //     // // std::cout << "render:queue pass" << std::endl;
// //     // /* // 更新渲染物体 */
// //     // for (auto object : this->_opaqueQueue)
// //     // {
// //     //     // std::cout << "render:opaque" << std::endl;
// //     //     object->render(imageIndex, this->_commandBuffers);
// //     // }s
// //     // VkCommandBuffer &commandBuffer = this->_commandBuffers[imageIndex];
// //     for (auto *object : this->_queueTransparentQueue)
// //     {
// //         if (object != nullptr)
// //         {
// //             this->_renderObject(imageIndex, object);
// //         }
// //     }
// //     // for (auto *object : this->_transparentQueue)
// //     // {
// //     //     if (object->getType() == GfxObjectType::UI_MASK)
// //     //     {
// //     //         if (object->getUIMaskBehavior() == 1)
// //     //         {
// //     //             this->_stencilRef++;
// //     //         }
// //     //         else
// //     //         {
// //     //             this->_stencilRef--;
// //     //         }
// //     //         vkCmdSetStencilReference(commandBuffer, VK_STENCIL_FACE_FRONT_AND_BACK, this->_stencilRef);  //增加和减少每次都是固定值1
// //     //         vkCmdSetStencilCompareMask(commandBuffer, VK_STENCIL_FACE_FRONT_AND_BACK, 0xFF); // 比较所有位
// //     //         vkCmdSetStencilWriteMask(commandBuffer, VK_STENCIL_FACE_FRONT_AND_BACK, 0xFF);   // 写入所有位
// //     //     }
// //     //     else
// //     //     {
// //     //         vkCmdSetStencilReference(commandBuffer, VK_STENCIL_FACE_FRONT_AND_BACK, this->_stencilRef);
// //     //         vkCmdSetStencilCompareMask(commandBuffer, VK_STENCIL_FACE_FRONT_AND_BACK, 0xFF); // 比较所有位
// //     //         vkCmdSetStencilWriteMask(commandBuffer, VK_STENCIL_FACE_FRONT_AND_BACK, 0x00);   // 不写入模板（保持遮罩）
// //     //     }
// //     //     object->render(imageIndex, this->_commandBuffers);
// //     // }

// //     /*  // 渲染结束 */
// //     vkCmdEndRenderPass(this->_queueCommandBuffers[imageIndex]);
// //     /*  // 结束渲染pass */
// //     if (vkEndCommandBuffer(this->_queueCommandBuffers[imageIndex]) == VK_SUCCESS)
// //     {
// //         commandBuffers.push_back(this->_queueCommandBuffers[imageIndex]);
// //     }
// //     else
// //     {
// //         // throw std::runtime_error("Failed to end recording command buffer!");
// //         std::cout << "GfxQueue render:Failed to end recording command buffer!" << std::endl;
// //     }

// //     this->_queueOpaqueQueue.clear();
// //     this->_queueTransparentQueue.clear();
// // }
// // /**
// //  * 绑定render pass
// //  */
// // void GfxQueue::_beginBindRenderPass(uint32_t imageIndex)
// // {
// //     /*  // 绑定render pass */
// //     VkRenderPassBeginInfo renderPassInfo{};
// //     renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
// //     renderPassInfo.renderPass = this->_pass->getVkRenderPass();
// //     renderPassInfo.framebuffer = this->_queueFramebuffers[imageIndex];
// //     renderPassInfo.renderArea.offset = {0, 0};
// //     renderPassInfo.renderArea.extent = this->_context->getSwapChainExtent();

// //     std::array<VkClearValue, 2> clearValues = {}; /*  // 至少4个，因为最高索引是3 */
// //     clearValues[0].color = {{0.0f, 0.0f, 0.0f, 1.0f}};
// //     clearValues[1].depthStencil = {1.0f, 0}; /* // 深度=1.0f，模板=0 */
// //     renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
// //     renderPassInfo.pClearValues = clearValues.data();
// //     vkCmdBeginRenderPass(this->_queueCommandBuffers[imageIndex], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
// // }
// // void GfxQueue::_renderObject(uint32_t imageIndex, GfxObject *object)
// // {
// //     // std::cout << "GfxQueue : _renderObject object " << object << std::endl;
// //     // 检测渲染管线是否发生了变换
// //     GfxPipeline *pipeline = object->getPipeline();
// //     GfxTexture *texture = object->getTexture();
// //     // std::cout << "GfxQueue : _renderObject object " << object->getUuid() << " type " << static_cast<int>(object->getType()) << static_cast<int>(GfxObjectType::UI_MASK) << " UIMaskBehavior " << object->getUIMaskBehavior() << std::endl;
// //     if (pipeline == nullptr)
// //     {
// //         return;
// //     }
// //     // 更新新的渲染管线
// //     // if (this->_rendererStatus.pipelineName != pipeline->getName())
// //     // {
// //     //     this->_rendererStatus.pipelineName = pipeline->getName();
// //     //     std::cout << "GfxQueue render:bind pipeline " << pipeline->getName() << std::endl;
// //     //     vkCmdBindPipeline(this->_queueCommandBuffers[imageIndex], VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->getVKPipeline());
// //     // }else{
// //     //     std::cout << "GfxQueue render:use pipeline " << pipeline->getName() << std::endl;
// //     // }
// //     vkCmdBindPipeline(this->_queueCommandBuffers[imageIndex], VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->getVKPipeline());
// //     if (object->getType() == GfxObjectType::UI_MASK)
// //     {
// //         if (object->getUIMaskBehavior() == 1)
// //         {
// //             this->_stencilRef++;
// //         }
// //         else
// //         {
// //             this->_stencilRef--;
// //         }
// //         // std::cout << "GfxQueue : _renderObject UIMask " << object->getUuid() << " _stencilRef1  " << this->_stencilRef << std::endl;
// //         vkCmdSetStencilReference(this->_queueCommandBuffers[imageIndex], VK_STENCIL_FACE_FRONT_AND_BACK, 1);      // 增加和减少每次都是固定值1
// //         vkCmdSetStencilCompareMask(this->_queueCommandBuffers[imageIndex], VK_STENCIL_FACE_FRONT_AND_BACK, 0xFF); // 比较所有位
// //         vkCmdSetStencilWriteMask(this->_queueCommandBuffers[imageIndex], VK_STENCIL_FACE_FRONT_AND_BACK, 0xFF);   // 写入所有位
// //         // std::cout << "GfxQueue : _renderObject UIMask " << object->getUuid() << " _stencilRef2   " << this->_stencilRef << std::endl;
// //     }
// //     else
// //     {
// //         // std::cout << "GfxQueue : _renderObject UI  _stencilRef" << this->_stencilRef << std::endl;
// //         vkCmdSetStencilReference(this->_queueCommandBuffers[imageIndex], VK_STENCIL_FACE_FRONT_AND_BACK, this->_stencilRef);
// //         vkCmdSetStencilCompareMask(this->_queueCommandBuffers[imageIndex], VK_STENCIL_FACE_FRONT_AND_BACK, 0xFF); // 比较所有位
// //         vkCmdSetStencilWriteMask(this->_queueCommandBuffers[imageIndex], VK_STENCIL_FACE_FRONT_AND_BACK, 0x00);   // 不写入模板（保持遮罩）
// //     }
// //     object->render(imageIndex, this->_queueCommandBuffers);
// //     // std::cout << "GfxQueue : _renderObject object end " << object->getUuid() << std::endl;

// //     // object->render(imageIndex, this->_queueCommandBuffers);
// //     //     if (object->getType() == GfxObjectType::UI_MASK)
// //     //     {
// //     //         if (object->getUIMaskBehavior() == 1)
// //     //         {
// //     //             this->_stencilRef++;
// //     //         }
// //     //         else
// //     //         {
// //     //             this->_stencilRef--;
// //     //         }
// //     //         vkCmdSetStencilReference(commandBuffer, VK_STENCIL_FACE_FRONT_AND_BACK, this->_stencilRef);  //增加和减少每次都是固定值1
// //     //         vkCmdSetStencilCompareMask(commandBuffer, VK_STENCIL_FACE_FRONT_AND_BACK, 0xFF); // 比较所有位
// //     //         vkCmdSetStencilWriteMask(commandBuffer, VK_STENCIL_FACE_FRONT_AND_BACK, 0xFF);   // 写入所有位
// //     //     }
// //     //     else
// //     //     {
// //     //         vkCmdSetStencilReference(commandBuffer, VK_STENCIL_FACE_FRONT_AND_BACK, this->_stencilRef);
// //     //         vkCmdSetStencilCompareMask(commandBuffer, VK_STENCIL_FACE_FRONT_AND_BACK, 0xFF); // 比较所有位
// //     //         vkCmdSetStencilWriteMask(commandBuffer, VK_STENCIL_FACE_FRONT_AND_BACK, 0x00);   // 不写入模板（保持遮罩）
// //     //     }
// //     //     object->render(imageIndex, this->_commandBuffers);
// // }

// // void GfxQueue::clear()
// // {
// //     this->_clearTextures();
// //     this->_cleanFramebuffers();
// //     this->_cleanCommandBuffers();
// // }
// // void GfxQueue::_clearTextures()
// // {
// //     if (this->_depthTexture != nullptr)
// //     {
// //         delete this->_depthTexture;
// //         this->_depthTexture = nullptr;
// //     }
// // }
// // void GfxQueue::_cleanFramebuffers()
// // {
// //     /*  // 销毁帧缓冲（Framebuffers） */
// //     for (auto framebuffer : this->_queueFramebuffers)
// //     {
// //         vkDestroyFramebuffer(Gfx::context->getVkDevice(), framebuffer, nullptr);
// //     }
// //     this->_queueFramebuffers.clear();
// // }
// // void GfxQueue::_cleanCommandBuffers()
// // {
// //     /* // 销毁命令缓冲（Command Buffers） */
// //     if (!this->_queueCommandBuffers.empty())

// //     {
// //         vkFreeCommandBuffers(Gfx::context->getVkDevice(), this->_context->getCommandPool(), static_cast<uint32_t>(_queueCommandBuffers.size()), _queueCommandBuffers.data());
// //         this->_queueCommandBuffers.clear();
// //     }
// // }
// // void GfxQueue::reset()
// // {
// //     this->_createBuffers();
// // }

// // GfxQueue::~GfxQueue()
// // {
// // }