#include "mat4.h"
#include <iostream>

Mat4::Mat4()
{
    /**
     * 初始化为单位矩阵
     */
    _m.fill(0.0f);
    _m[0] = _m[5] = _m[10] = _m[15] = 1.0f;
}
/**
 * @brief 单位矩阵
 *
 * @return Mat4
 */
Mat4 Mat4::identity()
{
    Mat4 mat;
    for (int i = 0; i < 4; ++i)
    {
        mat._m[i * 4 + i] = 1.0f;
    }
    return mat;
}
void Mat4::set(Mat4 &mat)
{
    for (int i = 0; i < 16; ++i)
    {
        _m[i] = mat._m[i]; // 直接访问私有成员
    }
}
void Mat4::translate(float x, float y, float z)
{
    this->_m[12] = x, this->_m[13] = y, this->_m[14] = z;
}
void Mat4::scale(float x, float y, float z)
{
    this->_m[0] = x, this->_m[5] = y, this->_m[10] = z;
}

/**
 * @brief 矩阵是否相等
 *
 * @param mat
 * @return true
 * @return false
 */
bool Mat4::equal(Mat4 &mat)
{
    for (int i = 0; i < 16; ++i)
    {
        if (this->_m[i] != mat._m[i])
        {
            return false;
        }
    }
    return true;
}
void Mat4::inverse(const Mat4 &mat, Mat4 &out)
{
    float m00 = mat.getM00(), m01 = mat.getM01(), m02 = mat.getM02(), m03 = mat.getM03();
    float m10 = mat.getM10(), m11 = mat.getM11(), m12 = mat.getM12(), m13 = mat.getM13();
    float m20 = mat.getM20(), m21 = mat.getM21(), m22 = mat.getM22(), m23 = mat.getM23();
    float m30 = mat.getM30(), m31 = mat.getM31(), m32 = mat.getM32(), m33 = mat.getM33();

    /*
     *计算辅助值（优化的行列式计算）
     */
    float a0 = m00 * m11 - m01 * m10;
    float a1 = m00 * m12 - m02 * m10;
    float a2 = m00 * m13 - m03 * m10;
    float a3 = m01 * m12 - m02 * m11;
    float a4 = m01 * m13 - m03 * m11;
    float a5 = m02 * m13 - m03 * m12;
    float b0 = m20 * m31 - m21 * m30;
    float b1 = m20 * m32 - m22 * m30;
    float b2 = m20 * m33 - m23 * m30;
    float b3 = m21 * m32 - m22 * m31;
    float b4 = m21 * m33 - m23 * m31;
    float b5 = m22 * m33 - m23 * m32;

    /**
     * 计算行列式
     */
    float det = a0 * b5 - a1 * b4 + a2 * b3 + a3 * b2 - a4 * b1 + a5 * b0;

    // 检查行列式是否为0（矩阵不可逆）
    if (det == 0.0f)
    {
        /**
         * 返回单位矩阵
         */
        out = Mat4::identity();
        return;
    }

    float invDet = 1.0f / det;

    /**
     * 计算逆矩阵的每个元素（使用优化的计算顺序）
     */
    out.setM00((m11 * b5 - m12 * b4 + m13 * b3) * invDet);
    out.setM01((-m01 * b5 + m02 * b4 - m03 * b3) * invDet);
    out.setM02((m31 * a5 - m32 * a4 + m33 * a3) * invDet);
    out.setM03((-m21 * a5 + m22 * a4 - m23 * a3) * invDet);

    out.setM10((-m10 * b5 + m12 * b2 - m13 * b1) * invDet);
    out.setM11((m00 * b5 - m02 * b2 + m03 * b1) * invDet);
    out.setM12((-m30 * a5 + m32 * a2 - m33 * a1) * invDet);
    out.setM13((m20 * a5 - m22 * a2 + m23 * a1) * invDet);

    out.setM20((m10 * b4 - m11 * b2 + m13 * b0) * invDet);
    out.setM21((-m00 * b4 + m01 * b2 - m03 * b0) * invDet);
    out.setM22((m30 * a4 - m31 * a2 + m33 * a0) * invDet);
    out.setM23((-m20 * a4 + m21 * a2 - m23 * a0) * invDet);

    out.setM30((-m10 * b3 + m11 * b1 - m12 * b0) * invDet);
    out.setM31((m00 * b3 - m01 * b1 + m02 * b0) * invDet);
    out.setM32((-m30 * a3 + m31 * a1 - m32 * a0) * invDet);
    out.setM33((m20 * a3 - m21 * a1 + m22 * a0) * invDet);
}
void Mat4::multiplyVec3(const Mat4 &mat, Vec3 &in, Vec3 &out)
{
    float x = mat.getM00() * in.getX() + mat.getM10() * in.getY() + mat.getM20() * in.getZ() + mat.getM30();
    float y = mat.getM01() * in.getX() + mat.getM11() * in.getY() + mat.getM21() * in.getZ() + mat.getM31();
    float z = mat.getM02() * in.getX() + mat.getM12() * in.getY() + mat.getM22() * in.getZ() + mat.getM32();
    out.setX(x);
    out.setY(y);
    out.setZ(z);
}
void Mat4::multiply(const Mat4 &mat,const  Mat4 &other, Mat4 &out)
{
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            out._m[i * 4 + j] = 0.0f;
            for (int k = 0; k < 4; ++k)
            {
                out._m[i * 4 + j] += mat._m[i * 4 + k] * other._m[k * 4 + j];
            }
        }
    }
}

Mat4::~Mat4()
{
}



// operator*：重载乘法运算符 *，使得 Mat4 对象可以直接用 * 相乘。
// const Mat4& other：参数是另一个矩阵的常量引用（避免拷贝，提高效率）。
// 末尾的 const：表示该成员函数不会修改当前对象（即 *this 是只读的）。
// Mat4 Mat4::operator*(const Mat4 &other) const
// {
//    // std::cout << "mat2  *********" << std::endl;
//    // 实现矩阵乘法
//    Mat4 result;
//    for (int i = 0; i < 4; ++i)
//    {
//        for (int j = 0; j < 4; ++j)
//        {
//            result._m[i * 4 + j] = 0;
//            for (int k = 0; k < 4; ++k)
//            {
//                result._m[i * 4 + j] += this->_m[i * 4 + k] * other._m[k * 4 + j];
//            }
//        }
//    }
//    // // 第一列
//    // result._m00 = this->_m00 * other._m00 + this->_m01 * other._m04 + this->_m02 * other._m08 + this->_m03 * other._m12;
//    // result._m01 = this->_m00 * other._m01 + this->_m01 * other._m05 + this->_m02 * other._m09 + this->_m03 * other._m13;
//    // result._m02 = this->_m00 * other._m02 + this->_m01 * other._m06 + this->_m02 * other._m10 + this->_m03 * other._m14;
//    // result._m03 = this->_m00 * other._m03 + this->_m01 * other._m07 + this->_m02 * other._m11 + this->_m03 * other._m15;

//    // // 第二列
//    // result._m04 = this->_m04 * other._m00 + this->_m05 * other._m04 + this->_m06 * other._m08 + this->_m07 * other._m12;
//    // result._m05 = this->_m04 * other._m01 + this->_m05 * other._m05 + this->_m06 * other._m09 + this->_m07 * other._m13;
//    // result._m06 = this->_m04 * other._m02 + this->_m05 * other._m06 + this->_m06 * other._m10 + this->_m07 * other._m14;
//    // result._m07 = this->_m04 * other._m03 + this->_m05 * other._m07 + this->_m06 * other._m11 + this->_m07 * other._m15;

//    // // 第三列
//    // result._m08 = this->_m08 * other._m00 + this->_m09 * other._m04 + this->_m10 * other._m08 + this->_m11 * other._m12;
//    // result._m09 = this->_m08 * other._m01 + this->_m09 * other._m05 + this->_m10 * other._m09 + this->_m11 * other._m13;
//    // result._m10 = this->_m08 * other._m02 + this->_m09 * other._m06 + this->_m10 * other._m10 + this->_m11 * other._m14;
//    // result._m11 = this->_m08 * other._m03 + this->_m09 * other._m07 + this->_m10 * other._m11 + this->_m11 * other._m15;

//    // // 第四列
//    // result._m12 = this->_m12 * other._m00 + this->_m13 * other._m04 + this->_m14 * other._m08 + this->_m15 * other._m12;
//    // // std::cout << this->_m12 << "  " << other._m00 << "  " << this->_m13 << "  " << other._m04 << "  " << this->_m14 << "  " << other._m08 << "  " << this->_m15 << "  " << other._m12 << std::endl;

//    // result._m13 = this->_m12 * other._m01 + this->_m13 * other._m05 + this->_m14 * other._m09 + this->_m15 * other._m13;
//    // result._m14 = this->_m12 * other._m02 + this->_m13 * other._m06 + this->_m14 * other._m10 + this->_m15 * other._m14;
//    // result._m15 = this->_m12 * other._m03 + this->_m13 * other._m07 + this->_m14 * other._m11 + this->_m15 * other._m15;
//    return result;
// }

// 矩阵乘法
// Mat4 result;
// for (int i = 0; i < 4; ++i) {
//     for (int j = 0; j < 4; ++j) {
//         result.m[i][j] = 0.0f;
//         for (int k = 0; k < 4; ++k) {
//             result.m[i][j] += m[i][k] * other.m[k][j];
//         }
//     }
// }
// return result;