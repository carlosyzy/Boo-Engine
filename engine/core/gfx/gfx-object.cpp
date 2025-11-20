#include "gfx-object.h"
#include "gfx-pipeline.h"
#include "gfx-texture.h"
#include "gfx-mgr.h"
#include "gfx-context.h"
#include "../../boo.h"

GfxObject::GfxObject(GfxContext *context) : _color{1.0f, 1.0f, 1.0f, 1.0f}
{
    this->_context = context;
    this->_pipeline = nullptr;
    this->_texture = nullptr;
    this->_pass = nullptr;
    this->_createUniformBuffers();
    this->_updateProjMatUniformBuffer();
    this->_updateViewMatUniformBuffer();

    // ===== 测试遮罩：添加一个矩形遮罩区域 =====
    // 坐标系统：屏幕中心为原点 (0, 0)，Y 轴向上
    // 暂时禁用测试遮罩，调试完成后再启用
    // 矩形：屏幕中心 200x200 像素矩形（-100 到 100）

    std::vector<float> testMaskRect = {
        // 第一个三角形（左下、右下、左上）
        -0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, // 左下角
        0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,  // 右下角
        -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,  // 左上角
        // 第二个三角形（右下、右上、左上）
        0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, // 右下角
        0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,  // 右上角
        -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, // 左上角
    };
    // this->addUIMask("test_rect_mask", testMaskRect);
    std::cout << "GfxObject: Test mask - Center 200x200 rect (-100,-100) to (100,100)" << std::endl;

    std::cout << "GfxObject: Test mask DISABLED for debugging" << std::endl;
}

void GfxObject::_createUniformBuffers()
{
    size_t swapChainSize = this->_context->getSwapChainImageViews().size();
    this->_uniformBuffers.resize(swapChainSize);
    this->_uniformBuffersMemory.resize(swapChainSize);
    this->_uniformBuffersMapped.resize(swapChainSize);
    VkDeviceSize bufferSize = sizeof(UniformBufferObject);

    for (size_t i = 0; i < swapChainSize; i++)
    {
        /*  // 创建缓冲区 */
        VkBufferCreateInfo bufferInfo{};
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferInfo.size = bufferSize;
        bufferInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
        bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        if (vkCreateBuffer(this->_context->getVkDevice(), &bufferInfo, nullptr, &this->_uniformBuffers[i]) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to create uniform buffer!");
        }

        /*  // 获取内存需求 */
        VkMemoryRequirements memRequirements;
        vkGetBufferMemoryRequirements(this->_context->getVkDevice(), this->_uniformBuffers[i], &memRequirements);

        /*  // 分配内存 */
        VkMemoryAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = this->_findMemoryType(
            memRequirements.memoryTypeBits,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

        if (vkAllocateMemory(this->_context->getVkDevice(), &allocInfo, nullptr, &this->_uniformBuffersMemory[i]) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to allocate uniform buffer memory!");
        }
        /* // 绑定内存 */
        vkBindBufferMemory(this->_context->getVkDevice(), this->_uniformBuffers[i], this->_uniformBuffersMemory[i], 0);
        /* // 映射内存 */
        vkMapMemory(this->_context->getVkDevice(), this->_uniformBuffersMemory[i], 0, bufferSize, 0, &this->_uniformBuffersMapped[i]);
    }

    /* // this->_Log(" success..."); */
}

void GfxObject::setPass(GfxPass *pass)
{
    this->_pass = pass;
}
void GfxObject::setVertexs(std::vector<float> points, std::vector<float> colors, std::vector<float> normals, std::vector<float> uvs, std::vector<uint32_t> indices)
{
    /* // 等待设备空闲 */
    vkDeviceWaitIdle(this->_context->getVkDevice());
    /*  // 清理旧缓冲区 */
    if (this->_vertexBuffer != VK_NULL_HANDLE)
    {
        vkDestroyBuffer(this->_context->getVkDevice(), this->_vertexBuffer, nullptr);
        this->_vertexBuffer = VK_NULL_HANDLE;
    }
    if (_vertexMemory != VK_NULL_HANDLE)
    {
        vkFreeMemory(this->_context->getVkDevice(), this->_vertexMemory, nullptr);
        this->_vertexMemory = VK_NULL_HANDLE;
    }
    if (this->_indexBuffer != VK_NULL_HANDLE)
    {
        vkDestroyBuffer(this->_context->getVkDevice(), this->_indexBuffer, nullptr);
        this->_indexBuffer = VK_NULL_HANDLE;
    }
    if (this->_indexMemory != VK_NULL_HANDLE)
    {
        vkFreeMemory(this->_context->getVkDevice(), this->_indexMemory, nullptr);
        this->_indexMemory = VK_NULL_HANDLE;
    }
    /* // 创建新的缓冲区
    //  验证数据完整性 */
    size_t vertexCount = points.size() / 3; /* // 每个顶点3个float (x,y,z) */
    if (vertexCount == 0 || colors.size() != vertexCount * 4 || normals.size() != vertexCount * 3 || uvs.size() != vertexCount * 2 || indices.size() == 0)
    {
        std::cerr << "Invalid vertex data sizes!" << std::endl;
        return;
    }
    this->_indexSize = indices.size();

    std::vector<float> interleavedVertices;
    interleavedVertices.reserve(vertexCount * (3 + 4 + 3 + 2)); /* // position + color + normal + uv */

    for (size_t i = 0; i < vertexCount; ++i)
    {
        /*  // 添加位置 (3 floats) */
        interleavedVertices.insert(interleavedVertices.end(), points.begin() + i * 3, points.begin() + (i + 1) * 3);
        /*  // 添加颜色 (4 floats) */
        interleavedVertices.insert(interleavedVertices.end(), colors.begin() + i * 4, colors.begin() + (i + 1) * 4);
        /*  // 添加法线 (3 floats) */
        interleavedVertices.insert(interleavedVertices.end(), normals.begin() + i * 3, normals.begin() + (i + 1) * 3);
        /* // 添加UV (2 floats) */
        interleavedVertices.insert(interleavedVertices.end(), uvs.begin() + i * 2, uvs.begin() + (i + 1) * 2);
    }
    // 顶点缓冲区
    GfxMgr::getInstance()->createBuffer(
        VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
        &this->_vertexBuffer,
        &this->_vertexMemory,
        interleavedVertices.size() * sizeof(float), /* // 总字节数 */
        interleavedVertices.data()                  /*  // 数据指针 */
    );
    /*  // 索引缓冲区（不变） */
    GfxMgr::getInstance()->createBuffer(
        VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
        &this->_indexBuffer,
        &this->_indexMemory,
        indices.size() * sizeof(uint32_t),
        indices.data());
}

void GfxObject::setPipeline(GfxPipeline *pipeline)
{
    this->_pipeline = pipeline;
    this->_updateDescriptorSet();
}
void GfxObject::setUIMaskPipeline(GfxPipeline *pipelineMask)
{
    this->_pipelineMask = pipelineMask;
}

void GfxObject::setColor(float r, float g, float b, float a)
{
    this->_color[0] = r;
    this->_color[1] = g;
    this->_color[2] = b;
    this->_color[3] = a;
    // std::cout << "setColor1:r:" << r << " g:" << g << " b:" << b << " a:" << a << std::endl;
}
void GfxObject::setTexture(GfxTexture *texture)
{
    // std::cout << "setTexture:id:" << " texture:" << std::endl;
    this->_texture = texture;
    this->_updateDescriptorSet();
}
void GfxObject::setModelMatrix(std::array<float, 16> &mat)
{
    // std::cout << "setModelMatrix" << std::endl;
    this->_modelMatrix = mat;
    this->_updateModelMatUniformBuffer();
}
void GfxObject::setViewMatrix(std::array<float, 16> &mat)
{
    // this->_viewMatrix = mat;
    // this->_updateViewMatUniformBuffer();
}
void GfxObject::setProjMatrix(std::array<float, 16> &mat)
{
    // this->_projMatrix = mat;
    // this->_updateProjMatUniformBuffer();
}
void GfxObject::addUIMask(std::string maskId, std::vector<float> mask)
{
    if (mask.size() < 7)
    {
        std::cerr << "Invalid mask size!" << std::endl;
        return;
    }
    this->_uiMasks[maskId] = mask;
}
/**
 * @brief 更新描述符集
 */
void GfxObject::_updateDescriptorSet()
{
    if (this->_texture == nullptr)
    {
        /* // std::cout << "texture is null" << std::endl; */
        return;
    }
    if (this->_pipeline == nullptr || this->_pipeline->getDescriptorSetLayout() == VK_NULL_HANDLE)
    {
        /*  // std::cout << "pipeline is null" << std::endl; */
        return;
    }
    std::vector<VkImageView> &swapChainImageViews = this->_context->getSwapChainImageViews();

    std::vector<VkDescriptorSetLayout> layouts(swapChainImageViews.size(), this->_pipeline->getDescriptorSetLayout());
    VkDescriptorSetAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = this->_context->getDescriptorPool();
    allocInfo.descriptorSetCount = static_cast<uint32_t>(swapChainImageViews.size());
    allocInfo.pSetLayouts = layouts.data();
    this->_descriptorSets.resize(swapChainImageViews.size());
    VkResult result = vkAllocateDescriptorSets(this->_context->getVkDevice(), &allocInfo, this->_descriptorSets.data());

    if (result != VK_SUCCESS)
    {
        throw std::runtime_error("failed to allocate descriptor sets!");
    }

    for (size_t i = 0; i < swapChainImageViews.size(); i++)
    {
        VkDescriptorBufferInfo bufferInfo{};
        bufferInfo.buffer = this->_uniformBuffers[i];
        bufferInfo.offset = 0;
        bufferInfo.range = sizeof(UniformBufferObject);

        VkDescriptorImageInfo imageInfo{};
        imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        imageInfo.imageView = this->_texture->getImageView();
        imageInfo.sampler = this->_texture->getSampler();

        std::array<VkWriteDescriptorSet, 2> descriptorWrites{};

        descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrites[0].dstSet = this->_descriptorSets[i];
        descriptorWrites[0].dstBinding = 0;
        descriptorWrites[0].dstArrayElement = 0;
        descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        descriptorWrites[0].descriptorCount = 1;
        descriptorWrites[0].pBufferInfo = &bufferInfo;

        descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrites[1].dstSet = this->_descriptorSets[i];
        descriptorWrites[1].dstBinding = 1;
        descriptorWrites[1].dstArrayElement = 0;
        descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        descriptorWrites[1].descriptorCount = 1;
        descriptorWrites[1].pImageInfo = &imageInfo;

        vkUpdateDescriptorSets(this->_context->getVkDevice(), static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
    }
    /*  // this->_Log("update descriptor set success..."); */
}

uint32_t GfxObject::_findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
{
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(this->_context->getPhysicalDevice(), &memProperties);

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
void GfxObject::_cleanUniformBuffers()
{
    for (size_t i = 0; i < this->_uniformBuffers.size(); i++)
    {
        if (this->_uniformBuffersMapped[i])
        {
            vkUnmapMemory(this->_context->getVkDevice(), this->_uniformBuffersMemory[i]);
            this->_uniformBuffersMapped[i] = nullptr;
        }
        if (this->_uniformBuffers[i] != VK_NULL_HANDLE)
        {
            vkDestroyBuffer(this->_context->getVkDevice(), this->_uniformBuffers[i], nullptr);
            this->_uniformBuffers[i] = VK_NULL_HANDLE;
        }
        if (this->_uniformBuffersMemory[i] != VK_NULL_HANDLE)
        {
            vkFreeMemory(this->_context->getVkDevice(), this->_uniformBuffersMemory[i], nullptr);
            this->_uniformBuffersMemory[i] = VK_NULL_HANDLE;
        }
    }
    this->_uniformBuffers.clear();
    this->_uniformBuffersMemory.clear();
    this->_uniformBuffersMapped.clear();
}

void GfxObject::_updateModelMatUniformBuffer()
{
    if (this->_uniformBuffersMapped.empty())
        return;
    size_t swapChainSize = this->_context->getSwapChainImageViews().size();
    for (size_t i = 0; i < swapChainSize; i++)
    {
        memcpy(this->_uniformBuffersMapped[i], this->_modelMatrix.data(), sizeof(float) * 16);
    }
}

void GfxObject::_updateViewMatUniformBuffer()
{
    if (this->_uniformBuffersMapped.empty())
        return;
    this->_viewMatrix[0] = this->_viewMatrix[5] = this->_viewMatrix[10] = this->_viewMatrix[15] = 1.0f;
    size_t swapChainSize = this->_context->getSwapChainImageViews().size();
    for (size_t i = 0; i < swapChainSize; i++)
    {
        memcpy((char *)this->_uniformBuffersMapped[i] + sizeof(float) * 16, this->_viewMatrix.data(), sizeof(float) * 16);
    }
}
void GfxObject::_updateProjMatUniformBuffer()
{
    this->_projMatrix[0] = this->_projMatrix[5] = this->_projMatrix[10] = this->_projMatrix[15] = 1.0f;
    this->_projMatrix[0] = 2.0f / Boo::game->view()->width;
    this->_projMatrix[5] = 2.0f / Boo::game->view()->height;

    if (this->_uniformBuffersMapped.empty())
        return;
    size_t swapChainSize = this->_context->getSwapChainImageViews().size();
    for (size_t i = 0; i < swapChainSize; i++)
    {
        memcpy((char *)this->_uniformBuffersMapped[i] + sizeof(float) * 16 * 2, this->_projMatrix.data(), sizeof(float) * 16);
    }
}

void GfxObject::render(uint32_t imageIndex, std::vector<VkCommandBuffer> &commandBuffers)
{

    if (!this->_pipeline || this->_vertexBuffer == VK_NULL_HANDLE || this->_indexBuffer == VK_NULL_HANDLE)
        return;

    // ===== 清理上一帧的临时遮罩 buffers =====
    for (size_t i = 0; i < this->_maskTempBuffers.size(); i++)
    {
        if (this->_maskTempBuffers[i] != VK_NULL_HANDLE)
        {
            vkDestroyBuffer(this->_context->getVkDevice(), this->_maskTempBuffers[i], nullptr);
        }
        if (this->_maskTempMemories[i] != VK_NULL_HANDLE)
        {
            vkFreeMemory(this->_context->getVkDevice(), this->_maskTempMemories[i], nullptr);
        }
    }
    this->_maskTempBuffers.clear();
    this->_maskTempMemories.clear();

    // 决定是否使用遮罩：检查遮罩管线和遮罩数据是否存在
    bool useMask = (this->_pipelineMask != nullptr && !this->_uiMasks.empty());
    uint32_t stencilRef = useMask ? 1 : 0; // 有遮罩：reference=1，无遮罩：reference=0
    useMask=false;
    if (useMask)
    {
        // 绑定UI遮罩渲染管线（管线配置：compareOp=Always, passOp=Increment）
        vkCmdBindPipeline(commandBuffers[imageIndex], VK_PIPELINE_BIND_POINT_GRAPHICS, this->_pipelineMask->getVKPipeline());

        // 绑定 descriptor sets（遮罩管线也需要，即使不使用纹理）
        vkCmdBindDescriptorSets(commandBuffers[imageIndex], VK_PIPELINE_BIND_POINT_GRAPHICS,
                                this->_pipelineMask->getVKPipelineLayout(), 0, 1,
                                &this->_descriptorSets[imageIndex], 0, nullptr);

        // 设置 Stencil 参数：写入模式
        vkCmdSetStencilReference(commandBuffers[imageIndex], VK_STENCIL_FACE_FRONT_AND_BACK, stencilRef);
        vkCmdSetStencilCompareMask(commandBuffers[imageIndex], VK_STENCIL_FACE_FRONT_AND_BACK, 0xFF); // 比较所有位
        vkCmdSetStencilWriteMask(commandBuffers[imageIndex], VK_STENCIL_FACE_FRONT_AND_BACK, 0xFF);   // 写入所有位

        // 遍历所有遮罩并绘制
        for (auto &[maskId, maskData] : this->_uiMasks)
        {
            // 验证顶点数据大小：每个顶点需要 12 个浮点数
            // position(3) + color(4) + normal(3) + uv(2) = 12 floats
            if (maskData.size() < 12 || maskData.size() % 12 != 0)
            {
                std::cerr << "Mask '" << maskId << "' has invalid size: " << maskData.size()
                          << " (must be multiple of 12)" << std::endl;
                continue;
            }

            VkBuffer maskVertexBuffer = VK_NULL_HANDLE;
            VkDeviceMemory maskVertexMemory = VK_NULL_HANDLE;
            GfxMgr::getInstance()->createBuffer(
                VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                &maskVertexBuffer,
                &maskVertexMemory,
                maskData.size() * sizeof(float),
                maskData.data());

            // // 保存到成员变量，下一帧渲染时清理
            // this->_maskTempBuffers.push_back(maskVertexBuffer);
            // this->_maskTempMemories.push_back(maskVertexMemory);

            // 绑定遮罩顶点缓冲区
            VkDeviceSize maskOffsets[1] = {0};
            // vkCmdBindVertexBuffers(commandBuffers[imageIndex], 0, 1, &maskVertexBuffer, maskOffsets);
            vkCmdBindVertexBuffers(commandBuffers[imageIndex], 0, 1, &this->_vertexBuffer, maskOffsets);
            vkCmdBindIndexBuffer(commandBuffers[imageIndex], this->_indexBuffer, 0, VK_INDEX_TYPE_UINT32);
            // 绘制遮罩（顶点数 = maskData.size() / 12）
            // 每个顶点：position(3) + color(4) + normal(3) + uv(2) = 12 floats
            uint32_t vertexCount = static_cast<uint32_t>(maskData.size() / 12);
            // vkCmdDraw(commandBuffers[imageIndex], this->_indexSize, 1, 0, 0);
            vkCmdDrawIndexed(
                commandBuffers[imageIndex],
                this->_indexSize, /* // 只绘制3个索引（第一个三角形） */
                1,                /* // 实例数 （2的话代表绘制2个实例，也就是绘制两次） */
                0,                /* // 第一个顶点的索引 每个 UI 元素占用 6 个顶点 */
                0,                /*  // 第一个实例的索引 从第 0 个实例开始绘制 */
                0                 /* // 实例偏移 */
            );
        }
    }

    // ===== 第二步：绘制 UI 内容 =====
    // 绑定 UI 渲染管线（管线配置：compareOp=Equal, passOp=Keep）
    vkCmdBindPipeline(commandBuffers[imageIndex], VK_PIPELINE_BIND_POINT_GRAPHICS, this->_pipeline->getVKPipeline());

    // 设置 Stencil 参数：
    // - 有遮罩：reference=1, 只在 Stencil=1 的遮罩区域绘制
    // - 无遮罩：reference=0, 在整个屏幕绘制（因为清除后 Stencil=0）
    vkCmdSetStencilReference(commandBuffers[imageIndex], VK_STENCIL_FACE_FRONT_AND_BACK, stencilRef);
    vkCmdSetStencilCompareMask(commandBuffers[imageIndex], VK_STENCIL_FACE_FRONT_AND_BACK, 0xFF); // 比较所有位
    vkCmdSetStencilWriteMask(commandBuffers[imageIndex], VK_STENCIL_FACE_FRONT_AND_BACK, 0x00);   // 不写入模板（保持遮罩）

    // 设置视口
    VkViewport _viewport{};
    // 裁剪区域 原点左上角为(0,0)
    VkRect2D _scissor{};
    _viewport.x = 0.0f;
    _viewport.y = 0.0f;
    _viewport.width = (float)Boo::game->view()->width;
    _viewport.height = (float)Boo::game->view()->height;
    _viewport.minDepth = 0.0f;
    _viewport.maxDepth = 1.0f;
    _scissor.offset = {0, 0};
    _scissor.extent = {(uint32_t)(Boo::game->view()->width), (uint32_t)(Boo::game->view()->height)};
    // vkCmdSetViewport(commandBuffers[imageIndex], 0, 1, &_viewport);
    // // 设置裁剪区域
    // vkCmdSetScissor(commandBuffers[imageIndex], 0, 1, &_scissor);

    // 该函数用于提交绘制操作到指令缓冲
    // 参数1：需要执行的指令缓冲对象
    // 参数2：顶点缓冲数量
    // 参数3：用于实例渲染，为1时表示不进行实例渲染
    // 参数4：用于定义着色器变量gl_VertexIndex的值
    // 参数5：用于定义着色器变量gl_InstanceIndex的值
    // std::cout << "this->_indexSize1: " << this->_indexSize << std::endl;
    // Render scene */

    VkBuffer maskVertexBuffer = VK_NULL_HANDLE;
    VkDeviceMemory maskVertexMemory = VK_NULL_HANDLE;

    std::vector<float> testMaskRect = {
        -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,  /** @brief 左上 */
        -0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, /** @brief 坐下 */
        0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,  /** @brief 右下 */
        0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,   /** @brief 右上 */
    };
    GfxMgr::getInstance()->createBuffer(
        VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
        &maskVertexBuffer,
        &maskVertexMemory,
        testMaskRect.size() * sizeof(float),
        testMaskRect.data());
        // 顶点缓冲区
  
    // 保存到成员变量，下一帧渲染时清理
    this->_maskTempBuffers.push_back(maskVertexBuffer);
    this->_maskTempMemories.push_back(maskVertexMemory);


    VkDeviceSize offsets[1] = {0};
    vkCmdBindVertexBuffers(commandBuffers[imageIndex], 0, 1, &maskVertexBuffer, offsets);
    vkCmdBindIndexBuffer(commandBuffers[imageIndex], this->_indexBuffer, 0, VK_INDEX_TYPE_UINT32);
    /*    //  std::cout << "render:1111 "<< std::endl;
       // 绑定描述符集
       // std::cout <<"1111:"<< this->_descriptorSets[imageIndex]<< std::endl; */
    vkCmdBindDescriptorSets(commandBuffers[imageIndex], VK_PIPELINE_BIND_POINT_GRAPHICS, this->_pipeline->getVKPipelineLayout(), 0, 1, &this->_descriptorSets[imageIndex], 0, nullptr);

    // std::cout << "render:22222 "<< std::endl;
    PushConstants pushConstants{};
    pushConstants.defaultColor[0] = this->_color[0];
    pushConstants.defaultColor[1] = this->_color[1];
    pushConstants.defaultColor[2] = this->_color[2];
    pushConstants.defaultColor[3] = this->_color[3];
    // pushConstants.maskRect[0] = this->_uiMask[0];
    // pushConstants.maskRect[1] = this->_uiMask[1];
    // pushConstants.maskRect[2] = this->_uiMask[2];
    // pushConstants.maskRect[3] = this->_uiMask[3];

    vkCmdPushConstants(commandBuffers[imageIndex], this->_pipeline->getVKPipelineLayout(), VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(PushConstants), &pushConstants);
    /*  // 绘制
     //  std::cout << "render:33333 "<< std::endl; */
    vkCmdDrawIndexed(
        commandBuffers[imageIndex],
        this->_indexSize, /* // 只绘制3个索引（第一个三角形） */
        1,                /* // 实例数 （2的话代表绘制2个实例，也就是绘制两次） */
        0,                /* // 第一个顶点的索引 每个 UI 元素占用 6 个顶点 */
        0,                /*  // 第一个实例的索引 从第 0 个实例开始绘制 */
        0                 /* // 实例偏移 */
    );
    // 临时遮罩 buffers 会在下一帧渲染开始时清理
}
void GfxObject::clear()
{
    /* // 销毁描述符集（Descriptor Sets） */
    for (auto descriptorSet : this->_descriptorSets)
    {
        vkFreeDescriptorSets(this->_context->getVkDevice(), this->_context->getDescriptorPool(), static_cast<uint32_t>(this->_descriptorSets.size()), &descriptorSet);
    }
    this->_descriptorSets.clear();

    // 清理临时遮罩 buffers
    for (size_t i = 0; i < this->_maskTempBuffers.size(); i++)
    {
        if (this->_maskTempBuffers[i] != VK_NULL_HANDLE)
        {
            vkDestroyBuffer(this->_context->getVkDevice(), this->_maskTempBuffers[i], nullptr);
        }
        if (this->_maskTempMemories[i] != VK_NULL_HANDLE)
        {
            vkFreeMemory(this->_context->getVkDevice(), this->_maskTempMemories[i], nullptr);
        }
    }
    this->_maskTempBuffers.clear();
    this->_maskTempMemories.clear();

    /*  // 销毁缓冲区 */
    this->_cleanUniformBuffers();
}
void GfxObject::reset()
{
    this->_createUniformBuffers();
    this->_updateDescriptorSet();
    this->_updateModelMatUniformBuffer();
    this->_updateViewMatUniformBuffer();
    this->_updateProjMatUniformBuffer();
}

void GfxObject::destroy()
{
    this->clear();
    this->_context = nullptr;
    this->_pipeline = nullptr;
    this->_pass = nullptr;
    this->_texture = nullptr;
    this->_vertexBuffer = VK_NULL_HANDLE;
    this->_indexBuffer = VK_NULL_HANDLE;
    this->_vertexMemory = VK_NULL_HANDLE;
    this->_indexMemory = VK_NULL_HANDLE;
}

void GfxObject::_Log(std::string msg)
{
    std::cout << "GfxObject: " << msg << std::endl;
}

GfxObject::~GfxObject()
{
}

// rivate:
//     void applyUIMasks(VkCommandBuffer cmdBuffer, uint32_t imageIndex) {
//         // 确保有可用的 Stencil Pipeline
//         if (!this->_pipeline || !this->_pipeline->hasStencilMaskPipeline()) {
//             return;
//         }

//         // 绑定 Stencil Write Pipeline
//         vkCmdBindPipeline(cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
//                          this->_pipeline->getStencilMaskPipeline());

//         // 为每个遮罩设置不同的 Stencil 引用值
//         uint32_t stencilRef = 1;

//         for (auto& [maskId, mask] : this->_uiMasks) {
//             if (mask.size() < 7) continue; // 需要至少7个参数: x,y,width,height,op,rotation,更多参数...

//             // 设置 Stencil 引用值
//             vkCmdSetStencilReference(cmdBuffer, VK_STENCIL_FACE_FRONT_AND_BACK, stencilRef);

//             // 根据遮罩操作类型设置不同的 Stencil 状态
//             MaskOperation op = static_cast<MaskOperation>(static_cast<int>(mask[4]));
//             setupStencilStateForOperation(cmdBuffer, op);

//             // 创建遮罩几何体并绘制
//             std::vector<Vertex> maskVertices = createMaskGeometry(mask);
//             drawMaskGeometry(cmdBuffer, maskVertices);

//             stencilRef++;
//         }

//         // 切换回正常的渲染 Pipeline（会在主render函数中重新绑定）
//     }

//     void setupStencilStateForOperation(VkCommandBuffer cmdBuffer, MaskOperation operation) {
//         // 动态设置 Stencil 操作
//         switch (operation) {
//             case MaskOperation::ADD:
//                 vkCmdSetStencilOp(cmdBuffer, VK_STENCIL_FACE_FRONT_AND_BACK,
//                                  VK_STENCIL_OP_KEEP, VK_STENCIL_OP_KEEP,
//                                  VK_STENCIL_OP_INCREMENT_AND_WRAP);
//                 break;
//             case MaskOperation::SUBTRACT:
//                 vkCmdSetStencilOp(cmdBuffer, VK_STENCIL_FACE_FRONT_AND_BACK,
//                                  VK_STENCIL_OP_KEEP, VK_STENCIL_OP_KEEP,
//                                  VK_STENCIL_OP_DECREMENT_AND_WRAP);
//                 break;
//             case MaskOperation::REPLACE:
//                 vkCmdSetStencilOp(cmdBuffer, VK_STENCIL_FACE_FRONT_AND_BACK,
//                                  VK_STENCIL_OP_KEEP, VK_STENCIL_OP_KEEP,
//                                  VK_STENCIL_OP_REPLACE);
//                 break;
//         }
//     }

//     std::vector<Vertex> createMaskGeometry(const std::vector<float>& mask) {
//         std::vector<Vertex> vertices;

//         if (mask.size() < 7) return vertices;

//         float x = mask[0];
//         float y = mask[1];
//         float width = mask[2];
//         float height = mask[3];
//         float rotation = mask[5]; // 旋转角度

//         // 创建矩形遮罩几何体（支持旋转）
//         if (rotation == 0.0f) {
//             // 无旋转的简单矩形
//             vertices = {
//                 {{x, y, 0.0f, 1.0f}},
//                 {{x + width, y, 0.0f, 1.0f}},
//                 {{x + width, y + height, 0.0f, 1.0f}},
//                 {{x, y + height, 0.0f, 1.0f}}
//             };
//         } else {
//             // 带旋转的矩形（需要计算旋转后的顶点）
//             float centerX = x + width / 2.0f;
//             float centerY = y + height / 2.0f;
//             float cosR = cos(rotation);
//             float sinR = sin(rotation);

//             std::vector<glm::vec2> corners = {
//                 {x - centerX, y - centerY},
//                 {x + width - centerX, y - centerY},
//                 {x + width - centerX, y + height - centerY},
//                 {x - centerX, y + height - centerY}
//             };

//             for (auto& corner : corners) {
//                 glm::vec2 rotated = {
//                     corner.x * cosR - corner.y * sinR + centerX,
//                     corner.x * sinR + corner.y * cosR + centerY
//                 };
//                 vertices.push_back({{rotated.x, rotated.y, 0.0f, 1.0f}});
//             }
//         }

//         return vertices;
//     }

//     void drawMaskGeometry(VkCommandBuffer cmdBuffer, const std::vector<Vertex>& vertices) {
//         if (vertices.size() < 3) return;

//         // 创建临时顶点缓冲区（在实际项目中应该预分配）
//         VkBuffer maskVertexBuffer;
//         VkDeviceMemory maskVertexMemory;

//         // 这里需要实现顶点缓冲区的创建和绑定
//         // createTemporaryVertexBuffer(vertices, maskVertexBuffer, maskVertexMemory);

//         // 绘制遮罩几何体（三角形列表）
//         // vkCmdDraw(cmdBuffer, static_cast<uint32_t>(vertices.size()), 1, 0, 0);

//         // 清理临时资源
//         // destroyTemporaryVertexBuffer(maskVertexBuffer, maskVertexMemory);
//     }

//     void cleanupUIMasks(VkCommandBuffer cmdBuffer) {
//         // 如果需要清理 Stencil Buffer，可以在这里执行清除操作
//         // 或者依赖 RenderPass 的自动清除
//     }