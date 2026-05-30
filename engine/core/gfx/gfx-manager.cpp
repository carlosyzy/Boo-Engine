
#define GLFW_INCLUDE_VULKAN
#include "gfx.h"
#include "gfx-manager.h"
#include "gfx-context.h"
#include "gfx-renderer.h"

#include "base/gfx-material.h"
#include "base/gfx-mesh.h"
#include "base/gfx-render-texture.h"
#include "../../log.h"

GfxManager *GfxManager::getInstance()
{
    static GfxManager _instance;
    return &_instance;
}

GfxManager::GfxManager()
{
    LOGI("[Gfx Context]:INIT");
    if (volkInitialize() != VK_SUCCESS)
    {
        std::cout << "Vulkan Driver Not Installed!" << std::endl;
        throw std::runtime_error("failed to initialize volk!");
        return;
    }
    uint32_t instanceVersion = 0;
    VkResult result = vkEnumerateInstanceVersion(&instanceVersion);
    if (result == VK_SUCCESS)
    {
        uint32_t major = VK_VERSION_MAJOR(instanceVersion);
        uint32_t minor = VK_VERSION_MINOR(instanceVersion);
        uint32_t patch = VK_VERSION_PATCH(instanceVersion);
        LOGI("Gfx :: Vulkan Instance (Driver) Version:%d.%d.%d", major, minor, patch);
    }
}
void GfxManager::init(Windows *windows)
{
    Gfx::_context = new GfxContext();
    Gfx::_context->init(windows);
    this->_initRenderer();
}
void GfxManager::init(Android *android)
{
    Gfx::_context = new GfxContext();
    Gfx::_context->init(android);
    this->_initRenderer();
}
void GfxManager::init(MacOS *macos)
{
    Gfx::_context = new GfxContext();
    Gfx::_context->init(macos);
    this->_initRenderer();
}
void GfxManager::init(HarmonyOS *harmonyos)
{
    Gfx::_context = new GfxContext();
    Gfx::_context->init(harmonyos);
    this->_initRenderer();
}
void GfxManager::_initRenderer()
{
    Gfx::_renderer = new GfxRenderer();
    Gfx::_renderer->init();
}
void GfxManager::resize(int width, int height)
{
    Gfx::_viewWidth = width;
    Gfx::_viewHeight = height;
}
void GfxManager::update(float dt)
{
    Gfx::_time += dt;
    Gfx::_drawCount = 0;
    std::chrono::high_resolution_clock::time_point frameStart = std::chrono::high_resolution_clock::now();
    Gfx::_context->frameFencesPrepare(this->_currentFrame);
    uint32_t imageIndex;
    VkResult result1 = Gfx::_context->frameAcquireNextImage(&imageIndex, this->_currentFrame);
    if (result1 == VK_ERROR_OUT_OF_DATE_KHR || result1 == VK_SUBOPTIMAL_KHR)
    {
        std::cout << "Gfx : Render :: update:'VK_ERROR_OUT_OF_DATE_KHR',The window size might have changed, and the swap chain needs to be recreated." << std::endl;
        this->resetSwapChain();
        return;
    }
    Gfx::_context->frameWaitImageInUse(imageIndex, this->_currentFrame);
    std::chrono::high_resolution_clock::time_point submitStart = std::chrono::high_resolution_clock::now();

    Gfx::_renderer->frameRendererBefore();

    std::vector<VkCommandBuffer> commandBuffers;
    Gfx::_renderer->frameRenderer(imageIndex, commandBuffers);
    Gfx::_context->frameSubmitCommands(imageIndex, commandBuffers, this->_currentFrame);
    VkResult result2 = Gfx::_context->framePresentFrame(imageIndex, this->_currentFrame);
    if (result2 == VK_ERROR_OUT_OF_DATE_KHR || result2 == VK_SUBOPTIMAL_KHR)
    {
        Gfx::_renderer->frameRendererAfter();
        this->resetSwapChain();
        return;
    }
    else if (result2 != VK_SUCCESS)
    {
        std::cout << "Gfx : Render :: Failed to present swap chain imag22222e!" << std::endl;
    }
    this->_currentFrame = (this->_currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
    Gfx::_renderer->frameRendererAfter();
    std::chrono::high_resolution_clock::time_point frameEnd = std::chrono::high_resolution_clock::now();
    Gfx::_frameTime = std::chrono::duration_cast<std::chrono::nanoseconds>(frameEnd - frameStart).count() / 1000000.0f;
    Gfx::_submitTime = std::chrono::duration_cast<std::chrono::nanoseconds>(frameEnd - submitStart).count() / 1000000.0f;
}
void GfxManager::resetSwapChain()
{
    vkDeviceWaitIdle(Gfx::_context->getVkDevice());
    Gfx::_renderer->_cleanRendererState();
    Gfx::_context->cleanSwapChain();
    Gfx::_context->resetSwapChain();
    Gfx::_renderer->_resetRendererState();
    std::cout << "GfxManager :reset swap chain end..." << std::endl;
}

void GfxManager::createPipeline(std::string pipelineName, GfxRendererState rendererState)
{
    Gfx::_renderer->createPipeline(pipelineName, rendererState);
}

GfxTexture *GfxManager::createTexture(std::string textureUuid, uint32_t width, uint32_t height, uint32_t channels,int textureType, const std::vector<uint8_t> *pixels, GfxTextureFormat format)
{
    return Gfx::_renderer->createTexture(textureUuid, width, height, channels, textureType, pixels, format);
}
void GfxManager::destroyTexture(GfxTexture *texture)
{
    Gfx::_renderer->destroyTexture(texture);
}

GfxShader *GfxManager::createGlslShader(const std::string &shaderName, const std::string &shaderType, const std::string &data, const std::map<std::string, int> &macros)
{
    return Gfx::_renderer->createGlslShader(shaderName, shaderType, data, macros);
}
GfxShader *GfxManager::createSpirvShader(const std::string &shaderName, const std::vector<uint32_t> &data)
{
    return Gfx::_renderer->createSpirvShader(shaderName, data);
}
void GfxManager::destroyShader(GfxShader *shader)
{
    Gfx::_renderer->destroyShader(shader);
}
GfxMesh *GfxManager::createMesh(std::string meshUuid, int meshType, const std::vector<float> &vertices, const std::vector<uint32_t> &indices)
{
    return Gfx::_renderer->createMesh(meshUuid, meshType, vertices, indices);
}
GfxMesh *GfxManager::createUIMesh(std::string meshUuid, int meshType, const std::vector<float> &vertices, const std::vector<uint32_t> &indices)
{
    return Gfx::_renderer->createUIMesh(meshUuid, meshType, vertices, indices);
}
void GfxManager::destroyMesh(GfxMesh *mesh)
{
    Gfx::_renderer->destroyMesh(mesh);
}

void GfxManager::createRenderQueue(std::string renderId, int priority, uint32_t width, uint32_t height)
{
    Gfx::_renderer->createRenderQueue(renderId, priority, width, height);
}
void GfxManager::setRenderQueuePriority(std::string renderId, int priority)
{
    Gfx::_renderer->setRenderQueuePriority(renderId, priority);
}
GfxRenderTexture *GfxManager::getRenderQueueRT(std::string renderId)
{
    return Gfx::_renderer->getRenderQueueRT(renderId);
}
void GfxManager::resizeRenderQueue(std::string renderId, uint32_t width, uint32_t height)
{
    Gfx::_renderer->resizeRenderQueue(renderId, width, height);
}
void GfxManager::delRenderQueue(std::string renderId)
{
    Gfx::_renderer->delRenderQueue(renderId);
}
void GfxManager::setMainLitDir(float x, float y, float z, float w)
{
    Gfx::_mainLitDir[0] = x;
    Gfx::_mainLitDir[1] = y;
    Gfx::_mainLitDir[2] = z;
    Gfx::_mainLitDir[3] = w;
}
void GfxManager::setMainLitColor(float r, float g, float b, float a)
{
    Gfx::_mainLitColor[0] = r;
    Gfx::_mainLitColor[1] = g;
    Gfx::_mainLitColor[2] = b;
    Gfx::_mainLitColor[3] = a;
}
void GfxManager::submitRenderData(std::string renderId, const std::array<float, 16> &viewMatrix, const std::array<float, 16> &projMatrix, bool isOnScreen, std::array<float, 4> &cameraPosition)
{
    Gfx::_renderer->submitRenderData(renderId, viewMatrix, projMatrix, isOnScreen, cameraPosition);
}
void GfxManager::submitRenderObject(std::string renderId, GfxMaterial *material, GfxMesh *mesh)
{
    Gfx::_renderer->submitRenderObject(renderId, material, mesh);
}
std::vector<char> GfxManager::readShaderFile(const std::string &filename)
{
    std::ifstream file(filename, std::ios::ate | std::ios::binary);
    if (!file.is_open())
    {
        throw std::runtime_error("failed to open file!");
    }
    size_t fileSize = (size_t)file.tellg();
    std::vector<char> buffer(fileSize);
    file.seekg(0);
    file.read(buffer.data(), fileSize);
    file.close();
    return buffer;
}

VkResult GfxManager::createBuffer(VkBufferUsageFlags usageFlags, VkMemoryPropertyFlags memoryPropertyFlags, VkBuffer *buffer, VkDeviceMemory *memory, VkDeviceSize size, const void *data)
{
    VkDevice device = Gfx::_context->getVkDevice();
    VkBufferCreateInfo bufferCreateInfo{};
    bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferCreateInfo.usage = usageFlags;
    bufferCreateInfo.size = size;
    bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    VkResult result = vkCreateBuffer(device, &bufferCreateInfo, nullptr, buffer);
    if (result != VK_SUCCESS)
        return result;
    VkMemoryRequirements memReqs;
    vkGetBufferMemoryRequirements(device, *buffer, &memReqs);
    VkMemoryAllocateInfo memAlloc{};
    memAlloc.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    memAlloc.allocationSize = memReqs.size;
    uint32_t memTypeIndex = getMemoryTypeIndex(memReqs.memoryTypeBits, memoryPropertyFlags);
    if (memTypeIndex == UINT32_MAX)
    {
        vkDestroyBuffer(device, *buffer, nullptr);
        *buffer = VK_NULL_HANDLE;
        return VK_ERROR_FEATURE_NOT_PRESENT;
    }
    memAlloc.memoryTypeIndex = memTypeIndex;
    result = vkAllocateMemory(device, &memAlloc, nullptr, memory);
    if (result != VK_SUCCESS)
    {
        vkDestroyBuffer(device, *buffer, nullptr);
        *buffer = VK_NULL_HANDLE;
        return result;
    }

    result = vkBindBufferMemory(device, *buffer, *memory, 0);
    if (result != VK_SUCCESS)
    {
        vkFreeMemory(device, *memory, nullptr);
        vkDestroyBuffer(device, *buffer, nullptr);
        *memory = VK_NULL_HANDLE;
        *buffer = VK_NULL_HANDLE;
        return result;
    }

    if (data != nullptr)
    {
        if (!(memoryPropertyFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT))
        {
            vkFreeMemory(device, *memory, nullptr);
            vkDestroyBuffer(device, *buffer, nullptr);
            *memory = VK_NULL_HANDLE;
            *buffer = VK_NULL_HANDLE;
            return VK_ERROR_MEMORY_MAP_FAILED;
        }

        void *mapped;
        result = vkMapMemory(device, *memory, 0, size, 0, &mapped);
        if (result != VK_SUCCESS)
        {
            vkFreeMemory(device, *memory, nullptr);
            vkDestroyBuffer(device, *buffer, nullptr);
            *memory = VK_NULL_HANDLE;
            *buffer = VK_NULL_HANDLE;
            return result;
        }
        memcpy(mapped, data, size);
        if (!(memoryPropertyFlags & VK_MEMORY_PROPERTY_HOST_COHERENT_BIT))
        {
            VkMappedMemoryRange mappedRange{};
            mappedRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
            mappedRange.memory = *memory;
            mappedRange.offset = 0;
            mappedRange.size = VK_WHOLE_SIZE;
            vkFlushMappedMemoryRanges(device, 1, &mappedRange);
        }
        vkUnmapMemory(device, *memory);
    }
    return VK_SUCCESS;
}
uint32_t GfxManager::getMemoryTypeIndex(uint32_t typeBits, VkMemoryPropertyFlags properties)
{
    VkPhysicalDeviceMemoryProperties deviceMemoryProperties;
    vkGetPhysicalDeviceMemoryProperties(Gfx::_context->getPhysicalDevice(), &deviceMemoryProperties);

    for (uint32_t i = 0; i < deviceMemoryProperties.memoryTypeCount; i++)
    {
        if ((typeBits & 1) == 1)
        {
            if ((deviceMemoryProperties.memoryTypes[i].propertyFlags & properties) == properties)
                return i;
        }
        typeBits >>= 1;
    }
    return UINT32_MAX;
}
void GfxManager::copyBuffer(VkBuffer src, VkBuffer dst, VkDeviceSize size)
{
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = Gfx::_context->getCommandPool();
    allocInfo.commandBufferCount = 1;

    VkCommandBuffer cmd;
    vkAllocateCommandBuffers(Gfx::_context->getVkDevice(), &allocInfo, &cmd);

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    vkBeginCommandBuffer(cmd, &beginInfo);

    VkBufferCopy copyRegion{};
    copyRegion.size = size;
    vkCmdCopyBuffer(cmd, src, dst, 1, &copyRegion);

    vkEndCommandBuffer(cmd);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &cmd;

    VkQueue queue = Gfx::_context->getGraphicsQueue();
    vkQueueSubmit(queue, 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(queue);

    vkFreeCommandBuffers(Gfx::_context->getVkDevice(), Gfx::_context->getCommandPool(), 1, &cmd);
}
GfxManager::~GfxManager()
{
}