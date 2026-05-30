#pragma once
#include <vector>
#include <unordered_map>
#include <string>
#include <vector>
#include <cstdint>
#include <algorithm>
#include <unordered_map>

namespace Boo
{

    class Camera;
    class Node;
    class Node2D;
    class Node3D;
    class Scene;

    class RendererMgr
    {
    private:
        RendererMgr(const RendererMgr &) = delete;            // 禁用拷贝构造
        RendererMgr &operator=(const RendererMgr &) = delete; // 禁用赋值操作符
        RendererMgr();                                        // 构造函数设为私有
        ~RendererMgr();
    private:
        /**
         * @brief 相机系统
         */
        std::unordered_map<std::string, Camera *> _cameras;
        std::vector<Camera *> _sortedCameras;  // 新增：缓存排好序的相机列表
        void _sortCameras();
        void _renderCameras(Camera *camera, Scene *scene);
        void _walkNode3D(Camera *camera, Node3D *node);
        void _walkNode2D(Camera *camera, Node2D *node);

    public:
        static RendererMgr *getInstance();
        void init();
        void mountCamera(Camera *camera);
        void unmountCamera(Camera *camera);
        void clearCameras();
        void updateViewSize();
        void render(Scene *scene);
    };

} // namespace Boo
