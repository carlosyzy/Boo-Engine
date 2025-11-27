// #include "editor-property-layout.h"
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
// #include "../../engine/core/math/vec3.h"

// EditorPropertyLayout::EditorPropertyLayout(Node2D *root)
// {
//     this->_root = root;
//     this->_initContent();
// }
// void EditorPropertyLayout::_initContent()
// {

//     this->_content = new Node2D("Content");
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
//     this->_initContentLayout();
//     // this->_initScrollView();
//     this->_content->onNodeInputEvent(NodeInput::TOUCH_START, [this](NodeInputResult &result)
//                                      { if (this->_onRootInputStartEventCallback != nullptr){
//                                       this->_onRootInputStartEventCallback(result);
//                                     } });
// }

// void EditorPropertyLayout::_initTitle()
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
//     txtName->setText("属性管理器");
// }
// void EditorPropertyLayout::onRootInputEndEvent(std::function<void(NodeInputResult &result)> callback)
// {
//     this->_onRootInputStartEventCallback = callback;
// }

// void EditorPropertyLayout::_initContentLayout()
// {
//     this->_contentLayout = new Node2D("ContentLayout");
//     this->_content->addChild(this->_contentLayout);
//     this->_contentLayout->setSize(100.0f, 20.0f);
//     // 添加sprite组件
//     Sprite *sprite = this->_contentLayout->addComponent<Sprite>("Sprite");
//     sprite->setColor(EditorConfig::theme);
//     // 添加wedget组件
//     WidgetHorizontalParam paramHorizontal{};
//     paramHorizontal.left = 0.0f;
//     paramHorizontal.right = 0.0f;
//     WidgetVerticalParam paramVertical{};
//     paramVertical.top = 26.0f;
//     paramVertical.bottom = 0.0f;
//     UIWidget *widget = this->_contentLayout->addComponent<UIWidget>("UIWidget");
//     widget->setHorizontal(WidgetHorizontal::ALL, paramHorizontal);
//     widget->setVertical(WidgetVertical::ALL, paramVertical);
// }

// void EditorPropertyLayout::_initScrollView()
// {
//     this->_scrollBarTouch = new Vec3();
//     // 滑动条背景
//     this->_ndScrollBarBg = new Node2D("ScrollBarBg");
//     this->_content->addChild(this->_ndScrollBarBg);
//     this->_ndScrollBarBg->setAnchor(0.5, 0.5);
//     this->_spScrollBarBg = this->_ndScrollBarBg->addComponent<Sprite>("Sprite");
//     this->_spScrollBarBg->setTextureAsset("resources/textures/ic-default.png");
//     this->_spScrollBarBg->setColor(24.0f / 255.0f, 32.0f / 255.0f, 43.0f / 255.0f, 1.0f);
//     this->_scrollBarBgWidth = 8.0f;
//     this->_scrollBarBgHeight = 100.0f;
//     this->_scrollBarBgX = 0.0f;
//     this->_scrollBarBgY = 0.0f;
//     this->_ndScrollBarBg->setPosition(this->_scrollBarBgX, this->_scrollBarBgY, 0);
//     this->_ndScrollBarBg->setSize(this->_scrollBarBgWidth, this->_scrollBarBgHeight);
//     // 滑动条
//     this->_ndScrollBar = new Node2D("ScrollBar");
//     this->_ndScrollBar->setAnchor(0.5, 0.5);
//     this->_ndScrollBarBg->addChild(this->_ndScrollBar);
//     this->_spScrollBar = this->_ndScrollBar->addComponent<Sprite>("Sprite");
//     this->_spScrollBar->setTextureAsset("resources/textures/ic-default.png");
//     this->_spScrollBar->setColor(140.0f / 255.0f, 210.0f / 255.0f, 240.0f / 255.0f, 1.0f);
//     this->_scrollBarWidth = this->_scrollBarBgWidth - 1.0f;
//     this->_scrollBarHeight = 20.0f;
//     this->_scrollBarX = 0.0f;
//     this->_scrollBarY = 0.0f;
//     this->_ndScrollBar->setPosition(this->_scrollBarX, this->_scrollBarY, 0);
//     this->_ndScrollBar->setSize(this->_scrollBarWidth, this->_scrollBarHeight);
//     this->_ndScrollBar->onNodeInputEvent(NodeInput::TOUCH_START, [this](NodeInputResult &result)
//                                          { this->_onScrollBarTouchStartEvent(result); }, true);
//     this->_ndScrollBar->onNodeInputEvent(NodeInput::TOUCH_MOVE, [this](NodeInputResult &result)
//                                          { this->_onScrollBarTouchMoveEvent(result); }, true);
//     this->_ndScrollBar->onNodeInputEvent(NodeInput::TOUCH_END, [this](NodeInputResult &result)
//                                          { this->_onScrollBarTouchEndEvent(result); }, true);
//     this->_ndScrollBar->onNodeInputEvent(NodeInput::TOUCH_CANCEL, [this](NodeInputResult &result)
//                                          { this->_onScrollBarTouchEndEvent(result); }, true);
// }
// void EditorPropertyLayout::_onScrollBarTouchStartEvent(NodeInputResult &result)
// {
//     // 选中状态
//     this->_isScrollBarTouching = true;
//     this->_scrollBarTouch->setX(result.worldX);
//     this->_scrollBarTouch->setY(result.worldY);
// }
// void EditorPropertyLayout::_onScrollBarTouchMoveEvent(NodeInputResult &result)
// {
//     if (!this->_isScrollBarTouching)
//         return;
//     // this->_scrollBarX = 0.0f;
//     // this->_scrollBarY += result.worldY - this->_scrollBarTouch.y();
//     // // std::cout << "滑动条移动:" << "    x:" << this->_scrollBarX << ",    y:" << this->_scrollBarY << std::endl;
//     // if (this->_scrollBarY > this->_scrollBarBgHeight / 2 - this->_scrollBarHeight / 2)
//     // {
//     //     this->_scrollBarY = this->_scrollBarBgHeight / 2 - this->_scrollBarHeight / 2;
//     // }
//     // if (this->_scrollBarY < -this->_scrollBarBgHeight / 2 + this->_scrollBarHeight / 2)
//     // {
//     //     this->_scrollBarY = -this->_scrollBarBgHeight / 2 + this->_scrollBarHeight / 2;
//     // }
//     // this->_ndScrollBar->setPosition(this->_scrollBarX, this->_scrollBarY, 0);
//     // this->_scrollBarTouch.setX(result.worldX);
//     // this->_scrollBarTouch.setY(result.worldY);

//     // this->_topLen = this->_scrollBarBgHeight / 2.0f - this->_scrollBarHeight / 2.0f - this->_scrollBarY;
//     // this->_topLen = this->_topLen * this->_scrollBarRate;
//     // Node2D *node = dynamic_cast<Node2D *>(this->_node);
//     // Size &size = node->size();
//     // this->_ndContent->setPosition(-size.width() / 2.0f + this->_leftLen, size.height() / 2 + this->_topLen, 0.0f);
// }
// // 取消选中状态
// void EditorPropertyLayout::_onScrollBarTouchEndEvent(NodeInputResult &result)
// {
//     this->_isScrollBarTouching = false;
// }

// void EditorPropertyLayout::setNode(Node *ndBind)
// {
//     // 销毁其他两个
//     std::cout << "aaaaa" << std::endl;
//     this->_clear();
//     std::cout << "0000000" << std::endl;
//     this->_nodeLayout = new PropertyNodeLayout(this, this->_content);
//     std::cout << "111111" << std::endl;
// }
// void EditorPropertyLayout::loseFocus()
// {
//     std::cout << "EditorProperty Lose the focus" << std::endl;
// }
// void EditorPropertyLayout::_clear()
// {
//     // if (this->_sceneLayou != nullptr)
//     // {
//     //     delete this->_sceneLayou;
//     // }
//     if (this->_nodeLayout != nullptr)
//     {
//         delete this->_nodeLayout;
//         this->_nodeLayout = nullptr;
//     }
//     // if (this->_imageLayou != nullptr)
//     // {
//     //     delete this->_imageLayou;
//     // }
// }
// EditorPropertyLayout::~EditorPropertyLayout()
// {
//     this->_clear();
// }

// EditorPropertyLayout::PropertyNodeLayout::PropertyNodeLayout(EditorPropertyLayout *layout, Node *ndBind)
// {
//     std::cout << "创建节点属性展示" << std::endl;
//     this->_layout = layout;
//     this->_ndBind = ndBind;
//     this->_initTitle();
// }
// void EditorPropertyLayout::PropertyNodeLayout::_initTitle()
// {
   
// }
// EditorPropertyLayout::PropertyNodeLayout::~PropertyNodeLayout()
// {
//     this->_layout->getContentLayout()->destroyAllChildren();
// }