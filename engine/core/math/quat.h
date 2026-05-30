#pragma once
#include "vec3.h"

namespace Boo
{
    enum class RotationOrder
    {
        YZX, // 默认顺序
        XYZ,
        XZY,
        YXZ,
        ZXY,
        ZYX
    };
    class Quat
    {
    private:
        float _x;
        float _y;
        float _z;
        float _w;

    public:
        Quat();
        Quat(float x, float y, float z, float w);
        void set(float x, float y, float z, float w);
        void setX(float x);
        void setY(float y);
        void setZ(float z);
        void setW(float w);
        float getX() const;
        float getY() const;
        float getZ() const;
        float getW() const;
        void normalize();

       
        ~Quat();

    public:
        static void conjugate(const Quat &q, Quat &out);
        static void multiply(const Quat &q1, const Quat &q2, Quat &out);
        /**
         * @brief 从3x3旋转矩阵创建四元数
         *
         * @param m 3x3矩阵 (按列主序存储: m00,m01,m02, m10,m11,m12, m20,m21,m22)
         * @param out
         */
        static void fromMat3(const float *m, Quat &out);
        /**
         * @brief 将四元数转换为欧拉角
         * @param out 输出的欧拉角 (x: bank, y: heading, z: attitude)
         * @param q 输入的四元数
         * @param outerZ 是否使用外Z轴旋转
         */
        static void toEuler(const Quat &q, bool outerZ,Vec3 &eulerAngles);
         /**
         * @brief 从欧拉角创建四元数
         * @brief 根据欧拉角信息计算四元数，旋转顺序为 YZX，即先绕Y旋转，再绕Z，最后绕X旋转。
         * @param x 欧拉角X轴
         * @param y 欧拉角Y轴
         * @param z 欧拉角Z轴
         * @param order 旋转顺序，默认YZX
         */
        
        static void fromEuler(float x, float y, float z, RotationOrder order,Quat &out);
        
        /**
         * @en Sets a third order matrix with view direction and up direction. Then save the results to out matrix
         * @zh 根据视口前方向和上方向计算矩阵
         * @param view The view direction, it`s must be normalized.
         * @param up The view up direction, it`s must be normalized, default value is (0, 1, 0).
         * @param out Output quaternion
         */
        static void fromViewUp(const Vec3 &view, const Vec3 &up, Quat &out);
    };
} // namespace Boo
