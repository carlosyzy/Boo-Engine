#include "camera.h"
#include "../../boo.h"
#include "../math/size.h"
#include "../math/vec3.h"
#include "../math/vec2.h"
#include "../scene/node.h"
#include "../scene/node-2d.h"

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
    Boo::game->extractCamera(this);
}
void Camera::Enable()
{
    Component::Enable();
}
void Camera::resize(int width, int height)
{
    this->_width = width;
    this->_height = height;
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
    Component::Render();
}
void Camera::Disable()
{
    Component::Disable();
}
void Camera::destroy()
{
    Component::destroy();
}
