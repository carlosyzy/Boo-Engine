#pragma once
#include <iostream>
#include <functional>
class Node;
class Node2D;
class NodeTree;
class Scene;
class UISprite;
struct UITreeStructure;

class EditorHierarchyLayout
{
private:
    Node2D *_root;
    Node2D *_content;
    void _initContent();
    void _initTitle();

public:
    EditorHierarchyLayout(Node2D *root);
    void init();

    void setScene(Scene *Scene);
    void setNode(Node *node);
    void loseFocus();
    ~EditorHierarchyLayout();
};

// class EditorHierarchyLayout
// {
// private:
//     Node2D *_root;
//     Node2D *_content;
//     void _initTitle();
//     void _initContent();

//     //添加按钮
//     Node2D *_ndOperate;
//     Node2D *_ndAdd;
//     UISprite *_spriteAdd;
//     Node2D *_ndFold;
//     UISprite *_spriteFold;
//     Node2D *_ndSearchIcon;
//     UISprite *_spriteSearchIcon;
//     Node2D *_ndSearchBox;
//     UISprite *_spriteSearchBox;
//     //折叠按钮
//     void _initOperate();
//     void _initOperateFold();
//     void _initOperateAdd();
//     void _initOperateSearch();

//     NodeTree *_nodeTree;
//     void _initNodeTree();
//     void _onNodeTreeSelectEvent(std::string uuid);
//     void _onNodeTreeMenuEvent(std::string uuid, int x, int y);
// public:
//     EditorHierarchyLayout(Node2D *root);
//     void setScene(Scene *Scene);
//     void setNode(Node *node);
//     void loseFocus();
//     ~EditorHierarchyLayout();
// };