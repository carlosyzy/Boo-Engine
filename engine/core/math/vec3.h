
#pragma once

namespace Boo
{

    class Quat;

    class Vec3
    {
    private:
        float _x;
        float _y;
        float _z;

    public:
        /**
         * 构造函数
         */
        Vec3();
        Vec3(float x, float y, float z);
        void set(float x, float y, float z);
        void set(Vec3 &v);
        void setX(float x);
        void setY(float y);
        void setZ(float z);
        void min(Vec3 &v);
        void max(Vec3 &v);
        float getX() const;
        float getY() const;
        float getZ() const;
        float len() const;
        void add(Vec3 &v);
        void subtract(Vec3 &v);
        void rotate(Quat &q);
        void normalize();
        /**
         * 析构函数 用于销毁释放内存
         */
        ~Vec3();

    public:
        static void add(Vec3 &a, Vec3 &b, Vec3 &out);
        static void subtract(const Vec3 &a, const Vec3 &b, Vec3 &out);
        static float distance(const Vec3 &a, const Vec3 &b);
        static float dot(const Vec3 &a, const Vec3 &b);
        /**
         * 计算向量长度的平方
         */
        static float lengthSqr(const Vec3 &v);
        /**
         * 计算两个向量的叉乘
         */
        static void cross(const Vec3 &a, const Vec3 &b, Vec3 &out);
        /**
         * 转换四元数
         */
        static void transformQuat(Vec3 &v, Quat &q, Vec3 &out);
        /**
         * 归一化
         */
        static void normalize(const Vec3 &v, Vec3 &out);
    };

} // namespace Boo
