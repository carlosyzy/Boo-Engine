#define GLFW_INCLUDE_VULKAN
#include "gfx-context.h"
#include "gfx-mgr.h"
#include "../../window/window.h"
#include "../../boo.h"

// 校验层
const bool enableValidationLayers = true;
std::vector<const char *> ValidationLayers = {"VK_LAYER_KHRONOS_validation"};
std::vector<const char *> DeviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};

GfxContext::GfxContext()
{
}
void GfxContext::init()
{
    this->_createInstance();
    this->_setupDebugMessenger();
    this->_createSurface();
    this->_initPhysicalDevice();
    this->_createLogicalDevice();
    this->_createCommandPool();
    this->_createDescriptorPool();
    this->_createSwapChainKHR();
    this->_createImageViews();
    this->_createSyncObjects();
    this->_createMsaaAttachmentTexture();
}
void GfxContext::_createInstance()
{
    if (!this->_checkValidationLayerSupport())
    {
        throw std::runtime_error("Validation layers requested, but not available!");
    }
    /**
     * 程序信息
     */
    VkApplicationInfo appInfo = {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Vulkan Engine";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "Vulkan Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;
    GfxMgr::Log("GfxContext", "VkApplicationInfo...");

    /**
     * 实例信息
     */
    VkInstanceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.flags = VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR; // 关键！

    createInfo.pApplicationInfo = &appInfo;
    /*  GfxMgr::Log("VkInstanceCreateInfo..."); */

    auto extensions = this->_getRequiredExtensions();
    createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    createInfo.ppEnabledExtensionNames = extensions.data();
    createInfo.enabledLayerCount = 0;
    createInfo.pNext = nullptr;

    if (enableValidationLayers)
    {
        VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
        createInfo.enabledLayerCount = static_cast<uint32_t>(ValidationLayers.size());
        createInfo.ppEnabledLayerNames = ValidationLayers.data();

        this->_populateDebugMessengerCreateInfo(debugCreateInfo);
        createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT *)&debugCreateInfo;
    }

    if (vkCreateInstance(&createInfo, nullptr, &this->_vkinstance) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create instance!");
    }
    /* GfxMgr::Log("VkInstance  Success..."); */
}
bool GfxContext::_checkValidationLayerSupport()
{
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());
    for (const char *layerName : ValidationLayers)
    {
        bool layerFound = false;

        for (const auto &layerProperties : availableLayers)
        {
            if (strcmp(layerName, layerProperties.layerName) == 0)
            {
                layerFound = true;
                break;
            }
        }

        if (!layerFound)
        {
            return false;
        }
    }
    return true;
}
/* 消息回调 创建一个getRequiredExtensions函数，这个函数根据是否启用校验层，返回所需的扩展列表 */
std::vector<const char *> GfxContext::_getRequiredExtensions()
{
    uint32_t glfwExtensionCount = 0;
    const char **glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    std::vector<const char *> extensions(glfwExtensions,
                                         glfwExtensions + glfwExtensionCount);
    if (enableValidationLayers)
    {
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME); // 添加调试扩展
    }
    extensions.push_back(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME);

#if defined(_WIN32) || defined(_WIN64)

#elif defined(__APPLE__)
    extensions.push_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
#else

#endif
   /*  extensions.push_back(VK_KHR_SURFACE_EXTENSION_NAME);  //windows renderdoc 报错 */

    return extensions;
}

void GfxContext::_setupDebugMessenger()
{
    if (!enableValidationLayers)
    {
        return;
    }

    VkDebugUtilsMessengerCreateInfoEXT createInfo;
    this->_populateDebugMessengerCreateInfo(createInfo);

    if (this->_createDebugUtilsMessengerEXT(this->_vkinstance, &createInfo, nullptr, &this->_debugMessenger) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to set up debug messenger!");
    }
}
void GfxContext::_populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &createInfo)
{
    createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    createInfo.pfnUserCallback = this->debugCallback;
}

/*  使用vkGetInstanceProcAddr获取某个api的函数指针 */
VkResult GfxContext::_createDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo, const VkAllocationCallbacks *pAllocator, VkDebugUtilsMessengerEXT *pDebugMessenger)
{
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    if (func != nullptr)
    {
        return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
    }
    else
    {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

void GfxContext::_createSurface()
{
    if (glfwCreateWindowSurface(this->_vkinstance, Boo::window->getWindow(), nullptr, &this->_surface) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create window surface!");
    }
}

void GfxContext::_initPhysicalDevice()
{
    this->_physicalDevice = VK_NULL_HANDLE;
    uint32_t deviceCount = 0;
     /* 第三个参数为空，则表示该函数为读取功能 */
    vkEnumeratePhysicalDevices(this->_vkinstance, &deviceCount, nullptr);
    if (deviceCount == 0)
    {
        throw std::runtime_error("Failed to find GPU with Vulkan Supported.");
    }
     /* GfxMgr::Log("Physical Device Count: " + deviceCount); */
    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(this->_vkinstance, &deviceCount, devices.data());

    for (const auto &device : devices)
    {
        if (this->_isDeviceSuitable(device))
        {
            this->_physicalDevice = device;
            break;
        }
    }
    if (this->_physicalDevice == VK_NULL_HANDLE)
    {
        throw std::runtime_error("Failed to find a suitable a GPU.");
    }
}

bool GfxContext::_isDeviceSuitable(VkPhysicalDevice device)
{
    VkPhysicalDeviceProperties deviceProperties;
    vkGetPhysicalDeviceProperties(device, &deviceProperties);

 /*     纹理压缩、64为浮点、多窗口渲染是否支持，通过下面函数查询 */
    VkPhysicalDeviceFeatures deviceFeatures;
    vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

    // 显卡支持集合着色器的判断条件
    bool isSupportSetShader = (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) && deviceFeatures.geometryShader;
   /*  std::cout << "isSupportSetShader: " << isSupportSetShader << std::endl; */

    QueueFamilyIndices indices = this->_findQueueFamilies(device);

    /* 由于我们只需要显示三角形，所以不需要额外特性，直接返回true即可，以上代码作为测试 */
    bool extensionsSupported = this->_checkDeviceExtensionSupport(device);

    bool swapChainAdequate = false;
    if (extensionsSupported)
    {
        SwapChainSupportDetails swapChainSupport = this->_querySwapChainSupport(device);
        swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
    }

    return indices.isComplete() && extensionsSupported && swapChainAdequate;
}

bool GfxContext::_checkDeviceExtensionSupport(VkPhysicalDevice device)
{
    uint32_t extensionCount;
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

    std::set<std::string> requiredExtensions(DeviceExtensions.begin(), DeviceExtensions.end());

    // for (auto extension : DeviceExtensions)
    // {
    //     std::cout << "extension: " << extension << std::endl;
    // }

    for (const auto &extension : availableExtensions)
    {
        requiredExtensions.erase(extension.extensionName);
    }

    return requiredExtensions.empty();
}

void GfxContext::_createLogicalDevice()
{
    QueueFamilyIndices indices = this->_findQueueFamilies(this->_physicalDevice);
    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::set<uint32_t> uniqueQueueFamilies = {indices.graphicsFamily, indices.presentFamily};
    float queuePriority = 1.0f;
    for (uint32_t queueFamily : uniqueQueueFamilies)
    {
        VkDeviceQueueCreateInfo queueCreateInfo{};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = queueFamily;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;
        queueCreateInfos.push_back(queueCreateInfo);
    }
    /* 指定应用程序使用的设备特性 */
    VkPhysicalDeviceFeatures deviceFeatures = {};
    deviceFeatures.geometryShader = VK_FALSE;
    deviceFeatures.tessellationShader = VK_FALSE; // 例如禁用曲面细分

  /*   // 创建逻辑设备 */
    VkDeviceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
    createInfo.pQueueCreateInfos = queueCreateInfos.data();
    createInfo.pEnabledFeatures = &deviceFeatures;

    /* // For macos */
    std::vector<const char *> localDeviceExtensions = DeviceExtensions;
    uint32_t extensionCount;
    vkEnumerateDeviceExtensionProperties(this->_physicalDevice, nullptr, &extensionCount, nullptr);
    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(this->_physicalDevice, nullptr, &extensionCount, availableExtensions.data());
    for (const auto &extension : availableExtensions)
    {
        if (strcmp(extension.extensionName, "VK_KHR_portability_subset") == 0)
        {
            localDeviceExtensions.push_back("VK_KHR_portability_subset");
            break;
        }
    }
    createInfo.enabledExtensionCount = static_cast<uint32_t>(localDeviceExtensions.size());
    createInfo.ppEnabledExtensionNames = localDeviceExtensions.data();

    if (enableValidationLayers)
    {
        createInfo.enabledLayerCount = static_cast<uint32_t>(ValidationLayers.size());
        createInfo.ppEnabledLayerNames = ValidationLayers.data();
    }
    else
    {
        createInfo.enabledLayerCount = 0;
    }

    if (vkCreateDevice(this->_physicalDevice, &createInfo, nullptr, &this->_vkdevice) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create logical device!");
    }

    /* / GfxMgr::Log("create logical device success"); */
    vkGetDeviceQueue(this->_vkdevice, indices.graphicsFamily, 0, &this->_graphicsQueue);
    vkGetDeviceQueue(this->_vkdevice, indices.presentFamily, 0, &this->_presentQueue);
}

/*
 * @brief 创建命令池
 */
void GfxContext::_createCommandPool()
{

   /*  // GfxMgr::Log("create command pool start..."); */
    QueueFamilyIndices queueFamilyIndices = this->_findQueueFamilies(this->_physicalDevice);

    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
   /*  // 每个命令池只能分配在单一类型队列上提交的Command Buffer。我们将记录绘图命令，这就是我们选择图形队列系列的原因。 */
    poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily;

   /*  // 创建Command Pool */
    if (vkCreateCommandPool(this->_vkdevice, &poolInfo, nullptr, &this->_commandPool) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create command pool!");
    }
    /* // GfxMgr::Log("create command pool success..."); */
}

void GfxContext::_createDescriptorPool()
{
    const uint32_t TOTAL_OBJECTS = 3000;
    std::vector<VkDescriptorPoolSize> poolSizes(2);
    poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    poolSizes[0].descriptorCount = MAX_FRAMES_IN_FLIGHT * TOTAL_OBJECTS; // 支持更多
    poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    poolSizes[1].descriptorCount = MAX_FRAMES_IN_FLIGHT * TOTAL_OBJECTS * 8; // 一个描述符最多只是8张图

    VkDescriptorPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
    poolInfo.pPoolSizes = poolSizes.data();
    poolInfo.maxSets = MAX_FRAMES_IN_FLIGHT * TOTAL_OBJECTS; // 支持更多描述符集
    if (vkCreateDescriptorPool(this->_vkdevice, &poolInfo, nullptr, &this->_descriptorPool) != VK_SUCCESS)

    {
        throw std::runtime_error("failed to create descriptor pool!");
    }
   /*  // GfxMgr::Log("create descriptor pool success...");

    // //需要销毁
    // if (this->_descriptorPool != VK_NULL_HANDLE) {
    //     vkDestroyDescriptorPool(this->_vkdevice, this->_descriptorPool, nullptr);
    //     this->_descriptorPool = VK_NULL_HANDLE;
    // } */
}

void GfxContext::resetSwapChain()
{
    this->_createSwapChainKHR();
    this->_createImageViews();
    this->_createSyncObjects();
    this->_createMsaaAttachmentTexture();
}

void GfxContext::cleanSwapChain()
{
    this->_cleanMsaaAttachmentTexture();
    this->_cleanSyncObjects();
    this->_cleanImageViews();
    this->_cleanSwapChainKHR();
}

void GfxContext::_createSwapChainKHR()
{
    /* // GfxMgr::Log("create swap chain start..."); */
    SwapChainSupportDetails swapChainSupport = this->_querySwapChainSupport(this->_physicalDevice);

    VkSurfaceFormatKHR surfaceFormat = this->_chooseSwapSurfaceFormat(swapChainSupport.formats);
    VkPresentModeKHR presentMode = this->_chooseSwapPresentMode(swapChainSupport.presentModes);
    VkExtent2D extent = this->_chooseSwapExtent(swapChainSupport.capabilities);

    uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
    if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount)
    {
        imageCount = swapChainSupport.capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = this->_surface;
    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1;                             /* // 用于指定每个图像所包含的层次。除了VR场景外，一般都是1. */
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;/*  // 指定我们在图像上的操作，此处我们将图像作为颜色来使用 */
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    /**
     * VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR
     * VK_COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR
     * VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR
     */

    QueueFamilyIndices indices = this->_findQueueFamilies(this->_physicalDevice);
    uint32_t queueFamilyIndices[] = {(uint32_t)indices.graphicsFamily, (uint32_t)indices.presentFamily};

    if (indices.graphicsFamily != indices.presentFamily)
    {
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndices;
    }
    else
    {
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        createInfo.queueFamilyIndexCount = 0;     /* // Optional */
        createInfo.pQueueFamilyIndices = nullptr; /* // Optional */
    }

    createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
    createInfo.presentMode = presentMode;
    createInfo.clipped = VK_TRUE;

    createInfo.oldSwapchain = VK_NULL_HANDLE;

    if (vkCreateSwapchainKHR(this->_vkdevice, &createInfo, nullptr, &this->_swapChain) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create swap chain!");
    }

   /*  // GfxMgr::Log("create swap chain success..."); */

    /* // 用vkGetSwapchainImagesKHR查询最终的图像数量 */
    vkGetSwapchainImagesKHR(this->_vkdevice, this->_swapChain, &imageCount, nullptr);
    /* // 创建交换链图像数组 */
    this->_swapChainImages.resize(imageCount);
    vkGetSwapchainImagesKHR(this->_vkdevice, this->_swapChain, &imageCount, this->_swapChainImages.data());
  /*   // 同时还需要将这个格式和分辨率保存下来。 */
    this->_swapChainImageFormat = surfaceFormat.format;
    this->_swapChainExtent = extent;
}

/* // https://www.iogl.cn/tutorial/swapchain */
VkSurfaceFormatKHR GfxContext::_chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats)
{
 /*    // 优先选择 B8G8R8A8_UNORM + SRGB */
    for (const auto &availableFormat : availableFormats)
    {
        if (availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
        {
            return availableFormat;
        }
    }
 /*    // 次优选择：任何带 Alpha 的格式 */
    for (const auto &format : availableFormats)
    {
        if (format.format == VK_FORMAT_B8G8R8A8_UNORM ||
            format.format == VK_FORMAT_R8G8B8A8_UNORM)
        {
            return format;
        }
    }

    return availableFormats[0];
}
/* // https://www.iogl.cn/tutorial/swapchain */
VkPresentModeKHR GfxContext::_chooseSwapPresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes)
{
    for (const auto &availablePresentMode : availablePresentModes)
    {
        if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
        {
            return availablePresentMode;
        }
    }

    return VK_PRESENT_MODE_FIFO_KHR;
}
VkExtent2D GfxContext::_chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities)
{
    if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
    {
        return capabilities.currentExtent;
    }
    else
    {
        int width, height;
        glfwGetFramebufferSize(Boo::window->getWindow(), &width, &height);

        VkExtent2D actualExtent = {
            static_cast<uint32_t>(width),
            static_cast<uint32_t>(height)};

        actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
        actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

        return actualExtent;
    }
}
void GfxContext::_cleanSwapChainKHR()
{
  /*   // 销毁交换链（Swapchain） */
    if (this->_swapChain != VK_NULL_HANDLE)
    {
        vkDestroySwapchainKHR(this->_vkdevice, this->_swapChain, nullptr);
        this->_swapChain = VK_NULL_HANDLE;
    }
}

/**
 * @brief 创建交换链图像视图
 *
 */
void GfxContext::_createImageViews()
{
   /*  // GfxMgr::Log("create ImageViews start..."); */
    this->_swapChainImageViews.resize(this->_swapChainImages.size());
    for (size_t i = 0; i < this->_swapChainImages.size(); i++)
    {
        VkImageViewCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        createInfo.image = this->_swapChainImages[i];
        createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        createInfo.format = this->_swapChainImageFormat;
        createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        createInfo.subresourceRange.baseMipLevel = 0;
        createInfo.subresourceRange.levelCount = 1;
        createInfo.subresourceRange.baseArrayLayer = 0;
        createInfo.subresourceRange.layerCount = 1;
        if (vkCreateImageView(this->_vkdevice, &createInfo, nullptr, &this->_swapChainImageViews[i]) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create image views!");
        }
    }
   /*  //  GfxMgr::Log("create ImageViews success..."); */
}
void GfxContext::_cleanImageViews()
{
    for (auto imageView : this->_swapChainImageViews)
    {
        vkDestroyImageView(this->_vkdevice, imageView, nullptr);
    }
    this->_swapChainImageViews.clear();
    /* // GfxMgr::Log("GfxContext", "clean ImageViews success..."); */
}

void GfxContext::_createSyncObjects()
{
    /* // GfxMgr::Log("GfxContext", "create sync objects start..."); */

    size_t imageCount = this->_swapChainImages.size(); /* // 使用交换链图像数量 */
    this->_imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    this->_renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    this->_inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);
    this->_imagesInFlight.resize(imageCount, VK_NULL_HANDLE);

    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    {
        vkCreateSemaphore(this->_vkdevice, &semaphoreInfo, nullptr, &this->_imageAvailableSemaphores[i]);
        vkCreateSemaphore(this->_vkdevice, &semaphoreInfo, nullptr, &this->_renderFinishedSemaphores[i]);
        vkCreateFence(this->_vkdevice, &fenceInfo, nullptr, &this->_inFlightFences[i]);
    }
   /*  // GfxMgr::Log("GfxContext", "create sync objects success..."); */
}
void GfxContext::_cleanSyncObjects()
{
   /*  // 1. 销毁旧的同步对象 */
    for (auto &fence : this->_inFlightFences)
    {
        if (fence != VK_NULL_HANDLE)
        {
            vkDestroyFence(this->_vkdevice, fence, nullptr);
        }
    }
    this->_inFlightFences.clear();

    /* // 1. 销毁旧的同步对象 */
    for (auto &flight : this->_imagesInFlight)
    {
        if (flight != VK_NULL_HANDLE)
        {
            vkDestroyFence(this->_vkdevice, flight, nullptr);
        }
    }
    this->_imagesInFlight.clear();

    for (auto &semaphore : this->_imageAvailableSemaphores)
    {
        if (semaphore != VK_NULL_HANDLE)
        {
            vkDestroySemaphore(this->_vkdevice, semaphore, nullptr);
        }
    }
    this->_imageAvailableSemaphores.clear();

    for (auto &semaphore : this->_renderFinishedSemaphores)
    {
        if (semaphore != VK_NULL_HANDLE)
        {
            vkDestroySemaphore(this->_vkdevice, semaphore, nullptr);
        }
    }
    this->_renderFinishedSemaphores.clear();
}
/**
 * @brief 创建MSAA附件纹理
 *
 */
void GfxContext::_createMsaaAttachmentTexture()
{
    this->_colorMsaaTexture = new GfxTexture(this);
    this->_colorMsaaTexture->createImage(this->_swapChainExtent.width, this->_swapChainExtent.height, this->_swapChainImageFormat, VK_IMAGE_TILING_OPTIMAL,
                                         VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT|VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
                                         VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, MsaaSamples);
    std::cout << "this->_colorMsaaTexture createImage end" << std::endl;
    this->_colorMsaaTexture->createImageView(this->_swapChainImageFormat, VK_IMAGE_ASPECT_COLOR_BIT);
    std::cout << "this->_colorMsaaTexture createImageView end" << std::endl;
    this->_depthMsaaTexture = new GfxTexture(this);
    this->_depthMsaaTexture->createImage(this->_swapChainExtent.width, this->_swapChainExtent.height, VK_FORMAT_D32_SFLOAT, VK_IMAGE_TILING_OPTIMAL,
                                         VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
                                         VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, MsaaSamples);
    this->_depthMsaaTexture->createImageView(VK_FORMAT_D32_SFLOAT, VK_IMAGE_ASPECT_DEPTH_BIT);
}
/**
 * @brief 清理MSAA附件纹理
 *
 */
void GfxContext::_cleanMsaaAttachmentTexture()
{
    if (this->_colorMsaaTexture != nullptr)
    {
        delete this->_colorMsaaTexture;
        this->_colorMsaaTexture = nullptr;
    }
    if (this->_depthMsaaTexture != nullptr)
    {
        delete this->_depthMsaaTexture;
        this->_depthMsaaTexture = nullptr;
    }
}

void GfxContext::frameFencesPrepare(size_t currentFrame)
{
    /* // std::cout << "update1" << this->_inFlightFences[this->_currentFrame] << std::endl;
    // 阻塞CPU，直到 GPU 完成 上一帧 的所有渲染任务（即关联到这个栅栏的命令全部执行完毕）。
    // 相当于对 GPU 说："你先把上一帧的活干完，我再给你新任务"。
    // 防止 CPU 过快提交新帧，导致 GPU 还在处理旧帧时，资源被意外覆盖（比如命令缓冲区被重用）
    // 确保交换链图像、Uniform 缓冲区等不会被同时读写。 */
    vkWaitForFences(this->_vkdevice, 1, &this->_inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);
    /* // 重置任务完成标志
    // 将栅栏的状态从 "已触发"（ signaled） 重置为 "未触发"（unsignaled）。
    // 相当于把 "任务完成" 的标志牌翻回 "未完成" 状态。 */
    vkResetFences(this->_vkdevice, 1, &this->_inFlightFences[currentFrame]);
}
VkResult GfxContext::frameAcquireNextImage(uint32_t *imageIndex, size_t currentFrame)
{
    return vkAcquireNextImageKHR(this->_vkdevice, this->_swapChain, UINT64_MAX, this->_imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, imageIndex);
}
void GfxContext::frameWaitImageInUse(uint32_t imageIndex, size_t currentFrame)
{
    /**
     * 检查当前获取索引图像是否已被其他帧使用
     */
    if (this->_imagesInFlight[imageIndex] != VK_NULL_HANDLE)
    {
       /*  // 先检查状态避免不必要等待 */
        if (vkGetFenceStatus(this->_vkdevice, this->_imagesInFlight[imageIndex]) == VK_NOT_READY)
        {
            /**
             * VK_NOT_READY:栅栏未触发，GPU仍在处理关联的操作
             * VK_SUCCESS:栅栏已触发，关联的GPU操作已完成，可以安全复用图像
             */
           /*  // std::cout << "renderer update:'VK_ERROR_OUT_OF_DATE_KHR',The image is being used by other frames and needs to wait." << std::endl; */
            vkWaitForFences(this->_vkdevice, 1, &this->_imagesInFlight[imageIndex], VK_TRUE, UINT64_MAX);
        }
    }
    this->_imagesInFlight[imageIndex] = this->_inFlightFences[currentFrame];
}
void GfxContext::frameSubmitCommands(uint32_t imageIndex, const std::vector<VkCommandBuffer> &commandBuffers, size_t currentFrame)
{
   /*  // 等待信号量配置（图像可用时触发） */
    VkSemaphore waitSemaphores[] = {this->_imageAvailableSemaphores[currentFrame]};
    VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};

   /*  // 完成渲染后触发的信号量 */
    VkSemaphore signalSemaphores[] = {this->_renderFinishedSemaphores[currentFrame]};

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;
    submitInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());
    submitInfo.pCommandBuffers = commandBuffers.data();
    submitInfo.signalSemaphoreCount = 1;

    submitInfo.pSignalSemaphores = signalSemaphores;

    if (vkQueueSubmit(this->_graphicsQueue, 1, &submitInfo, this->_inFlightFences[currentFrame]) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to submit draw command buffer!");
    }
}
VkResult GfxContext::framePresentFrame(uint32_t imageIndex, size_t currentFrame)
{
    VkSemaphore signalSemaphores[] = {this->_renderFinishedSemaphores[currentFrame]};
    VkSwapchainKHR swapChains[] = {this->_swapChain};
    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;
    presentInfo.pImageIndices = &imageIndex;
    return vkQueuePresentKHR(this->_presentQueue, &presentInfo);
}

SwapChainSupportDetails GfxContext::_querySwapChainSupport(VkPhysicalDevice device)
{
    SwapChainSupportDetails details;

    /* // 与交换链相关的函数都需要device和surface这两个参数
    // 查询基础表面特性 */
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, this->_surface, &details.capabilities);

   /*  // 查询表面支持格式 */
    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, this->_surface, &formatCount, nullptr);
    if (formatCount != 0)
    {
        details.formats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, this->_surface, &formatCount, details.formats.data());
    }

    /* // 查询表面支持呈现模式 */
    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, this->_surface, &presentModeCount, nullptr);

    if (presentModeCount != 0)
    {
        details.presentModes.resize(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, this->_surface, &presentModeCount, details.presentModes.data());
    }

    return details;
}

QueueFamilyIndices GfxContext::_findQueueFamilies(VkPhysicalDevice device)
{
    QueueFamilyIndices indices;

    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

    int index = 0;
    for (const auto &queueFamily : queueFamilies)
    {
        if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
        { /* // 目前，我们只要寻找图形队列即可 */
            indices.graphicsFamily = index;
        }

        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(device, index, this->_surface, &presentSupport);
        if (presentSupport)
        {
            indices.presentFamily = index;
        }

        if (indices.isComplete())
        {
            break;
        }
        index++;
    }

    return indices;
}
GfxContext::~GfxContext()
{
}

