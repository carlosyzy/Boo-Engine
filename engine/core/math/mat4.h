#pragma once
#include <array>
#include "vec3.h"
#include "quat.h"

namespace Boo
{

    class Mat4
    {
    private:
        std::array<float, 16> _m = {
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1};

    public:
        const std::array<float, 16> &data() const { return this->_m; }

        float getM0() const { return _m[0]; }
        float getM1() const { return _m[1]; }
        float getM2() const { return _m[2]; }
        float getM3() const { return _m[3]; }
        float getM4() const { return _m[4]; }
        float getM5() const { return _m[5]; }
        float getM6() const { return _m[6]; }
        float getM7() const { return _m[7]; }
        float getM8() const { return _m[8]; }
        float getM9() const { return _m[9]; }
        float getM10() const { return _m[10]; }
        float getM11() const { return _m[11]; }
        float getM12() const { return _m[12]; }
        float getM13() const { return _m[13]; }
        float getM14() const { return _m[14]; }
        float getM15() const { return _m[15]; }

        void setM0(float v) { _m[0] = v; }
        void setM1(float v) { _m[1] = v; }
        void setM2(float v) { _m[2] = v; }
        void setM3(float v) { _m[3] = v; }
        void setM4(float v) { _m[4] = v; }
        void setM5(float v) { _m[5] = v; }
        void setM6(float v) { _m[6] = v; }
        void setM7(float v) { _m[7] = v; }
        void setM8(float v) { _m[8] = v; }
        void setM9(float v) { _m[9] = v; }
        void setM10(float v) { _m[10] = v; }
        void setM11(float v) { _m[11] = v; }
        void setM12(float v) { _m[12] = v; }
        void setM13(float v) { _m[13] = v; }
        void setM14(float v) { _m[14] = v; }
        void setM15(float v) { _m[15] = v; }

    public:
        Mat4();
        void set(Mat4 &mat);
        void set(const std::array<float, 16> &data);
        void fromTRS(Vec3 &pos, Quat &quat, Vec3 &scale);
        bool equal(Mat4 &mat);
        void scale(const Vec3 &scale);
        ~Mat4();

    public:
        static Mat4 identity();
        static void copy(const Mat4 &mat, Mat4 &out);
        /**
         * @brief 交换矩阵的行和列
         *
         * @param mat
         * @param out
         */
        static void transpose(const Mat4 &mat, Mat4 &out);
        /**
         * @brief 计算矩阵的行列式
         *
         * @param mat
         * @return float
         */
        static float determinant(const Mat4 &mat);
        /**
         * @brief 计算矩阵的逆矩阵
         *
         * @param mat
         * @param out
         */
        static void inverse(const Mat4 &mat, Mat4 &out);
        /**
         * @brief 计算矩阵的逆矩阵的转置
         *
         * @param mat
         * @param out
         */
        static void multiplyVec3(const Mat4 &mat, Vec3 &in, Vec3 &out);
        /**
         * @brief 计算矩阵的乘积
         *
         * @param mat
         * @param other
         * @param out
         */
        static void multiply(const Mat4 &mat, const Mat4 &other, Mat4 &out);
        /**
         * @brief 计算矩阵的逆矩阵的转置
         *
         * @param mat
         * @param out
         */
        static void inverseTranspose(const Mat4 &mat, Mat4 &out);
        /**
         * @brief 从矩阵中提取位置
         *
         * @param mat
         * @param out
         */
        static void getPosition(const Mat4 &mat, Vec3 &out);
        /**
         * @brief 从矩阵中提取旋转
         *
         * @param mat
         * @param out
         */
        static void getRotation(const Mat4 &mat, Quat &out);
        /**
         * @brief 从矩阵中提取缩放
         *
         * @param mat
         * @param out
         */
        static void getScale(const Mat4 &mat, Vec3 &out);

        /**
         * @brief 从矩阵中提取位移、旋转、缩放 (SRT)
         * 假设矩阵以 Scale -> Rotation -> Translation 顺序构建
         *
         * @param mat 输入矩阵
         * @param outPos 输出的位移 (可为nullptr)
         * @param outQuat 输出的旋转四元数 (可为nullptr)
         * @param outScale 输出的缩放 (可为nullptr)
         */
        static void toSRT(const Mat4 &mat, Vec3 *outPos, Quat *outQuat, Vec3 *outScale);

        static void ortho(Mat4 &out, float left, float right, float bottom, float top, float near, float far, float projectionSignY, int orientation); // float minClipZ,
        static void perspective(Mat4 &out, float fov, float aspect, float near, float far, float projectionSignY, int orientation);                    // bool isFOVY, float minClipZ,
        /**
         * @en Rotates the transform by the given angle and save the results into the out matrix, the rotate is applied before
         * the matrix, i.e. (out = a * R)
         * @zh 在给定矩阵变换基础上加入新旋转变换, 并将结果保存到 out 矩阵中，旋转变换将应用在矩阵变换之前，即 (out = a * R)
         * @param a Input matrix
         * @param rad Angle of rotation (in radians)
         * @param axis axis of rotation
         * @param out Output matrix
         * @return true if rotation is successful, false if axis length is too small
         */
        static bool rotate(const Mat4 &a, float rad, const Vec3 &axis, Mat4 &out);
        
    };

} // namespace Boo