#pragma once
#include <vulkan/vulkan_core.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <vector>
#include <set>
#include <map>
#include "gfx-texture.h"

#include "gfx-struct.h"
class GfxContext
{
private:
    /**
     * @brief Vulkan实例句柄
     */
    VkInstance _vkinstance;
    void _createInstance();
    bool _checkValidationLayerSupport();
    std::vector<const char *> _getRequiredExtensions();
    void _populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &createInfo);
    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType,
                                                        const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData, void *pUserData)
    {
        std::cerr << "Error Message: " << pCallbackData->pMessage << std::endl;
        return VK_FALSE;
    }
    VkDebugUtilsMessengerEXT _debugMessenger;
    void _setupDebugMessenger();
    VkResult _createDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo, const VkAllocationCallbacks *pAllocator, VkDebugUtilsMessengerEXT *pDebugMessenger);

    /**
     * @brief Vulkan表面句柄
     *  用于显示的窗口句柄
     */
    VkSurfaceKHR _surface; 
    /**
     * @brief 创建Vulkan表面
     */
    void _createSurface();

    /**
     * @brief 物理设备句柄
     */
    VkPhysicalDevice _physicalDevice;
    /**
     * @brief 初始化物理设备
     */
    void _initPhysicalDevice();
    bool _isDeviceSuitable(VkPhysicalDevice device);
    bool _checkDeviceExtensionSupport(VkPhysicalDevice device);
    

    /**
     * @brief 逻辑设备句柄
     */
    VkDevice _vkdevice;
    /**
     * 用来存储逻辑设备的图形呈现队列句柄
     */
    VkQueue _presentQueue; 
    VkQueue _graphicsQueue;
    void _createLogicalDevice();

    /**
     * @brief 命令池
     */
    VkCommandPool _commandPool;
    void _createCommandPool();
    /**
     * @brief 描述符池
     */
    VkDescriptorPool _descriptorPool;
    void _createDescriptorPool();

    /**
     * @brief 交换链
     * 本质上是一个等待被呈现在屏幕上的图像队列
     * 程序会在图像上进行绘制，然后再将其返回到队列中
     * Swap Chain的一般目的是使图像的呈现与屏幕的刷新率同步
     */
    VkSwapchainKHR _swapChain;
    std::vector<VkImage> _swapChainImages;
    VkFormat _swapChainImageFormat;
    VkExtent2D _swapChainExtent;
    void _createSwapChainKHR();
    VkSurfaceFormatKHR _chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats);
    VkPresentModeKHR _chooseSwapPresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes);
    VkExtent2D _chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities);
    void _cleanSwapChainKHR();
    void _createSwapChain();

    /**
     * @brief 交换链图像视图
     * 为了在PipeLine中使用任何VkImage，包括Swap Chain中的那些Image，
     * 我们必须创建一个VkImageView对象。ImageView实际上是对图像的一种观察。
     * 它描述了如何访问图像以及访问图像的哪一部分，我们必须通过ImageView来能够读取Image
     */
    std::vector<VkImageView> _swapChainImageViews;
    void _createImageViews();
    void _cleanImageViews();

    /**
     * 表示 交换链图像已经准备好可以被渲染。
     * vkAcquireNextImageKHR
     * 在 vkQueueSubmit() 中作为 等待条件（告诉 GPU："必须等图像准备好了才能开始渲染"）
     */
    std::vector<VkSemaphore> _imageAvailableSemaphores;
    /**
     * 表示 当前帧的渲染已经完成。
     * 由 vkQueueSubmit() 提交的命令执行完成后触发
     * 在 vkQueuePresentKHR() 中作为 等待条件（告诉 GPU："必须等渲染完成了才能显示"）
     */
    std::vector<VkSemaphore> _renderFinishedSemaphores;
    /**
     * 帧飞行栅栏
     * 表示 整个帧的 GPU 工作（提交的命令）是否完成。
     * vkQueueSubmit()
     * 通过 vkWaitForFences() 让 CPU 等待 GPU 完成工作
     * 通过 vkResetFences() 重置状态供下一帧使用
     */
    std::vector<VkFence> _inFlightFences;
    /**
     * 图像占用栅栏
     * 跟踪 每个交换链图像正在被哪一帧使用（防止多帧同时写同一图像
     * 数组大小 = 交换链图像数量
     * 当 vkAcquireNextImageKHR() 获取图像时，检查并关联当前帧的栅栏
     */
    std::vector<VkFence> _imagesInFlight;
    void _createSyncObjects();
    void _cleanSyncObjects();

    /**
     * 多重采样-颜色附件贴图和深度附件贴图
     */
    GfxTexture *_colorMsaaTexture;
    GfxTexture *_depthMsaaTexture;
    void _createMsaaAttachmentTexture();
    void _cleanMsaaAttachmentTexture();
    

    QueueFamilyIndices _findQueueFamilies(VkPhysicalDevice device);
    SwapChainSupportDetails _querySwapChainSupport(VkPhysicalDevice device);
public:
    GfxContext();
    void init();
    void resetSwapChain();
    void cleanSwapChain();

    void frameFencesPrepare(size_t currentFrame);
    VkResult frameAcquireNextImage(uint32_t *imageIndex, size_t currentFrame);
    void frameWaitImageInUse(uint32_t imageIndex, size_t currentFrame);
    void frameSubmitCommands(uint32_t imageIndex, const std::vector<VkCommandBuffer> &commandBuffers, size_t currentFrame);
    VkResult framePresentFrame(uint32_t imageIndex, size_t currentFrame);
    

    VkDevice getVkDevice() const
    {
        return this->_vkdevice;
    }
    VkPhysicalDevice getPhysicalDevice() const
    {
        return this->_physicalDevice;
    }
    VkFormat getSwapChainImageFormat() const
    {
        return this->_swapChainImageFormat;
    }
    VkCommandPool getCommandPool() const
    {
        return this->_commandPool;
    }
    VkQueue getPresentQueue() const
    {
        return this->_presentQueue;
    }
    VkQueue getGraphicsQueue() const
    {
        return this->_graphicsQueue;
    }
    VkExtent2D getSwapChainExtent() const
    {
        return this->_swapChainExtent;
    }
    std::vector<VkImageView> &getSwapChainImageViews()
    {
        return this->_swapChainImageViews;
    }
    std::vector<VkImage> &getSwapChainImages()
    {
        return this->_swapChainImages;
    }
    VkDescriptorPool getDescriptorPool() const
    {
        return this->_descriptorPool;
    }
    GfxTexture *getColorMsaaTexture() const
    {
        return this->_colorMsaaTexture;
    }
    GfxTexture *getDepthMsaaTexture() const
    {
        return this->_depthMsaaTexture;
    }

    ~GfxContext();
};
