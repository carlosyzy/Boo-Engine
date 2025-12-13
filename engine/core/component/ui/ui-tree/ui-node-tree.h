#pragma once
#include <unordered_map>
#include <vector>
#include <string>
#include <cstdint>
#include "ui-tree.h"
#include "../../component-register.h"

class Node;
class Scene;

class UINodeTree : public UITree
{
private:
    Node *_rootNode = nullptr;
    Scene *_rootScene = nullptr; // 改为弱引用，不拥有所有权

    void _setTrees(Node *root, UITreeStructure &uiTreeData, int layer);

public:
    UINodeTree(std::string name, Node *node, std::string uuid = "");
    void setNode(Node *node);
    void setScene(Scene *scene);
    void updateTree() override;
    void Update(float deltaTime) override;
    void LateUpdate(float deltaTime) override;
    void destroy() override;
    ~UINodeTree();
};
REGISTER_COMPONENT(UINodeTree, "UI Node Tree Component");
