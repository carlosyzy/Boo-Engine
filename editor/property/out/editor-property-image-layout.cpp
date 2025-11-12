// #include "editor-property-image-layout.h"
// #include "../editor-config.h"
// #include "../ipc/editor-ipc.h"
// #include "../../engine/core/renderer/scene.h"
// #include "../../engine/core/renderer/node/node.h"
// #include "../../engine/core/renderer/node/node-2d.h"
// #include "../../engine/core/renderer/ui/sprite.h"
// #include "../../engine/core/ui/ui-widget.h"
// #include "../../engine/core/ui/ui-tree/node-tree.h"
// #include "../../engine/core/ui/ui-tree/file-tree.h"
// #include "../../engine/core/ui/ui-mask.h"
// #include "../../engine/core/input/input-mgr.h"

// EditorPropertyImageLayout::EditorPropertyImageLayout(Node2D *root)
// {
//     this->_root = root;
//     this->_initContent();
// }
// void EditorPropertyImageLayout::_initContent()
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

//     this->_content->onNodeInputEvent(NodeInput::TOUCH_START, [this](NodeInputResult &result)
//                                   { if (this->_onRootInputEndEventCallback != nullptr){
//                                       this->_onRootInputEndEventCallback(result);
//                                     } });
// }
// void EditorPropertyImageLayout::_initTitle()
// {
//     float width = 120.0f;
//     float height = 25.0f;
//     Node2D *ndTitle = new Node2D("Title");
//     this->_content->addChild(ndTitle);
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
//     spriteIcon->setTexture("resources/textures/ic-title-assets.png");
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

// void EditorPropertyImageLayout::_initOperate()
// {
//     Node2D *ndOperate = new Node2D("Operate");
//     this->_content->addChild(ndOperate);
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
//     spriteAdd->setTexture("resources/textures/ic-add.png");
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
//     spriteCollaps->setTexture("resources/textures/ic-flod.png");
//     UIWidget *widgetCollaps = ndOperateCollapse->addComponent<UIWidget>("UIWidget");
//     WidgetHorizontalParam widgetCollapsHParam{};
//     widgetCollapsHParam.right = 5.0f;
//     widgetCollaps->setHorizontal(WidgetHorizontal::RIGHT, widgetCollapsHParam);
// }

// void EditorPropertyImageLayout::setActive(bool active)
// {
//     this->_content->setActive(active);
// }

// void EditorPropertyImageLayout::onRootInputEndEvent(std::function<void(NodeInputResult &result)> callback)
// {
//     this->_onRootInputEndEventCallback = callback;
// }

// void EditorPropertyImageLayout::loseFocus()
// {
//     std::cout << "EditorProperty Lose the focus" << std::endl;
// }

// EditorPropertyImageLayout::~EditorPropertyImageLayout()
// {
// }