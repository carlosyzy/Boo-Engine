#pragma once
#include <unordered_map>
#include <vector>
#include <string>
#include <cstdint>
#include "ui-tree.h"
class Node;
class Scene;

class NodeTree : public UITree
{
private:
    Node *_rootNode = nullptr;
    Scene *_rootScene = nullptr; // 改为弱引用，不拥有所有权

    void _setTrees(Node *root, UITreeStructure &uiTreeData, int layer);

public:
    NodeTree(Node *node, std::string uuid = "");
    void setNode(Node *node);
    void setScene(Scene *scene);
    void updateTree() override;
    void update(float deltaTime) override;
    void lateUpdate(float deltaTime) override;
    void render() override;
    void destroy() override;
    ~NodeTree();
};