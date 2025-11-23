// #pragma once
// #include <iostream>
// #include <functional>
// #include "../ipc/editor-ipc.h"
// class Scene;
// class Node;
// class Node2D;
// class EditorHierarchyLayout;

// enum class NodeInput;
// struct NodeInputResult;
// struct UITreeStructure;

// class EditorHierarchy
// {
// private:
//     EditorHierarchyLayout *_layout;
//     Scene *_rootScene;
//     Node *_rootNode;
//     void _onNodeTreeUpdateEvent();

// public:
//     EditorHierarchy(Node2D *root);
//     void setNode(Node *node);
//     void setScene(Scene *Scene);
//     void update(float dt);
//     /*
//      * @brief 更新树
//      */
//     void updateRoot();
//     ~EditorHierarchy();
// };