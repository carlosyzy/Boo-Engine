#include "editor-hierarchy-node-tree.h"

#include "../../../boo-editor.h"

#include "../../../../engine/core/scene/scene.h"
#include "../../../../engine/core/scene/node-2d.h"
#include "../../../../engine/core/renderer/ui/ui-sprite.h"
#include "../../../../engine/core/renderer/ui/ui-text.h"
#include "../../../../engine/core/renderer/ui/ui-mask.h"
#include "../../../../engine/core/input/input.h"
#include "../../../../engine/core/component/ui/ui-widget.h"

EditorHierarchyNodeTree::EditorHierarchyNodeTree(std::string name, Node *node, std::string uuid) : Component(name, node, uuid)
{
    this->_layer = NodeLayer::Node2D;
    this->_topLen = 0.0f;
    this->_leftLen = 0.0f;
    this->_nodeIndex = 0;
}

void EditorHierarchyNodeTree::_deserialized()
{
    Component::_deserialized();
}
void EditorHierarchyNodeTree::Awake()
{
    Component::Awake();
    this->_initContent();
}
void EditorHierarchyNodeTree::_initContent()
{
    this->_ndMask = dynamic_cast<Node2D *>(this->_node->getChildByName("Content"));
    if (this->_ndMask == nullptr)
    {
        this->_ndMask = new Node2D("Content");
        this->_node->addChild(this->_ndMask);
        // 添加widget组件
        UIWidget *widget = dynamic_cast<UIWidget *>(this->_ndMask->addComponent("UIWidget"));
        WidgetHorizontalParam paramHorizontal{};
        paramHorizontal.left = 0.0f;
        paramHorizontal.right = 0.0f;
        WidgetVerticalParam paramVertical{};
        paramVertical.top = 0.0f;
        paramVertical.bottom = 0.0f;
        widget->setHorizontal(WidgetHorizontal::ALL, paramHorizontal);
        widget->setVertical(WidgetVertical::ALL, paramVertical);
        // 添加mask组件
        UIMask *mask = dynamic_cast<UIMask *>(this->_ndMask->addComponent("UIMask"));
    }
    else
    {
        UIMask *mask = dynamic_cast<UIMask *>(this->_ndMask->getComponent("UIMask"));
        if (mask == nullptr)
        {
            mask = dynamic_cast<UIMask *>(this->_ndMask->addComponent("UIMask"));
        }
    }
    Node *content = this->_ndMask->getChildByName("Content");
    if (content != nullptr)
    {
        this->_ndContent = dynamic_cast<Node2D *>(content);
    }
    else
    {
        this->_ndContent = new Node2D("Content");
        this->_ndMask->addChild(this->_ndContent);
    }
    this->_ndContent->setAnchor(0, 1);
    this->_ndContent->setSize(300.0f, 200.0f);
}

void EditorHierarchyNodeTree::Enable()
{
    Component::Enable();
    this->_isDirty;
    // 事件系统
    Node2D *node2d = dynamic_cast<Node2D *>(this->_node);
    this->_contentTouchID = node2d->onNodeInputEvent(NodeInput::TOUCH_END, &EditorHierarchyNodeTree::_onTreeContentTouchEvent, this);
    this->_contentHoverID = node2d->onNodeInputEvent(NodeInput::CURSOR_HOVER, &EditorHierarchyNodeTree::_onTreeContentCursorHoverEvent, this);
    this->_contentHoverOutID = node2d->onNodeInputEvent(NodeInput::CURSOR_HOVER_OUT, &EditorHierarchyNodeTree::_onTreeContentCursorHoverOutEvent, this);
}
void EditorHierarchyNodeTree::setNode(Node *node)
{
    this->_rootNode = node;
    this->_isDirty = true;
    this->_setTrees(node, this->_uiTreeData, 0);
}
void EditorHierarchyNodeTree::setScene(Scene *scene)
{
    this->_isDirty = true;
    this->_rootScene = scene;
    // 直接设置场景
    this->_uiTreeData.name = scene->getName();
    this->_uiTreeData.uuid = scene->getUuid();
    this->_uiTreeData.isFold = false;
    this->_uiTreeData.ndBind = nullptr;
    this->_uiTreeData.icon = "ic-scene.png";
    this->_uiTreeData.layer = 0;
    this->_uiTreeData.children.resize(scene->getChildren().size());
    std::cout << "EditorHierarchyNodeTree::setScene: " << scene->getName() << std::endl;
    for (int i = 0; i < scene->getChildren().size(); i++)
    {
        this->_setTrees(scene->getChildren()[i], this->_uiTreeData.children[i], 1);
    }
}
void EditorHierarchyNodeTree::_setTrees(Node *root, NodeTreeStructure &uiTreeData, int layer)
{
    // this->_nodes[root->getUuid()] = root;
    std::cout << "EditorHierarchyNodeTree::_setTrees: " << root->getName() << std::endl;
    uiTreeData.name = root->getName();
    uiTreeData.uuid = root->getUuid();
    if (root->getLayer() == NodeLayer::Node2D)
    {
        uiTreeData.icon = "ic-2d.png";
    }
    else
    {
        uiTreeData.icon = "ic-3d.png";
    }
    uiTreeData.isFold = false;
    uiTreeData.ndBind = nullptr;
    uiTreeData.layer = layer;
    uiTreeData.children.resize(root->getChildren().size());
    for (int i = 0; i < root->getChildren().size(); i++)
    {
        this->_setTrees(root->getChildren()[i], uiTreeData.children[i], layer + 1);
    }
}
void EditorHierarchyNodeTree::updateTree()
{
    if (this->_rootNode != nullptr)
    {
        this->setNode(this->_rootNode);
    }
    else if (this->_rootScene != nullptr)
    {
        this->setScene(this->_rootScene);
    }
}

void EditorHierarchyNodeTree::Update(float deltaTime)
{
    Component::Update(deltaTime);
}
void EditorHierarchyNodeTree::LateUpdate(float deltaTime)
{
    Component::LateUpdate(deltaTime);
    if (!this->_isEnabledInHierarchy)
        return; // 组件未激活
    if (this->_isDirty || this->_node->hasFrameTransformFlag())
    {
        // std::cout << "UITree UITree::update  0" << std::endl;
        this->_updateTreeContent();
        this->_isDirty = false;
    }
}
void EditorHierarchyNodeTree::_updateTreeContent()
{
    // std::cout << "UITree UITree::update  1" << std::endl;
    this->_nodeIndex = 0;
    this->_treeNodes.clear();
    this->_treeNodeDataMap.clear();
    // this->_foldUIMap.clear();

    Node2D *node = dynamic_cast<Node2D *>(this->_node);
    const Size &size = node->getSize();
    if (this->_uiTreeData.layer >= 0)
    {
        this->_updateTreesItems(this->_uiTreeData);
    }
    // 刷新节点池状态
    for (int i = this->_nodeIndex; i < this->_nodePools.size(); i++)
    {
        // std::cout << "UITree UITree::update  2:" << this->_nodePools.size() << "  " << i << std::endl;
        this->_nodePools[i]->setActive(false);
    }
    this->_contentWidth = size.getWidth() - 10.0f;
    this->_contentHeight = this->_nodeIndex * 25.0f;
    // 重新计算_topLen
    if (this->_contentHeight - this->_topLen < size.getHeight())
    {
        // 底部已经到底部
        this->_topLen = this->_contentHeight - size.getHeight();
    }
    if (this->_topLen < 0.0f)
    {
        // 顶部已经到底部
        this->_topLen = 0.0f;
    }
    this->_contentX = -size.getWidth() / 2.0f + this->_leftLen;
    this->_contentY = size.getHeight() / 2.0f + this->_topLen;
    this->_ndContent->setPosition(this->_contentX, this->_contentY, 0.0f);
    this->_ndContent->setSize(this->_contentWidth, this->_contentHeight);
    // std::cout << "UITree UITree::update  3" << this->_contentWidth << " " << this->_contentHeight << std::endl;
}
void EditorHierarchyNodeTree::_updateTreesItems(NodeTreeStructure &uiTreeData)
{
    Node2D *root = dynamic_cast<Node2D *>(this->_node);
    const Size &rootSize = root->getSize();
    uiTreeData.ndBind = nullptr;
    for (int i = this->_nodePools.size(); i <= this->_nodeIndex; i++)
    {
        this->_createNodeItem();
    }
    Node2D *node = this->_nodePools[this->_nodeIndex];
    float _width = 0;
    node->setName(uiTreeData.name);
    node->setActive(true);
    uiTreeData.ndBind = node;

    this->_treeNodes.push_back(node);
    this->_treeNodeDataMap.emplace(node->getUuid(), &uiTreeData);

    this->_updateTreeItemSelect(node, uiTreeData);
    this->_updateTreeItemFold(node, uiTreeData, _width);
    this->_updateTreeItemIcon(node, uiTreeData, _width);
    this->_updateTreeItemName(node, uiTreeData, _width);

    node->setSize(1000.0f, this->_itemHeight);
    node->setPosition(0.0f, -this->_itemHeight / 2.0f - this->_nodeIndex * this->_itemHeight, 0.0f);

    if (_width > this->_contentWidth)
    {
        this->_contentWidth = _width;
    }
    this->_nodeIndex++;
    if (!uiTreeData.isFold)
    {
        for (int i = 0; i < uiTreeData.children.size(); i++)
        {
            this->_updateTreesItems(uiTreeData.children[i]);
        }
    }
}
void EditorHierarchyNodeTree::_createNodeItem()
{
    // std::cout << "EditorHierarchyNodeTree::_createNodeItem" << std::endl;
    // item 节点
    Node2D *node = new Node2D("NodeTreeItem");
    node->setAnchor(0.0f, 0.5f);
    this->_ndContent->addChild(node);
    // 后续删除
    // sp = dynamic_cast<UISprite *>(node->addComponent("UISprite"));
    // sp->setColor(340.0f / 255.0f, 42.0f / 255.0f, 53.0f / 255.0f, 1.0f);
    // sp->setTexture("resources/texture/ic-default.png");
    node->onNodeInputEvent(NodeInput::TOUCH_END, &EditorHierarchyNodeTree::_onTreeItemTouchEvent, this);
    node->onNodeInputEvent(NodeInput::CURSOR_HOVER, &EditorHierarchyNodeTree::_onTreeItemCursorHoverEvent, this);
    // 选择框
    Node2D *ndSelect = new Node2D("NodeTreeItemSelect");
    node->addChild(ndSelect);
    ndSelect->setSize(1000.0f, this->_itemHeight);
    UISprite *spSelect = dynamic_cast<UISprite *>(ndSelect->addComponent("UISprite"));
    spSelect->setColor(0.0f / 250.0f, 74.0f / 255.0f, 93.0f / 255.0f, 0.0f);
    // 折叠符号
    Node2D *ndFold = new Node2D("NodeTreeItemFold");
    node->addChild(ndFold);
    ndFold->setSize(this->_itemHeight * 0.8, this->_itemHeight * 0.8);
    UISprite *spFold = dynamic_cast<UISprite *>(ndFold->addComponent("UISprite"));
    ndFold->onNodeInputEvent(NodeInput::TOUCH_END, &EditorHierarchyNodeTree::_onTreeItemFoldTouchEvent, this);
    // 图标
    Node2D *ndIcon = new Node2D("NodeTreeItemIcon");
    node->addChild(ndIcon);
    ndIcon->setSize(this->_itemHeight * 0.8, this->_itemHeight * 0.8);
    UISprite *spIcon = dynamic_cast<UISprite *>(ndIcon->addComponent("UISprite"));
    // 名字
    Node2D *ndName = new Node2D("NodeTreeItemName");
    ndName->setSize(this->_itemHeight * 0.8, this->_itemHeight * 0.8);
    node->addChild(ndName);
    UIText *txtName = dynamic_cast<UIText *>(ndName->addComponent("UIText"));
    txtName->setColor(204.0f / 255.0f, 207.0f / 255.0f, 213.0f / 255.0f, 1.0f);
    this->_nodePools.push_back(node);
}
void EditorHierarchyNodeTree::_updateTreeItemSelect(Node2D *ndItem, NodeTreeStructure &uiTreeData)
{
    Node *ndSelect = ndItem->getChildByName("NodeTreeItemSelect");
    Node2D *ndSelect2d = dynamic_cast<Node2D *>(ndSelect);
    UISprite *spSelect = dynamic_cast<UISprite *>(ndSelect2d->getComponent("UISprite"));
    if (this->_selectTreeItem != nullptr && uiTreeData.uuid == this->_selectTreeItem->uuid)
    {
        spSelect->setColor(9.0f / 250.0f, 74.0f / 255.0f, 93.0f / 255.0f, 1.0f);
        return;
    }
    if (this->_hoverTreeItem != nullptr && uiTreeData.uuid == this->_hoverTreeItem->uuid)
    {
        spSelect->setColor(9.0f / 250.0f, 74.0f / 255.0f, 93.0f / 255.0f, 0.3f);
        return;
    }
    spSelect->setColor(82.0f / 250.0f, 82.0f / 255.0f, 82.0f / 255.0f, 0.0f);
}
void EditorHierarchyNodeTree::_updateTreeItemFold(Node2D *ndItem, NodeTreeStructure &uiTreeData, float &_width)
{
    Node2D *ndFold = dynamic_cast<Node2D *>(ndItem->getChildByName("NodeTreeItemFold"));
    UISprite *spFold = dynamic_cast<UISprite *>(ndFold->getComponent("UISprite"));
    if (uiTreeData.children.size() > 0)
    {
        ndFold->setActive(true);
        if (uiTreeData.isFold)
        {
            TextureAsset *tex = BooEditor::cache->getEditorTexture("ic-arrow-right.png");
            spFold->setTexture(tex);
        }
        else
        {
            TextureAsset *tex = BooEditor::cache->getEditorTexture("ic-arrow-bottom.png");
            spFold->setTexture(tex);
        }
    }
    else
    {
        ndFold->setActive(false);
    }
    _width += this->_itemElemBorder;
    const Size &size = ndFold->getSize();
    float offset = this->_itemOffset * uiTreeData.layer;
    ndFold->setPosition(offset + size.getWidth() / 2.0f, 0.0f, 0.0f);
    _width += size.getWidth() + offset;
    // std::cout << "EditorHierarchyNodeTree::_updateTreeItemFold: " << uiTreeData.name << " width: " << _width << " layer: " << uiTreeData.layer << " offset: " << offset << std::endl;
}
void EditorHierarchyNodeTree::_updateTreeItemIcon(Node2D *ndItem, NodeTreeStructure &uiTreeData, float &_width)
{
    Node2D *ndIcon = dynamic_cast<Node2D *>(ndItem->getChildByName("NodeTreeItemIcon"));
    UISprite *spIcon = dynamic_cast<UISprite *>(ndIcon->getComponent("UISprite"));
    TextureAsset *tex = BooEditor::cache->getEditorTexture(uiTreeData.icon);
    spIcon->setTexture(tex);
    _width += this->_itemElemBorder;
    const Size &size = ndIcon->getSize();
    ndIcon->setPosition(_width + size.getWidth() / 2.0f, 0.0f, 0.0f);
    _width += size.getWidth();
}
void EditorHierarchyNodeTree::_updateTreeItemName(Node2D *ndItem, NodeTreeStructure &uiTreeData, float &_width)
{
    Node2D *ndName = dynamic_cast<Node2D *>(ndItem->getChildByName("NodeTreeItemName"));
    UIText *txtName = dynamic_cast<UIText *>(ndName->getComponent("UIText"));
    txtName->setText(uiTreeData.name);
    _width += this->_itemElemBorder;
    const Size &size = ndName->getSize();
    float itemNameWidth = ndName->getSize().getWidth();
    float itemNameHight = ndName->getSize().getHeight();
    ndName->setSize(itemNameWidth, itemNameHight);
    ndName->setPosition(_width + size.getWidth() / 2.0f, 0.0f, 0.0f);
    _width += size.getWidth();
}

void EditorHierarchyNodeTree::_onTreeContentTouchEvent(NodeInputResult &result)
{
    // std::cout << "EditorHierarchyNodeTree::_onTreeContentTouchEvent: " << result.node->getName() << std::endl;
    this->_refreshTreeItemState(nullptr, 0);
}
void EditorHierarchyNodeTree::_onTreeContentCursorHoverEvent(NodeInputResult &result)
{
    this->_refreshTreeItemState(nullptr, 2);
}
void EditorHierarchyNodeTree::_onTreeContentCursorHoverOutEvent(NodeInputResult &result)
{
    this->_refreshTreeItemUI(this->_hoverTreeItem, 0);
    this->_hoverTreeItem = nullptr;
}

void EditorHierarchyNodeTree::_onTreeItemTouchEvent(NodeInputResult &result)
{
    // std::cout << "EditorHierarchyNodeTree::_onTreeItemTouchEvent: " << result.node->getName() << std::endl;
    Node2D *ndItem = dynamic_cast<Node2D *>(result.node);
    this->_refreshTreeItemState(ndItem, 1);
    if (this->_selectTreeItem != nullptr && result.button == 1)
    {
        // 弹出菜单界面
        std::cout << "EditorHierarchyNodeTree::_onTreeItemTouchEvent: show menu panel" << std::endl;
    }
}
void EditorHierarchyNodeTree::_onTreeItemCursorHoverEvent(NodeInputResult &result)
{
    // std::cout << "EditorHierarchyNodeTree::_onTreeItemCursorHoverEvent: " << result.node->getName() << std::endl;
    Node2D *ndItem = dynamic_cast<Node2D *>(result.node);
    this->_refreshTreeItemState(ndItem, 2);
}

void EditorHierarchyNodeTree::_refreshTreeItemState(Node2D *ndItem, int state)
{
    if (state == 0)
    {
        // 取消悬浮和选中
        this->_refreshTreeItemUI(this->_hoverTreeItem, 0);
        this->_refreshTreeItemUI(this->_selectTreeItem, 0);
        this->_hoverTreeItem = nullptr;
        this->_selectTreeItem = nullptr;
        if (this->_selectCallback != nullptr)
        {
            this->_selectCallback("");
        }
    }
    else if (state == 1)
    {
        // 选中
        NodeTreeStructure *_tree = this->_treeNodeDataMap[ndItem->getUuid()];
        if (_tree == nullptr)
        {
            return;
        }
        if (this->_selectTreeItem != nullptr && this->_selectTreeItem->uuid == _tree->uuid)
        {
            // 重命名
            this->_refreshTreeItemUI(this->_selectTreeItem, 1);
        }
        else
        {
            this->_refreshTreeItemUI(this->_selectTreeItem, 0);
            this->_selectTreeItem = _tree;
            this->_refreshTreeItemUI(this->_selectTreeItem, 1);
            if (this->_selectCallback != nullptr)
            {
                this->_selectCallback(_tree->uuid);
            }
        }
    }
    else if (state == 2)
    {
        // 悬浮
        if (ndItem == nullptr)
        {
            if (this->_hoverTreeItem != nullptr && this->_selectTreeItem != nullptr && this->_hoverTreeItem->uuid == this->_selectTreeItem->uuid)
            {
                this->_hoverTreeItem = nullptr;
                return;
            }
            else
            {
                this->_refreshTreeItemUI(this->_hoverTreeItem, 0);
                this->_hoverTreeItem = nullptr;
            }
            return;
        }
        NodeTreeStructure *_tree = this->_treeNodeDataMap[ndItem->getUuid()];
        if (_tree == nullptr)
        {
            return;
        }
        // 重置上一个悬浮
        if (this->_hoverTreeItem != nullptr && this->_selectTreeItem != nullptr && this->_hoverTreeItem->uuid == this->_selectTreeItem->uuid)
        {
            // 排除选中的
            this->_hoverTreeItem = _tree;
            return;
        }
        this->_refreshTreeItemUI(this->_hoverTreeItem, 0);

        // 新的悬浮
        if (this->_selectTreeItem != nullptr && this->_selectTreeItem->uuid == _tree->uuid)
        {
            // 悬浮在选中的上面，不处理
            this->_hoverTreeItem = _tree;
            return;
        }
        else
        {
            this->_hoverTreeItem = _tree;
            this->_refreshTreeItemUI(this->_hoverTreeItem, 2);
        }
    }
}
void EditorHierarchyNodeTree::_refreshTreeItemUI(NodeTreeStructure *tree, int state)
{
    if (tree == nullptr)
    {
        return;
    }
    Node *ndItem = tree->ndBind;
    if (ndItem == nullptr)
    {
        return;
    }
    Node *ndSelect = ndItem->getChildByName("NodeTreeItemSelect");
    if (ndSelect == nullptr)
    {
        return;
    }
    Node2D *ndSelect2d = dynamic_cast<Node2D *>(ndSelect);
    if (ndSelect2d == nullptr)
    {
        return;
    }
    UISprite *spSelect = dynamic_cast<UISprite *>(ndSelect2d->getComponent("UISprite"));
    if (spSelect == nullptr)
    {
        return;
    }
    if (state == 0)
    {
        // 正常
        spSelect->setColor(82.0f / 250.0f, 82.0f / 255.0f, 82.0f / 255.0f, 0.0f);
    }
    else if (state == 1)
    {
        // 选中
        spSelect->setColor(9.0f / 250.0f, 74.0f / 255.0f, 93.0f / 255.0f, 1.0f);
    }
    else if (state == 2)
    {
        // 悬浮
        spSelect->setColor(9.0f / 250.0f, 74.0f / 255.0f, 93.0f / 255.0f, 0.3f);
    }
}
void EditorHierarchyNodeTree::_onTreeItemFoldTouchEvent(NodeInputResult &result)
{
    Node2D *ndFold = result.node;
    if (ndFold == nullptr)
    {
        return;
    }
    Node2D *ndItem = dynamic_cast<Node2D *>(ndFold->getParent());
    if (ndItem == nullptr)
    {
        return;
    }
    NodeTreeStructure *_tree = this->_treeNodeDataMap[ndItem->getUuid()];
    if (_tree == nullptr)
    {
        return;
    }
    _tree->isFold = !_tree->isFold;
    this->_isDirty = true;
}

void EditorHierarchyNodeTree::onSelectEvent(std::function<void(std::string)> callback)
{
    this->_selectCallback = callback;
}

void EditorHierarchyNodeTree::Disable()
{
    Component::Disable();
    Node2D *node2d = dynamic_cast<Node2D *>(this->_node);
    node2d->offNodeInputEvent(this->_contentTouchID);
    node2d->offNodeInputEvent(this->_contentHoverID);
    node2d->offNodeInputEvent(this->_contentHoverOutID);
}
void EditorHierarchyNodeTree::destroy()
{
    Component::destroy();
}
EditorHierarchyNodeTree::~EditorHierarchyNodeTree()
{
}