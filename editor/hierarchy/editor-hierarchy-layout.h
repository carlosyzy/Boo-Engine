#pragma once
#include <iostream>
#include <functional>
class Node;
class Node2D;
class NodeTree;
class Scene;
class UISprite;
struct UITreeStructure;
// enum class NodeInput;
// struct NodeInputResult;

class EditorHierarchyLayout
{
private:
    Node2D *_root;
    Node2D *_content;
    void _initTitle();
    void _initContent();

    //添加按钮
    Node2D *_ndOperate;
    Node2D *_ndAdd;
    UISprite *_spriteAdd;
    Node2D *_ndFold;
    UISprite *_spriteFold;
    Node2D *_ndSearchIcon;
    UISprite *_spriteSearchIcon;
    Node2D *_ndSearchBox;
    UISprite *_spriteSearchBox;
    //折叠按钮
    void _initOperate();
    void _initOperateFold();
    void _initOperateAdd();
    void _initOperateSearch();

    NodeTree *_nodeTree;
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