#include "ui-renderer.h"
#include "../../gfx/gfx-mgr.h"
#include "../../game.h"
#include "../../scene/node.h"
#include "../../scene/node-2d.h"
#define STB_IMAGE_IMPLEMENTATION
#include "../../../libs/stb/stb_image.h"

UIRenderer::UIRenderer(Node *node, std::string uuid) : Component(node, uuid)
{
    this->_flag = static_cast<uint32_t>(UIFlag::UI_ALL);
    this->_layer = NodeLayer::Node2D;
    this->_init();
}

void UIRenderer::_init()
{
    this->_initDefaultGfxPipeline();
    this->_initDefaultGfxTexture();
    this->_initDefaultGfxRenderObject();
}
void UIRenderer::_initDefaultGfxPipeline()
{
    this->_shaderVert = "resources/effects/ui/ui.vert";
    this->_shaderFrag = "resources/effects/ui/ui.frag";
    this->_pipelineState = "Blend:1|DepthTest:0|DepthWrite:0|vert:" + this->_shaderVert + "|frag:" + this->_shaderFrag;
    /**
    * 创建管道
    */
    GfxMgr::getInstance()->createPipeline("ui", this->_pipelineState);
}
void UIRenderer::_initDefaultGfxTexture()
{
    this->_texture = "resources/textures/ic-default.png";
    if (!GfxMgr::getInstance()->isExistTexture(this->_texture))
    {
        this->_createGfxTexture();
    }
}
void UIRenderer::_initDefaultGfxRenderObject()
{
    GfxMgr::getInstance()->createObject(this->_uuid, "ui", this->_pipelineState, this->_positions, this->_colors, this->_normals, this->_uvs, this->_indices);
    GfxMgr::getInstance()->setObjectTexture(this->_uuid, this->_texture);
}
void UIRenderer::setShader(std::string vert, std::string frag)
{
    this->_shaderVert = vert;
    this->_shaderFrag = frag;
    this->_pipelineState = "Blend:1|DepthTest:0|DepthWrite:0|vert:" + this->_shaderVert + "|frag:" + this->_shaderFrag;
    /**
    * 创建管道
    */
    GfxMgr::getInstance()->createPipeline("ui", this->_pipelineState);
    /**
    * 更新渲染对象的管线
    */
    GfxMgr::getInstance()->setObjectPipeline(this->_uuid, this->_pipelineState);
}

void UIRenderer::setColor(float r, float g, float b, float a)
{
    this->_color.set(r, g, b, a);
    this->_flag |= static_cast<uint32_t>(UIFlag::UI_COLOR);
}
void UIRenderer::setColor(std::string color)
{
    this->_color.set(color);
    this->_flag |= static_cast<uint32_t>(UIFlag::UI_COLOR);
}
void UIRenderer::setTexture(std::string texture)
{
    if (this->_texture == texture)
        return;
    this->_texture = texture;
    this->_flag |= static_cast<uint32_t>(UIFlag::UI_TEXTURE);
    if (!GfxMgr::getInstance()->isExistTexture(this->_texture))
    {
        this->_createGfxTexture();
    }
}

void UIRenderer::_createGfxTexture()
{
    int _width=0;
    int _height=0;
    int _channels=0;
    const void *_pixels;
    _pixels = stbi_load(this->_texture.c_str(), &_width, &_height, &_channels, STBI_rgb_alpha);
    if (_pixels == nullptr)
    {
        std::cerr << "Failed to load texture: " << this->_texture << std::endl;
        return;
    }
    _channels = 4;
    /**
    *  使用实际通道数
    */
    std::vector<uint8_t> pixelsVector(static_cast<const uint8_t*>(_pixels),
        static_cast<const uint8_t*>(_pixels) + (_width * _height * _channels));
    /**
    * 记得释放内存！
    */
    stbi_image_free((void*)_pixels);
    GfxMgr::getInstance()->createTexture(this->_texture, _width, _height, _channels, &pixelsVector);
}

void UIRenderer::update(float deltaTime) {
    if (!this->isEnabled())
        return; // 渲染组件未激活
    if (this->_color.getA() <= 0)
        return; // 颜色透明
    if (this->_modelMatrix.getM00() <= 0 || this->_modelMatrix.getM10() <= 0)
    {
        return;
    }
    // 节点变换矩阵更新了
    if (this->_node->hasFrameTransformFlag())
    {
        Node2D* node2D = dynamic_cast<Node2D*>(this->_node);
        GfxMgr::getInstance()->setObjectModelMatrix(this->_uuid, node2D->uiWorldMatrix().data());
        /*UIMaskRect& maskRect = node2D->maskRect();
        GfxMgr::getInstance()->setUIObjectMask(this->_uuid, maskRect.x, maskRect.y, maskRect.width, maskRect.height, maskRect.angle);*/
    }

    if (this->_flag & static_cast<uint32_t>(UIFlag::UI_TEXTURE))
    {
        // 更新纹理
        GfxMgr::getInstance()->setObjectTexture(this->_uuid, this->_texture);
    }
    if (this->_flag & static_cast<uint32_t>(UIFlag::UI_COLOR))
    {
        // 更新颜色
        GfxMgr::getInstance()->setObjectColor(this->_uuid, this->_color.getA(), this->_color.getA(), this->_color.getG(), this->_color.getR());
    }

    this->_flag = static_cast<uint32_t>(UIFlag::UI_NONE);
}
UIRenderer::~UIRenderer()
{

}
