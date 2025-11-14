#include "editor-hierarchy-layout.h"
#include "../editor-config.h"
#include "../ipc/editor-ipc.h"
#include "../../engine/core/scene/node.h"
#include "../../engine/core/scene/node-2d.h"
#include "../../engine/core/scene/scene.h"
#include "../../engine/core/renderer/ui/ui-sprite.h"
#include "../../engine/core/component/ui/ui-widget.h"
#include "../../engine/core/component/ui/ui-tree/node-tree.h"

EditorHierarchyLayout::EditorHierarchyLayout(Node2D *root)
{
    this->_root = root;
    this->_initTitle();
    this->_initContent();
}
void EditorHierarchyLayout::_initContent()
{
    std::cout << "EditorHierarchyLayout::_initContent" << std::endl;
    this->_content = new Node2D("NodeHierarchy");
    this->_root->addChild(this->_content);
    this->_content->setSize(100.0f, 100.0f);
    // 添加wedget组件
    UIWidget *widget = dynamic_cast<UIWidget *>(this->_content->addComponent("UIWidget"));
    WidgetHorizontalParam paramHorizontal{};
    paramHorizontal.left = 0.0f;
    paramHorizontal.right = 0.0f;
    WidgetVerticalParam paramVertical{};
    paramVertical.top = 25.0f;
    paramVertical.bottom = 0.0f;
    widget->setHorizontal(WidgetHorizontal::ALL, paramHorizontal);
    widget->setVertical(WidgetVertical::ALL, paramVertical);
    // 渲染组件 后续不需要
    // UISprite *sprite = dynamic_cast<UISprite *>(this->_content->addComponent("UISprite"));
    // sprite->setColor("#ffe1e1ff");
    // sprite->setTexture(EditorConfig::texture);
    // sprite->setMaterial(nullptr);
    // std::cout << "EditorHierarchyLayout::_initContent1:" << sprite << std::endl;

    this->_initOperate();
    this->_initNodeTree();
    // this->_content->onNodeInputEvent(NodeInput::TOUCH_START, [this](NodeInputResult &result)
    //                                  {if (this->_onRootInputStartEventCallback != nullptr){
    //                                         this->_onRootInputStartEventCallback(result);
    //                                     } });
}
void EditorHierarchyLayout::_initTitle()
{
    // float width = 120.0f;
    // float height = 25.0f;
    Node2D *ndTitle = new Node2D("Title");
    this->_root->addChild(ndTitle);
    ndTitle->setSize(100, 100);
    // Sprite *sprite = ndTitle->addComponent<Sprite>("Sprite");
    // sprite->setColor(EditorConfig::theme);
    // // 添加wedget组件
    // UIWidget *widget = ndTitle->addComponent<UIWidget>("UIWidget");
    // if (widget != nullptr)
    // {
    //     WidgetHorizontalParam paramHorizontal{};
    //     WidgetVerticalParam paramVertical{};
    //     widget->setHorizontal(WidgetHorizontal::LEFT, paramHorizontal);
    //     widget->setVertical(WidgetVertical::TOP, paramVertical);
    // }
    // // icon
    // float iconWidth = 20.0f;
    // float iconHeight = 20.0f;
    // Node2D *ndIcon = new Node2D("TitleIcon");
    // ndTitle->addChild(ndIcon);
    // ndIcon->setSize(iconWidth, iconHeight);
    // ndIcon->setPosition(-width / 2.0 + 5.0f + iconWidth / 2.0, 0.0, 0.0);
    // Sprite *spriteIcon = ndIcon->addComponent<Sprite>("Sprite");
    // spriteIcon->setColor(EditorConfig::iconColor);
    // spriteIcon->setTexture("resources/textures/ic-title-hierarchy.png");
    // // name
    // float nameWidth = 72.0f;
    // float nameHeight = 16.0f;
    // Node2D *ndName = new Node2D("TitleName");
    // ndTitle->addChild(ndName);
    // ndName->setSize(nameWidth, nameHeight);
    // ndName->setPosition(-width / 2.0 + 2.0f + iconWidth + 5.0f + nameWidth / 2.0, 0.0f, 0.0f);
    // Text *txtName = ndName->addComponent<Text>("Text");
    // txtName->setColor(EditorConfig::fontColor);
    // txtName->setText("层级管理器");
}
void EditorHierarchyLayout::_initOperate()
{
    // Node2D *ndOperate = new Node2D("Operate");
    // this->_content->addChild(ndOperate);
    // ndOperate->setSize(100.0f, 20.0f);
    // Sprite *sprite = ndOperate->addComponent<Sprite>("Sprite");
    // sprite->setColor(EditorConfig::theme);
    // // 添加wedget组件
    // UIWidget *widget = ndOperate->addComponent<UIWidget>("UIWidget");
    // if (widget != nullptr)
    // {
    //     WidgetHorizontalParam paramHorizontal{};
    //     paramHorizontal.left = 0.0f;
    //     paramHorizontal.right = 0.0f;
    //     WidgetVerticalParam paramVertical{};
    //     paramVertical.top = 26.0f;
    //     widget->setHorizontal(WidgetHorizontal::ALL, paramHorizontal);
    //     widget->setVertical(WidgetVertical::TOP, paramVertical);
    // }
    // // 添加按钮
    // Node2D *ndOperateAdd = new Node2D("OperateAdd");
    // ndOperate->addChild(ndOperateAdd);
    // ndOperateAdd->setSize(14.0f, 14.0f);
    // Sprite *spriteAdd = ndOperateAdd->addComponent<Sprite>("Sprite");
    // spriteAdd->setColor(EditorConfig::iconColor);
    // spriteAdd->setTexture("resources/textures/ic-add.png");
    // UIWidget *widgetAdd = ndOperateAdd->addComponent<UIWidget>("UIWidget");
    // WidgetHorizontalParam paramHorizontal{};
    // paramHorizontal.left = 5.0f;
    // widgetAdd->setHorizontal(WidgetHorizontal::LEFT, paramHorizontal);
    // // 折叠按钮
    // Node2D *ndOperateCollapse = new Node2D("OperateCollapse");
    // ndOperate->addChild(ndOperateCollapse);
    // ndOperateCollapse->setSize(14.0f, 14.0f);
    // Sprite *spriteCollaps = ndOperateCollapse->addComponent<Sprite>("Sprite");
    // spriteCollaps->setColor(EditorConfig::iconColor);
    // spriteCollaps->setTexture("resources/textures/ic-flod.png");
    // UIWidget *widgetCollaps = ndOperateCollapse->addComponent<UIWidget>("UIWidget");
    // WidgetHorizontalParam widgetCollapsHParam{};
    // widgetCollapsHParam.right = 5.0f;
    // widgetCollaps->setHorizontal(WidgetHorizontal::RIGHT, widgetCollapsHParam);

    // // 搜索
    // Node2D *ndOperateSearch = new Node2D("OperateSearch");
    // ndOperate->addChild(ndOperateSearch);
    // ndOperateSearch->setSize(14.0f, 16.0f);
    // Sprite *spriteeSearch = ndOperateSearch->addComponent<Sprite>("Sprite");
    // spriteeSearch->setColor(24.0f / 255.0f, 30.0f / 255.0f, 35.0f / 255.0f, 1.0f);
    // UIWidget *widgetSearch = ndOperateSearch->addComponent<UIWidget>("UIWidget");
    // WidgetHorizontalParam widgetSearchHParam{};
    // widgetSearchHParam.left = 28.0f;
    // widgetSearchHParam.right = 28.0f;
    // widgetSearch->setHorizontal(WidgetHorizontal::ALL, widgetSearchHParam);
    // // 搜素图标
    // Node2D *ndOperateSearchIcon = new Node2D("OperateSearch");
    // ndOperateSearch->addChild(ndOperateSearchIcon);
    // ndOperateSearchIcon->setSize(14.0f, 14.0f);
    // Sprite *spriteeSearchIcon = ndOperateSearchIcon->addComponent<Sprite>("Sprite");
    // spriteeSearchIcon->setColor(EditorConfig::iconColor);
    // spriteeSearchIcon->setTexture("resources/textures/ic-search.png");
    // UIWidget *widgetSearchIcon = ndOperateSearchIcon->addComponent<UIWidget>("UIWidget");
    // WidgetHorizontalParam widgetSearchIconHParam{};
    // widgetSearchIconHParam.left = 4.0f;
    // widgetSearchIcon->setHorizontal(WidgetHorizontal::LEFT, widgetSearchIconHParam);
}
void EditorHierarchyLayout::_initNodeTree()
{
    Node2D *ndNodeTree = new Node2D("NodeTree");
    this->_content->addChild(ndNodeTree);
    // 添加wedget组件
    UIWidget *widget = static_cast<UIWidget *>(ndNodeTree->addComponent("UIWidget"));
    WidgetHorizontalParam paramHorizontal{};
    paramHorizontal.left = 10.0f;
    paramHorizontal.right = 0.0f;
    WidgetVerticalParam paramVertical{};
    paramVertical.top = 25.0f;
    paramVertical.bottom = 0.0f;
    widget->setHorizontal(WidgetHorizontal::ALL, paramHorizontal);
    widget->setVertical(WidgetVertical::ALL, paramVertical);
    // 渲染组件-后续删除
    // UISprite *sprite = static_cast<UISprite *>(ndNodeTree->addComponent("UISprite"));
    // // sprite->setColor(EditorConfig::theme);
    // sprite->setColor("#8f0077ff");
    // sprite->setTexture(EditorConfig::texture);
    // sprite->setMaterial(nullptr);
    // 添加节点树
    this->_nodeTree = static_cast<NodeTree *>(ndNodeTree->addComponent("NodeTree"));
    this->_nodeTree->setIcon("iconScene", "res/ic-scene.png");
    this->_nodeTree->setIcon("iconNode2D", "res/ic-2d.png");
    this->_nodeTree->setIcon("iconNode3D", "res/ic-3d.png");
    this->_nodeTree->setScene(new Scene("Test"));
    // // 添加mask组件
    // UIMask *uiMask = ndContent->addComponent<UIMask>("UIMask");
    // if (uiMask != nullptr)
    // {
    //     std::cout << "EditorHierarchyLayout create UIMask:" << uiMask << std::endl;
    // }
    // // 添加节点树
    // this->_nodeTree = ndContent->addComponent<NodeTree>("NodeTree");
    // this->_nodeTree->onSelectEvent([this](UITreeStructure *uiTreeData)
    //                                { if (this->_onNodeTreeSelectEventCallback != nullptr){
    //                                  this->_onNodeTreeSelectEventCallback(uiTreeData);
    //                                } });
    // this->_nodeTree->onUpdateEvent([this]()
    //                                { if (this->_onNodeTreeUpdateEventCallback != nullptr){
    //                                  this->_onNodeTreeUpdateEventCallback();
    //                                } });
}

// void EditorHierarchyLayout::onRootInputStartEvent(std::function<void(NodeInputResult &result)> callback)
// {
//     this->_onRootInputStartEventCallback = callback;
// }
// void EditorHierarchyLayout::onNodeTreeSelectEvent(std::function<void(UITreeStructure *uiTreeData)> callback)
// {
//     this->_onNodeTreeSelectEventCallback = callback;
// }
// void EditorHierarchyLayout::onNodeTreeUpdateEvent(std::function<void()> callback)
// {
//     this->_onNodeTreeUpdateEventCallback = callback;
// }

/*
 * @brief 失去焦点
 */
void EditorHierarchyLayout::loseFocus()
{
    // if (this->_nodeTree != nullptr)
    // {
    //     std::cout << "EditorHierarchyLayout lose focus" << std::endl;
    //     this->_nodeTree->loseFocusSelect();
    // }
}
void EditorHierarchyLayout::clearNodeTreeSelect()
{
    // if (this->_nodeTree != nullptr)
    // {
    //     std::cout << "EditorHierarchyLayout clear node tree select" << std::endl;
    //     this->_nodeTree->clearSelect();
    // }
}

void EditorHierarchyLayout::setScene(Scene *scene)
{
    // if (this->_nodeTree != nullptr)
    // {
    //     this->_nodeTree->setScene(scene);
    // }
}
void EditorHierarchyLayout::setNode(Node *node)
{
    // if (this->_nodeTree != nullptr)
    // {
    //     this->_nodeTree->setNode(node);
    // }
}
/*
 * @brief 更新树
 */
void EditorHierarchyLayout::updateTree()
{
    // if (this->_nodeTree != nullptr)
    // {
    //     this->_nodeTree->updateTree();
    // }
}

EditorHierarchyLayout::~EditorHierarchyLayout()
{
}