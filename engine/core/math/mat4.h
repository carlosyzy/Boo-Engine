//#pragma once
//#include <array>
//#include "vec3.h"
//
//
//class Mat4
//{
//private:
//    // | m[0][0] m[0][1] m[0][2] m[0][3] |   | 旋转/缩放     | 透视 |
//    // | m[1][0] m[1][1] m[1][2] m[1][3] | = | 旋转/缩放     | 透视 |
//    // | m[2][0] m[2][1] m[2][2] m[2][3] |   | 旋转/缩放     | 透视 |
//    // | m[3][0] m[3][1] m[3][2] m[3][3] |   | 位移 (X,Y,Z) | 1    |
//    std::array<float, 16> _m = {
//        1, 0, 0, 0,
//        0, 1, 0, 0,
//        0, 0, 1, 0,
//        0, 0, 0, 1};
//
//public:
//    const float &m00() const { return this->_m[0]; }
//    const float &m01() const { return this->_m[1]; }
//    const float &m02() const { return this->_m[2]; }
//    const float &m03() const { return this->_m[3]; }
//    const float &m04() const { return this->_m[4]; }
//    const float &m05() const { return this->_m[5]; }
//    const float &m06() const { return this->_m[6]; }
//    const float &m07() const { return this->_m[7]; }
//    const float &m08() const { return this->_m[8]; }
//    const float &m09() const { return this->_m[9]; }
//    const float &m10() const { return this->_m[10]; }
//    const float &m11() const { return this->_m[11]; }
//    const float &m12() const { return this->_m[12]; }
//    const float &m13() const { return this->_m[13]; }
//    const float &m14() const { return this->_m[14]; }
//    const float &m15() const { return this->_m[15]; }
//    const std::array<float, 16> &data() const { return this->_m; }
//    
//    void setM00(float value) { this->_m[0] = value; }
//    void setM01(float value) { this->_m[1] = value; }
//    void setM02(float value) { this->_m[2] = value; }
//    void setM03(float value) { this->_m[3] = value; }
//    void setM04(float value) { this->_m[4] = value; }
//    void setM05(float value) { this->_m[5] = value; }
//    void setM06(float value) { this->_m[6] = value; }
//    void setM07(float value) { this->_m[7] = value; }
//    void setM08(float value) { this->_m[8] = value; }
//    void setM09(float value) { this->_m[9] = value; }
//    void setM10(float value) { this->_m[10] = value; }
//    void setM11(float value) { this->_m[11] = value; }
//    void setM12(float value) { this->_m[12] = value; }
//    void setM13(float value) { this->_m[13] = value; }
//    void setM14(float value) { this->_m[14] = value; }
//    void setM15(float value) { this->_m[15] = value; }
//
//    Mat4();
//    void set(Mat4 &mat);
//    /**
//     * @brief 平移
//     * @param x
//     * @param y
//     * @param z
//     */
//    void translate(float x, float y, float z);
//    /**
//     * @brief 缩放
//     * @param x
//     * @param y
//     * @param z
//     */
//    void scale(float x, float y, float z);
//    Mat4 operator*(const Mat4 &other) const;
//    bool equal(Mat4 &mat);
//
//
//    /**
//     * @brief 单位矩阵
//     *
//     * @return Mat4
//     */
//    static Mat4 identity();
//    static void inverse(Mat4 &mat, Mat4 &out);
//    static void multiplyVec3(Mat4 &mat, Vec3 &in, Vec3 &out);
//
//    ~Mat4();
//};
