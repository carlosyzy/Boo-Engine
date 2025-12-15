
#define GLFW_INCLUDE_VULKAN
#include "gfx.h"
#include "gfx-mgr.h"
#include "gfx-context.h"
#include "gfx-renderer.h"
#include "base/gfx-material.h"
#include "base/gfx-mesh.h"
#include "base/gfx-render-texture.h"

#include "../utils/time-util.h"

GfxMgr *GfxMgr::getInstance()
{
    static GfxMgr _instance;
    return &_instance;
}

GfxMgr::GfxMgr()
{
    uint32_t instanceVersion = 0;
    VkResult result = vkEnumerateInstanceVersion(&instanceVersion);
    if (result == VK_SUCCESS)
    {
        uint32_t major = VK_VERSION_MAJOR(instanceVersion);
        uint32_t minor = VK_VERSION_MINOR(instanceVersion);
        uint32_t patch = VK_VERSION_PATCH(instanceVersion);
        std::cout << "Gfx :: Vulkan Instance (Driver) Version:" << major << "." << minor << "." << patch << std::endl;
    }
}

void GfxMgr::init()
{
    Gfx::context = new GfxContext();
    Gfx::context->init();
    Gfx::renderer = new GfxRenderer();
    Gfx::renderer->init();
}
void GfxMgr::setLockRender(bool lock)
{
    this->_lockRender = lock;
}

void GfxMgr::update(float dt)
{
    Gfx::time += dt;
    
    if (this->_lockRender)
    {
        return;
    }

    // Gfx::renderer->clearDestroyObjects();
    std::cout << "renderer update" << std::endl;
    Gfx::context->frameFencesPrepare(this->_currentFrame);
    // std::cout << "renderer update1" << std::endl;
    /* // 可用的图像的索引 */
    uint32_t imageIndex;
    Gfx::renderer->frameRendererBefore();
    /**
     * 从交换链申请一个可渲染的图像
     * 通过 _imageAvailableSemaphores[_currentFrame] 信号量，通知 GPU："必须等这个信号量触发后，才能开始渲染该图像"。
     */
    VkResult result1 = Gfx::context->frameAcquireNextImage(&imageIndex, this->_currentFrame);
    /*  // 如果交换链已过期（如窗口大小改变），会返回 VK_ERROR_OUT_OF_DATE_KHR，触发重建交换链 */
    if (result1 == VK_ERROR_OUT_OF_DATE_KHR || result1 == VK_SUBOPTIMAL_KHR)
    {
        std::cout << "Gfx : Render :: update:'VK_ERROR_OUT_OF_DATE_KHR',The window size might have changed, and the swap chain needs to be recreated." << std::endl;
        this->resetSwapChain();
        return;
    }
    // std::cout << "renderer update2" << std::endl;
    /* // 检查当前索引图像是否被其他帧使用，若已被使用，则等待其他帧完成 */
    Gfx::context->frameWaitImageInUse(imageIndex, this->_currentFrame);
    // std::cout << "renderer update:'VK_SUCCESS',imageIndex:" << imageIndex << std::endl;
    /*  // 准备渲染buffer */
    std::vector<VkCommandBuffer> commandBuffers;
    Gfx::renderer->frameRenderer(imageIndex, commandBuffers);
    /*  // 提交渲染命令 */
    Gfx::context->frameSubmitCommands(imageIndex, commandBuffers, this->_currentFrame);
    /*  // 显示图像 */
    VkResult result2 = Gfx::context->framePresentFrame(imageIndex, this->_currentFrame);
    if (result2 == VK_ERROR_OUT_OF_DATE_KHR || result2 == VK_SUBOPTIMAL_KHR)
    {
        Gfx::renderer->frameRendererAfter();
        this->resetSwapChain();
        return;
    }
    else if (result2 != VK_SUCCESS)
    {
        std::cout << "Gfx : Render :: Failed to present swap chain imag22222e!" << std::endl;
    }
    /*  // 0 1 0 1 0 1 */
    /**
     * 帧0	提交命令，信号量A	开始渲染帧0
     * 帧1	提交命令，信号量B	渲染帧0完成，开始渲染帧1
     * 帧0	等待帧0栅栏，信号量A复用	渲染帧1完成，显示帧0
     */
    this->_currentFrame = (this->_currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
    Gfx::renderer->frameRendererAfter();
}
/**
 * 关键时刻手动重置交换链
 */
void GfxMgr::resetSwapChain()
{
    vkDeviceWaitIdle(Gfx::context->getVkDevice()); /*  // 等待所有操作完成 */
    Gfx::renderer->_cleanRendererState();
    Gfx::context->cleanSwapChain();
    /*  // 后重置 */
    Gfx::context->resetSwapChain();
    Gfx::renderer->_resetRendererState();
    std::cout << "GfxMgr :reset swap chain end..." << std::endl;
}

void GfxMgr::createTexture(std::string texture, uint32_t width, uint32_t height, uint32_t channels, const std::vector<uint8_t> *pixels)
{
    Gfx::renderer->createTexture(texture, width, height, channels, pixels);
}
void GfxMgr::destroyTexture(std::string texture)
{
    Gfx::renderer->destroyTexture(texture);
}
bool GfxMgr::isExistTexture(std::string texture)
{
    return Gfx::renderer->isExistTexture(texture);
}
void GfxMgr::createGlslShader(const std::string &shaderName, const std::string &shaderType, const std::string &data, const std::map<std::string, std::string> &macros)
{
    Gfx::renderer->createGlslShader(shaderName, shaderType, data, macros);
}
void GfxMgr::createSpirvShader(const std::string &shaderName, const std::vector<char> &data)
{
    Gfx::renderer->createSpirvShader(shaderName, data);
}
void GfxMgr::initRenderQueue(std::string renderId, GfxRenderTexture *renderTexture)
{
    Gfx::renderer->initRenderQueue(renderId, renderTexture);
}
void GfxMgr::delRenderQueue(std::string renderId)
{
    Gfx::renderer->delRenderQueue(renderId);
}
void GfxMgr::submitRenderMat(std::string renderId, const std::array<float, 16> &viewMatrix, const std::array<float, 16> &projMatrix)
{
    Gfx::renderer->submitRenderMat(renderId, viewMatrix, projMatrix);
}
void GfxMgr::submitRenderObject(std::string renderId, GfxMaterial *material, GfxMesh *mesh, std::vector<float> &instanceData)
{
    Gfx::renderer->submitRenderObject(renderId, material, mesh, instanceData);
}
std::vector<char> GfxMgr::readShaderFile(const std::string &filename)
{
    /*   // ate: 表示从文件末未开始读取
      // binary: 表示以二进制方式读取 */
    std::ifstream file(filename, std::ios::ate | std::ios::binary);
    /*    // std::cout << filename << std::endl; */
    if (!file.is_open())
    {
        throw std::runtime_error("failed to open file!");
    }
    /*  // tellg()返回当前定位指针的位置，也代表着输入流的大小。 */
    size_t fileSize = (size_t)file.tellg();
    /*  // std::cout << fileSize << std::endl; */
    std::vector<char> buffer(fileSize);
    /*  // seekg()是对输入流的操作 g是get缩写，0是代表从开头读起。 */
    file.seekg(0);
    /* // 读入到Buffer当中 */
    file.read(buffer.data(), fileSize);
    file.close();
    /* // std::cout << "read file success: " << filename << std::endl; */
    return buffer;
}

VkResult GfxMgr::createBuffer(VkBufferUsageFlags usageFlags, VkMemoryPropertyFlags memoryPropertyFlags, VkBuffer *buffer, VkDeviceMemory *memory, VkDeviceSize size, void *data = nullptr)
{
    VkBufferCreateInfo bufferCreateInfo{};
    bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferCreateInfo.usage = usageFlags;
    bufferCreateInfo.size = size;
    bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    vkCreateBuffer(Gfx::context->getVkDevice(), &bufferCreateInfo, nullptr, buffer);

    VkMemoryRequirements memReqs;
    VkMemoryAllocateInfo memAlloc{};
    memAlloc.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    vkGetBufferMemoryRequirements(Gfx::context->getVkDevice(), *buffer, &memReqs);
    memAlloc.allocationSize = memReqs.size;
    memAlloc.memoryTypeIndex = getMemoryTypeIndex(memReqs.memoryTypeBits, memoryPropertyFlags);
    vkAllocateMemory(Gfx::context->getVkDevice(), &memAlloc, nullptr, memory);

    if (data != nullptr)
    {
        void *mapped;
        vkMapMemory(Gfx::context->getVkDevice(), *memory, 0, size, 0, &mapped);
        memcpy(mapped, data, size);
        vkUnmapMemory(Gfx::context->getVkDevice(), *memory);
    }

    vkBindBufferMemory(Gfx::context->getVkDevice(), *buffer, *memory, 0);
    return VK_SUCCESS;
}
uint32_t GfxMgr::getMemoryTypeIndex(uint32_t typeBits, VkMemoryPropertyFlags properties)
{
    VkPhysicalDeviceMemoryProperties deviceMemoryProperties;
    vkGetPhysicalDeviceMemoryProperties(Gfx::context->getPhysicalDevice(), &deviceMemoryProperties);
    for (uint32_t i = 0; i < deviceMemoryProperties.memoryTypeCount; i++)
    {
        if ((typeBits & 1) == 1)
        {
            if ((deviceMemoryProperties.memoryTypes[i].propertyFlags & properties) == properties)
            {
                return i;
            }
        }
        typeBits >>= 1;
    }
    return 0;
}
GfxMgr::~GfxMgr()
{
}













// void GfxMgr::initTestInfo()
// {
//     // this->_testMSAASample();
//     // this->_testBindless();
// }
// /**
//  * @brief 测试MSAA采样次数
//  */
// void GfxMgr::_testMSAASample()
// {
//     // VkPhysicalDeviceProperties physicalDeviceProperties;
//     // vkGetPhysicalDeviceProperties(this->_context->getPhysicalDevice(), &physicalDeviceProperties);
//     // /* // 采样次数 */
//     // VkSampleCountFlags counts = physicalDeviceProperties.limits.framebufferColorSampleCounts &
//     //                             physicalDeviceProperties.limits.framebufferDepthSampleCounts;
//     // VkSampleCountFlagBits sampleCount = this->_getMaxMSAAUsableSampleCount();
//     // GfxMgr::Log("GfxMgr", "GPU Maximum supported sampling:" + std::to_string(sampleCount));
// }
// VkSampleCountFlagBits GfxMgr::_getMaxMSAAUsableSampleCount()
// {
//     // VkPhysicalDeviceProperties physicalDeviceProperties;
//     // vkGetPhysicalDeviceProperties(this->_context->getPhysicalDevice(), &physicalDeviceProperties);

//     // VkSampleCountFlags counts = physicalDeviceProperties.limits.framebufferColorSampleCounts &
//     //                             physicalDeviceProperties.limits.framebufferDepthSampleCounts;

//     // if (counts & VK_SAMPLE_COUNT_64_BIT)
//     // {
//     //     return VK_SAMPLE_COUNT_64_BIT;
//     // }
//     // if (counts & VK_SAMPLE_COUNT_32_BIT)
//     // {
//     //     return VK_SAMPLE_COUNT_32_BIT;
//     // }
//     // if (counts & VK_SAMPLE_COUNT_16_BIT)
//     // {
//     //     return VK_SAMPLE_COUNT_16_BIT;
//     // }
//     // if (counts & VK_SAMPLE_COUNT_8_BIT)
//     // {
//     //     return VK_SAMPLE_COUNT_8_BIT;
//     // }
//     // if (counts & VK_SAMPLE_COUNT_4_BIT)
//     // {
//     //     return VK_SAMPLE_COUNT_4_BIT;
//     // }
//     // if (counts & VK_SAMPLE_COUNT_2_BIT)
//     // {
//     //     return VK_SAMPLE_COUNT_2_BIT;
//     // }

//     return VK_SAMPLE_COUNT_1_BIT;
// }
// void GfxMgr::_testBindless()
// {
//     // // const std::vector<const char *> deviceExtensions = {
//     // //     VK_EXT_DESCRIPTOR_INDEXING_EXTENSION_NAME,
//     // //     VK_KHR_MAINTENANCE3_EXTENSION_NAME,
//     // // };
//     // // 检查支持
//     // VkPhysicalDeviceDescriptorIndexingFeaturesEXT indexingFeatures = {};
//     // indexingFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_INDEXING_FEATURES_EXT;

//     // VkPhysicalDeviceFeatures2 features2 = {};
//     // features2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2;
//     // features2.pNext = &indexingFeatures;

//     // vkGetPhysicalDeviceFeatures2(this->_context->getPhysicalDevice(), &features2);

//     // // 必须支持的特性
//     // if (!indexingFeatures.descriptorBindingPartiallyBound ||
//     //     !indexingFeatures.descriptorBindingSampledImageUpdateAfterBind ||
//     //     !indexingFeatures.runtimeDescriptorArray)
//     // {
//     //     throw std::runtime_error("Bindless textures not supported!");
//     // }
//     // else
//     // {
//     //     GfxMgr::Log("GfxMgr", "Bindless textures supported!");
//     // }
// }

// // void GfxMgr::createRenderPass(std::string name, GfxPassStruct passStruct)
// // {
// //     // this->_renderer->createRenderPass(name, passStruct);
// // }
// // void GfxMgr::createPipeline(std::string pipelineName, GfxPipelineStruct pipelineStruct)
// // {
// //     // this->_renderer->createPipeline(pipelineName, pipelineStruct);
// // }

// /**
//  * 旧的
//  * 过时的
//  */
// void GfxMgr::createObject(std::string id, std::string renderPassType, std::vector<float> points, std::vector<float> colors, std::vector<float> normals, std::vector<float> uvs, std::vector<uint32_t> indices)
// {
//     // this->_renderer->createObject(id, renderPassType, points, colors, normals, uvs, indices);
// }
// void GfxMgr::createUIObject(std::string id, std::vector<float> &points, std::vector<float> &colors, std::vector<float> &normals, std::vector<float> &uvs, std::vector<uint32_t> &indices)
// {
//     // this->_renderer->createUIObject(id, points, colors, normals, uvs, indices);
// }
// void GfxMgr::createUIMaskObject(std::string id, std::vector<float> &points, std::vector<float> &colors, std::vector<float> &normals, std::vector<float> &uvs, std::vector<uint32_t> &indices)
// {
//     // this->_renderer->createUIMaskObject(id, points, colors, normals, uvs, indices);
// }
// void GfxMgr::setObjectUIMaskBehavior(std::string id, uint32_t behavior)
// {
//     // this->_renderer->setObjectUIMaskBehavior(id, behavior);
// }
// void GfxMgr::setObjectPass(std::string id, std::string pass)
// {
//     // this->_renderer->setObjectPass(id, pass);
// }
// void GfxMgr::setObjectPipeline(std::string id, std::string pipeline)
// {
//     // this->_renderer->setObjectPipeline(id, pipeline);
// }

// void GfxMgr::destroyObject(std::string id)
// {
//     // this->_renderer->destroyObject(id);
// }
// void GfxMgr::setObjectModelMatrix(std::string id, const std::array<float, 16> &modelMatrix)
// {
//     // this->_renderer->setObjectModelMatrix(id, modelMatrix);
// }
// void GfxMgr::setObjectViewMatrix(std::string id, const std::array<float, 16> &viewMatrix)
// {
//     // this->_renderer->setObjectViewMatrix(id, viewMatrix);
// }
// void GfxMgr::setObjectProjMatrix(std::string id, const std::array<float, 16> &projMatrix)
// {
//     // this->_renderer->setObjectProjMatrix(id, projMatrix);
// }
// void GfxMgr::setObjectColor(std::string id, float r, float g, float b, float a)
// {
//     // this->_renderer->setObjectColor(id, r, g, b, a);
// }
// void GfxMgr::setObjectTexture(const std::string &id, const std::string &texture)
// {
//     // this->_renderer->setObjectTexture(id, texture);
// }

// void GfxMgr::submitObjectRender(std::string id)
// {
//     // this->_renderer->submitObjectRender(id);
// }

/* // 读取shader内容 */
