#include "camera.h"
#include "../../boo.h"
#include "../math/size.h"
#include "../math/vec3.h"
#include "../math/vec2.h"
#include "../math/mat4.h"
#include "../scene/node.h"
#include "../scene/node-2d.h"
#include "../gfx/gfx-mgr.h"
#include "../gfx/base/gfx-render-texture.h"

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
    this->_matView = Mat4::identity();
    this->_matProj = Mat4::identity();
    this->_createRenderPipeline();
}
void Camera::_createRenderPipeline()
{
    this->_renderTexture = new GfxRenderTexture(this->_uuid);
    GfxMgr::getInstance()->initRenderQueue(this->_uuid, this->_renderTexture);
    if (this->_width > 0 && this->_height > 0)
    {
        this->_renderTexture->resize(this->_width * 2.0f, this->_height * 2.0f);
    }
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
        this->_renderTexture->resize(this->_width * 2.0f, this->_height * 2.0f);
    }
    this->_matProj.setM00(2.0f / (float)this->_width);
    this->_matProj.setM11(2.0f / (float)this->_height);
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
    GfxMgr::getInstance()->submitRenderMat(this->_uuid, this->_matView.data(), this->_matProj.data());
}
void Camera::Disable()
{
    Component::Disable();
    Boo::game->removeCamera(this);
}
void Camera::destroy()
{
    Component::destroy();
    GfxMgr::getInstance()->delRenderQueue(this->_uuid);
}
Camera::~Camera()
{
    std::cout << "Camera::~destructor" << this->_node->getName() << std::endl;
}
