#include "ui-tree.h"
#include "../../../math/size.h"
#include "../../../math/vec3.h"
#include "../../../math/vec2.h"
#include "../../../scene/node.h"
#include "../../../scene/node-2d.h"
#include "../../../renderer/ui/ui-sprite.h"
#include "../../../renderer/ui/ui-text.h"

UITree::UITree(Node *node, std::string uuid) : Component(node, uuid)
{
    this->_layer = NodeLayer::Node2D;
    this->_topLen = 0.0f;
    this->_leftLen = 0.0f;
    this->_nodeIndex = 0;
    this->_initContent();

    // 事件系统
    Node2D *node2d = dynamic_cast<Node2D *>(this->_node);
    node2d->onNodeInputEvent(NodeInput::TOUCH_END, &UITree::_onTreeContentTouchEvent, this);
    node2d->onNodeInputEvent(NodeInput::CURSOR_HOVER, &UITree::_onTreeContentHoverEvent, this);
}
void UITree::Awake()
{
    Component::Awake();
}
void UITree::Enable()
{
    Component::Enable();
    this->_isDirty;
}
void UITree::_initContent()
{
    // 在当前组件的子节点中获取第一个名字为Content的节点
    // 获取失败时创建
    Node *content = this->_node->getChildByName("Content");
    if (content != nullptr)
    {
        this->_ndContent = dynamic_cast<Node2D *>(content);
    }
    else
    {
        this->_ndContent = new Node2D("Content");
        this->_node->addChild(this->_ndContent);
    }
    this->_ndContent->setAnchor(0, 1);
    this->_ndContent->setSize(300.0f, 200.0f);

    // // 确定后不需要渲染组件
    // this->_spContent = static_cast<UISprite *>(this->_ndContent->addComponent("UISprite"));
    // if (this->_spContent != nullptr)
    // {
    //     this->_spContent->setColor(34.0f / 255.0f, 42.0f / 255.0f, 53.0f / 255.0f, 1.0f);
    //     this->_spContent->setTexture("resources/texture/ic-default.png");
    //     this->_spContent->setMaterial(nullptr);
    // }
}
void UITree::setIcon(std::string iconKey, std::string iconPath)
{
    if (iconKey == "iconScene")
    {
        this->_iconMaps.iconScene = iconPath;
    }
    else if (iconKey == "iconNode2D")
    {
        this->_iconMaps.iconNode2D = iconPath;
    }
    else if (iconKey == "iconNode3D")
    {
        this->_iconMaps.iconNode3D = iconPath;
    }
}
// void UITree::_initSelect()
// {
//     // this->_ndSelect = new Node2D("Select");
//     // this->_ndContent->addChild(this->_ndSelect);
//     // this->_ndSelect->setAnchor(0.5, 0.5);
//     // this->_spSelect = this->_ndSelect->addComponent<Sprite>("Sprite");
//     // this->_spSelect->setTexture("resources/textures/ic-default.png");
//     // this->_spSelect->setColor(9.0f / 250.0f, 74.0f / 255.0f, 53.0f / 255.0f, 1.0f);
//     // this->_selectWidth = 100.0f;
//     // this->_selectHeight = 20.0f;
//     // this->_ndSelect->setSize(this->_selectWidth, this->_selectHeight);
//     // this->_ndSelect->setPosition(0, 0, 0);
//     // this->_ndSelect->setActive(false);
// }
// void UITree::_initScrollView()
// {
//     // // 滑动条背景
//     // this->_ndScrollBarBg = new Node2D("ScrollBarBg");
//     // this->_node->addChild(this->_ndScrollBarBg);
//     // this->_ndScrollBarBg->setAnchor(0.5, 0.5);
//     // this->_spScrollBarBg = this->_ndScrollBarBg->addComponent<Sprite>("Sprite");
//     // this->_spScrollBarBg->setTexture("resources/textures/ic-default.png");
//     // this->_spScrollBarBg->setColor(24.0f / 255.0f, 32.0f / 255.0f, 43.0f / 255.0f, 1.0f);
//     // this->_scrollBarBgWidth = 8.0f;
//     // this->_scrollBarBgHeight = 20.0f;
//     // // 滑动条
//     // this->_ndScrollBar = new Node2D("ScrollBar");
//     // this->_ndScrollBar->setAnchor(0.5, 0.5);
//     // this->_ndScrollBarBg->addChild(this->_ndScrollBar);
//     // this->_spScrollBar = this->_ndScrollBar->addComponent<Sprite>("Sprite");
//     // this->_spScrollBar->setTexture("resources/textures/ic-default.png");
//     // this->_spScrollBar->setColor(140.0f / 255.0f, 210.0f / 255.0f, 240.0f / 255.0f, 1.0f);
//     // this->_scrollBarWidth = this->_scrollBarBgWidth - 1.0f;
//     // this->_scrollBarHeight = 20;
//     // this->_ndScrollBar->setSize(this->_scrollBarWidth, this->_scrollBarHeight);
//     // this->_ndScrollBar->onNodeInputEvent(NodeInput::TOUCH_START, [this](NodeInputResult &result)
//     //                                      { this->_onScrollBarTouchStartEvent(result); }, true);
//     // this->_ndScrollBar->onNodeInputEvent(NodeInput::TOUCH_MOVE, [this](NodeInputResult &result)
//     //                                      { this->_onScrollBarTouchMoveEvent(result); }, true);
//     // this->_ndScrollBar->onNodeInputEvent(NodeInput::TOUCH_END, [this](NodeInputResult &result)
//     //                                      { this->_onScrollBarTouchEndEvent(result); }, true);
//     // this->_ndScrollBar->onNodeInputEvent(NodeInput::TOUCH_CANCEL, [this](NodeInputResult &result)
//     //                                      { this->_onScrollBarTouchEndEvent(result); }, true);
// }

// void UITree::_onScrollBarTouchStartEvent(NodeInputResult &result)
// {
//     // 选中状态
//     this->_isScrollBarTouching = true;
//     this->_scrollBarTouch.setX(result.worldX);
//     this->_scrollBarTouch.setY(result.worldY);
// }
// void UITree::_onScrollBarTouchMoveEvent(NodeInputResult &result)
// {
//     if (!this->_isScrollBarTouching)
//         return;
//     // Mat4 mat{};
//     // Vec3 local{};
//     // local.setX(result.worldX);
//     // local.setY(result.worldY);
//     // local.setZ(0);
//     // Mat4::inverse(this->_ndScrollBar->worldMatrix(), mat);
//     // Mat4::multiplyVec3(mat, local, local);
//     this->_scrollBarX = 0.0f;
//     this->_scrollBarY += result.worldY - this->_scrollBarTouch.y();
//     // std::cout << "滑动条移动:" << "    x:" << this->_scrollBarX << ",    y:" << this->_scrollBarY << std::endl;
//     if (this->_scrollBarY > this->_scrollBarBgHeight / 2 - this->_scrollBarHeight / 2)
//     {
//         this->_scrollBarY = this->_scrollBarBgHeight / 2 - this->_scrollBarHeight / 2;
//     }
//     if (this->_scrollBarY < -this->_scrollBarBgHeight / 2 + this->_scrollBarHeight / 2)
//     {
//         this->_scrollBarY = -this->_scrollBarBgHeight / 2 + this->_scrollBarHeight / 2;
//     }

//     this->_ndScrollBar->setPosition(this->_scrollBarX, this->_scrollBarY, 0);
//     this->_scrollBarTouch.setX(result.worldX);
//     this->_scrollBarTouch.setY(result.worldY);

//     this->_topLen = this->_scrollBarBgHeight / 2.0f - this->_scrollBarHeight / 2.0f - this->_scrollBarY;
//     this->_topLen = this->_topLen * this->_scrollBarRate;
//     Node2D *node = dynamic_cast<Node2D *>(this->_node);
//     Size &size = node->size();
//     this->_ndContent->setPosition(-size.width() / 2.0f + this->_leftLen, size.height() / 2 + this->_topLen, 0.0f);
// }
// // 取消选中状态
// void UITree::_onScrollBarTouchEndEvent(NodeInputResult &result)
// {
//     this->_isScrollBarTouching = false;
// }
void UITree::onSelectEvent(std::function<void(std::string)> callback)
{
    this->_selectCallback = callback;
}
/**
 * 上下文菜单事件
 */
void UITree::onMenuEvent(std::function<void(std::string, int x, int y)> callback)
{
    this->_menuCallback = callback;
}

void UITree::_onTreeContentTouchEvent(NodeInputResult &result)
{
    if (result.button == 1)
    {
        // 右键
        if (this->_menuCallback != nullptr)
        {
            this->_menuCallback(this->_selectTreeItem ? this->_selectTreeItem->uuid : "", result.worldX, result.worldY);
        }
        return;
    }
    else if (result.button == 2)
    {
        // 中键
        return;
    }

    this->clearSelect();
}
void UITree::_onTreeContentHoverEvent(NodeInputResult &result)
{
    if (this->_hoverTreeItem != nullptr && this->_selectTreeItem != nullptr && this->_hoverTreeItem->uuid == this->_selectTreeItem->uuid)
    {
        this->_hoverTreeItem = nullptr;
        return;
    }
    if (this->_hoverTreeItem != nullptr)
    {
        this->_refreshTreeItemUI(this->_hoverTreeItem->ndBind, 0);
        this->_hoverTreeItem = nullptr;
    }
}

void UITree::updateTree()
{
    this->_isDirty = true;
}
void UITree::Update(float deltaTime)
{
    Component::Update(deltaTime);
    if (!this->_isEnabledInHierarchy)
        return; // 组件未激活
}
void UITree::LateUpdate(float deltaTime)
{
    Component::LateUpdate(deltaTime);
    if (!this->_isEnabledInHierarchy)
        return; // 组件未激活
    if (this->_isDirty || this->_node->hasFrameTransformFlag())
    {
        this->_updateTreeContent();
        this->_isDirty = false;
    }
}

void UITree::_updateTreeContent()
{
    std::cout << "UITree UITree::update  1" << std::endl;
    this->_nodeIndex = 0;
    this->_treeUIMap.clear();
    this->_foldUIMap.clear();

    Node2D *node = dynamic_cast<Node2D *>(this->_node);
    const Size &size = node->getSize();
    if (this->_uiTreeData.layer >= 0)
    {
        this->_updateTreesItems(this->_uiTreeData);
    }
    // 刷新节点池状态
    for (int i = this->_nodeIndex; i < this->_nodePools.size(); i++)
    {
        std::cout << "UITree UITree::update  2:" << this->_nodePools.size() << "  " << i << std::endl;
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
}

void UITree::_updateTreesItems(UITreeStructure &uiTreeData)
{
    uiTreeData.ndBind = nullptr;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> randomGet(0.0, 1.0);

    Node2D *root = dynamic_cast<Node2D *>(this->_node);
    const Size &size = root->getSize();

    float offset = 10.0f;
    float itemWidth = 10.0f;
    float itemHeight = 22.0f;
    Node2D *node = nullptr;
    UISprite *sp = nullptr;
    Node2D *ndSelect = nullptr;
    UISprite *spSelect = nullptr;
    Node2D *ndFold = nullptr;
    UISprite *spFold = nullptr;
    Node2D *ndIcon = nullptr;
    UISprite *spIcon = nullptr;
    Node2D *ndName = nullptr;
    UIText *txtName = nullptr;

    if (this->_nodeIndex >= this->_nodePools.size())
    {
        // item 节点
        node = new Node2D("NodeTreeItem");
        this->_ndContent->addChild(node);
        // 后续删除
        // sp = dynamic_cast<UISprite *>(node->addComponent("UISprite"));
        // sp->setColor(340.0f / 255.0f, 42.0f / 255.0f, 53.0f / 255.0f, 1.0f);
        // sp->setTexture("resources/texture/ic-default.png");
        // sp->setMaterial(nullptr);
        node->onNodeInputEvent(NodeInput::TOUCH_END, &UITree::_onTreeItemTouchEvent, this);
        node->onNodeInputEvent(NodeInput::CURSOR_HOVER, &UITree::_onTreeItemCursorHoverEvent, this);
        // 选择框
        ndSelect = new Node2D("NodeTreeItemSelect");
        node->addChild(ndSelect);
        ndSelect->setSize(1000.0f, itemHeight - 4);
        spSelect = dynamic_cast<UISprite *>(ndSelect->addComponent("UISprite"));
        spSelect->setTexture("resources/texture/ic-default.png");
        spSelect->setMaterial(nullptr);
        spSelect->setColor(9.0f / 250.0f, 74.0f / 255.0f, 93.0f / 255.0f, 0.0f);
        // 折叠符号
        ndFold = new Node2D("NodeTreeItemFold");
        node->addChild(ndFold);
        ndFold->setSize(16.0f, 16.0f);
        spFold = dynamic_cast<UISprite *>(ndFold->addComponent("UISprite"));
        spFold->setMaterial(nullptr);
        ndFold->onNodeInputEvent(NodeInput::TOUCH_END, &UITree::_onTreeItemFoldTouchEvent, this);
        // 图标
        ndIcon = new Node2D("NodeTreeItemIcon");
        node->addChild(ndIcon);
        ndIcon->setSize(16.0f, 16.0f);
        spIcon = dynamic_cast<UISprite *>(ndIcon->addComponent("UISprite"));
        spIcon->setMaterial(nullptr);
        // 名字
        ndName = new Node2D("NodeTreeItemName");
        ndName->setSize(14.0f, 14.0f);
        node->addChild(ndName);
        txtName = dynamic_cast<UIText *>(ndName->addComponent("UIText"));
        txtName->setColor(204.0f / 255.0f, 207.0f / 255.0f, 213.0f / 255.0f, 1.0f);
        txtName->setMaterial(nullptr);
        this->_nodePools.push_back(node);
    }
    else
    {
        node = this->_nodePools[this->_nodeIndex];
        Node *select = node->getChildByName("NodeTreeItemSelect");
        Node *fold = node->getChildByName("NodeTreeItemFold");
        Node *icon = node->getChildByName("NodeTreeItemIcon");
        Node *name = node->getChildByName("NodeTreeItemName");
        if (select != nullptr)
        {
            ndSelect = dynamic_cast<Node2D *>(select);
            spSelect = dynamic_cast<UISprite *>(ndSelect->getComponent("UISprite"));
        }
        if (fold != nullptr)
        {
            ndFold = dynamic_cast<Node2D *>(fold);
            spFold = dynamic_cast<UISprite *>(ndFold->getComponent("UISprite"));
        }
        if (icon != nullptr)
        {
            ndIcon = dynamic_cast<Node2D *>(icon);
            spIcon = dynamic_cast<UISprite *>(ndIcon->getComponent("UISprite"));
        }
        if (name != nullptr)
        {
            ndName = dynamic_cast<Node2D *>(name);
            txtName = dynamic_cast<UIText *>(ndName->getComponent("UIText"));
        }
    }
    uiTreeData.ndBind = node;
    uiTreeData.index = this->_nodeIndex;
    this->_treeUIMap.emplace(node->getUuid(), &uiTreeData);
    this->_foldUIMap.emplace(ndFold->getUuid(), &uiTreeData);
    node->setActive(true);
    ndSelect->setSize(1000.0f, itemHeight - 4);
    ndSelect->setPosition(0.0f, 0.0f, 0.0f);
    itemWidth += (uiTreeData.layer * offset);
    // 折叠图标
    ndFold->setSize(16.0f, 16.0f);
    // std::cout << "UITree UITree::name: " << uiTreeData.name << " fold: " << uiTreeData.isFold << std::endl;
    if (uiTreeData.isFold)
    {
        spFold->setTexture("resources/texture/ic-arrow-right.png");
    }
    else
    {
        spFold->setTexture("resources/texture/ic-arrow-bottom.png");
    }
    if (uiTreeData.children.size() > 0)
    {
        ndFold->setActive(true);
    }
    else
    {
        ndFold->setActive(false);
    }

    itemWidth += ndFold->getSize().getWidth();
    // 图标
    ndIcon->setSize(16.0f, 16.0f);
    spIcon->setTexture(uiTreeData.iconKey);
    itemWidth += ndIcon->getSize().getWidth();
    // 创建名字
    txtName->setText(uiTreeData.name);
    float itemNameWidth = ndName->getSize().getWidth();
    float itemNameHight = ndName->getSize().getHeight();
    ndName->setSize(itemNameWidth, itemNameHight);
    itemWidth += itemNameWidth;
    itemWidth += 5.0f * 3;

    // 设置总宽度
    if (itemWidth < size.getWidth() - 3)
    {
        itemWidth = size.getWidth() - 3;
    }
    // 折叠按钮
    float startX = -itemWidth / 2.0 + (uiTreeData.layer * offset);
    ndFold->setPosition(startX + ndFold->getSize().getWidth() / 2.0, 0.0f, 0.0f);
    // 图标
    startX += ndFold->getSize().getWidth() + 5.0f;
    ndIcon->setPosition(startX + ndIcon->getSize().getWidth() / 2.0, 0.0f, 0.0f);
    // 名字
    startX += ndIcon->getSize().getWidth() + 5.0f;
    ndName->setPosition(startX + itemNameWidth / 2.0, 0.0f, 0.0f);

    node->setSize(itemWidth, itemHeight);
    node->setPosition(itemWidth / 2.0f, -itemHeight / 2.0f - this->_nodeIndex * itemHeight, 0.0f);
    if (itemWidth > this->_contentWidth)
    {
        this->_contentWidth = itemWidth;
    }
    // 递归更新子节点
    this->_nodeIndex++;
    if (!uiTreeData.isFold)
    {
        // std::cout << "UITree UITree::name: " << uiTreeData.name << " children size: " << uiTreeData.children.size() << std::endl;
        for (int i = 0; i < uiTreeData.children.size(); i++)
        {
            this->_updateTreesItems(uiTreeData.children[i]);
        }
    }
}

void UITree::_onTreeItemCursorHoverEvent(NodeInputResult &result)
{
    if (this->_treeUIMap.find(result.node->getUuid()) == this->_treeUIMap.end())
    {
        return;
    }
    UITreeStructure *uiTreeData = this->_treeUIMap[result.node->getUuid()];
    // 悬停在了同一个节点上
    if (this->_hoverTreeItem && this->_hoverTreeItem->uuid == uiTreeData->uuid)
    {
        // 重复悬停在同一个节点上
        this->_hoverTreeItem = uiTreeData;
        return;
    }
    // 悬停在了选中节点上
    if (this->_selectTreeItem && this->_selectTreeItem->uuid == uiTreeData->uuid)
    {
        if (this->_hoverTreeItem)
        {
            this->_refreshTreeItemUI(this->_hoverTreeItem->ndBind, 0); // 取消悬停
        }
        this->_hoverTreeItem = uiTreeData;
        return;
    }

    if (this->_hoverTreeItem == nullptr)
    {
        // 之前没有悬停的节点 直接悬停在节点上
        this->_hoverTreeItem = uiTreeData;
        this->_refreshTreeItemUI(this->_hoverTreeItem->ndBind, 2);
        return;
    }
    // 切换前 悬停在选中节点上，不取消选中
    if (this->_selectTreeItem && this->_hoverTreeItem->uuid == this->_selectTreeItem->uuid)
    {
        this->_hoverTreeItem = uiTreeData;
        this->_refreshTreeItemUI(this->_hoverTreeItem->ndBind, 2); // 悬停在节点上
        return;
    }
    this->_refreshTreeItemUI(this->_hoverTreeItem->ndBind, 0); // 取消悬停
    this->_hoverTreeItem = uiTreeData;
    this->_refreshTreeItemUI(this->_hoverTreeItem->ndBind, 2); // 悬停在节点上
}
void UITree::_onTreeItemTouchEvent(NodeInputResult &result)
{
    if (result.button == 1)
    {
        // 右键
        if (this->_menuCallback != nullptr)
        {
            this->_menuCallback(this->_selectTreeItem ? this->_selectTreeItem->uuid : "", result.worldX, result.worldY);
        }
        return;
    }
    else if (result.button == 2)
    {
        // 中键
        return;
    }

    if (this->_treeUIMap.find(result.node->getUuid()) == this->_treeUIMap.end())
    {
        return;
    }
    UITreeStructure *uiTreeData = this->_treeUIMap[result.node->getUuid()];

    if (this->_selectTreeItem != nullptr && this->_selectTreeItem->uuid == uiTreeData->uuid)
    {
        // 点击了已经选中的
        if (this->_isSelectHover)
        {
            this->_refreshTreeItemUI(result.node, 1); // 取消悬停，重新选中
        }
        else
        {
            this->_selectTreeItem = nullptr;
            this->_refreshTreeItemUI(result.node, 0); // 取消悬停，重新选中
        }
    }
    else
    {
        if (this->_selectTreeItem != nullptr)
        {
            this->_refreshTreeItemUI(this->_selectTreeItem->ndBind, 0); // 取消之前选中的节点
        }
        this->_selectTreeItem = uiTreeData;
        this->_refreshTreeItemUI(this->_selectTreeItem->ndBind, 1); // 重新选中选中
        this->_isSelectHover = false;
    }
    if (this->_selectCallback != nullptr)
    {
        this->_selectCallback(this->_selectTreeItem ? this->_selectTreeItem->uuid : "");
    }
}
void UITree::_refreshTreeItemUI(Node2D *ndItem, int state)
{
    if (ndItem == nullptr)
    {
        return;
    }
    Node *ndSelect = ndItem->getChildByName("NodeTreeItemSelect");
    if (ndSelect != nullptr)
    {
        Node2D *ndSelect2d = dynamic_cast<Node2D *>(ndSelect);
        UISprite *spSelect = dynamic_cast<UISprite *>(ndSelect2d->getComponent("UISprite"));
        if (spSelect != nullptr)
        {
            if (state == 0)
            {
                spSelect->setColor(82.0f / 250.0f, 82.0f / 255.0f, 82.0f / 255.0f, 0.0f);
            }
            else if (state == 1)
            {
                spSelect->setColor(9.0f / 250.0f, 74.0f / 255.0f, 93.0f / 255.0f, 1.0f);
            }
            else if (state == 2)
            {
                spSelect->setColor(82.0f / 250.0f, 82.0f / 255.0f, 82.0f / 255.0f, 0.5f);
            }
        }
    }
}
void UITree::_onTreeItemFoldTouchEvent(NodeInputResult &result)
{
    if (this->_foldUIMap.find(result.node->getUuid()) == this->_foldUIMap.end())
    {
        return;
    }
    UITreeStructure *uiTreeData = this->_foldUIMap[result.node->getUuid()];
    uiTreeData->isFold = uiTreeData->isFold ? false : true;
    this->_isDirty = true;
    std::cout << "fold touch event" << result.node->getName() << " is fold: " << uiTreeData->isFold << std::endl;
}

void UITree::clearSelect()
{
    if (this->_selectTreeItem != nullptr)
    {
        this->_refreshTreeItemUI(this->_selectTreeItem->ndBind, 0); // 取消悬停，重新选中
    }
    this->_selectTreeItem = nullptr;
    this->_isSelectHover = false;
}
void UITree::hoverSelect()
{
    if (this->_selectTreeItem != nullptr)
    {
        this->_refreshTreeItemUI(this->_selectTreeItem->ndBind, 2); // 悬停在节点上
        this->_isSelectHover = true;
    }
}

void UITree::Render()
{
    Component::Render();
    if (!this->_isEnabledInHierarchy)
        return; // 组件未激活
}
void UITree::LateRender()
{
    Component::LateRender();
    if (!this->_isEnabledInHierarchy)
        return; // 组件未激活
}

void UITree::Disable()
{
    Component::Disable();
}

void UITree::destroy()
{
    this->_selectCallback = nullptr;
    Component::destroy();
}
void UITree::clearGfxObject()
{
    Component::clearGfxObject();
}
UITree::~UITree()
{
}
