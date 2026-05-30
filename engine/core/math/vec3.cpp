#include "vec3.h"
#include <cstdio>
#include "math.h"
#include "quat.h"
#include <algorithm>

namespace Boo
{

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
        this->_y = v.getY();
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
    float Vec3::getX() const
    {
        return this->_x;
    }
    float Vec3::getY() const
    {
        return this->_y;
    }
    // float Vec2::getX() const
    float Vec3::getZ() const
    {
        return this->_z;
    }
    float Vec3::len() const
    {
        return sqrt(this->_x * this->_x + this->_y * this->_y + this->_z * this->_z);
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
    void Vec3::rotate(Quat &q)
    {
        // 四元数旋转向量的核心公式： v‘=q*v*q-1;
        // q=(w,x,y,z) 是单位四元数（表示旋转）。
        // v=(0,x,y,z) 是被旋转的向量（转换为纯四元数）。
        // q-1=(w,−x,−y,−z) 是四元数的逆（单位四元数的逆即共轭

        const float qx = q.getX();
        const float qy = q.getY();
        const float qz = q.getZ();
        const float qw = q.getW();
        // 计算q*v
        const float ix = qw * this->_x + qy * this->_z - qz * this->_y; // 实部: - (qx*x + qy*y + qz*z)
        const float iy = qw * this->_y + qz * this->_x - qx * this->_z;
        const float iz = qw * this->_z + qx * this->_y - qy * this->_x;
        const float iw = -qx * this->_x - qy * this->_y - qz * this->_z; // 虚部: qw*x + qy*z - qz*y 等

        this->_x = ix * qw + iw * -qx + iy * -qz - iz * -qy;
        this->_y = iy * qw + iw * -qy + iz * -qx - ix * -qz;
        this->_z = iz * qw + iw * -qz + ix * -qy - iy * -qx;
    }
    void Vec3::normalize()
    {
        float len = this->len();
        if (len > 0.0f)
        {
            len = 1.0f / len;
            this->set(this->_x * len, this->_y * len, this->_z * len);
        }else
        {
            this->set(0.0f, 0.0f, 0.0f);
        }
    }
    Vec3::~Vec3()
    {
    }

} // namespace Boo
namespace Boo
{
    void Vec3::add(Vec3 &a, Vec3 &b, Vec3 &out)
    {
        out.set(a.getX() + b.getX(), a.getY() + b.getY(), a.getZ() + b.getZ());
    }
    void Vec3::subtract(const Vec3 &a, const Vec3 &b, Vec3 &out)
    {
        out.set(a.getX() - b.getX(), a.getY() - b.getY(), a.getZ() - b.getZ());
    }
    void Vec3::transformQuat(Vec3 &v, Quat &q, Vec3 &out)
    {
        const float qx = q.getX();
        const float qy = q.getY();
        const float qz = q.getZ();
        const float qw = q.getW();
        // 计算q*v
        const float ix = qw * v.getX() + qy * v.getZ() - qz * v.getY(); // 实部: - (qx*x + qy*y + qz*z)
        const float iy = qw * v.getY() + qz * v.getX() - qx * v.getZ();
        const float iz = qw * v.getZ() + qx * v.getY() - qy * v.getX();
        const float iw = -qx * v.getX() - qy * v.getY() - qz * v.getZ(); // 虚部: qw*x + qy*z - qz*y 等

        out.setX(ix * qw + iw * -qx + iy * -qz - iz * -qy);
        out.setY(iy * qw + iw * -qy + iz * -qx - ix * -qz);
        out.setZ(iz * qw + iw * -qz + ix * -qy - iy * -qx);
    }
    float Vec3::dot(const Vec3 &a, const Vec3 &b)
    {
        return a.getX() * b.getX() + a.getY() * b.getY() + a.getZ() * b.getZ();
    }
    void Vec3::normalize(const Vec3 &v, Vec3 &out)
    {
        float len = v.len();
        if (len > 0.0f)
        {
            len = 1.0f / len;
            out.set(v.getX() * len, v.getY() * len, v.getZ() * len);
        }else
        {
            out.set(0.0f, 0.0f, 0.0f);
        }
    }
    float Vec3::distance(const Vec3 &a, const Vec3 &b)
    {
        return sqrt((a.getX() - b.getX()) * (a.getX() - b.getX()) + (a.getY() - b.getY()) * (a.getY() - b.getY()) + (a.getZ() - b.getZ()) * (a.getZ() - b.getZ()));
    }

    float Vec3::lengthSqr(const Vec3 &v)
    {
        return v.getX() * v.getX() + v.getY() * v.getY() + v.getZ() * v.getZ();
    }

    void Vec3::cross(const Vec3 &a, const Vec3 &b, Vec3 &out)
    {
        out.set(
            a.getY() * b.getZ() - a.getZ() * b.getY(),
            a.getZ() * b.getX() - a.getX() * b.getZ(),
            a.getX() * b.getY() - a.getY() * b.getX()
        );
    }

}
