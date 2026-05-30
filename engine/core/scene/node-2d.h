#pragma once
#include <iostream>
#include "node.h"
namespace Boo
{
    // // enum
    // enum class NodeInput;
    // // struct
    // struct NodeInputResult;
    // class
    class UIRenderer;

    class Node2D : public Node
    {
    private:
        float _angle;
        Vec2 _anchor;
        Size _size;
        Mat4 _uiWorldMatrix;
        UIRenderer *_uiRenderer;
        int _sizeLock;

    protected:
        void _updateWorldTransform() override;

    public:
        Node2D(const std::string name, const std::string uuid = "");
        /**
         * 2d 节点的锚点
         * @param x
         * @param y
         */
        void setAnchor(float x, float y);
        /**
         * 2d 节点的大小
         * @param width
         * @param height
         */
        void setSize(float width, float height);
        const Vec2 &getAnchor();
        const Size &getSize();
        const Mat4 &getUIWorldMatrix();

        void setPosition(float x, float y, float z) override;
        void setWorldPosition(float x, float y, float z) override;
        void setScale(float x, float y, float z) override;
        void setWorldScale(float x, float y, float z) override;
        void setAngle(float angle);
        void setEulerAngles(float x, float y, float z) override;
        void setRotation(float x, float y, float z, float w) override;
        void setWorldRotation(float x, float y, float z, float w) override;
        void setActive(bool active) override;
        
        void addSizeLock(ENode2DSizeLock sizeLock);
        void removeSizeLock(ENode2DSizeLock sizeLock);
        void setSizeLock(ENode2DSizeLock sizeLock);
        int getSizeLock();
        /**
         * 添加组件
         */
        Component *addComponent(std::string name, std::string uuid = "") override;
        /**
         * 获取UI渲染组件
         */
        UIRenderer *getUIRenderer();

        // template <typename T, typename Func>
        // int onNodeInputEvent(NodeInput input, Func func, T *instance, bool isIntercept = true);
        // void offNodeInputEvent(int inputID);
        // void offAllNodeInputEvent();

        bool inHitMask(float x, float y);
        bool inHitOnNode(float x, float y);

        void update(float deltaTime) override;
        void lateUpdate(float deltaTime) override;
        void clearNodeFrameFlag() override;
        void destroy() override;
        ~Node2D();
    };

} // namespace Boo

// #include "node-2d-impl.h"