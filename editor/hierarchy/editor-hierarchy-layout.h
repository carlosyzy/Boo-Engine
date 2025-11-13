#pragma once
#include <iostream>
#include <functional>
class Node;
class Node2D;
class NodeTree;
class Scene;
struct UITreeStructure;
// enum class NodeInput;
// struct NodeInputResult;

class EditorHierarchyLayout
{
private:
    Node2D *_root;
    Node2D *_content;
    void _initContent();
    NodeTree *_nodeTree;
    void _initTitle();
    void _initOperate();
    void _initNodeTree();

    // std::function<void(NodeInputResult &result)> _onRootInputStartEventCallback;
    // std::function<void(UITreeStructure *uiTreeData)> _onNodeTreeSelectEventCallback;
    // std::function<void()> _onNodeTreeUpdateEventCallback;

public:
    EditorHierarchyLayout(Node2D *root);
    void setScene(Scene *Scene);
    void setNode(Node *node);
    /*
     * @brief 更新树
     */
    void updateTree();
    /*
     * @brief 失去焦点
     */
    void loseFocus();
    void clearNodeTreeSelect();

    // void onRootInputStartEvent(std::function<void(NodeInputResult &result)> callback);
    // void onNodeTreeSelectEvent(std::function<void(UITreeStructure *uiTreeData)> callback);
    // void onNodeTreeUpdateEvent(std::function<void()> callback);
    ~EditorHierarchyLayout();
};