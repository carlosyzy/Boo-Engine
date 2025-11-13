#pragma once
#include <iostream>
#include <functional>
#include "../ipc/editor-ipc.h"
class Scene;
class Node;
class Node2D;
class EditorHierarchyLayout;

enum class NodeInput;
struct NodeInputResult;
struct UITreeStructure;

class EditorHierarchy
{
private:
    EditorHierarchyLayout *_layout;
    Scene *_rootScene;
    Node *_rootNode;
    /**
     * @brief 焦点事件
     */
    // void _onFocusEvent(const EventValue& value);
    // void _onRootInputStartEvent(NodeInputResult &result);
    // void _onNodeTreeSelectEvent(UITreeStructure *uiTreeData);
    void _onNodeTreeUpdateEvent();
public:
    EditorHierarchy(Node2D *root);
    void setNode(Node *node);
    void setScene(Scene *Scene);
    void update(float dt);
    /*
     * @brief 更新树
     */
    void updateRoot();
    ~EditorHierarchy();
};