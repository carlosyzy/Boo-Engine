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

struct FileTreeStructure
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
    std::vector<FileTreeStructure> children;
};

class EditorAssetsFileTree : public Component
{
private:
    int _contentTouchID = -1;
    int _contentHoverID = -1;
    int _contentCursorHoverID = -1;
    void _onTreeContentTouchEvent(NodeInputResult &result);
    void _onTreeContentCursorHoverEvent(NodeInputResult &result);
    void _onTreeContentCursorHoverOutEvent(NodeInputResult &result);


    // 选择事件回调
    std::function<void(std::string)> _selectCallback;

private:
    float _topLen;
    float _leftLen;
    Node2D *_ndMask;
    Node2D *_ndContent;
    UISprite *_spContent;
    float _contentWidth;
    float _contentHeight;
    float _contentX;
    float _contentY;
    void _initContent();

private:
    std::string _root;
    FileTreeStructure _uiTreeData; // 节点树数据
    int _nodeIndex;
    float _itemHeight = 20.0f;
    float _itemOffset = 15.0f;
    float _itemElemBorder = 3.0f;
    std::vector<Node2D *> _nodePools;
    std::vector<Node2D *> _treeNodes; // 节点-节点树item 映射
    std::map<std::string, FileTreeStructure *> _treeNodeDataMap;
    /** 是否需要刷新 */
    bool _isDirty = true;
    void _setTrees(std::filesystem::directory_entry entry, FileTreeStructure &uiTreeData, int layer);
    void _updateTreeContent();
    void _updateTreesItems(FileTreeStructure &uiTreeData);
    void _createNodeItem();

    void _updateTreeItemSelect(Node2D *ndItem, FileTreeStructure &uiTreeData);
    void _updateTreeItemFold(Node2D *ndItem, FileTreeStructure &uiTreeData, float &_width);
    void _updateTreeItemIcon(Node2D *ndItem, FileTreeStructure &uiTreeData, float &_width);
    void _updateTreeItemName(Node2D *ndItem, FileTreeStructure &uiTreeData, float &_width);

    // void _onTreeContentHoverEvent(NodeInputResult &result);

private:
    FileTreeStructure *_hoverTreeItem = nullptr;
    FileTreeStructure *_selectTreeItem = nullptr;
    /**
     * 刷新节点树item的UI
     * @param ndItem 节点树item
     * @param state 刷新状态 0-正常 1-选中 2-悬浮
     */
    void _refreshTreeItemState(Node2D *ndItem, int state);
    void _refreshTreeItemUI(FileTreeStructure *tree, int state);

    void _onTreeItemTouchEvent(NodeInputResult &result);
    void _onTreeItemCursorHoverEvent(NodeInputResult &result);
    void _onTreeItemFoldTouchEvent(NodeInputResult &result);

private:
protected:
    void _deserialized() override;

public:
    EditorAssetsFileTree(std::string name, Node *node, std::string uuid = "");

    // 组件生命周期
    void Awake() override;
    void Enable() override;
    void setRoot(std::string root);
    void updateTree();
    void onSelectEvent(std::function<void(std::string)> callback);
    void Update(float deltaTime) override;
    void LateUpdate(float deltaTime) override;
    void Disable() override;
    void destroy() override;
    ~EditorAssetsFileTree();
};
REGISTER_COMPONENT(EditorAssetsFileTree, "Editor Assets File Tree Component");
