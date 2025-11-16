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
    Node *ndBind;
    // /** 节点-文件树时使用 */
    // std::string pathBind;
    /** 子节点 */
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

    int _nodeIndex;
    std::vector<Node2D *> _nodePools;
    void _updateTreeContent();
    void _updateTreesItems(UITreeStructure &uiTreeData);
    void _onTreeItemTouchEvent(NodeInputResult &result);

    //   // 当前选中的节点
    // std::string _selectUUID;

    // 
    // std::function<void(UITreeStructure *)> _selectCallback;

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
    // item 选中节点树映射
    std::map<std::string, UITreeStructure *> _treeSelectMap;
    // 折叠按钮-节点树item 映射
    std::map<std::string, UITreeStructure *> _treeFoldMap;

public:
    UITree(Node *node, std::string uuid = "");

    void setIcon(std::string iconKey, std::string iconPath);

    // void onSelectEvent(std::function<void(UITreeStructure *)> callback);
    // void onUpdateEvent(std::function<void()> callback);

    void clearSelect();
    /** 失去焦点时淡化 */
    void loseFocusSelect();

    virtual void updateTree() = 0;
    void update(float deltaTime) override;
    void lateUpdate(float deltaTime) override;
    void render() override;
    void destroy() override;
    ~UITree();
};