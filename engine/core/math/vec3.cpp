#include "vec3.h"
#include <cstdio>
#include "math.h"
#include "quat.h"
#include <algorithm>

Vec3::Vec3()
{
    this->_x = 0.0;
    this->_y = 0.0;
    this->_z = 0.0;
}
Vec3::Vec3(float x, float y, float z)
{
    this->_x = x;
    this->_y = y;
    this->_z = z;
}
void Vec3::set(float x, float y, float z)
{
    this->_x = x;
    this->_y = y;
    this->_z = z;
}
void Vec3::set(Vec3 &v)
{
    this->_x = v.getX();
    this->_y = v.getX();
    this->_z = v.getZ();
}
void Vec3::setX(float x)
{
    this->_x = x;
}
void Vec3::setY(float y)
{
    this->_y = y;
}
void Vec3::setZ(float z)
{
    this->_z = z;
}
float Vec3::getX()
{
    return this->_x;
}
float Vec3::getY()
{
    return this->_y;
}
float Vec3::getZ()
{
    return this->_z;
}
float Vec3::len()
{
    return sqrt(this->_x * this->_x + this->_y * this->_y + this->_z + this->_z);
}
void Vec3::add(Vec3 &v)
{
    this->_x = this->_x + v.getX();
    this->_y = this->_y + v.getY();
    this->_z = this->_z + v.getZ();
}
void Vec3::subtract(Vec3 &v)
{
    this->_x = this->_x - v.getX();
    this->_y = this->_y - v.getY();
    this->_z = this->_z - v.getZ();
}
void Vec3::add(Vec3 *out, Vec3 *a, Vec3 *b)
{
    out->set(a->getX() + b->getX(), a->getY() + b->getY(), a->getZ() + b->getZ());
}
void Vec3::subtract(Vec3 *out, Vec3 *a, Vec3 *b)
{
    out->set(a->getX() - b->getX(), a->getY() - b->getY(), a->getZ() - b->getZ());
}

void Vec3::min(Vec3 &v)
{
    this->_x = std::min(this->_x, v.getX());
    this->_y = std::min(this->_y, v.getY());
    this->_z = std::min(this->_z, v.getZ());
}
void Vec3::max(Vec3 &v)
{
    this->_x = std::max(this->_x, v.getX());        
    this->_y = std::max(this->_y, v.getY());
    this->_z = std::max(this->_z, v.getZ());
}
void Vec3::rotate(Quat *q)
{
    // 四元数旋转向量的核心公式： v‘=q*v*q-1;
    // q=(w,x,y,z) 是单位四元数（表示旋转）。
    // v=(0,x,y,z) 是被旋转的向量（转换为纯四元数）。
    // q-1=(w,−x,−y,−z) 是四元数的逆（单位四元数的逆即共轭

    const float qx = q->getX();
    const float qy = q->getY();
    const float qz = q->getZ();
    const float qw = q->getW();
    // 计算q*v
    const float ix = qw * this->_x + qy * this->_z - qz * this->_y; // 实部: - (qx*x + qy*y + qz*z)
    const float iy = qw * this->_y + qz * this->_x - qx * this->_z;
    const float iz = qw * this->_z + qx * this->_y - qy * this->_x;
    const float iw = -qx * this->_x - qy * this->_y - qz * this->_z; // 虚部: qw*x + qy*z - qz*y 等

    this->_x = ix * qw + iw * -qx + iy * -qz - iz * -qy;
    this->_y = iy * qw + iw * -qy + iz * -qx - ix * -qz;
    this->_z = iz * qw + iw * -qz + ix * -qy - iy * -qx;
}

void Vec3::test()
{
    Vec3 v(0, 0, 0);
    for (int i = 0; i < 100000000; i++)
    {
        v.set(99.99, 88.88, 0.99);
        this->add(v);
    }
}

Vec3::~Vec3()
{
}
