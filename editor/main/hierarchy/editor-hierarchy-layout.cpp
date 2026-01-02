#include "editor-hierarchy-layout.h"
#include "../../config/editor-config.h"
#include "../../boo-editor.h"
#include "editor-hierarchy-node-tree.h"

#include "../../../engine/core/scene/node.h"
#include "../../../engine/core/scene/node-2d.h"
#include "../../../engine/core/scene/scene.h"
#include "../../../engine/core/renderer/ui/ui-sprite.h"
#include "../../../engine/core/renderer/ui/ui-mask.h"
#include "../../../engine/core/component/ui/ui-widget.h"
#include "../../../engine/core/assets/assets-manager.h"
#include "../../../engine/core/assets/texture-asset.h"

EditorHierarchyLayout::EditorHierarchyLayout(Node2D *root)
{
    this->_root = root;
}
void EditorHierarchyLayout::init()
{
    this->_initContent();
    this->_initTitle();
    this->_initOperate();
    this->_initNodeTree();
}
void EditorHierarchyLayout::_initContent()
{
    this->_content = new Node2D("NodeHierarchy");
    this->_root->addChild(this->_content);
    this->_content->setSize(100.0f, 100.0f);
    // 添加wedget组件
    UIWidget *widget = dynamic_cast<UIWidget *>(this->_content->addComponent("UIWidget"));
    WidgetHorizontalParam paramHorizontal{};
    paramHorizontal.left = 0.0f;
    paramHorizontal.right = 0.0f;
    WidgetVerticalParam paramVertical{};
    paramVertical.top = 0.0f; // 25.0f;
    paramVertical.bottom = 0.0f;
    widget->setHorizontal(WidgetHorizontal::ALL, paramHorizontal);
    widget->setVertical(WidgetVertical::ALL, paramVertical);
    // 渲染组件 后续不需要
    UISprite *sprite = dynamic_cast<UISprite *>(this->_content->addComponent("UISprite"));
    sprite->setColor("#000000ff");
}
void EditorHierarchyLayout::_initTitle()
{
    Node2D *ndTitle = new Node2D("Title");
    this->_content->addChild(ndTitle);
    ndTitle->setSize(120.0f, 30.0f);
    // 后续不需要这个组件
    UISprite *sprite = dynamic_cast<UISprite *>(ndTitle->addComponent("UISprite"));
    sprite->setColor(EditorConfig::title);
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
    TextureAsset *tex = BooEditor::cache->getEditorTexture("ic-title-hierarchy.png");
    spriteName->setTexture(tex);
    // 计算尺寸
    float width = ndTitle->getSize().getWidth() * 0.95f;
    float scale = width / 300.0f;
    ndTitleName->setSize(width, 60.0f * scale);
}
void EditorHierarchyLayout::_initOperate()
{
    this->_ndOperate = new Node2D("Operate");
    this->_content->addChild(this->_ndOperate);
    this->_ndOperate->setSize(150.0f, 33.0f);

    UISprite *sprite = dynamic_cast<UISprite *>(this->_ndOperate->addComponent("UISprite"));
    sprite->setColor(EditorConfig::operate);
    sprite->setTexture(EditorConfig::txDefault);
    // 添加wedget组件
    UIWidget *widget = dynamic_cast<UIWidget *>(this->_ndOperate->addComponent("UIWidget"));
    WidgetHorizontalParam paramHorizontal{};
    paramHorizontal.left = 0.0f;
    paramHorizontal.right = 0.0f;
    WidgetVerticalParam paramVertical{};
    paramVertical.top = 30.0f;
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
    // this->_spriteAdd->setTexture("resources/texture/ic-add.png");
    TextureAsset *tex = BooEditor::cache->getEditorTexture("ic-add.png");
    this->_spriteAdd->setTexture(tex);

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
    // this->_spriteFold->setTexture("resources/texture/icon/ic-flod.png");
    TextureAsset *tex = BooEditor::cache->getEditorTexture("ic-flod.png");
    this->_spriteFold->setTexture(tex);
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
    TextureAsset *tex = BooEditor::cache->getEditorTexture("ic-search.png");
    this->_spriteSearchIcon->setTexture(tex);
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
    this->_ndNodeTree = new Node2D("NodeTree");
    this->_content->addChild(this->_ndNodeTree);
    this->_ndNodeTree->setSize(100.0f, 100.0f);
    // this->_spriteNodeTree = dynamic_cast<UISprite *>(this->_ndNodeTree->addComponent("UISprite"));
    // this->_spriteNodeTree->setColor(EditorConfig::content);
    // 添加wedget组件
    UIWidget *widget = dynamic_cast<UIWidget *>(this->_ndNodeTree->addComponent("UIWidget"));
    WidgetHorizontalParam paramHorizontal{};
    paramHorizontal.left = 0.0f;
    paramHorizontal.right = 0.0f;
    WidgetVerticalParam paramVertical{};
    paramVertical.top = 63.0f;
    paramVertical.bottom = 0.0f;
    widget->setHorizontal(WidgetHorizontal::ALL, paramHorizontal);
    widget->setVertical(WidgetVertical::ALL, paramVertical);
    //添加mask组件
    UIMask *mask = dynamic_cast<UIMask *>(this->_ndNodeTree->addComponent("UIMask"));


    this->_nodeTree = dynamic_cast<EditorHierarchyNodeTree *>(this->_ndNodeTree->addComponent("EditorHierarchyNodeTree"));
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


// void EditorHierarchyLayout::_initTitle()
// {
//     Node2D *ndTitle = new Node2D("Title");
//     this->_root->addChild(ndTitle);
//     ndTitle->setSize(120.0f, 30.0f);
//     // 后续不需要这个组件
//     UISprite *sprite = dynamic_cast<UISprite *>(ndTitle->addComponent("UISprite"));
//     sprite->setColor(EditorConfig::title);
//     sprite->setTexture(EditorConfig::txDefault);
//     sprite->setMaterialAsset(nullptr);
//     // 添加wedget组件
//     UIWidget *widget = dynamic_cast<UIWidget *>(ndTitle->addComponent("UIWidget"));
//     WidgetHorizontalParam paramHorizontal{};
//     paramHorizontal.left = 0.0f;
//     paramHorizontal.right = 0.0f;
//     WidgetVerticalParam paramVertical{};
//     paramVertical.top = 0.0f;
//     paramVertical.bottom = 0.0f;
//     widget->setHorizontal(WidgetHorizontal::LEFT, paramHorizontal);
//     widget->setVertical(WidgetVertical::TOP, paramVertical);

//     Node2D *ndTitleName = new Node2D("TitleName");
//     ndTitle->addChild(ndTitleName);
//     ndTitleName->setSize(100.0f, 25.0f);
//     ndTitleName->setPosition(-5.0f, 0.0f, 0.0f);
//     UISprite *spriteName = dynamic_cast<UISprite *>(ndTitleName->addComponent("UISprite"));
//     spriteName->setColor("#ffffffff");
//     spriteName->setTexture("res/ic-title-hierarchy.png");
//     spriteName->setMaterialAsset(nullptr);

//     // 计算尺寸
//     float width = ndTitle->getSize().getWidth() * 0.95f;
//     float scale = width / 300.0f;
//     ndTitleName->setSize(width, 60.0f * scale);
// }
// void EditorHierarchyLayout::_initContent()
// {
//     std::cout << "EditorHierarchyLayout::_initContent" << std::endl;
//     this->_content = new Node2D("NodeHierarchy");
//     this->_root->addChild(this->_content);
//     this->_content->setSize(100.0f, 100.0f);
//     // 添加wedget组件
//     UIWidget *widget = dynamic_cast<UIWidget *>(this->_content->addComponent("UIWidget"));
//     WidgetHorizontalParam paramHorizontal{};
//     paramHorizontal.left = 0.0f;
//     paramHorizontal.right = 0.0f;
//     WidgetVerticalParam paramVertical{};
//     paramVertical.top = 25.0f;
//     paramVertical.bottom = 0.0f;
//     widget->setHorizontal(WidgetHorizontal::ALL, paramHorizontal);
//     widget->setVertical(WidgetVertical::ALL, paramVertical);
//     // 渲染组件 后续不需要
//     // UISprite *sprite = dynamic_cast<UISprite *>(this->_content->addComponent("UISprite"));
//     // sprite->setColor("#ffe1e1ff");
//     // sprite->setTexture(EditorConfig::txDefault);
//     // sprite->setMaterialAsset(nullptr);
//     // std::cout << "EditorHierarchyLayout::_initContent1:" << sprite << std::endl;

//     this->_initOperate();
//     this->_initNodeTree();
//     // this->_content->onNodeInputEvent(NodeInput::TOUCH_START, [this](NodeInputResult &result)
//     //                                  {if (this->_onRootInputStartEventCallback != nullptr){
//     //                                         this->_onRootInputStartEventCallback(result);
//     //                                     } });
// }

// void EditorHierarchyLayout::_initNodeTree()
// {
//     Node2D *ndNodeTree = new Node2D("NodeTree");
//     this->_content->addChild(ndNodeTree);
//     // 添加wedget组件
//     UIWidget *widget = static_cast<UIWidget *>(ndNodeTree->addComponent("UIWidget"));
//     WidgetHorizontalParam paramHorizontal{};
//     paramHorizontal.left = 0.0f;
//     paramHorizontal.right = 0.0f;
//     WidgetVerticalParam paramVertical{};
//     paramVertical.top = 33.0f;
//     paramVertical.bottom = 0.0f;
//     widget->setHorizontal(WidgetHorizontal::ALL, paramHorizontal);
//     widget->setVertical(WidgetVertical::ALL, paramVertical);
//     // 渲染组件-后续删除
//     UISprite *sprite = static_cast<UISprite *>(ndNodeTree->addComponent("UISprite"));
//     sprite->setColor(EditorConfig::content);
//     sprite->setTexture(EditorConfig::txDefault);
//     sprite->setMaterialAsset(nullptr);
//     // Mask
//     UIMask *uiMask = static_cast<UIMask *>(ndNodeTree->addComponent("UIMask"));

//     // 添加节点树
//     this->_nodeTree = static_cast<NodeTree *>(ndNodeTree->addComponent("NodeTree"));
//     this->_nodeTree->onSelectEvent([this](std::string uuid)
//                                    { this->_onNodeTreeSelectEvent(uuid); });
//     this->_nodeTree->onMenuEvent([this](std::string uuid, int x, int y)
//                                  { this->_onNodeTreeMenuEvent(uuid, x, y); });
// }
// void EditorHierarchyLayout::setScene(Scene *scene)
// {
//     if (this->_nodeTree != nullptr)
//     {
//         this->_nodeTree->setScene(scene);
//     }
// }
// void EditorHierarchyLayout::_onNodeTreeSelectEvent(std::string uuid)
// {
//     std::cout << "EditorHierarchyLayout _onNodeTreeSelectEvent:" << uuid << std::endl;
// }

// void EditorHierarchyLayout::_onNodeTreeMenuEvent(std::string uuid, int x, int y)
// {
//     std::cout << "EditorHierarchyLayout _onNodeTreeMenuEvent:" << x << ", " << y << ", " << uuid << std::endl;
// }

// void EditorHierarchyLayout::loseFocus()
// {
//     if (this->_nodeTree != nullptr)
//     {
//         this->_nodeTree->hoverSelect();
//     }
// }

// EditorHierarchyLayout::~EditorHierarchyLayout()
// {
// }