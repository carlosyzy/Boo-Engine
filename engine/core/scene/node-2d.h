#pragma once
#include <iostream>
#include "node.h"
#include "../utils/uuid-util.h"
#include "../math/vec2.h"
#include "../math/size.h"
#include "../math/mat4.h"

struct NodeInputResult;
enum class NodeInput;

class Node2D : public Node
{
private:
    Vec2 _anchor;
    Size _size;
    Mat4 _uiWorldMatrix{};

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
    const Vec2 &getAnchor() { return this->_anchor; }
    const Size &getSize() { return this->_size; }
    const Mat4 &uiWorldMatrix()
    {
        this->_updateWorldTransform();
        return this->_uiWorldMatrix;
    }
    // 重写基类方法
    void update(float deltaTime) override;
    void lateUpdate(float deltaTime) override;
    void render() override;

    /**
     * 添加组件
     */
    Component *addComponent(std::string name, std::string uuid = "");
    void addComponent(Component *component);

    /*
     * 获取组件
     */
    Component *getComponent(std::string name);
    void clearNodeFrameFlag() override;

    template <typename T, typename Func>
    int onNodeInputEvent(NodeInput input, Func func, T *instance, bool isIntercept = true);
    void offNodeInputEvent(int inputID);
    void offAllNodeInputEvent();

    bool inHitMask(float x, float y);
    bool inHitOnNode(float x, float y);

    void clearAllEvent() override;
    void destroy() override;
    ~Node2D();
};

#include "node-2d-impl.h"