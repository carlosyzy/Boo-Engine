
#define GLFW_INCLUDE_VULKAN
#include "gfx-mgr.h"

#include "gfx-texture.h"
#include "gfx-context.h"
#include "gfx-struct.h"
#include "gfx-renderer.h"
#include "gfx-pass.h"
#include "gfx-pass-struct.h"
#include "gfx-pipeline-struct.h"
#include "gfx-texture.h"
#include "gfx-shader.h"
#include "gfx-queue.h"
#include "gfx-object.h"

#include "../utils/time-util.h"

GfxMgr *GfxMgr::getInstance()
{
    static GfxMgr _instance;
    return &_instance;
}

GfxMgr::GfxMgr()
{
}

void GfxMgr::init()
{
    this->showVersion();
    this->_context = new GfxContext();
    this->_context->init();
    this->_renderer = new GfxRenderer(this->_context);
    this->_renderer->init();
    /* // 初始化显示测试参数 */
    this->initTestInfo();
}
void GfxMgr::showVersion()
{
    uint32_t instanceVersion = 0;
    VkResult result = vkEnumerateInstanceVersion(&instanceVersion);

    if (result == VK_SUCCESS)
    {
        uint32_t major = VK_VERSION_MAJOR(instanceVersion);
        uint32_t minor = VK_VERSION_MINOR(instanceVersion);
        uint32_t patch = VK_VERSION_PATCH(instanceVersion);
        GfxMgr::Log("GfxMgr", "Vulkan Instance (Driver) Version:" + std::to_string(major) + "." + std::to_string(minor) + "." + std::to_string(patch));
    }
}

void GfxMgr::initTestInfo()
{
    this->_testMSAASample();
}
/**
 * @brief 测试MSAA采样次数
 */
void GfxMgr::_testMSAASample()
{
    VkPhysicalDeviceProperties physicalDeviceProperties;
    vkGetPhysicalDeviceProperties(this->_context->getPhysicalDevice(), &physicalDeviceProperties);
    /* // 采样次数 */
    VkSampleCountFlags counts = physicalDeviceProperties.limits.framebufferColorSampleCounts &
                                physicalDeviceProperties.limits.framebufferDepthSampleCounts;
    VkSampleCountFlagBits sampleCount = this->_getMaxMSAAUsableSampleCount();
    GfxMgr::Log("GfxMgr", "GPU Maximum supported sampling:" + std::to_string(sampleCount));
}
VkSampleCountFlagBits GfxMgr::_getMaxMSAAUsableSampleCount()
{
    VkPhysicalDeviceProperties physicalDeviceProperties;
    vkGetPhysicalDeviceProperties(this->_context->getPhysicalDevice(), &physicalDeviceProperties);

    VkSampleCountFlags counts = physicalDeviceProperties.limits.framebufferColorSampleCounts &
                                physicalDeviceProperties.limits.framebufferDepthSampleCounts;

    if (counts & VK_SAMPLE_COUNT_64_BIT)
    {
        return VK_SAMPLE_COUNT_64_BIT;
    }
    if (counts & VK_SAMPLE_COUNT_32_BIT)
    {
        return VK_SAMPLE_COUNT_32_BIT;
    }
    if (counts & VK_SAMPLE_COUNT_16_BIT)
    {
        return VK_SAMPLE_COUNT_16_BIT;
    }
    if (counts & VK_SAMPLE_COUNT_8_BIT)
    {
        return VK_SAMPLE_COUNT_8_BIT;
    }
    if (counts & VK_SAMPLE_COUNT_4_BIT)
    {
        return VK_SAMPLE_COUNT_4_BIT;
    }
    if (counts & VK_SAMPLE_COUNT_2_BIT)
    {
        return VK_SAMPLE_COUNT_2_BIT;
    }

    return VK_SAMPLE_COUNT_1_BIT;
}
void GfxMgr::createRenderPass(std::string name, GfxPassStruct passStruct)
{
    this->_renderer->createRenderPass(name, passStruct);
}
void GfxMgr::createPipeline(std::string pipelineName, GfxPipelineStruct pipelineStruct)
{
    this->_renderer->createPipeline(pipelineName, pipelineStruct);
}

void GfxMgr::createTexture(std::string texture, uint32_t width, uint32_t height, uint32_t channels, const std::vector<uint8_t> *pixels)
{
    this->_renderer->createTexture(texture, width, height, channels, pixels);
}
void GfxMgr::destroyTexture(std::string texture)
{
    this->_renderer->destroyTexture(texture);
}
bool GfxMgr::isExistTexture(std::string texture)
{
    return this->_renderer->isExistTexture(texture);
}
void GfxMgr::createGlslShader(const std::string &shaderName,const std::string &shaderType, const std::string &data, const std::map<std::string, std::string> &macros)
{
    this->_renderer->createGlslShader(shaderName, shaderType, data, macros);
}
void GfxMgr::createSpirvShader(const std::string &shaderName, const std::vector<char> &data)
{
    this->_renderer->createSpirvShader(shaderName, data);
}


/**
 * 旧的
 * 过时的
 */
void GfxMgr::createObject(std::string id, std::string renderPassType, std::vector<float> points, std::vector<float> colors, std::vector<float> normals, std::vector<float> uvs, std::vector<uint32_t> indices)
{
    this->_renderer->createObject(id, renderPassType,  points, colors, normals, uvs, indices);
}
void GfxMgr::createUIObject(std::string id, std::vector<float> &points, std::vector<float> &colors, std::vector<float> &normals, std::vector<float> &uvs, std::vector<uint32_t> &indices)
{
    this->_renderer->createUIObject(id, points, colors, normals, uvs, indices);
}
void GfxMgr::createUIMaskObject(std::string id, std::vector<float> &points, std::vector<float> &colors, std::vector<float> &normals, std::vector<float> &uvs, std::vector<uint32_t> &indices)
{
    this->_renderer->createUIMaskObject(id, points, colors, normals, uvs, indices);
}
void GfxMgr::setObjectUIMaskBehavior(std::string id, uint32_t behavior)
{
    this->_renderer->setObjectUIMaskBehavior(id, behavior);
}
void GfxMgr::setObjectPass(std::string id, std::string pass)
{
    this->_renderer->setObjectPass(id, pass);
}
void GfxMgr::setObjectPipeline(std::string id, std::string pipeline)
{
    this->_renderer->setObjectPipeline(id, pipeline);
}



void GfxMgr::destroyObject(std::string id)
{
    this->_renderer->destroyObject(id);
}
void GfxMgr::setObjectModelMatrix(std::string id, const std::array<float, 16> &modelMatrix)
{
    this->_renderer->setObjectModelMatrix(id, modelMatrix);
}
void GfxMgr::setObjectViewMatrix(std::string id, const std::array<float, 16> &viewMatrix)
{
    this->_renderer->setObjectViewMatrix(id, viewMatrix);
}
void GfxMgr::setObjectProjMatrix(std::string id, const std::array<float, 16> &projMatrix)
{
    this->_renderer->setObjectProjMatrix(id, projMatrix);
}
void GfxMgr::setObjectColor(std::string id, float r, float g, float b, float a)
{
    this->_renderer->setObjectColor(id, r, g, b, a);
}
void GfxMgr::setObjectTexture(const std::string& id, const  std::string& texture)
{
    this->_renderer->setObjectTexture(id, texture);
}


void GfxMgr::submitObjectRender(std::string id)
{
    this->_renderer->submitObjectRender(id);
}

void GfxMgr::update()
{
    // std::cout << "renderer update" << std::endl;
    this->_context->frameFencesPrepare(this->_currentFrame);
    /* // 可用的图像的索引 */
    uint32_t imageIndex;
    /**
     * 从交换链申请一个可渲染的图像
     * 通过 _imageAvailableSemaphores[_currentFrame] 信号量，通知 GPU："必须等这个信号量触发后，才能开始渲染该图像"。
     */
    VkResult result1 = this->_context->frameAcquireNextImage(&imageIndex, this->_currentFrame);
   /*  // 如果交换链已过期（如窗口大小改变），会返回 VK_ERROR_OUT_OF_DATE_KHR，触发重建交换链 */
    if (result1 == VK_ERROR_OUT_OF_DATE_KHR || result1 == VK_SUBOPTIMAL_KHR)
    {
       std::cout << "renderer update:'VK_ERROR_OUT_OF_DATE_KHR',The window size might have changed, and the swap chain needs to be recreated." << std::endl;  
        this->resetSwapChain();
        return;
    }
    /* // 检查当前索引图像是否被其他帧使用，若已被使用，则等待其他帧完成 */
    this->_context->frameWaitImageInUse(imageIndex, this->_currentFrame);

   /*  // 准备渲染buffer */
    std::vector<VkCommandBuffer> commandBuffers;
    this->_renderer->frameRenderer(imageIndex, commandBuffers);

   /*  // 提交渲染命令 */
    this->_context->frameSubmitCommands(imageIndex, commandBuffers, this->_currentFrame);
   /*  // 显示图像 */
    VkResult result2 = this->_context->framePresentFrame(imageIndex, this->_currentFrame);
    if (result2 == VK_ERROR_OUT_OF_DATE_KHR || result2 == VK_SUBOPTIMAL_KHR)
    {
        this->resetSwapChain();
        return;
    }
    else if (result2 != VK_SUCCESS)
    {
        GfxMgr::Log("GfxMgr", "Failed to present swap chain imag22222e!");
    }
   /*  // 0 1 0 1 0 1 */
    /**
     * 帧0	提交命令，信号量A	开始渲染帧0
     * 帧1	提交命令，信号量B	渲染帧0完成，开始渲染帧1
     * 帧0	等待帧0栅栏，信号量A复用	渲染帧1完成，显示帧0
     */
    this->_currentFrame = (this->_currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}
void GfxMgr::resetSwapChain()
{
    GfxMgr::Log("GfxMgr", "reset swap chain start...");
    vkDeviceWaitIdle(this->_context->getVkDevice());/*  // 等待所有操作完成 */
   /*  // 线清除， */
    this->_renderer->cleanRendererState();
    this->_context->cleanSwapChain();
   /*  // 后重置 */
    this->_context->resetSwapChain();
    this->_renderer->resetRendererState();
    GfxMgr::Log("GfxMgr", "reset swap chain end...");
}

/* // 读取shader内容 */
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
    vkCreateBuffer(this->_context->getVkDevice(), &bufferCreateInfo, nullptr, buffer);

    VkMemoryRequirements memReqs;
    VkMemoryAllocateInfo memAlloc{};
    memAlloc.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    vkGetBufferMemoryRequirements(this->_context->getVkDevice(), *buffer, &memReqs);
    memAlloc.allocationSize = memReqs.size;
    memAlloc.memoryTypeIndex = getMemoryTypeIndex(memReqs.memoryTypeBits, memoryPropertyFlags);
    vkAllocateMemory(this->_context->getVkDevice(), &memAlloc, nullptr, memory);

    if (data != nullptr)
    {
        void *mapped;
        vkMapMemory(this->_context->getVkDevice(), *memory, 0, size, 0, &mapped);
        memcpy(mapped, data, size);
        vkUnmapMemory(this->_context->getVkDevice(), *memory);
    }

    vkBindBufferMemory(this->_context->getVkDevice(), *buffer, *memory, 0);
    return VK_SUCCESS;
}
uint32_t GfxMgr::getMemoryTypeIndex(uint32_t typeBits, VkMemoryPropertyFlags properties)
{
    VkPhysicalDeviceMemoryProperties deviceMemoryProperties;
    vkGetPhysicalDeviceMemoryProperties(this->_context->getPhysicalDevice(), &deviceMemoryProperties);
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