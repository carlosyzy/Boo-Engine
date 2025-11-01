
#pragma once
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
    float getX();
    float getY();
    float getZ();
    float len();
    void add(Vec3 &v);
    void subtract(Vec3 &v);
    void rotate(Quat *q);
    void test();

    static void add(Vec3 *out, Vec3 *a, Vec3 *b);
    static void subtract(Vec3 *out, Vec3 *a, Vec3 *b);
    /**
     * 析构函数 用于销毁释放内存
     */
    ~Vec3();
};
