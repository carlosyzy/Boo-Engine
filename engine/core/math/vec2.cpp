#include "vec2.h"

Vec2::Vec2(){
    this->_x = 0;
    this->_y = 0;
}

Vec2::Vec2(float x, float y){
    this->_x = x;
    this->_y = y;
}
void Vec2::set(float x, float y)
{
    this->_x = x;
    this->_y = y;
}
float Vec2::getX() const
{
    return this->_x;
}
float Vec2::getY() const
{
    return this->_y;
}
Vec2::~Vec2()
{
}