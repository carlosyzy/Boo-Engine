#include "gfx-object.h"
#include "gfx-pipeline.h"
#include "gfx-texture.h"
#include "gfx-mgr.h"
#include "gfx-context.h"
#include "../game.h"

GfxObject::GfxObject(GfxContext *context) : _color{1.0f, 1.0f, 1.0f, 1.0f},
                                            _uiMask{0.0f, 0.0f, 0.0f, 0.0f, 0.0f}

{
    this->_context = context;
    this->_pipeline = nullptr;
    this->_texture = nullptr;
    this->_pass = nullptr;
    this->_createUniformBuffers();
    this->_updateProjMatUniformBuffer();
    this->_updateViewMatUniformBuffer();
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

void GfxObject::setColor(float r, float g, float b, float a)
{
    this->_color[0] = r;
    this->_color[1] = g;
    this->_color[2] = b;
    this->_color[3] = a;
    std::cout << "setColor1:r:" << r << " g:" << g << " b:" << b << " a:" << a << std::endl;

}
void GfxObject::setTexture(GfxTexture *texture)
{
    std::cout << "setTexture:id:" << " texture:" << std::endl;
    this->_texture = texture;
    this->_updateDescriptorSet();
}
void GfxObject::setModelMatrix(std::array<float, 16> &mat)
{
    std::cout << "setModelMatrix" << std::endl;
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
void GfxObject::setUIMask(float x, float y, float width, float height, float angle)
{
    this->_uiMask[0] = x;
    this->_uiMask[1] = y;
    this->_uiMask[2] = width;
    this->_uiMask[3] = height;
    this->_uiMask[4] = angle;
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
    this->_projMatrix[0] = 2.0f / Game::getInstance()->view().width;
    this->_projMatrix[5] = 2.0f / Game::getInstance()->view().height;

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
    // std::cout << "render: "<< std::endl;
    /*  // 绑定渲染管线 */
    vkCmdBindPipeline(commandBuffers[imageIndex], VK_PIPELINE_BIND_POINT_GRAPHICS, this->_pipeline->getVKPipeline());

    /* // // 设置视口
    // vkCmdSetViewport(commandBuffers[imageIndex], 0, 1, &this->_viewport);
    // // 设置裁剪区域
    // vkCmdSetScissor(commandBuffers[imageIndex], 0, 1, &this->_scissor);



    // 该函数用于提交绘制操作到指令缓冲
    // 参数1：需要执行的指令缓冲对象
    // 参数2：顶点缓冲数量
    // 参数3：用于实例渲染，为1时表示不进行实例渲染
    // 参数4：用于定义着色器变量gl_VertexIndex的值
    // 参数5：用于定义着色器变量gl_InstanceIndex的值
    // std::cout << "this->_indexSize1: " << this->_indexSize << std::endl;
    // Render scene */
    VkDeviceSize offsets[1] = {0};
    vkCmdBindVertexBuffers(commandBuffers[imageIndex], 0, 1, &this->_vertexBuffer, offsets);
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
    pushConstants.maskRect[0] = this->_uiMask[0];
    pushConstants.maskRect[1] = this->_uiMask[1];
    pushConstants.maskRect[2] = this->_uiMask[2];
    pushConstants.maskRect[3] = this->_uiMask[3];

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
}
void GfxObject::clear()
{
    /* // 销毁描述符集（Descriptor Sets） */
    for (auto descriptorSet : this->_descriptorSets)
    {
        vkFreeDescriptorSets(this->_context->getVkDevice(), this->_context->getDescriptorPool(), static_cast<uint32_t>(this->_descriptorSets.size()), &descriptorSet);
    }
    this->_descriptorSets.clear();
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

void GfxObject::_Log(std::string msg)
{
    std::cout << "GfxObject: " << msg << std::endl;
}

GfxObject::~GfxObject()
{
}
