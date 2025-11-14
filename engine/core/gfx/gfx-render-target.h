#pragma once
#include <vulkan/vulkan_core.h>
#include <vector>
#include <string>

class GfxContext;
class GfxTexture;
class GfxPass;

/**
 * @brief 离屏渲染目标
 * 封装渲染到纹理所需的资源：颜色附件、深度附件、MSAA 纹理和 Framebuffer
 */
class GfxRenderTarget
{
private:
    std::string _name;
    GfxContext *_context;
    GfxPass *_pass;

    uint32_t _width;
    uint32_t _height;

    /* // 颜色附件纹理 */
    GfxTexture *_colorTexture;
    /* // 深度附件纹理（可选） */
    GfxTexture *_depthTexture;
    /* // MSAA 颜色附件（如果启用 MSAA） */
    GfxTexture *_colorMsaaTexture;

    VkFramebuffer _framebuffer;

    void _createTextures();
    void _createFramebuffer();
    void _cleanTextures();
    void _cleanFramebuffer();

    void _Log(std::string msg);

public:
    GfxRenderTarget(std::string name, GfxContext *context, uint32_t width, uint32_t height);

    /**
     * @brief 初始化渲染目标（需要在设置 Pass 后调用）
     */
    void create(GfxPass *pass);
    void clear();
    void reset();

    std::string name() const { return _name; }
    uint32_t width() const { return _width; }
    uint32_t height() const { return _height; }

    VkFramebuffer getFramebuffer() const { return _framebuffer; }

    /**
     * @brief 获取渲染结果纹理（可以作为其他 Pass 的输入）
     */
    GfxTexture* getColorTexture() const { return _colorTexture; }

    /**
     * @brief 保存渲染结果到文件
     * @param filePath 输出文件路径（支持 .png, .jpg, .bmp）
     * @return true 保存成功，false 保存失败
     */
    bool saveToFile(const std::string& filePath);

    ~GfxRenderTarget();
};
