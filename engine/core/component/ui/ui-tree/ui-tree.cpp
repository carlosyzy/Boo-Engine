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
    this->_nodeTransform_ID = this->_node->onTransformChange(&UITree::_onNodeTransformChange, this);
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
    // 确定后不需要渲染组件
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

// void UITree::onSelectEvent(std::function<void(UITreeStructure *)> callback)
// {
//     this->_selectCallback = callback;
// }
// void UITree::onUpdateEvent(std::function<void()> callback)
// {
//     this->_updateCallback = callback;
// }

void UITree::updateTree()
{
    this->_updateTreeContent();
}
void UITree::_onNodeTransformChange(int nodeTransform)
{
    this->_updateTreeContent();
}

void UITree::_updateTreeContent()
{
    // std::cout << "UITree UITree::update  1" << std::endl;
    Node2D *node = dynamic_cast<Node2D *>(this->_node);
    const Size &size = node->getSize();

    std::cout << "UITree UITree::update  1 " << size.getWidth() << std::endl;
    std::cout << "UITree UITree::update  1 " << size.getHeight() << std::endl;

    this->_nodeIndex = 0;
    //     this->_treeSelectMap.clear();
    //     this->_treeFoldMap.clear();
    //     this->_ndSelect->setActive(false);
    // std::cout << "UITree UITree::update  2" << std::endl;
    if (this->_uiTreeData.layer >= 0)
    {
        this->_updateTreesItems(this->_uiTreeData);
    }
    // 刷新节点池状态
    for (int i = this->_nodeIndex; i < this->_nodePools.size(); i++)
    {
        this->_nodePools[i]->setActive(false);
    }
    std::cout << "UITree UITree::update  4" << std::endl;
    // 刷新容器的大小

    //     // 刷新content的大小,位置===============================================start
    this->_contentWidth = size.getWidth() - 10.0f;
    this->_contentHeight = this->_nodeIndex * 25.0f;
    std::cout << "UITree UITree::update  31 " << this->_nodeIndex * 25.0f << std::endl;
    std::cout << "UITree UITree::update  32 " << size.getHeight() << std::endl;
    std::cout << "UITree UITree::update  33 " << this->_topLen << std::endl;
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
    std::cout << "UITree UITree::update  5" << this->_contentWidth << ", " << this->_contentHeight << std::endl;
    // 刷新content的大小,位置===============================================end

    //     // 选择框 =============================================================start
    //     this->_selectWidth = this->_contentWidth;
    //     this->_selectHeight = 22.0f;
    //     this->_ndSelect->setSize(this->_selectWidth, this->_selectHeight);
    //     // 选择框 =============================================================end
    //     // 滑动条==========================================================start
    //     float offsetY = this->_contentHeight - size.height();
    //     if (offsetY > 0)
    //     {
    //         // 内容高度大于容器高度,需要滑动条  更新背景
    //         this->_scrollBarBgHeight = size.height() - 10.0f;
    //         this->_scrollBarBgX = size.width() / 2 - this->_scrollBarBgWidth;
    //         this->_scrollBarBgY = 0.0f;
    //         this->_ndScrollBarBg->setPosition(size.width() / 2 - this->_scrollBarBgWidth, this->_scrollBarBgY, 0.0f);
    //         this->_ndScrollBarBg->setSize(this->_scrollBarBgWidth, this->_scrollBarBgHeight);
    //         this->_ndScrollBarBg->setActive(true);
    //         this->_scrollBarHeight = this->_scrollBarBgHeight - offsetY;
    //         if (this->_scrollBarHeight <= 20)
    //         {
    //             this->_scrollBarHeight = 20.f;
    //         }
    //         this->_scrollBarRate = offsetY / (this->_scrollBarBgHeight - this->_scrollBarHeight);
    //         this->_ndScrollBar->setSize(this->_scrollBarWidth, this->_scrollBarHeight);
    //         this->_scrollBarY = this->_scrollBarBgHeight / 2.0f - this->_scrollBarHeight / 2.0f - this->_topLen / this->_scrollBarRate;
    //         this->_ndScrollBar->setPosition(this->_scrollBarX, this->_scrollBarY, 0);
    //     }
    //     else
    //     {
    //         this->_scrollBarRate = 1.0f;
    //         this->_ndScrollBarBg->setActive(false);
    //     }
    //     if (this->_updateCallback != nullptr)
    //     {
    //         this->_updateCallback();
    //     }
    // }
}

void UITree::_updateTreesItems(UITreeStructure &uiTreeData)
{
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
        // node->onNodeInputEvent(NodeInput::TOUCH_START, [this](NodeInputResult &result)
        //                        { this->_onTreeItemTouchEvent(result); }, true);
        node->onNodeInputEvent(NodeInput::TOUCH_START, &UITree::_onTreeItemTouchEvent,this);
        // 选择框
        ndSelect = new Node2D("NodeTreeItemSelect");
        node->addChild(ndSelect);
        ndSelect->setSize(1000.0f, itemHeight);
        spSelect = dynamic_cast<UISprite *>(ndSelect->addComponent("UISprite"));
        spSelect->setTexture("resources/texture/ic-default.png");
        spSelect->setMaterial(nullptr);
        spSelect->setColor(9.0f / 250.0f, 74.0f / 255.0f, 53.0f / 255.0f, 1.0f);
        // 折叠符号
        ndFold = new Node2D("NodeTreeItemFold");
        node->addChild(ndFold);
        ndFold->setSize(16.0f, 16.0f);
        spFold = dynamic_cast<UISprite *>(ndFold->addComponent("UISprite"));
        spFold->setMaterial(nullptr);
        // ndFold->onNodeInputEvent(NodeInput::TOUCH_START, [this](NodeInputResult &result)
        //                          { this->_onTreeItemFoldTouchEvent(result); }, true);
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
    uiTreeData.index = this->_nodeIndex;
    // std::cout << "当前索引：" << this->_nodeIndex << "layer：" << uiTreeData.layer << std::endl;

    node->setActive(true);
    ndSelect->setActive(false);
    itemWidth += (uiTreeData.layer * offset);
    // this->_treeSelectMap.emplace(node->uuid(), &uiTreeData);
    // this->_treeFoldMap.emplace(ndFold->uuid(), &uiTreeData);
    // 折叠图标
    if (uiTreeData.isFold)
    {
        spFold->setTexture("resources/texture/ic-arrow-right.png");
        // spFold->setTexture("resources/texture/ic-default.png");
    }
    else
    {
        spFold->setTexture("resources/texture/ic-arrow-bottom.png");
        // spFold->setTexture("resources/texture/ic-default.png");
    }
    if (uiTreeData.children.size() > 0)
    {
        ndFold->setActive(true);
    }
    else
    {
        // ndFold->setActive(false);
    }
    itemWidth += ndFold->getSize().getWidth();
    // 图标
    spIcon->setTexture(uiTreeData.iconKey);
    // spIcon->setTexture("resources/texture/ic-default.png");
    itemWidth += ndIcon->getSize().getWidth();
    // 创建名字
    std::cout << "name:" << uiTreeData.name << std::endl;
    txtName->setText(uiTreeData.name);
    float itemNameWidth = ndName->getSize().getWidth();
    float itemNameHight = ndName->getSize().getHeight();
    itemWidth += itemNameWidth;
    itemWidth += 5.0f * 3;

    // 设置总宽度
    if (itemWidth < size.getWidth() - 10)
    {
        itemWidth = size.getWidth() - 10;
    }
    // 折叠按钮
    float startX = -itemWidth / 2.0 + (uiTreeData.layer * offset);
    ndFold->setPosition(startX + ndFold->getSize().getWidth() / 2.0, 0.0f, 0.0f);
    // 图标
    startX += ndFold->getSize().getWidth() + 5.0f;
    ndIcon->setPosition(startX + ndIcon->getSize().getWidth() / 2.0, 0.0f, 0.0f);
    std::cout << "icon:" << ndIcon->getSize().getWidth() << " name:" << ndIcon->getSize().getHeight() << std::endl;
    // 名字
    startX += ndIcon->getSize().getWidth() + 5.0f;
    ndName->setPosition(startX + itemNameWidth / 2.0, 0.0f, 0.0f);

    node->setSize(itemWidth, itemHeight);
    node->setPosition(itemWidth / 2.0f, -itemHeight / 2.0f - this->_nodeIndex * itemHeight, 0.0f);

    if (itemWidth > this->_contentWidth)
    {
        this->_contentWidth = itemWidth;
    }
    // if (this->_selectUUID == uiTreeData.uuid)
    // {
    //     this->_ndSelect->setActive(true);
    //     this->_selectX = itemWidth / 2.0f;
    //     this->_selectY = -itemHeight / 2.0f - this->_nodeIndex * itemHeight;
    //     this->_ndSelect->setPosition(this->_selectX, this->_selectY, 0.0f);
    // }
    // 递归更新子节点
    this->_nodeIndex++;
    if (!uiTreeData.isFold)
    {
        for (int i = 0; i < uiTreeData.children.size(); i++)
        {
            this->_updateTreesItems(uiTreeData.children[i]);
        }
    }
}
void UITree::_onTreeItemTouchEvent(NodeInputResult &result)
{
    std::cout << "_onTreeItemTouchEvent:" << result.node->getUuid() << ": " << std::endl;
}
// void UITree::_onTreeItemTouchEvent(NodeInputResult &result)
// {
//     std::cout << "_onTreeItemTouchEvent:" << result.node->uuid() << ": " << std::endl;
//     if (this->_treeSelectMap.find(result.node->uuid()) != this->_treeSelectMap.end())
//     {
//         UITreeStructure *uiTreeData = this->_treeSelectMap[result.node->uuid()];
//         Node2D *node2d = dynamic_cast<Node2D *>(this->_node);
//         if (this->_selectUUID == uiTreeData->uuid)
//         {
//             this->_selectUUID = "";
//             this->_ndSelect->setActive(false);
//             if (this->_selectCallback != nullptr)
//             {
//                 this->_selectCallback(nullptr);
//             }
//         }
//         else
//         {
//             Size &size = node2d->size();
//             this->_ndSelect->setActive(true);
//             this->_selectUUID = uiTreeData->uuid;
//             this->_selectX = size.width() / 2.0f;
//             this->_selectY = -22.0f / 2.0f - uiTreeData->index * 22.0f;
//             this->_ndSelect->setPosition(this->_selectX, this->_selectY, 0.0f);
//             this->_spSelect->setColor(9.0f / 250.0f, 74.0f / 255.0f, 53.0f / 255.0f, 1.0f);
//             if (this->_selectCallback != nullptr)
//             {
//                 this->_selectCallback(uiTreeData);
//             }
//         }
//     }
//     else
//     {
//         this->_selectUUID = "";
//     }
// }

void UITree::update(float deltaTime)
{
    Component::update(deltaTime);
}
void UITree::lateUpdate(float deltaTime)
{
    Component::lateUpdate(deltaTime);
}
void UITree::render()
{
    Component::render();
}

void UITree::clearSelect()
{
    // this->_selectUUID = "";
    // this->_ndSelect->setActive(false);
}
/** 失去焦点时淡化 */
void UITree::loseFocusSelect()
{
    // this->_spSelect->setColor(0.5f, 0.5f, 0.5f, 0.3f);
}
// void UITree::_onTreeItemFoldTouchEvent(NodeInputResult &result)
// {
//     if (this->_treeFoldMap.find(result.node->uuid()) != this->_treeFoldMap.end())
//     {
//         UITreeStructure *uiTreeData = this->_treeFoldMap[result.node->uuid()];
//         uiTreeData->isFold = !uiTreeData->isFold;
//         this->_isDirty = true;
//     }
// }

void UITree::destroy()
{
    this->_node->offTransformChange(this->_nodeTransform_ID);
    Component::destroy();
}
UITree::~UITree()
{
}
