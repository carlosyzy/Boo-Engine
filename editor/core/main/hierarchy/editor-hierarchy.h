#pragma once
#include <iostream>
#include <functional>

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
    Node2D *_rootNode;
    EditorHierarchyLayout *_layout;

public:
    EditorHierarchy(Node2D *root);
    void init();
    void setNode(Node *node);
    void setScene(Scene *Scene);

    void update(float dt);

    ~EditorHierarchy();
};