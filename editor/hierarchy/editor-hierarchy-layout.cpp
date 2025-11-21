#include "editor-hierarchy-layout.h"
#include "../editor-config.h"
#include "../ipc/editor-ipc.h"
#include "../../engine/core/scene/node.h"
#include "../../engine/core/scene/node-2d.h"
#include "../../engine/core/scene/scene.h"
#include "../../engine/core/renderer/ui/ui-sprite.h"
#include "../../engine/core/renderer/ui/ui-mask.h"
#include "../../engine/core/component/ui/ui-widget.h"
#include "../../engine/core/component/ui/ui-tree/node-tree.h"

EditorHierarchyLayout::EditorHierarchyLayout(Node2D *root)
{
    this->_root = root;
    this->_initTitle();
    this->_initContent();
}
void EditorHierarchyLayout::_initTitle()
{
    Node2D *ndTitle = new Node2D("Title");
    this->_root->addChild(ndTitle);
    ndTitle->setSize(120.0f, 30.0f);
    // 后续不需要这个组件
    UISprite *sprite = dynamic_cast<UISprite *>(ndTitle->addComponent("UISprite"));
    sprite->setColor(EditorConfig::title);
    sprite->setTexture(EditorConfig::txDefault);
    sprite->setMaterial(nullptr);
    // 添加wedget组件
    UIWidget *widget = dynamic_cast<UIWidget *>(ndTitle->addComponent("UIWidget"));
    WidgetHorizontalParam paramHorizontal{};
    paramHorizontal.left = 0.0f;
    paramHorizontal.right = 0.0f;
    WidgetVerticalParam paramVertical{};
    paramVertical.top = 0.0f;
    paramVertical.bottom = 0.0f;
    widget->setHorizontal(WidgetHorizontal::LEFT, paramHorizontal);
    widget->setVertical(WidgetVertical::TOP, paramVertical);

    Node2D *ndTitleName = new Node2D("TitleName");
    ndTitle->addChild(ndTitleName);
    ndTitleName->setSize(100.0f, 25.0f);
    ndTitleName->setPosition(-5.0f, 0.0f, 0.0f);
    UISprite *spriteName = dynamic_cast<UISprite *>(ndTitleName->addComponent("UISprite"));
    spriteName->setColor("#ffffffff");
    spriteName->setTexture("res/ic-title-hierarchy.png");
    spriteName->setMaterial(nullptr);

    // 计算尺寸
    float width = ndTitle->getSize().getWidth() * 0.95f;
    float scale = width / 300.0f;
    ndTitleName->setSize(width, 60.0f * scale);
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
    // sprite->setTexture(EditorConfig::txDefault);
    // sprite->setMaterial(nullptr);
    // std::cout << "EditorHierarchyLayout::_initContent1:" << sprite << std::endl;

    this->_initOperate();
    this->_initNodeTree();
    // this->_content->onNodeInputEvent(NodeInput::TOUCH_START, [this](NodeInputResult &result)
    //                                  {if (this->_onRootInputStartEventCallback != nullptr){
    //                                         this->_onRootInputStartEventCallback(result);
    //                                     } });
}

void EditorHierarchyLayout::_initOperate()
{
    this->_ndOperate = new Node2D("Operate");
    this->_content->addChild(this->_ndOperate);
    this->_ndOperate->setSize(150.0f, 33.0f);

    UISprite *sprite = dynamic_cast<UISprite *>(this->_ndOperate->addComponent("UISprite"));
    sprite->setColor(EditorConfig::operate);
    sprite->setTexture(EditorConfig::txDefault);
    sprite->setMaterial(nullptr);
    // 添加wedget组件
    UIWidget *widget = dynamic_cast<UIWidget *>(this->_ndOperate->addComponent("UIWidget"));
    WidgetHorizontalParam paramHorizontal{};
    paramHorizontal.left = 0.0f;
    paramHorizontal.right = 0.0f;
    WidgetVerticalParam paramVertical{};
    paramVertical.top = 0.0f;
    paramVertical.bottom = 0.0f;
    widget->setHorizontal(WidgetHorizontal::ALL, paramHorizontal);
    widget->setVertical(WidgetVertical::TOP, paramVertical);

    this->_initOperateAdd();
    this->_initOperateFold();
    this->_initOperateSearch();
}
void EditorHierarchyLayout::_initOperateAdd()
{
    this->_ndAdd = new Node2D("Add");
    this->_ndOperate->addChild(this->_ndAdd);
    this->_ndAdd->setSize(16.0f, 16.0f);
    this->_spriteAdd = dynamic_cast<UISprite *>(this->_ndAdd->addComponent("UISprite"));
    this->_spriteAdd->setTexture("resources/texture/ic-add.png");
    this->_spriteAdd->setMaterial(nullptr);

    // 添加wedget组件
    UIWidget *widget = dynamic_cast<UIWidget *>(this->_ndAdd->addComponent("UIWidget"));
    WidgetHorizontalParam paramHorizontal{};
    paramHorizontal.left = 4.0f;
    paramHorizontal.right = 0.0f;
    WidgetVerticalParam paramVertical{};
    paramVertical.top = 0.0f;
    paramVertical.bottom = 0.0f;
    widget->setHorizontal(WidgetHorizontal::LEFT, paramHorizontal);
    widget->setVertical(WidgetVertical::NONE, paramVertical);
}

void EditorHierarchyLayout::_initOperateFold()
{
    this->_ndFold = new Node2D("Fold");
    this->_ndOperate->addChild(this->_ndFold);
    this->_ndFold->setSize(16, 16);
    this->_spriteFold = dynamic_cast<UISprite *>(this->_ndFold->addComponent("UISprite"));
    this->_spriteFold->setTexture("resources/texture/icon/ic-flod.png");
    this->_spriteFold->setMaterial(nullptr);
    // 添加wedget组件
    UIWidget *widget = dynamic_cast<UIWidget *>(this->_ndFold->addComponent("UIWidget"));
    WidgetHorizontalParam paramHorizontal{};
    paramHorizontal.left = 0.0f;
    paramHorizontal.right = 2.0f;
    WidgetVerticalParam paramVertical{};
    paramVertical.top = 0.0f;
    paramVertical.bottom = 0.0f;
    widget->setHorizontal(WidgetHorizontal::RIGHT, paramHorizontal);
    widget->setVertical(WidgetVertical::NONE, paramVertical);
}
void EditorHierarchyLayout::_initOperateSearch()
{
    Node2D *ndSearchBg = new Node2D("NodeRearchBg");
    ndSearchBg->setSize(100.0f, 20.0f);
    this->_ndOperate->addChild(ndSearchBg);
    // 渲染组件
    UISprite *sprite = static_cast<UISprite *>(ndSearchBg->addComponent("UISprite"));
    sprite->setColor("#151e2bff");
    sprite->setTexture(EditorConfig::txDefault);
    sprite->setMaterial(nullptr);
    UIWidget *widget = static_cast<UIWidget *>(ndSearchBg->addComponent("UIWidget"));
    WidgetHorizontalParam paramHorizontal{};
    paramHorizontal.left = 28.0f;
    paramHorizontal.right = 28.0f;
    widget->setHorizontal(WidgetHorizontal::ALL, paramHorizontal);
    // 搜索icon
    this->_ndSearchIcon = new Node2D("SearchIcon");
    this->_ndOperate->addChild(this->_ndSearchIcon);
    this->_ndSearchIcon->setSize(14.0f, 14.0f);
    this->_spriteSearchIcon = dynamic_cast<UISprite *>(this->_ndSearchIcon->addComponent("UISprite"));
    this->_spriteSearchIcon->setMaterial(nullptr);
    this->_spriteSearchIcon->setTexture("resources/texture/ic-search.png");
    // 添加wedget组件
    UIWidget *widget0 = dynamic_cast<UIWidget *>(this->_ndSearchIcon->addComponent("UIWidget"));
    WidgetHorizontalParam paramHorizontal0{};
    paramHorizontal0.left = 32.0f;
    widget0->setHorizontal(WidgetHorizontal::LEFT, paramHorizontal0);

    // 搜索框
    this->_ndSearchBox = new Node2D("SearchBox");
    this->_ndOperate->addChild(this->_ndSearchBox);
    this->_ndSearchBox->setSize(100.0f, 18.0f);
    this->_spriteSearchBox = dynamic_cast<UISprite *>(this->_ndSearchBox->addComponent("UISprite"));
    this->_spriteSearchBox->setMaterial(nullptr);
    this->_spriteSearchBox->setColor("#ffffffff");
    this->_spriteSearchBox->setTexture(EditorConfig::txDefault);
    // 添加wedget组件
    UIWidget *widget1 = dynamic_cast<UIWidget *>(this->_ndSearchBox->addComponent("UIWidget"));
    WidgetHorizontalParam paramHorizontal1{};
    paramHorizontal1.left = 50.0f;
    paramHorizontal1.right = 30.0f;
    widget1->setHorizontal(WidgetHorizontal::ALL, paramHorizontal1);
}

void EditorHierarchyLayout::_initNodeTree()
{
    Node2D *ndNodeTree = new Node2D("NodeTree");
    this->_content->addChild(ndNodeTree);
    // 添加wedget组件
    UIWidget *widget = static_cast<UIWidget *>(ndNodeTree->addComponent("UIWidget"));
    WidgetHorizontalParam paramHorizontal{};
    paramHorizontal.left = 0.0f;
    paramHorizontal.right = 0.0f;
    WidgetVerticalParam paramVertical{};
    paramVertical.top = 33.0f;
    paramVertical.bottom = 0.0f;
    widget->setHorizontal(WidgetHorizontal::ALL, paramHorizontal);
    widget->setVertical(WidgetVertical::ALL, paramVertical);
    // 渲染组件-后续删除
    UISprite *sprite = static_cast<UISprite *>(ndNodeTree->addComponent("UISprite"));
    sprite->setColor(EditorConfig::content);
    sprite->setTexture(EditorConfig::txDefault);
    sprite->setMaterial(nullptr);
    //Mask
    UIMask *uiMask = static_cast<UIMask *>(ndNodeTree->addComponent("UIMask"));

    // 添加节点树
    this->_nodeTree = static_cast<NodeTree *>(ndNodeTree->addComponent("NodeTree"));
    this->_nodeTree->onSelectEvent([this](std::string uuid)
                                   { this->_onNodeTreeSelectEvent(uuid); });
}
void EditorHierarchyLayout::setScene(Scene *scene)
{
    if (this->_nodeTree != nullptr)
    {
        this->_nodeTree->setScene(scene);
    }
}
void EditorHierarchyLayout::_onNodeTreeSelectEvent(std::string uuid)
{
    std::cout << "EditorHierarchyLayout _onNodeTreeSelectEvent:" << uuid << std::endl;
}
void EditorHierarchyLayout::loseFocus()
{
    if (this->_nodeTree != nullptr)
    {
        this->_nodeTree->hoverSelect();
    }
}

EditorHierarchyLayout::~EditorHierarchyLayout()
{
}