#pragma once
#include "core/component/component.h"
#include "core/component/component-register.h"
#include "core/scene/node-type.h"
class GfxRenderTexture;
namespace Boo
{
    enum class ECameraProjection
    {
        /**
         * 正交投影
         */
        Ortho,
        /**
         * 透视投影
         */
        Perspective,
    };

    // 相机组件
    // 每个相机对应一个离屏渲染target
    // 直接将camera的可见物体渲染到一个贴图上
    // 然后再将这个贴图渲染到屏幕上
    class Camera : public Component
    {
    public:
    private:
        /**
         * 相机渲染优先级
         * 优先级越低，越先渲染
         */
        int _priority = 0;
        /**
         * 相机是否可见
         */
        int _groupIds = 1;
        /**
         * 相机类型
         * 0：正交相机
         * 1：透视相机
         */
        ECameraProjection _projection = ECameraProjection::Perspective;
        /**
         * 相机视场角度
         */
        float _fov = 45.0f;
        /**
         * 相机近裁剪平面距离
         */
        float _nearClip = 0.1f;
        /**
         * 相机远裁剪平面距离
         */
        float _farClip = 100.0f;
        /**
         * 相机正交高度
         */
        float _orthoHeight = 1.0f;
        /**
         * 相机是否在屏幕上
         */
        bool _isOnScreen = false;

    protected:
        /**
         * 视图矩阵
         * 将3D世界空间 → 3D相机空间
         * 作用：将世界坐标变换到摄像机/观察空间
         * 定义：摄像机的位置、朝向、上方向
         * 本质：世界到摄像机的变换矩阵
         */
        Mat4 _matView = Mat4::identity();
        /**
         * 投影矩阵
         * 将3D观察空间 → 2D裁剪空间（NDC空间：-1到1）
         * 作用：将摄像机空间的坐标变换到裁剪空间
         * 定义：摄像机的视场角度、近裁剪平面距离、远裁剪平面距离
         * 本质：摄像机到裁剪空间的变换矩阵
         * 类型：透视投影（3D）或正交投影（2D/UI）
         */
        Mat4 _matProj = Mat4::identity();
        void _createRenderPipeline();
        /**
         * @brief 更新视图矩阵
         *
         */
        void _updateViewMatrix();
        /**
         * @brief 更新投影矩阵
         *
         */
        void _updateProjectionMatrix();

    public:
        Camera(std::string name, Node *node, std::string uuid = "");
        void OnAwake() override;
        void OnEnable() override;
        void setProperty(json &data) override;
        /**
         * @brief 设置相机是否在屏幕上
         *
         * @param isOnScreen
         */
        void setIsOnScreen(bool isOnScreen);
        bool getIsOnScreen();
        void setPriority(int priority);
        int getPriority();
        void setGroupIDs(int groupIDs);
        void addGroupID(int groupID);
        void addGroupID(ENodeGroup groupID);
        int getGroupIDs();
        void setProjection(ECameraProjection projection);
        void updateViewSize();
        void Update(float deltaTime) override;
        void LateUpdate(float deltaTime) override;
        virtual void Render();
        void OnDisable() override;
        void OnDestroy() override;
        void destroy() override;
        ~Camera();
    };
    REGISTER_COMPONENT(Camera, "Camera Component");
} // namespace Boo
