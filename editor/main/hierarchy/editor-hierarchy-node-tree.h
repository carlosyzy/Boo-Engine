#pragma once
#include <unordered_map>
#include <vector>
#include <string>
#include <cstdint>
#include "../../../engine/core/component/component-register.h"
#include "../../../engine/core/component/component.h"
#include "../../../engine/core/input/input-struct.h"

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
    std::string icon = "";
    int index;
    // 节点树中的显示节点
    Node2D *ndBind;
    /** 子节点 */
    std::vector<NodeTreeStructure> children;
};

class EditorHierarchyNodeTree : public Component
{
private:
    int _contentTouchID = -1;
    int _contentHoverID = -1;
    void _onTreeContentTouchEvent(NodeInputResult &result);
    void _onTreeContentHoverEvent(NodeInputResult &result);
    // 选择事件回调
    std::function<void(std::string)> _selectCallback;

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

private:
    Node *_rootNode = nullptr;
    Scene *_rootScene = nullptr;   // 改为弱引用，不拥
    NodeTreeStructure _uiTreeData; // 节点树数据
    int _nodeIndex;
    float _itemHeight = 20.0f;
    float _itemOffset = 15.0f;
    float _itemElemBorder = 3.0f;
    std::vector<Node2D *> _nodePools;
    std::vector<Node2D *> _treeNodes; // 节点-节点树item 映射
    std::map<std::string, NodeTreeStructure *> _treeNodeDataMap;
    /** 是否需要刷新 */
    bool _isDirty = true;
    void _setTrees(Node *root, NodeTreeStructure &uiTreeData, int layer);
    void _updateTreeContent();
    void _updateTreesItems(NodeTreeStructure &uiTreeData);
    void _createNodeItem();

    void _updateTreeItemSelect(Node2D *ndItem, NodeTreeStructure &uiTreeData);
    void _updateTreeItemFold(Node2D *ndItem, NodeTreeStructure &uiTreeData, float &_width);
    void _updateTreeItemIcon(Node2D *ndItem, NodeTreeStructure &uiTreeData, float &_width);
    void _updateTreeItemName(Node2D *ndItem, NodeTreeStructure &uiTreeData, float &_width);

    // void _onTreeContentHoverEvent(NodeInputResult &result);

private:
    NodeTreeStructure *_hoverTreeItem = nullptr;
    NodeTreeStructure *_selectTreeItem = nullptr;
    /**
     * 刷新节点树item的UI
     * @param ndItem 节点树item
     * @param state 刷新状态 0-正常 1-选中 2-悬浮
     */
    void _refreshTreeItemState(Node2D *ndItem, int state);
    void _refreshTreeItemUI(NodeTreeStructure *tree, int state);

    void _onTreeItemTouchEvent(NodeInputResult &result);
    void _onTreeItemCursorHoverEvent(NodeInputResult &result);

private:
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
    void onSelectEvent(std::function<void(std::string)> callback);
    void Update(float deltaTime) override;
    void LateUpdate(float deltaTime) override;
    void Disable() override;
    void destroy() override;
    ~EditorHierarchyNodeTree();
};
REGISTER_COMPONENT(EditorHierarchyNodeTree, "Editor Hierarchy Node Tree Component");
