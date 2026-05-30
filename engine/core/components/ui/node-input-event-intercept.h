#pragma once
#include "core/component/component.h"
#include "core/component/component-register.h"
#include "core/input/input-type.h"

namespace Boo
{
    class Node2D;

    class NodeInputEventIntercept : public Component
    {
    public:
        NodeInputEventIntercept(std::string name, Node *node, std::string uuid = "");
        void OnAwake() override;
        void OnEnable() override;
        void Update(float deltaTime) override;
        void LateUpdate(float deltaTime) override;
        void OnDisable() override;
        void OnDestroy() override;
        ~NodeInputEventIntercept() override;

    private:
        Node2D *_node2d;
        uint64_t _touchStartID = 0;
        uint64_t _touchMoveID = 0;
        uint64_t _touchEndID = 0;

        void _onTouchStart(FNodeTouchEvent &event);
        void _onTouchMove(FNodeTouchEvent &event);
        void _onTouchEnd(FNodeTouchEvent &event);
    };

    REGISTER_COMPONENT(NodeInputEventIntercept, "NodeInputEventIntercept")
}
