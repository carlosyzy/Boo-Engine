
#include "macos.h"
#include "boo.h"
#include "log.h"
#include "libs/stb/stb_image.h"
#include "core/game.h"

MacOS::MacOS(int width, int height)
{
    this->_width = width;
    this->_height = height;
#if defined(BOO_PLATFORM_MACOS)
    this->_window = nullptr;
    if (!glfwInit())
    {
        return;
    }
    this->_initAssetsRoot();
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE); // 窗口不可调整大小
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    const int C_GLFW_TRANSPARENT_FRAMEBUFFER = 131082;
    glfwWindowHint(C_GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);

    // 创建窗口
    this->_window = glfwCreateWindow(this->_width, this->_height, "", nullptr, nullptr);
    if (!this->_window)
    {
        glfwTerminate();
        return;
    }
    glfwSetWindowSizeLimits(this->_window, this->_width, this->_height, GLFW_DONT_CARE, GLFW_DONT_CARE);
    glfwSetWindowUserPointer(this->_window, this);
    glfwSetCursorPosCallback(this->_window, MacOS::cursorPosCallback);
    glfwSetMouseButtonCallback(this->_window, MacOS::mouseButtonCallback);
    glfwSetKeyCallback(this->_window, MacOS::keyCallback);
    glfwSetWindowSizeCallback(this->_window, MacOS::windowSizeCallback);
    glfwSetScrollCallback(this->_window, MacOS::scrollCallback);
    this->onWindowSize();
#endif
}
#if defined(BOO_PLATFORM_MACOS)
GLFWwindow *MacOS::getWindow()
{
    return this->_window;
}
void MacOS::_initAssetsRoot()
{
    uint32_t size = 0;
    _NSGetExecutablePath(nullptr, &size);
    std::vector<char> buffer(size);
    if (_NSGetExecutablePath(buffer.data(), &size) == 0)
    {
        try
        {
            std::string appPath = std::string(buffer.data());
            this->_assetsRoot = std::filesystem::path(appPath).parent_path().string();
            this->_assetsRoot += "/assets";
            std::cout << "assets path:" << this->_assetsRoot << std::endl;
        }
        catch (const std::filesystem::filesystem_error &ex)
        {
            std::cerr << "file system error: " << ex.what() << std::endl;
        }
    }
}

void MacOS::cursorPosCallback(GLFWwindow *window, double xpos, double ypos)
{
    void *userPointer = glfwGetWindowUserPointer(window);
    if (userPointer == nullptr)
    {
        return;
    }
    MacOS *manager = static_cast<MacOS *>(userPointer);
    if (manager == nullptr)
    {
        return;
    }
    manager->onCursorPos(xpos, ypos);
}
void MacOS::mouseButtonCallback(GLFWwindow *window, int button, int action, int mods)
{
    void *userPointer = glfwGetWindowUserPointer(window);
    if (userPointer == nullptr)
    {
        return;
    }
    MacOS *manager = static_cast<MacOS *>(userPointer);
    if (manager == nullptr)
    {
        return;
    }
    manager->onMouseButton(button, action, mods);
}
void MacOS::windowSizeCallback(GLFWwindow *window, int width, int height)
{
    void *userPointer = glfwGetWindowUserPointer(window);
    if (userPointer == nullptr)
    {
        return;
    }
    MacOS *manager = static_cast<MacOS *>(userPointer);
    if (manager == nullptr) // 检查非空
    {
        return;
    }
    manager->onWindowSize();
};
void MacOS::keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    void *userPointer = glfwGetWindowUserPointer(window);
    if (userPointer == nullptr)
    {
        return;
    }
    MacOS *manager = static_cast<MacOS *>(userPointer);
    if (manager == nullptr) // 检查非空
    {
        return;
    }
    manager->onKey(key, scancode, action, mods);
}
void MacOS::scrollCallback(GLFWwindow *window, double xoffset, double yoffset)
{
    void *userPointer = glfwGetWindowUserPointer(window);
    if (userPointer == nullptr)
    {
        return;
    }
    MacOS *manager = static_cast<MacOS *>(userPointer);
    if (manager == nullptr) // 检查非空
    {
        return;
    }
    manager->onScroll(xoffset, yoffset);
}
void MacOS::onWindowSize()
{
    glfwGetWindowSize(this->_window, &this->_width, &this->_height);
    if (Boo::game == nullptr)
        return;
    Boo::game->resizeView(this->_width, this->_height);
};
#endif

void MacOS::onCursorPos(double xpos, double ypos)
{
    if (Boo::game == nullptr)
        return;
    // 通知引擎更新鼠标位置
    Boo::game->updateMousePos(xpos, ypos);
};
void MacOS::onMouseButton(int button, int action, int mods)
{
    if (Boo::game == nullptr)
        return;
    // 通知引擎更新鼠标状态
    Boo::game->updateMouseState(button, action, mods);
};
void MacOS::onKey(int key, int scancode, int action, int mods)
{
    if (Boo::game == nullptr)
        return;
    // 通知引擎更新键盘状态
    Boo::game->updateKeyState(key, scancode, action, mods);
}
void MacOS::onScroll(double xoffset, double yoffset)
{
    if (Boo::game == nullptr)
        return;
    // 通知引擎更新滚动状态
    Boo::game->updateScroll(xoffset, yoffset);
}
bool MacOS::isRunning()
{
#if defined(BOO_PLATFORM_MACOS)
    return !glfwWindowShouldClose(this->_window);
#endif
    return false;
}
void MacOS::tick()
{
#if defined(BOO_PLATFORM_MACOS)
    glfwPollEvents();
#endif
}
const int MacOS::getWidth()
{
    return this->_width;
}
const int MacOS::getHeight()
{
    return this->_height;
}
std::string MacOS::getAssetsRoot()
{
    return this->_assetsRoot;
}
void MacOS::loadImage(const std::string &path, std::vector<uint8_t> &pixels, int &width, int &height, int &channels)
{
    width = 0;
    height = 0;
    channels = 0;
    std::filesystem::path fullPath = (std::filesystem::path(this->_assetsRoot) / path).generic_string();
    if (!std::filesystem::exists(fullPath))
    {
        LOGW("AssetLoad:No such file or directory: %s", fullPath.c_str());
        return;
    }
    const void *_pixels = stbi_load(fullPath.string().c_str(), &width, &height, &channels, STBI_rgb_alpha);
    if (_pixels == nullptr)
    {
        LOGW("Failed to load TextureAsset: %s", fullPath.c_str());
        return;
    }
    channels = STBI_rgb_alpha;
    pixels = std::vector<uint8_t>(static_cast<const uint8_t *>(_pixels), static_cast<const uint8_t *>(_pixels) + (width * height * channels));
    stbi_image_free((void *)_pixels);
}
void MacOS::loadText(const std::string &path, std::string &text)
{
    text = "";
    std::filesystem::path fullPath = (std::filesystem::path(this->_assetsRoot) / path).generic_string();
    if (!std::filesystem::exists(fullPath))
    {
        LOGW("AssetLoad:No such file or directory: %s", fullPath.c_str());
        return;
    }
    std::ifstream file(fullPath, std::ios::binary | std::ios::ate);
    if (!file.is_open())
    {
        LOGW("Failed to open file: %s", fullPath.c_str());
        return;
    }
    size_t fileSize = static_cast<size_t>(file.tellg());
    text.resize(fileSize);
    file.seekg(0);
    file.read((char *)text.data(), fileSize);
    file.close();
}
void MacOS::loadBinary(const std::string &path, std::vector<uint8_t> &bytes)
{
    bytes.clear();
    std::filesystem::path fullPath = (std::filesystem::path(this->_assetsRoot) / path).generic_string();
    if (!std::filesystem::exists(fullPath))
    {
        LOGW("AssetLoad:No such file or directory: %s", fullPath.c_str());
        return;
    }
    std::ifstream file(fullPath.string().c_str(), std::ios::binary | std::ios::ate);
    if (!file || !file.is_open())
    {
        LOGW("Failed to open file: %s", fullPath.c_str());
        return;
    }
    size_t fileSize = static_cast<size_t>(file.tellg());
    bytes.resize(fileSize);
    file.seekg(0);
    file.read((char *)bytes.data(), fileSize);
    file.close();
}
void MacOS::destroy()
{
#if defined(BOO_PLATFORM_MACOS)
    glfwDestroyWindow(this->_window);
    glfwTerminate();
#endif
}
MacOS::~MacOS()
{
}
