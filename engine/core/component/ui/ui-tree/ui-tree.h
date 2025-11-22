/**
 * 树形结构ui展示组件
 */
#pragma once
#include <string>
#include <cstdint>
#include <vector>
#include <map>
#include <unordered_map>
#include <functional>
#include <random>
#include "../../component.h"

#include "../../../math/mat4.h"
#include "../../../math/vec3.h"
#include "../../../input/input.h"

class Node;
class Node2D;
class UISprite;
class Scene;

struct UITreeIconMap
{
    std::string iconScene = "resources/texture/icon/ic-scene.png";
    std::string iconNode2D = "resources/texture/icon/ic-2d.png";
    std::string iconNode3D = "resources/texture/icon/ic-3d.png";
};

struct UITreeStructure
{
    /** 节点名称 */
    std::string name = "";
    /** 节点唯一标识 */
    std::string uuid = "";
    /** 是否折叠 */
    bool isFold = false;
    /**层级 */
    int layer = -1;
    /** 节点图标 */
    std::string iconKey;
    /** 节点在树中的索引 */
    int index;
    /** 节点-节点树时使用 */
    Node2D *ndBind;
    std::vector<UITreeStructure> children;
};

class UITree : public Component
{
private:
    int _nodeTransform_ID = 0;
    void _onNodeTransformChange(int nodeTransform);

    float _topLen;
    float _leftLen;
    Node2D *_ndContent;
    UISprite *_spContent;
    float _contentWidth;
    float _contentHeight;
    float _contentX;
    float _contentY;
    void _initContent();
    void _onTreeContentTouchEvent(NodeInputResult &result);
    void _onTreeContentHoverEvent(NodeInputResult &result);

    int _nodeIndex;
    std::vector<Node2D *> _nodePools;
    UITreeStructure _hoverTreeItem;
    void _updateTreeContent();
    void _updateTreesItems(UITreeStructure &uiTreeData);
    void _onTreeItemTouchEvent(NodeInputResult &result);
    void _onTreeItemCursorHoverEvent(NodeInputResult &result);

    // 当前选中的节点树item
    UITreeStructure _selectTreeItem;
    bool _isSelectHover = false;
    // 当前显示的节点树映射
    std::map<std::string, UITreeStructure> _treeUIMap;
    // 选择事件回调
    std::function<void(std::string)> _selectCallback;
    void _refreshTreeItemUI(Node2D *ndItem, int state);

    // item 的touch 事件

    // // item 折叠按钮的touch 事件
    // void _onTreeItemFoldTouchEvent(NodeInputResult &result);
    // // 滑动条touch 事件
    // bool _isScrollBarTouching = false;
    // void _onScrollBarTouchStartEvent(NodeInputResult &result);
    // void _onScrollBarTouchMoveEvent(NodeInputResult &result);
    // // 滑动条touch 结束事件
    // void _onScrollBarTouchEndEvent(NodeInputResult &result);

    // std::function<void()> _updateCallback;

protected:
    /** 是否需要刷新 */
    bool _isDirty = true;
    /** 图标映射 */
    UITreeIconMap _iconMaps;
    // 节点树数据
    UITreeStructure _uiTreeData;

    // 折叠按钮-节点树item 映射
    std::map<std::string, UITreeStructure *> _treeFoldMap;

public:
    UITree(Node *node, std::string uuid = "");
    void Awake() override;
    void setIcon(std::string iconKey, std::string iconPath);

    void onSelectEvent(std::function<void(std::string)> callback);

    void clearSelect();
    /**
     * 悬停选中节点树item
     */
    void hoverSelect();
    virtual void updateTree() = 0;
    void Enable() override;
    void Update(float deltaTime) override;
    void LateUpdate(float deltaTime) override;
    void Render() override;
    void LateRender() override;
    void Disable() override;
    void destroy() override;
    ~UITree();
};