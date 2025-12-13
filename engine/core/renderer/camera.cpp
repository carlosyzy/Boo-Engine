#include "camera.h"
#include "../../boo.h"
#include "../math/size.h"
#include "../math/vec3.h"
#include "../math/vec2.h"
#include "../scene/node.h"
#include "../scene/node-2d.h"
#include "../gfx/gfx-mgr.h"
#include "../gfx/gfx-render-texture.h"

Camera::Camera(std::string name, Node *node, std::string uuid) : Component(name, node, uuid)
{
    this->_layer = NodeLayer::Default;
}
void Camera::_deserialized()
{
    Component::_deserialized();
}
void Camera::Awake()
{
    Component::Awake();
}
void Camera::Enable()
{
    Component::Enable();
    Boo::game->extractCamera(this);
}
void Camera::resize(int width, int height)
{
    if (this->_width == width && this->_height == height)
    {
        return;
    }
    std::cout << "Camera::resize: width:" << width << " height:" << height << std::endl;
    this->_width = width;
    this->_height = height;
    if (this->_width > 0 && this->_height > 0 && this->_renderTexture != nullptr)
    {
        this->_renderTexture->resize(this->_width, this->_height);
    }
}
void Camera::setPipeline(std::string pipeline)
{
    if (this->_pipeline == pipeline)
    {
        return;
    }
    this->_clearOldRenderPipeline();
    this->_pipeline = pipeline;
    this->_createRenderPipeline();
}
void Camera::_clearOldRenderPipeline()
{
    // 渲染管线变了后重制渲染目标
    if (this->_renderTexture != nullptr)
    {
        this->_renderTexture->destroy();
        this->_renderTexture = nullptr;
        GfxMgr::getInstance()->delRenderQueue(this->_pipeline, this->_uuid);
    }
}
void Camera::_createRenderPipeline()
{
    this->_renderTexture = new GfxRenderTexture(this->_uuid);
    GfxMgr::getInstance()->initRenderQueue(this->_pipeline, this->_uuid, this->_renderTexture);
    if (this->_width > 0 && this->_height > 0)
    {
        this->_renderTexture->resize(this->_width, this->_height);
    }
}
std::string Camera::getPipeline()
{
    return this->_pipeline;
}
void Camera::setPriority(int priority)
{
    this->_priority = priority;
}
int Camera::getPriority()
{
    return this->_priority;
}
void Camera::setVisibility(int visibility)
{
    this->_visibility = visibility;
}
int Camera::getVisibility()
{
    return this->_visibility;
}
int Camera::getWidth()
{
    return this->_width;
}
int Camera::getHeight()
{
    return this->_height;
}
void Camera::Update(float deltaTime)
{
    Component::Update(deltaTime);
}
void Camera::LateUpdate(float deltaTime)
{
    Component::LateUpdate(deltaTime);
}
void Camera::Render()
{
    GfxMgr::getInstance()->submitRenderMat(this->_pipeline, this->_uuid, this->_matView.data(), this->_matProj.data());
}
void Camera::Disable()
{
    Component::Disable();
    Boo::game->removeCamera(this);
}
void Camera::destroy()
{
    Component::destroy();
}
Camera::~Camera()
{
    std::cout << "Camera::~destructor" << this->_node->getName() << std::endl;
}
