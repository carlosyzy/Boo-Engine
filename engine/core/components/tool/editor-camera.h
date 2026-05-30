#pragma once
#include <string>
#include "core/component/component.h"
#include "core/component/component-register.h"
#include "core/math/vec3.h"
#include "core/math/vec2.h"
#include "core/math/quat.h"
namespace Boo
{
    class Node;
    class Camera;
    class Node3D;
    struct FInputScrollEvent;
    struct FInputMouseEvent;

    class EditorCamera : public Component
    {
    private:
        Node3D *_node3D;
        Camera *_camera;
        Vec3 _worldFocusPos;
        float _focusRadius = 10.0f;
        float _rotateOffsetH = 0.0f;
        float _rotateOffsetV = 0.0f;

        uint64_t _mouseScrollId = 0;
        uint64_t _touchpadScrollId = 0;
        void _onMouseScroll(FInputScrollEvent &event);
        void _onTouchpadScroll(FInputScrollEvent &event);

        // 鼠标按下事件
        bool _isMiddleMouseDown = false;
        float _mouseX = 0.0f;
        float _mouseY = 0.0f;
        uint64_t _mouseDownId = 0;
        uint64_t _mouseUpId = 0;
        uint64_t _mouseMoveId = 0;
        void _onMouseDown(FInputMouseEvent &event);
        void _onMouseUp(FInputMouseEvent &event);
        void _onMouseMove(FInputMouseEvent &event);

        void _resetFocus();
        void _updateCameraNodePos();
        void _updateCameraNodeAngle();

    public:
        EditorCamera(std::string name, Node *node, std::string uuid);
        void OnAwake() override;
        void OnEnable() override;
        void setProperty(json &data) override;
        void Update(float deltaTime) override;
        void LateUpdate(float deltaTime) override;
        void OnDisable() override;
        void OnDestroy() override {};
        void destroy() override;
        ~EditorCamera() override;
    };
    REGISTER_COMPONENT(EditorCamera, "Virtual Camera Component")
}