#include "quat.h"
#include "math.h"
#include <cmath>
namespace Boo
{

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
    void Quat::setX(float x)
    {
        this->_x = x;
    }
    void Quat::setY(float y)
    {
        this->_y = y;
    }
    void Quat::setZ(float z)
    {
        this->_z = z;
    }
    void Quat::setW(float w)
    {
        this->_w = w;
    }
    float Quat::getX() const
    {
        return this->_x;
    }
    float Quat::getY() const
    {
        return this->_y;
    }
    float Quat::getZ() const
    {
        return this->_z;
    }
    float Quat::getW() const
    {
        return this->_w;
    }
    void Quat::normalize()
    {
        float length = _x * _x + _y * _y + _z * _z + _w * _w;
        if (length > 0) {
            length = 1 / sqrt(length);
            _x *= length;
            _y *= length;
            _z *= length;
            _w *= length;
        } else {
            _x = 0;
            _y = 0;
            _z = 0;
            _w = 0;
        }
    }

    

    Quat::~Quat()
    {
    }

} // namespace Boo
namespace Boo
{
    void Quat::conjugate(const Quat &q, Quat &out)
    {
        out.set(-q._x, -q._y, -q._z, q._w);
    }
    void Quat::multiply(const Quat &q1, const Quat &q2, Quat &out)
    {
        float x = q1.getX() * q2.getW() + q1.getW() * q2.getX() + q1.getY() * q2.getZ() - q1.getZ() * q2.getY();
        float y = q1.getY() * q2.getW() + q1.getW() * q2.getY() + q1.getZ() * q2.getX() - q1.getX() * q2.getZ();
        float z = q1.getZ() * q2.getW() + q1.getW() * q2.getZ() + q1.getX() * q2.getY() - q1.getY() * q2.getX();
        float w = q1.getW() * q2.getW() - q1.getX() * q2.getX() - q1.getY() * q2.getY() - q1.getZ() * q2.getZ();
        out.setX(x);
        out.setY(y);
        out.setZ(z);
        out.setW(w);
    }

    void Quat::fromMat3(const float *m, Quat &out)
    {
        // 列主序3x3矩阵: m[0]=m00, m[1]=m01, m[2]=m02, m[3]=m10, m[4]=m11, m[5]=m12, m[6]=m20, m[7]=m21, m[8]=m22
        float m00 = m[0];
        float m01 = m[1];
        float m02 = m[2];
        float m10 = m[3];
        float m11 = m[4];
        float m12 = m[5];
        float m20 = m[6];
        float m21 = m[7];
        float m22 = m[8];

        float fourXSquaredMinus1 = m00 - m11 - m22;
        float fourYSquaredMinus1 = m11 - m00 - m22;
        float fourZSquaredMinus1 = m22 - m00 - m11;
        float fourWSquaredMinus1 = m00 + m11 + m22;

        int biggestIndex = 0;
        float fourBiggestSquaredMinus1 = fourWSquaredMinus1;

        if (fourXSquaredMinus1 > fourBiggestSquaredMinus1)
        {
            fourBiggestSquaredMinus1 = fourXSquaredMinus1;
            biggestIndex = 1;
        }
        if (fourYSquaredMinus1 > fourBiggestSquaredMinus1)
        {
            fourBiggestSquaredMinus1 = fourYSquaredMinus1;
            biggestIndex = 2;
        }
        if (fourZSquaredMinus1 > fourBiggestSquaredMinus1)
        {
            fourBiggestSquaredMinus1 = fourZSquaredMinus1;
            biggestIndex = 3;
        }

        float biggestVal = std::sqrt(fourBiggestSquaredMinus1 + 1.0f) * 0.5f;
        float mult = 0.25f / biggestVal;

        switch (biggestIndex)
        {
        case 0:
            out.setW(biggestVal);
            out.setX((m12 - m21) * mult);
            out.setY((m20 - m02) * mult);
            out.setZ((m01 - m10) * mult);
            break;
        case 1:
            out.setW((m12 - m21) * mult);
            out.setX(biggestVal);
            out.setY((m01 + m10) * mult);
            out.setZ((m20 + m02) * mult);
            break;
        case 2:
            out.setW((m20 - m02) * mult);
            out.setX((m01 + m10) * mult);
            out.setY(biggestVal);
            out.setZ((m12 + m21) * mult);
            break;
        case 3:
            out.setW((m01 - m10) * mult);
            out.setX((m20 + m02) * mult);
            out.setY((m12 + m21) * mult);
            out.setZ(biggestVal);
            break;
        default:
            out.set(0, 0, 0, 1);
            break;
        }
    }
    void Quat::fromEuler(float x, float y, float z, RotationOrder order,Quat &out)
    {
        float halfToRad = 0.5f * 3.14159265358979323846f / 180.0f;
        // 转换为弧度的一半
        float fx = x * halfToRad;
        float fy = y * halfToRad;
        float fz = z * halfToRad;

        float sx = sin(fx);
        float cx = cos(fx);
        float sy = sin(fy);
        float cy = cos(fy);
        float sz = sin(fz);
        float cz = cos(fz);
        switch (order)
        {
        case RotationOrder::XYZ:
            // 旋转顺序：XYZ (先X，再Y，最后Z)
            // Q = Qz * Qy * Qx
            out.setX(sx * cy * cz - cx * sy * sz);
            out.setY(cx * sy * cz + sx * cy * sz);
            out.setZ(cx * cy * sz - sx * sy * cz);
            out.setW(cx * cy * cz + sx * sy * sz);
            break;

        case RotationOrder::XZY:
            // 旋转顺序：XZY (先X，再Z，最后Y)
            // Q = Qy * Qz * Qx
            out.setX(sx * cy * cz + cx * sy * sz);
            out.setY(cx * sy * cz - sx * cy * sz);
            out.setZ(cx * cy * sz + sx * sy * cz);
            out.setW(cx * cy * cz - sx * sy * sz);
            break;

        case RotationOrder::YXZ:
            // 旋转顺序：YXZ (先Y，再X，最后Z)
            // Q = Qz * Qx * Qy
            out.setX(sx * cy * cz + cx * sy * sz);
            out.setY(cx * sy * cz - sx * cy * sz);
            out.setZ(cx * cy * sz - sx * sy * cz);
            out.setW(cx * cy * cz + sx * sy * sz);
            break;

        case RotationOrder::YZX:
            // 旋转顺序：YZX (先Y，再Z，最后X)
            // Q = Qx * Qz * Qy
            out.setX(sx * cy * cz + cx * sy * sz);
            out.setY(cx * sy * cz + sx * cy * sz);
            out.setZ(cx * cy * sz - sx * sy * cz);
            out.setW(cx * cy * cz - sx * sy * sz);
            break;

        case RotationOrder::ZXY:
            // 旋转顺序：ZXY (先Z，再X，最后Y)
            // Q = Qy * Qx * Qz
            out.setX(sx * cy * cz - cx * sy * sz);
            out.setY(cx * sy * cz + sx * cy * sz);
            out.setZ(cx * cy * sz + sx * sy * cz);
            out.setW(cx * cy * cz - sx * sy * sz);
            break;

        case RotationOrder::ZYX:
            // 旋转顺序：ZYX (先Z，再Y，最后X)
            // Q = Qx * Qy * Qz
            out.setX(sx * cy * cz - cx * sy * sz);
            out.setY(cx * sy * cz - sx * cy * sz);
            out.setZ(cx * cy * sz + sx * sy * cz);
            out.setW(cx * cy * cz + sx * sy * sz);
            break;

        default:
            // 默认使用 YZX
            out.setX(sx * cy * cz + cx * sy * sz);
            out.setY(cx * sy * cz + sx * cy * sz);
            out.setZ(cx * cy * sz - sx * sy * cz);
            out.setW(cx * cy * cz - sx * sy * sz);
            break;
        }
    }
    // 辅助函数：弧度转角度
    float toDegree(float rad)
    {
        return rad * 180.0f / 3.14159265358979323846f;
    }

    // 辅助函数：符号函数
    float sign(float x)
    {
        if (x > 0.0f) return 1.0f;
        if (x < 0.0f) return -1.0f;
        return 0.0f;
    }

    void Quat::toEuler(const Quat &q, bool outerZ,Vec3 &eulerAngles)
    {
        float x = q.getX();
        float y = q.getY();
        float z = q.getZ();
        float w = q.getW();
        float bank = 0.0f;
        float heading = 0.0f;
        float attitude = 0.0f;
        float test = x * y + z * w;

        if (test > 0.499999f)
        {
            bank = 0.0f; // default to zero
            heading = toDegree(2.0f * std::atan2(x, w));
            attitude = 90.0f;
        }
        else if (test < -0.499999f)
        {
            bank = 0.0f; // default to zero
            heading = -toDegree(2.0f * std::atan2(x, w));
            attitude = -90.0f;
        }
        else
        {
            float sqx = x * x;
            float sqy = y * y;
            float sqz = z * z;
            bank = toDegree(std::atan2(2.0f * x * w - 2.0f * y * z, 1.0f - 2.0f * sqx - 2.0f * sqz));
            heading = toDegree(std::atan2(2.0f * y * w - 2.0f * x * z, 1.0f - 2.0f * sqy - 2.0f * sqz));
            attitude = toDegree(std::asin(2.0f * test));

            if (outerZ)
            {
                bank = -180.0f * sign(bank + 1e-6f) + bank;
                heading = -180.0f * sign(heading + 1e-6f) + heading;
                attitude = 180.0f * sign(attitude + 1e-6f) - attitude;
            }
        }

       eulerAngles.setX(bank);
       eulerAngles.setY(heading);
       eulerAngles.setZ(attitude);
    }

    void Quat::fromViewUp(const Vec3 &view, const Vec3 &up, Quat &out)
    {
        const float EPSILON = 1e-6f;
        
        // 检查view向量是否有效
        if (Vec3::lengthSqr(view) < EPSILON * EPSILON)
        {
            out.set(0, 0, 0, 1); // 单位四元数
            return;
        }

        // 计算右向量：up和view的叉乘并归一化
        Vec3 right;
        Vec3::cross(up, view, right);
        Vec3::normalize(right, right);

        // 检查right向量是否有效
        if (Vec3::lengthSqr(right) < EPSILON * EPSILON)
        {
            out.set(0, 0, 0, 1); // 单位四元数
            return;
        }

        // 计算新的上向量：view和right的叉乘
        Vec3 newUp;
        Vec3::cross(view, right, newUp);

        // 构建3x3旋转矩阵（列主序）
        float mat3[9] = {
            right.getX(), right.getY(), right.getZ(),
            newUp.getX(), newUp.getY(), newUp.getZ(),
            view.getX(), view.getY(), view.getZ()
        };

        // 从矩阵创建四元数
        Quat::fromMat3(mat3, out);
        out.normalize();

    }
}
