#include "size.h"

Size Size::Default{0,0};

Size::Size(){
    this->_width = 0;
    this->_height = 0;
}
Size::Size(float width, float height)
{
    this->_width = width;
    this->_height = height;
}

void Size::set(float width, float height)
{
    this->_width = width;
    this->_height = height;
}
float Size::getWidth() const
{
    return this->_width;
}
float Size::getHeight() const
{
    return this->_height;
}
void Size::setWidth(float width)
{
    this->_width = width;
}
void Size::setHeight(float height)
{
    this->_height = height;
}

Size::~Size()
{
}