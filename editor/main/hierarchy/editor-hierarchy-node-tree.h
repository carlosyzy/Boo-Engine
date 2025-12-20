#pragma once
#include <unordered_map>
#include <vector>
#include <string>
#include <cstdint>
#include "../../../engine/core/component/component-register.h"
#include "../../../engine/core/component/component.h"

class Node;
class Node2D;
class UISprite;
class Scene;

struct NodeTreeStructure
{
    /** 节点名称 */
    std::string name = "";
    /** 节点唯一标识 */
    std::string uuid = "";
    /** 是否折叠 */
    bool isFold = false;
    /**层级 */
    int layer = -1;
    /** 节点在树中的索引 */
    int index;
    /** 节点-节点树时使用 */
    Node2D *ndBind;
    /** 子节点 */
    std::vector<NodeTreeStructure> children;
};

class EditorHierarchyNodeTree : public Component
{
private:
    float _topLen;
    float _leftLen;
    Node2D *_ndContent;
    UISprite *_spContent;
    float _contentWidth;
    float _contentHeight;
    float _contentX;
    float _contentY;
    void _initContent();

    Node *_rootNode = nullptr;
    Scene *_rootScene = nullptr; // 改为弱引用，不拥

    std::unordered_map<std::string, Node *> _nodes; // 节点-节点树item 映射


    int _nodeIndex;
    std::vector<Node2D *> _nodePools;
    NodeTreeStructure *_hoverTreeItem;
    void _setTrees(Node *root, NodeTreeStructure &uiTreeData, int layer);
    void _updateTreeContent();
    void _updateTreesItems(NodeTreeStructure &uiTreeData);
    void _onTreeItemTouchEvent(NodeTreeStructure &result);
    void _onTreeItemCursorHoverEvent(NodeTreeStructure &result);

    // 当前显示的节点树映射
    std::map<std::string, NodeTreeStructure *> _treeUIMap;
     // item 折叠按钮的touch 事件
    std::map<std::string, NodeTreeStructure *> _foldUIMap; // 折叠按钮-节点树item 映射
    /** 是否需要刷新 */
    bool _isDirty = true;
    // 节点树数据
    NodeTreeStructure _uiTreeData;

protected:
    void _deserialized() override;

public:
    EditorHierarchyNodeTree(std::string name, Node *node, std::string uuid = "");

    // 组件生命周期
    void Awake() override;
    void Enable() override;
    void setNode(Node *node);
    void setScene(Scene *scene);
    void updateTree();
    void Update(float deltaTime) override;
    void LateUpdate(float deltaTime) override;
    void Disable() override;
    void destroy() override;
    ~EditorHierarchyNodeTree();
};
REGISTER_COMPONENT(EditorHierarchyNodeTree, "Editor Hierarchy Node Tree Component");
