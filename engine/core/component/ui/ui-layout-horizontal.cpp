#include "ui-layout-horizontal.h"
#include "../../math/size.h"
#include "../../math/vec3.h"
#include "../../math/vec2.h"
#include "../../scene/node.h"
#include "../../scene/node-2d.h"
#include "../../renderer/ui/ui-sprite.h"

UILayoutHorizontal::UILayoutHorizontal(Node *node, std::string uuid) : Component(node, uuid)
{
    this->_layer = NodeLayer::Node2D;
    // Node2D *node2D = dynamic_cast<Node2D *>(this->_node);
    // if (node2D == nullptr)
    // {
    //     return;
    // }
    // const Size &ndSize = node2D->getSize();

    // for (int i = 0; i < 3; i++)
    // {
    //     Node2D *ndLeft = new Node2D("Editor-Left");
    //     ndLeft->setSize(ndSize.getWidth() / 3.0f, ndSize.getHeight());
    //     ndLeft->setPosition(-ndSize.getWidth() / 2.0f + i * ndSize.getWidth() / 3.0f, 0, 0.0f);
    //     std::cout<<"UILayoutHorizontal::lateUpdate ndLeft: "<<i<<", "<<ndLeft->getPosition().getX()<<", "<<ndLeft->getPosition().getY()<<", "<<ndLeft->getPosition().getZ()<<std::endl;
    //     this->_node->addChild(ndLeft);
    //     UISprite *spriteLeft = dynamic_cast<UISprite *>(ndLeft->addComponent("UISprite"));
    //     if (spriteLeft != nullptr)
    //     {
    //         std::cout << "EditorLayout::_initLayoutUI1 UISprite" << std::endl;
    //         spriteLeft->setTexture("resources/texture/ic-default.png");
    //         spriteLeft->setMaterial(nullptr);
    //         spriteLeft->setColor(0.3f, 0.1f, 0.1f, 1.0);
    //     }

    // }
    //this->_nodeTransformEventId = this->_node->onTransformChange(&UILayoutHorizontal::_onNodeTransformEvent, this);
}

void UILayoutHorizontal::Awake()
{
    Component::Awake();
}
void UILayoutHorizontal::Enable()
{
    Component::Enable();
}

void UILayoutHorizontal::setSize(int size)
{
    //this->_clear();
    //this->_size = size;
    //Node2D *node2D = dynamic_cast<Node2D *>(this->_node);
    //if (node2D == nullptr)
    //{
    //    return;
    //}
  

    //for (int i = 0; i < this->_size; i++)
    //{
    //    UILayoutHorizontalConfig config;
    //    Node2D *ndContent = new Node2D("Editor-Content");
    //    this->_node->addChild(ndContent);
    //    UISprite *spriteContent = dynamic_cast<UISprite *>(ndContent->addComponent("UISprite"));
    //    if (spriteContent != nullptr)
    //    {
    //        std::cout << "EditorLayout::_initLayoutUI1 UISprite" << std::endl;
    //        spriteContent->setTexture("resources/texture/ic-default.png");
    //        spriteContent->setMaterial(nullptr);
    //        spriteContent->setColor(0.3f, 0.1f, 0.1f, 1.0);
    //    }
    //    // ndContent->setSize(ndSize.getWidth() / this->_size, ndSize.getHeight());
    //    // ndContent->setPosition(-ndSize.getWidth() / 2.0f + i * ndSize.getWidth() / this->_size, 0, 0.0f);
    //    // config.width = ndSize.getWidth() / this->_size;
    //    config.node = ndContent;
    //    this->_configs.push_back(config);
    //}
    //this->_onNodeTransformEvent(NodeTransformFlag::SIZE_FLAG);
}
void UILayoutHorizontal::_onNodeTransformEvent(uint32_t eventId)
{
    if (eventId != NodeTransformFlag::SIZE_FLAG)
    {
        return;
    }

}
void UILayoutHorizontal::Update(float deltaTime)
{
    Component::Update(deltaTime);
}
void UILayoutHorizontal::LateUpdate(float deltaTime)
{
    Component::LateUpdate(deltaTime);
}
void UILayoutHorizontal::Render()
{
    Component::Render();
}
void UILayoutHorizontal::LateRender()
{
    Component::LateRender();
}
void UILayoutHorizontal::_clear()
{
   
   /* this->_configs.clear();*/
}
void UILayoutHorizontal::Disable()
{
    Component::Disable();
}



void UILayoutHorizontal::destroy()
{
    Component::destroy();
    this->_clear();
    this->_node->offTransformChange(this->_nodeTransformEventId);
}

UILayoutHorizontal::~UILayoutHorizontal()
{
}