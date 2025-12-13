#pragma once
#include "../component/component.h"
#include "../component/component-register.h"
#include "../scene/node-struct.h"

class GfxRenderTexture;

// 相机组件
// 每个相机对应一个离屏渲染target
// 直接将camera的可见物体渲染到一个贴图上
// 然后再将这个贴图渲染到屏幕上
class Camera : public Component
{
public:
    // /**
    //  * 相机渲染目标矩形
    //  */
    // Rect viewportRect = Rect(0, 0, 1, 1);

private:
    std::string _pipeline = "";
    /**
     * 相机渲染优先级
     * 优先级越低，越先渲染
     */
    int _priority = 0;
    /**
     * 相机是否可见
     */
    int _visibility = 1;
    /**
     * 相机渲染目标宽度
     */
    int _width = 0;
    /**
     * 相机渲染目标高度
     */
    int _height = 0;
    /**
     * 视图矩阵
     * 将3D世界空间 → 3D相机空间
     */
    Mat4 _matView = Mat4::identity();
    /**
     * 投影矩阵
     * 将3D观察空间 → 2D裁剪空间（NDC空间：-1到1）
     */
    Mat4 _matProj = Mat4::identity();

    GfxRenderTexture *_renderTexture = nullptr;
    void _clearOldRenderPipeline();
    void _createRenderPipeline();
protected:
    void _deserialized() override;

public:
    Camera(std::string name, Node *node, std::string uuid = "");
    void Awake() override;
    void Enable() override;
    void setPipeline(std::string pipeline);
    std::string getPipeline();
    void setPriority(int priority);
    int getPriority();
    void setVisibility(int visibility);
    int getVisibility();
    int getWidth();
    int getHeight();
    void resize(int width, int height);
    void Update(float deltaTime) override;
    void LateUpdate(float deltaTime) override;
    void Render();
    void Disable() override;
    void destroy() override;
    ~Camera();
};
REGISTER_COMPONENT(Camera, "Camera Component");
