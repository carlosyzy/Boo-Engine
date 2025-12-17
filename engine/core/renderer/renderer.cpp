
#include "renderer.h"
#include "camera.h"
#include "../scene/scene.h"
#include "../scene/node.h"
#include "../scene/node-3d.h"
#include "../scene/node-2d.h"
#include "../math/vec3.h"
#include "../math/color.h"
#include "ui/ui-renderer.h"

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
    camera->Render();
    const std::vector<Node *> &nodes = scene->getChildren();
    for (auto node : nodes)
    {
        this->_walkNode(camera, node);
    }
}
void Renderer::_walkNode(Camera *camera, Node *node)
{
    if (!node->isActiveInHierarchy())
    {
        // 层级不可见
        return;
    }
    if (!(node->getVisibility() | camera->getVisibility()))
    {
        // 摄像机不可见
        return;
    }
    Vec3 worldScale = node->getWorldScale();

    if (node->getLayer() == NodeLayer::Node3D)
    {
        // 3D节点
        if (worldScale.getX() == 0 || worldScale.getY() == 0 || worldScale.getZ() == 0)
        {
            // 缩放为0 不渲染
            return;
        }
        Node3D *node3D = dynamic_cast<Node3D *>(node);
        if (node3D == nullptr)
        {
            // 3D节点 类型错误
            return;
        }
    }
    else if (node->getLayer() == NodeLayer::Node2D)
    {
        // 2D节点
        if (worldScale.getX() == 0 || worldScale.getY() == 0)
        {
            // 缩放为0 不渲染
            return;
        }
        Node2D *node2D = dynamic_cast<Node2D *>(node);
        if (node2D == nullptr)
        {
            // 2D节点 类型错误
            return;
        }
        UIRenderer *uiRenderer = dynamic_cast<UIRenderer *>(node2D->getUIRenderComponent());
        if (uiRenderer != nullptr && uiRenderer->isEnabledInHierarchy())
        {
            // std::cout << "Renderer::_walkNode5:" << node->getName() << std::endl;
            // 存在UI渲染器
            const Color &color = uiRenderer->getColor();
            float alpha = color.getA();
            if (alpha <= 0.0f)
            {
                // 透明度为0 不渲染
                return;
            }
            // 渲染UI
            uiRenderer->Render(camera);
        }
    }
    const std::vector<Node *> &nodes = node->getChildren();
    for (auto node : nodes)
    {
        this->_walkNode(camera, node);
    }
}
Renderer::~Renderer()
{
}
