#include "ui-widget.h"
#include "../../math/size.h"
#include "../../math/vec3.h"
#include "../../math/vec2.h"
#include "../../scene/node.h"
#include "../../scene/node-2d.h"

UIWidget::UIWidget(Node *node, std::string uuid) : Component(node, uuid)
{
    this->_layer = NodeLayer::Node2D;
    this->_flag = 1;
}
void UIWidget::setHorizontal(WidgetHorizontal horizontal, WidgetHorizontalParam param)
{
    this->_horizontal = horizontal;
    this->_horizontalParam = param;
    this->_flag++;
}
void UIWidget::setVertical(WidgetVertical vertical, WidgetVerticalParam param)
{
    this->_vertical = vertical;
    this->_verticalParam = param;
    this->_flag++;
}
void UIWidget::update(float deltaTime)
{
    Component::update(deltaTime);
}
void UIWidget::lateUpdate(float deltaTime)
{
    Component::lateUpdate(deltaTime);
    if (this->_node->hasFrameTransformFlag() || (this->_node->getParent() != nullptr && this->_node->getParent()->hasFrameTransformFlag()) || this->_flag > 0)
    {
        Node2D *node = dynamic_cast<Node2D *>(this->_node);
        if (node == nullptr)
        {
            return;
        }
        Node2D *parent = dynamic_cast<Node2D *>(this->_node->getParent());
        if (parent == nullptr)
        {
            return;
        }
        std::cout << "UIWidget::lateUpdate" << std::endl;
        // 父节点尺寸
        const Size &parentSize = parent->getSize();
        // 当前节点尺寸
        const Size &ndSize = node->getSize();
        const Vec2 &ndAnchor = node->getAnchor();
        const Vec3 &ndScale = node->getScale();
        std::cout << "UIWidget::lateUpdate parentSize: " << parentSize.getWidth() << ", " << parentSize.getHeight() << std::endl;
        std::cout << "UIWidget::lateUpdate ndAnchor: " << ndAnchor.getX() << ", " << ndAnchor.getY() << std::endl;
        std::cout << "UIWidget::lateUpdate ndScale: " << ndScale.getX() << ", " << ndScale.getY() << ", " << ndScale.getZ() << std::endl;
        std::cout << "UIWidget::lateUpdate ndSize: " << ndSize.getWidth() << ", " << ndSize.getHeight() << std::endl;

        Size newSize;
        newSize.set(ndSize.getWidth(), ndSize.getHeight());
        Vec3 newPosition;
        if (this->_horizontal == WidgetHorizontal::LEFT) // 左对齐
        {
            newPosition.setX(-parentSize.getWidth() / 2.0f + this->_horizontalParam.left + ndSize.getWidth() * ndAnchor.getX() * ndScale.getX());
        }
        else if (this->_horizontal == WidgetHorizontal::RIGHT) // 右对齐
        {
            newPosition.setX(parentSize.getWidth() / 2.0f - this->_horizontalParam.right - ndSize.getWidth() * ndAnchor.getX() * ndScale.getX());
        }
        else if (this->_horizontal == WidgetHorizontal::CENTER) // 居中对齐
        {
            newPosition.setX(this->_horizontalParam.center);
        }
        else if (this->_horizontal == WidgetHorizontal::ALL) // 全部对齐 尺寸发生了变化
        {
            newSize.setWidth((parentSize.getWidth() - this->_horizontalParam.left - this->_horizontalParam.right) / ndScale.getX());
            newPosition.setX(-parentSize.getWidth() / 2.0f + this->_horizontalParam.left + newSize.getWidth() * ndAnchor.getX() * ndScale.getX());
        }

        // 垂直对齐
        if (this->_vertical == WidgetVertical::TOP) // 上对齐-
        {
            newPosition.setY(parentSize.getHeight() / 2.0f - this->_verticalParam.top - ndSize.getHeight() * (1.0 - ndAnchor.getY()) * ndScale.getY());
        }
        else if (this->_vertical == WidgetVertical::BOTTOM)
        { // 下对齐
            newPosition.setY(-parentSize.getHeight() / 2.0f + this->_verticalParam.bottom + ndSize.getHeight() * (1.0 - ndAnchor.getY()) * ndScale.getY());
        }
        else if (this->_vertical == WidgetVertical::CENTER)
        { // 居中对齐
            newPosition.setY(this->_verticalParam.center);
        }
        else if (this->_vertical == WidgetVertical::ALL)
        { // 全部对齐 尺寸发生了变化
            newSize.setHeight((parentSize.getHeight() - this->_verticalParam.top - this->_verticalParam.bottom) / ndScale.getY());
            newPosition.setY(parentSize.getHeight() / 2.0f - this->_verticalParam.top - newSize.getHeight() * (1.0 - ndAnchor.getY()) * ndScale.getY());
        }
        // 更新节点的位置和尺寸
        node->setPosition(newPosition.getX(), newPosition.getY(), newPosition.getZ());
        node->setSize(newSize.getWidth(), newSize.getHeight()); /* code */
        std::cout << "UIWidget::lateUpdate newPosition: " << newPosition.getX() << ", " << newPosition.getY() << ", " << newPosition.getZ() << std::endl;
        std::cout << "UIWidget::lateUpdate newSize: " << newSize.getWidth() << ", " << newSize.getHeight() << std::endl;
        this->_flag = 0;
    }
}
void UIWidget::render()
{
}

void UIWidget::destroy()
{
    Component::destroy();
    std::cout << "UIWidget::destroy" << std::endl;
}
UIWidget::~UIWidget()
{
}
