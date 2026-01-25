
#include "renderer.h"
#include "camera.h"
#include "../scene/scene.h"
#include "../scene/node.h"
#include "../scene/node-3d.h"
#include "../scene/node-2d.h"
#include "../math/vec3.h"
#include "../math/color.h"
#include "ui/ui-renderer.h"
#include "ui/ui-mask.h"

Renderer::Renderer(/* args */)
{
}
void Renderer::init()
{
}
void Renderer::render(std::unordered_map<std::string, Camera *> &cameras, Scene *scene)
{
    // 相机排序 按照从小到大优先级
    std::vector<Camera *> sortedCameras;
    for (auto &camera : cameras)
    {
        sortedCameras.push_back(camera.second);
    }
    std::sort(sortedCameras.begin(), sortedCameras.end(), [](Camera *a, Camera *b)
              { return a->getPriority() < b->getPriority(); });

    // 渲染相机
    for (auto camera : sortedCameras)
    {
        this->_renderCameras(camera, scene);
    }
}
void Renderer::_renderCameras(Camera *camera, Scene *scene)
{
    if (camera == nullptr)
    {
        return;
    }
    if (scene == nullptr)
    {
        return;
    }
    if (!camera->isEnabledInHierarchy())
    {
        return;
    }
    std::cout << "render camera: " << camera->getName() << std::endl;
    camera->Render();
    // this->_walkNode3D(camera, scene->getRoot3D());
    this->_walkNode2D(camera, scene->getRoot2D());
}
void Renderer::_walkNode3D(Camera *camera, Node *node)
{
}
void Renderer::_walkNode2D(Camera *camera, Node2D *node)
{
    if (node == nullptr)
    {
        return;
    }
    if (!node->isActiveInHierarchy())
    {
        return;
    }
    if (!(node->getVisibility() | camera->getVisibility()))
    {
        return;
    }
    Vec3 worldScale = node->getWorldScale();
    if (worldScale.getX() == 0 || worldScale.getY() == 0)
    {
        // 缩放为0 不渲染
        return;
    }
    UIRenderer *uiRenderer = node->getUIRenderComponent();
    if (uiRenderer != nullptr && uiRenderer->isEnabledInHierarchy())
    {
        const Color &color = uiRenderer->getColor();
        float alpha = color.getA();
        if (alpha <= 0.0f)
        {
            // 透明度为0 不渲染
            return;
        }
        // std::cout << "********:" << node->getName() << "    " << uiRenderer->getName() << std::endl;
        uiRenderer->Render(camera);
        const std::vector<Node *> &nodes = node->getChildren();
        for (auto node : nodes)
        {
            this->_walkNode2D(camera, dynamic_cast<Node2D *>(node));
        }
        UIMask *uiMask = dynamic_cast<UIMask *>(uiRenderer);
        if (uiMask != nullptr)
        {
            uiMask->lateRender(camera);
        }
    }
    else
    {
        const std::vector<Node *> &nodes = node->getChildren();
        for (auto node : nodes)
        {
            this->_walkNode2D(camera, dynamic_cast<Node2D *>(node));
        }
    }
}
Renderer::~Renderer()
{
}
