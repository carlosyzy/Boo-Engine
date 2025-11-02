#pragma once
#include <array>
#include "vec3.h"


class Mat4
{
private:
   /**
    * | m[0][0] m[0][1] m[0][2] m[0][3] |   | 旋转/缩放     | 透视 |
    * | m[1][0] m[1][1] m[1][2] m[1][3] | = | 旋转/缩放     | 透视 |
    * | m[2][0] m[2][1] m[2][2] m[2][3] |   | 旋转/缩放     | 透视 |
    * | m[3][0] m[3][1] m[3][2] m[3][3] |   | 位移 (X,Y,Z) | 1    |
    */
   std::array<float, 16> _m = {
       1, 0, 0, 0,
       0, 1, 0, 0,
       0, 0, 1, 0,
       0, 0, 0, 1};

public:
    const std::array<float, 16> &data() const { return this->_m; }

   const float &getM00() const { return this->_m[0]; }
   const float &getM01() const { return this->_m[1]; }
   const float &getM02() const { return this->_m[2]; }
   const float &getM03() const { return this->_m[3]; }
   const float &getM10() const { return this->_m[4]; }
   const float &getM11() const { return this->_m[5]; }
   const float &getM12() const { return this->_m[6]; }
   const float &getM13() const { return this->_m[7]; }
   const float &getM20() const { return this->_m[8]; }
   const float &getM21() const { return this->_m[9]; }
   const float &getM22() const { return this->_m[10]; }
   const float &getM23() const { return this->_m[11]; }
   const float &getM30() const { return this->_m[12]; }
   const float &getM31() const { return this->_m[13]; }
   const float &getM32() const { return this->_m[14]; }
   const float &getM33() const { return this->_m[15]; }
   
   
   void setM00(float value) { this->_m[0] = value; }
   void setM01(float value) { this->_m[1] = value; }
   void setM02(float value) { this->_m[2] = value; }
   void setM03(float value) { this->_m[3] = value; }
   void setM10(float value) { this->_m[4] = value; }
   void setM11(float value) { this->_m[5] = value; }
   void setM12(float value) { this->_m[6] = value; }
   void setM13(float value) { this->_m[7] = value; }
   void setM20(float value) { this->_m[8] = value; }
   void setM21(float value) { this->_m[9] = value; }
   void setM22(float value) { this->_m[10] = value; }
   void setM23(float value) { this->_m[11] = value; }
   void setM30(float value) { this->_m[12] = value; }
   void setM31(float value) { this->_m[13] = value; }
   void setM32(float value) { this->_m[14] = value; }
   void setM33(float value) { this->_m[15] = value; }
   

   Mat4();
   void set(Mat4 &mat);
   /**
    * @brief 平移
    * @param x
    * @param y
    * @param z
    */
   void translate(float x, float y, float z);
   /**
    * @brief 缩放
    * @param x
    * @param y
    * @param z
    */
   void scale(float x, float y, float z);
   bool equal(Mat4 &mat);


   /**
    * @brief 单位矩阵
    *
    * @return Mat4
    */
   static Mat4 identity();
   static void inverse(const Mat4 &mat, Mat4 &out);
   static void multiplyVec3(const Mat4 &mat, Vec3 &in, Vec3 &out);
   static void multiply(const Mat4 &mat, const Mat4 &other, Mat4 &out);

   ~Mat4();
};
