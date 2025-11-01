#include "quat.h"
Quat::Quat() : _x(0), _y(0), _z(0), _w(1)
{
}
Quat::Quat(float x, float y, float z, float w) : _x(x), _y(y), _z(z), _w(w)
{

}
void Quat::set(float x, float y, float z, float w)
{
    this->_x = x;
    this->_y = y;
    this->_z = z;
    this->_w = w;
}
float Quat::getX()
{
    return this->_x;
}
float Quat::getY()
{
    return this->_y;
}
float Quat::getZ()
{
    return this->_z;
}
float Quat::getW()
{
    return this->_w;
}

Quat::~Quat()
{
}
