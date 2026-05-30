
#include "renderer-mgr.h"
#include "core/renderer/camera.h"
#include "core/renderer/3d/mesh-renderer.h"
#include "core/renderer/ui/ui-renderer.h"
#include "core/renderer/ui/ui-mask.h"
#include "core/scene/node.h"
#include "core/scene/node-2d.h"
#include "core/scene/node-3d.h"
#include "core/scene/scene.h"
#include "log.h"

namespace Boo
{

    RendererMgr *RendererMgr::getInstance()
    {
        static RendererMgr _instance;
        return &_instance;
    }
    RendererMgr::RendererMgr()
    {
    }
    void RendererMgr::init()
    {
    }
    void RendererMgr::mountCamera(Camera *camera)
    {
        if (this->_cameras.find(camera->getUuid()) != this->_cameras.end())
        {
            return;
        }
        this->_cameras[camera->getUuid()] = camera;
        this->_sortCameras();
        LOGI("[RendererMgr]:mount camera: %s", camera->getUuid().c_str());
    }
    void RendererMgr::unmountCamera(Camera *camera)
    {
        if (this->_cameras.find(camera->getUuid()) == this->_cameras.end())
        {
            return;
        }
        this->_cameras.erase(camera->getUuid());
        this->_sortCameras();
        LOGI("[RendererMgr]:unmount camera: %s", camera->getUuid().c_str());
    }
    void RendererMgr::clearCameras()
    {
        this->_cameras.clear();
        this->_sortedCameras.clear();
    }
    void RendererMgr::_sortCameras()
    {
        this->_sortedCameras.clear();
        for (auto &camera : this->_cameras)
        {
            this->_sortedCameras.push_back(camera.second);
        }
        std::sort(this->_sortedCameras.begin(), this->_sortedCameras.end(), [](Camera *a, Camera *b)
                  { return a->getPriority() < b->getPriority(); });
    }
    void RendererMgr::updateViewSize()
    {
        for (auto &camera : this->_cameras)
        {
            camera.second->updateViewSize();
        }
    }
    void RendererMgr::render(Scene *scene)
    {
        // LOGI("[RendererMgr]:render 1: %s", scene->getName().c_str());
        // 相机排序 按照从小到大优先级
        // LOGI("[RendererMgr]:render 2 size: %d", sortedCameras.size());
        for (auto camera : this->_sortedCameras)
        {
            this->_renderCameras(camera, scene);
        }
    }
    void RendererMgr::_renderCameras(Camera *camera, Scene *scene)
    {
        // LOGI("[RendererMgr]:_renderCameras 1: %s", camera->getName().c_str());
        if (camera == nullptr)
        {
            return;
        }
        // LOGI("[RendererMgr]:_renderCameras 2: %s", camera->getName().c_str());
        if (scene == nullptr)
        {
            return;
        }
        // LOGI("[RendererMgr]:_renderCameras 3: %s", camera->getName().c_str());
        if (!camera->isEnabledInHierarchy())
        {
            return;
        }
        // LOGI("[RendererMgr]:_renderCameras 4: %s", camera->getName().c_str());
        camera->Render();
        // LOGI("[RendererMgr]:_renderCameras 5: %s", camera->getName().c_str());
        // std::chrono::high_resolution_clock::time_point frameStart = std::chrono::high_resolution_clock::now();
        this->_walkNode3D(camera, scene->getRoot3D());
        /*std::chrono::high_resolution_clock::time_point frameEnd = std::chrono::high_resolution_clock::now();
        float frameDuration = std::chrono::duration_cast<std::chrono::nanoseconds>(frameEnd - frameStart).count() / 1000000.0f;
        LOGI("render frame 3D submit object duration: %f", frameDuration);*/
        this->_walkNode2D(camera, scene->getRoot2D());
    }
    void RendererMgr::_walkNode3D(Camera *camera, Node3D *node)
    {
        // std::cout << "node: " << node->getName().c_str() << std::endl;
        // std::cout << "node groupID: " << node->getGroupId() << std::endl;
        // std::cout << "camera groupIDs: " << camera->getGroupIDs() << std::endl;
        if (node == nullptr)
        {
            return;
        }
        if (!node->getActiveInHierarchy())
        {
            return;
        }
        if (!(node->getGroupId() & camera->getGroupIDs()))
        {
            return;
        }
        Vec3 worldScale = node->getWorldScale();
        if (worldScale.getX() == 0 || worldScale.getY() == 0)
        {
            // 缩放为0 不渲染
            return;
        }
        MeshRenderer *meshRenderer = node->getMeshRenderer();
        if (meshRenderer != nullptr && meshRenderer->isEnabledInHierarchy())
        {
            meshRenderer->Render(camera);
        }
        std::vector<Node *> &nodes = node->getChildren();
        for (auto node : nodes)
        {
            if (node->getLayer() == ENodeLayer::Node3D)
            {
                this->_walkNode3D(camera, static_cast<Node3D *>(node));
            }
        }
    }
    void RendererMgr::_walkNode2D(Camera *camera, Node2D *node)
    {
        if (node == nullptr)
        {
            return;
        }
        if (!node->getActiveInHierarchy())
        {
            return;
        }
        if (!(node->getGroupId() & camera->getGroupIDs()))
        {
            return;
        }
        Vec3 worldScale = node->getWorldScale();
        if (worldScale.getX() == 0 || worldScale.getY() == 0)
        {
            // 缩放为0 不渲染
            return;
        }
        UIRenderer *uiRenderer = node->getUIRenderer();
        if (uiRenderer != nullptr && uiRenderer->isEnabledInHierarchy())
        {
            uiRenderer->Render(camera);
        }
        std::vector<Node *> &nodes = node->getChildren();
        for (auto node : nodes)
        {
            if (node->getLayer() == ENodeLayer::Node2D)
            {
                this->_walkNode2D(camera, static_cast<Node2D *>(node));
            }
        }
        if (uiRenderer != nullptr && uiRenderer->isEnabledInHierarchy())
        {
            if (uiRenderer->getName() == "UIMask")
            {
                static_cast<UIMask *>(uiRenderer)->lateRender(camera);
            }
        }
    }
    RendererMgr::~RendererMgr()
    {
    }

} // namespace Boo
