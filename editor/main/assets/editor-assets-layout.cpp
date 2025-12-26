#include "editor-assets-layout.h"
#include "editor-assets-file-tree.h"

#include "../../config/editor-config.h"
#include "../../boo-editor.h"


#include "../../../engine/core/scene/node.h"
#include "../../../engine/core/scene/node-2d.h"
#include "../../../engine/core/scene/scene.h"
#include "../../../engine/core/renderer/ui/ui-sprite.h"
#include "../../../engine/core/renderer/ui/ui-mask.h"
#include "../../../engine/core/component/ui/ui-widget.h"
#include "../../../engine/core/assets/assets-manager.h"
#include "../../../engine/core/assets/texture-asset.h"

EditorAssetsLayout::EditorAssetsLayout(Node2D *root)
{
    this->_root = root;
}
void EditorAssetsLayout::init()
{
    this->_initContent();
    this->_initTitle();
    this->_initOperate();
    this->_initNodeTree();
}
void EditorAssetsLayout::_initContent()
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
void EditorAssetsLayout::_initTitle()
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
    TextureAsset *tex = BooEditor::cache->getEditorTexture("ic-title-assets.png");
    spriteName->setTexture(tex);
    // 计算尺寸
    float width = ndTitle->getSize().getWidth() * 0.95f;
    float scale = width / 300.0f;
    ndTitleName->setSize(width, 60.0f * scale);
}
void EditorAssetsLayout::_initOperate()
{
    this->_ndOperate = new Node2D("Operate");
    this->_content->addChild(this->_ndOperate);
    this->_ndOperate->setSize(150.0f, 33.0f);

    UISprite *sprite = dynamic_cast<UISprite *>(this->_ndOperate->addComponent("UISprite"));
    sprite->setColor(EditorConfig::operate);
    sprite->setTextureAsset(EditorConfig::txDefault);
    sprite->setMaterialAsset(nullptr);
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
void EditorAssetsLayout::_initOperateAdd()
{
    this->_ndAdd = new Node2D("Add");
    this->_ndOperate->addChild(this->_ndAdd);
    this->_ndAdd->setSize(16.0f, 16.0f);
    this->_spriteAdd = dynamic_cast<UISprite *>(this->_ndAdd->addComponent("UISprite"));
    // this->_spriteAdd->setTextureAsset("resources/texture/ic-add.png");
    // this->_spriteAdd->setMaterialAsset(nullptr);
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

void EditorAssetsLayout::_initOperateFold()
{
    this->_ndFold = new Node2D("Fold");
    this->_ndOperate->addChild(this->_ndFold);
    this->_ndFold->setSize(16, 16);
    this->_spriteFold = dynamic_cast<UISprite *>(this->_ndFold->addComponent("UISprite"));
    // this->_spriteFold->setTextureAsset("resources/texture/icon/ic-flod.png");
    // this->_spriteFold->setMaterialAsset(nullptr);
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
void EditorAssetsLayout::_initOperateSearch()
{
    Node2D *ndSearchBg = new Node2D("NodeRearchBg");
    ndSearchBg->setSize(100.0f, 20.0f);
    this->_ndOperate->addChild(ndSearchBg);
    // 渲染组件
    UISprite *sprite = static_cast<UISprite *>(ndSearchBg->addComponent("UISprite"));
    sprite->setColor("#151e2bff");
    sprite->setTextureAsset(EditorConfig::txDefault);
    sprite->setMaterialAsset(nullptr);
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
    // this->_spriteSearchIcon->setMaterialAsset(nullptr);
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
    this->_spriteSearchBox->setMaterialAsset(nullptr);
    this->_spriteSearchBox->setColor("#ffffffff");
    this->_spriteSearchBox->setTextureAsset(EditorConfig::txDefault);
    // 添加wedget组件
    UIWidget *widget1 = dynamic_cast<UIWidget *>(this->_ndSearchBox->addComponent("UIWidget"));
    WidgetHorizontalParam paramHorizontal1{};
    paramHorizontal1.left = 50.0f;
    paramHorizontal1.right = 30.0f;
    widget1->setHorizontal(WidgetHorizontal::ALL, paramHorizontal1);
}

void EditorAssetsLayout::_initNodeTree()
{
    this->_ndNodeTree = new Node2D("NodeTree");
    this->_content->addChild(this->_ndNodeTree);
    this->_ndNodeTree->setSize(100.0f, 100.0f);
    this->_spriteNodeTree = dynamic_cast<UISprite *>(this->_ndNodeTree->addComponent("UISprite"));
    this->_spriteNodeTree->setColor(EditorConfig::content);
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
    this->_fileTree = dynamic_cast<EditorAssetsFileTree *>(this->_ndNodeTree->addComponent("EditorAssetsFileTree"));
    this->_fileTree->onSelectEvent([this](std::string uuid)
                                       { this->_onFileTreeSelectEvent(uuid); });
}
void EditorAssetsLayout::setRoot(std::string root)
{
    this->_fileTree->setRoot(root);
}
void EditorAssetsLayout::_onFileTreeSelectEvent(std::string uuid)
{
    std::cout << "EditorAssetsLayout::_onFileTreeSelectEvent: " << uuid << std::endl;
}


EditorAssetsLayout::~EditorAssetsLayout()
{
}



// void EditorAssetsLayout::_initContent()
// {

//     this->_content = new Node2D("NodeTree");
//     this->_root->addChild(this->_content);
//     this->_content->setSize(100.0f, 20.0f);
//     // Sprite *sprite = this->_content->addComponent<Sprite>("Sprite");
//     // sprite->setColor(EditorConfig::theme);
//     // 添加wedget组件
//     UIWidget *widget = this->_content->addComponent<UIWidget>("UIWidget");
//     WidgetHorizontalParam paramHorizontal{};
//     paramHorizontal.left = 0.0f;
//     paramHorizontal.right = 0.0f;
//     WidgetVerticalParam paramVertical{};
//     paramVertical.top = 0.0f;
//     paramVertical.bottom = 0.0f;
//     widget->setHorizontal(WidgetHorizontal::ALL, paramHorizontal);
//     widget->setVertical(WidgetVertical::ALL, paramVertical);

//     this->_initTitle();
//     this->_initOperate();
//     this->_initFileTree();
//     this->_content->onNodeInputEvent(NodeInput::TOUCH_START, [this](NodeInputResult &result)
//                                      { this->_onRootInputStartEvent(result); });
// }

// void EditorAssetsLayout::_initTitle()
// {
//     float width = 120.0f;
//     float height = 25.0f;
//     Node2D *ndTitle = new Node2D("Title");
//     this->_root->addChild(ndTitle);
//     ndTitle->setSize(width, height);
//     Sprite *sprite = ndTitle->addComponent<Sprite>("Sprite");
//     sprite->setColor(EditorConfig::theme);
//     // 添加wedget组件
//     UIWidget *widget = ndTitle->addComponent<UIWidget>("UIWidget");
//     if (widget != nullptr)
//     {
//         WidgetHorizontalParam paramHorizontal{};
//         WidgetVerticalParam paramVertical{};
//         widget->setHorizontal(WidgetHorizontal::LEFT, paramHorizontal);
//         widget->setVertical(WidgetVertical::TOP, paramVertical);
//     }

//     // icon
//     float iconWidth = 20.0f;
//     float iconHeight = 20.0f;
//     Node2D *ndIcon = new Node2D("TitleIcon");
//     ndTitle->addChild(ndIcon);
//     ndIcon->setSize(iconWidth, iconHeight);
//     ndIcon->setPosition(-width / 2.0 + 5.0f + iconWidth / 2.0, 0.0, 0.0);
//     Sprite *spriteIcon = ndIcon->addComponent<Sprite>("Sprite");
//     spriteIcon->setColor(EditorConfig::iconColor);
//     spriteIcon->setTextureAsset("resources/textures/ic-title-assets.png");
//     // name
//     float nameWidth = 72.0f;
//     float nameHeight = 16.0f;
//     Node2D *ndName = new Node2D("TitleName");
//     ndTitle->addChild(ndName);
//     ndName->setSize(nameWidth, nameHeight);
//     ndName->setPosition(-width / 2.0 + 2.0f + iconWidth + 5.0f + nameWidth / 2.0, 0.0f, 0.0f);
//     Text *txtName = ndName->addComponent<Text>("Text");
//     txtName->setColor(EditorConfig::fontColor);
//     txtName->setText("资源管理器");
// }

// void EditorAssetsLayout::_initOperate()
// {
//     Node2D *ndOperate = new Node2D("Operate");
//     this->_root->addChild(ndOperate);
//     ndOperate->setSize(100.0f, 20.0f);
//     Sprite *sprite = ndOperate->addComponent<Sprite>("Sprite");
//     sprite->setColor(EditorConfig::theme);
//     // 添加wedget组件
//     UIWidget *widget = ndOperate->addComponent<UIWidget>("UIWidget");
//     if (widget != nullptr)
//     {
//         WidgetHorizontalParam paramHorizontal{};
//         paramHorizontal.left = 0.0f;
//         paramHorizontal.right = 0.0f;
//         WidgetVerticalParam paramVertical{};
//         paramVertical.top = 26.0f;
//         widget->setHorizontal(WidgetHorizontal::ALL, paramHorizontal);
//         widget->setVertical(WidgetVertical::TOP, paramVertical);
//     }
//     // 添加按钮
//     Node2D *ndOperateAdd = new Node2D("OperateAdd");
//     ndOperate->addChild(ndOperateAdd);
//     ndOperateAdd->setSize(14.0f, 14.0f);
//     Sprite *spriteAdd = ndOperateAdd->addComponent<Sprite>("Sprite");
//     spriteAdd->setColor(EditorConfig::iconColor);
//     spriteAdd->setTextureAsset("resources/textures/ic-add.png");
//     UIWidget *widgetAdd = ndOperateAdd->addComponent<UIWidget>("UIWidget");
//     WidgetHorizontalParam paramHorizontal{};
//     paramHorizontal.left = 5.0f;
//     widgetAdd->setHorizontal(WidgetHorizontal::LEFT, paramHorizontal);
//     // 折叠按钮
//     Node2D *ndOperateCollapse = new Node2D("OperateCollapse");
//     ndOperate->addChild(ndOperateCollapse);
//     ndOperateCollapse->setSize(14.0f, 14.0f);
//     Sprite *spriteCollaps = ndOperateCollapse->addComponent<Sprite>("Sprite");
//     spriteCollaps->setColor(EditorConfig::iconColor);
//     spriteCollaps->setTextureAsset("resources/textures/ic-flod.png");
//     UIWidget *widgetCollaps = ndOperateCollapse->addComponent<UIWidget>("UIWidget");
//     WidgetHorizontalParam widgetCollapsHParam{};
//     widgetCollapsHParam.right = 5.0f;
//     widgetCollaps->setHorizontal(WidgetHorizontal::RIGHT, widgetCollapsHParam);
// }

// void EditorAssetsLayout::_initFileTree()
// {
//     Node2D *ndContent = new Node2D("NodeTree");
//     this->_root->addChild(ndContent);
//     ndContent->setSize(100.0f, 20.0f);
//     Sprite *sprite = ndContent->addComponent<Sprite>("Sprite");
//     sprite->setColor(EditorConfig::theme);
//     // 添加wedget组件
//     UIWidget *widget = ndContent->addComponent<UIWidget>("UIWidget");
//     WidgetHorizontalParam paramHorizontal{};
//     paramHorizontal.left = 0.0f;
//     paramHorizontal.right = 0.0f;
//     WidgetVerticalParam paramVertical{};
//     paramVertical.top = 47.0f;
//     paramVertical.bottom = 0.0f;
//     widget->setHorizontal(WidgetHorizontal::ALL, paramHorizontal);
//     widget->setVertical(WidgetVertical::ALL, paramVertical);
//     // 添加mask组件
//     UIMask *uiMask = ndContent->addComponent<UIMask>("UIMask");
//     if (uiMask != nullptr)
//     {
//         std::cout << "EditorHierarchyLayout create UIMask:" << uiMask << std::endl;
//     }
//     // 添加节点树
//     this->_fileTree = ndContent->addComponent<FileTree>("FileTree");
//     std::cout << "EditorHierarchyLayout create FileTree:" << this->_fileTree << std::endl;
//     this->_fileTree->onSelectEvent([this](UITreeStructure *uiTreeData)
//                                    { this->_onFileTreeSelectEvent(uiTreeData); });
// }
// void EditorAssetsLayout::setRoot(std::string root)
// {
//     this->_fileTree->setRoot(root);
// }
// void EditorAssetsLayout::_onRootInputStartEvent(NodeInputResult &result)
// {
//     std::cout << "EditorAssets Lock the focus" << std::endl;
//     EditorIpc::getInstance()->send(IpcEvent::FOCUS, std::string("EditorAssets"));
//     if (this->_fileTree != nullptr)
//     {
//         // 点击了空白区域，清除选择
//         this->_fileTree->clearSelect();
//         EditorIpc::getInstance()->send(IpcEvent::SELECT_ASSETS, std::string(""));
//     }
// }
// void EditorAssetsLayout::_onFileTreeSelectEvent(UITreeStructure *uiTreeData)
// {
//     EditorIpc::getInstance()->send(IpcEvent::FOCUS, std::string("EditorAssets"));
//     if (uiTreeData != nullptr)
//     {
//         std::cout << "EditorAssets select file:" << uiTreeData->name << std::endl;
//         EditorIpc::getInstance()->send(IpcEvent::SELECT_ASSETS, uiTreeData->name);
//     }
//     else
//     {
//         std::cout << "EditorAssets select file: nullptr" << std::endl;
//         this->_fileTree->clearSelect();
//         EditorIpc::getInstance()->send(IpcEvent::SELECT_ASSETS, std::string(""));
//     }
// }

// void EditorAssetsLayout::loseFocus()
// {
//     std::cout << "EditorAssets Lose the focus" << std::endl;
//     if (this->_fileTree != nullptr)
//     {
//         this->_fileTree->loseFocusSelect();
//     }
// }
