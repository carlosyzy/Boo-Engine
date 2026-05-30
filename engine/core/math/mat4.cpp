#include "mat4.h"
#include <cmath>
#include <iostream>

namespace Boo
{

    Mat4::Mat4()
    {
        _m.fill(0.0f);
        _m[0] = _m[5] = _m[10] = _m[15] = 1.0f;
    }

    void Mat4::set(Mat4 &mat)
    {
        for (int i = 0; i < 16; ++i)
        {
            _m[i] = mat._m[i];
        }
    }

    void Mat4::set(const std::array<float, 16> &data)
    {
        this->_m = data;
    }

    void Mat4::fromTRS(Vec3 &pos, Quat &quat, Vec3 &scale)
    {
        float x = quat.getX();
        float y = quat.getY();
        float z = quat.getZ();
        float w = quat.getW();

        float x2 = x + x;
        float y2 = y + y;
        float z2 = z + z;

        float xx = x * x2;
        float xy = x * y2;
        float xz = x * z2;

        float yy = y * y2;
        float yz = y * z2;
        float zz = z * z2;

        float wx = w * x2;
        float wy = w * y2;
        float wz = w * z2;

        // 列主序：m[col * 4 + row]
        // 列0 (X轴方向)
        float r00 = 1 - (yy + zz);
        float r10 = xy + wz;
        float r20 = xz - wy;

        // 列1 (Y轴方向)
        float r01 = xy - wz;
        float r11 = 1 - (xx + zz);
        float r21 = yz + wx;

        // 列2 (Z轴方向)
        float r02 = xz + wy;
        float r12 = yz - wx;
        float r22 = 1 - (xx + yy);

        // 应用缩放
        _m[0] = r00 * scale.getX();
        _m[1] = r10 * scale.getX();
        _m[2] = r20 * scale.getX();
        _m[3] = 0;

        _m[4] = r01 * scale.getY();
        _m[5] = r11 * scale.getY();
        _m[6] = r21 * scale.getY();
        _m[7] = 0;

        _m[8] = r02 * scale.getZ();
        _m[9] = r12 * scale.getZ();
        _m[10] = r22 * scale.getZ();
        _m[11] = 0;

        // 位移
        _m[12] = pos.getX();
        _m[13] = pos.getY();
        _m[14] = pos.getZ();
        _m[15] = 1.0f;
    }

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

    void Mat4::scale(const Vec3 &scale)
    {
        float x = scale.getX();
        float y = scale.getY();
        float z = scale.getZ();
        _m[0] *= x;
        _m[1] *= x;
        _m[2] *= x;
        _m[3] *= x;
        _m[4] *= y;
        _m[5] *= y;
        _m[6] *= y;
        _m[7] *= y;
        _m[8] *= z;
        _m[9] *= z;
        _m[10] *= z;
        _m[11] *= z;
    }

    Mat4::~Mat4()
    {
    }

} // namespace Boo

namespace Boo
{
    Mat4 Mat4::identity()
    {
        Mat4 mat;
        for (int i = 0; i < 4; ++i)
        {
            mat._m[i * 5] = 1.0f;
        }
        return mat;
    }

    void Mat4::inverse(const Mat4 &mat, Mat4 &out)
    {
        float m00 = mat._m[0], m01 = mat._m[1], m02 = mat._m[2], m03 = mat._m[3];
        float m10 = mat._m[4], m11 = mat._m[5], m12 = mat._m[6], m13 = mat._m[7];
        float m20 = mat._m[8], m21 = mat._m[9], m22 = mat._m[10], m23 = mat._m[11];
        float m30 = mat._m[12], m31 = mat._m[13], m32 = mat._m[14], m33 = mat._m[15];

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

        float det = a0 * b5 - a1 * b4 + a2 * b3 + a3 * b2 - a4 * b1 + a5 * b0;

        const float EPSILON = 1e-8f;
        if (std::abs(det) < EPSILON)
        {
            out = Mat4::identity();
            return;
        }

        float invDet = 1.0f / det;

        out._m[0] = (m11 * b5 - m12 * b4 + m13 * b3) * invDet;
        out._m[1] = (m01 * b5 - m02 * b4 + m03 * b3) * -invDet;
        out._m[2] = (m31 * a5 - m32 * a4 + m33 * a3) * invDet;
        out._m[3] = (m21 * a5 - m22 * a4 + m23 * a3) * -invDet;

        out._m[4] = (m10 * b5 - m12 * b2 + m13 * b1) * -invDet;
        out._m[5] = (m00 * b5 - m02 * b2 + m03 * b1) * invDet;
        out._m[6] = (m30 * a5 - m32 * a2 + m33 * a1) * -invDet;
        out._m[7] = (m20 * a5 - m22 * a2 + m23 * a1) * invDet;

        out._m[8] = (m10 * b4 - m11 * b2 + m13 * b0) * invDet;
        out._m[9] = (m00 * b4 - m01 * b2 + m03 * b0) * -invDet;
        out._m[10] = (m30 * a4 - m31 * a2 + m33 * a0) * invDet;
        out._m[11] = (m20 * a4 - m21 * a2 + m23 * a0) * -invDet;

        out._m[12] = (m10 * b3 - m11 * b1 + m12 * b0) * -invDet;
        out._m[13] = (m00 * b3 - m01 * b1 + m02 * b0) * invDet;
        out._m[14] = (m30 * a3 - m31 * a1 + m32 * a0) * -invDet;
        out._m[15] = (m20 * a3 - m21 * a1 + m22 * a0) * invDet;
    }

    void Mat4::multiplyVec3(const Mat4 &mat, Vec3 &in, Vec3 &out)
    {
        float x = mat._m[0] * in.getX() + mat._m[4] * in.getY() + mat._m[8] * in.getZ() + mat._m[12];
        float y = mat._m[1] * in.getX() + mat._m[5] * in.getY() + mat._m[9] * in.getZ() + mat._m[13];
        float z = mat._m[2] * in.getX() + mat._m[6] * in.getY() + mat._m[10] * in.getZ() + mat._m[14];
        out.setX(x);
        out.setY(y);
        out.setZ(z);
    }

    void Mat4::multiply(const Mat4 &mat, const Mat4 &other, Mat4 &out)
    {
        for (int col = 0; col < 4; ++col)
        {
            for (int row = 0; row < 4; ++row)
            {
                float sum = 0.0f;
                for (int k = 0; k < 4; ++k)
                {
                    sum += mat._m[col * 4 + k] * other._m[k * 4 + row];
                }
                out._m[col * 4 + row] = sum;
            }
        }
    }
    void Mat4::copy(const Mat4 &mat, Mat4 &out)
    {
        for (int i = 0; i < 16; ++i)
        {
            out._m[i] = mat._m[i];
        }
    }
    void Mat4::inverseTranspose(const Mat4 &mat, Mat4 &out)
    {
        Mat4 inv;
        inverse(mat, inv);

        // 转置: out[row][col] = inv[col][row]
        // 列主序: out[col*4+row] = inv[row*4+col]
        out._m[0] = inv._m[0];
        out._m[1] = inv._m[4];
        out._m[2] = inv._m[8];
        out._m[3] = 0;

        out._m[4] = inv._m[1];
        out._m[5] = inv._m[5];
        out._m[6] = inv._m[9];
        out._m[7] = 0;

        out._m[8] = inv._m[2];
        out._m[9] = inv._m[6];
        out._m[10] = inv._m[10];
        out._m[11] = 0;

        out._m[12] = 0;
        out._m[13] = 0;
        out._m[14] = 0;
        out._m[15] = 1;
    }
    void Mat4::transpose(const Mat4 &mat, Mat4 &out)
    {
        if (&out == &mat)
        {
            float a01 = mat._m[1];
            float a02 = mat._m[2];
            float a03 = mat._m[3];
            float a12 = mat._m[6];
            float a13 = mat._m[7];
            float a23 = mat._m[11];

            out._m[1] = mat._m[4];
            out._m[2] = mat._m[8];
            out._m[3] = mat._m[12];
            out._m[4] = a01;
            out._m[6] = mat._m[9];
            out._m[7] = mat._m[13];
            out._m[8] = a02;
            out._m[9] = a12;
            out._m[11] = mat._m[14];
            out._m[12] = a03;
            out._m[13] = a13;
            out._m[14] = a23;
        }
        else
        {
            out._m[0] = mat._m[0];
            out._m[1] = mat._m[4];
            out._m[2] = mat._m[8];
            out._m[3] = mat._m[12];
            out._m[4] = mat._m[1];
            out._m[5] = mat._m[5];
            out._m[6] = mat._m[9];
            out._m[7] = mat._m[13];
            out._m[8] = mat._m[2];
            out._m[9] = mat._m[6];
            out._m[10] = mat._m[10];
            out._m[11] = mat._m[14];
            out._m[12] = mat._m[3];
            out._m[13] = mat._m[7];
            out._m[14] = mat._m[11];
            out._m[15] = mat._m[15];
        }
    }

    float Mat4::determinant(const Mat4 &mat)
    {
        float m00 = mat._m[0], m01 = mat._m[1], m02 = mat._m[2], m03 = mat._m[3];
        float m10 = mat._m[4], m11 = mat._m[5], m12 = mat._m[6], m13 = mat._m[7];
        float m20 = mat._m[8], m21 = mat._m[9], m22 = mat._m[10], m23 = mat._m[11];
        float m30 = mat._m[12], m31 = mat._m[13], m32 = mat._m[14], m33 = mat._m[15];

        float b00 = m00 * m11 - m01 * m10;
        float b01 = m00 * m12 - m02 * m10;
        float b02 = m00 * m13 - m03 * m10;
        float b03 = m01 * m12 - m02 * m11;
        float b04 = m01 * m13 - m03 * m11;
        float b05 = m02 * m13 - m03 * m12;
        float b06 = m20 * m31 - m21 * m30;
        float b07 = m20 * m32 - m22 * m30;
        float b08 = m20 * m33 - m23 * m30;
        float b09 = m21 * m32 - m22 * m31;
        float b10 = m21 * m33 - m23 * m31;
        float b11 = m22 * m33 - m23 * m32;

        return b00 * b11 - b01 * b10 + b02 * b09 + b03 * b08 - b04 * b07 + b05 * b06;
    }

    void Mat4::getPosition(const Mat4 &mat, Vec3 &out)
    {
        out.setX(mat._m[12]);
        out.setY(mat._m[13]);
        out.setZ(mat._m[14]);
    }

    void Mat4::getRotation(const Mat4 &mat, Quat &out)
    {
        double trace = mat._m[0] + mat._m[5] + mat._m[10];
        if (trace > 0)
        {
            double s = std::sqrt(trace + 1.0) * 2.0;
            out.setW(0.25 * s);
            out.setX((mat._m[9] - mat._m[6]) / s);
            out.setY((mat._m[2] - mat._m[8]) / s);
            out.setZ((mat._m[4] - mat._m[1]) / s);
        }
        else if (mat._m[0] > mat._m[5] && mat._m[0] > mat._m[10])
        {
            double s = std::sqrt(1.0 + mat._m[0] - mat._m[5] - mat._m[10]) * 2.0;
            out.setW((mat._m[9] - mat._m[6]) / s);
            out.setX(0.25 * s);
            out.setY((mat._m[1] + mat._m[4]) / s);
            out.setZ((mat._m[2] + mat._m[8]) / s);
        }
        else if (mat._m[5] > mat._m[10])
        {
            double s = std::sqrt(1.0 + mat._m[5] - mat._m[0] - mat._m[10]) * 2.0;
            out.setW((mat._m[2] - mat._m[8]) / s);
            out.setX((mat._m[1] + mat._m[4]) / s);
            out.setY(0.25 * s);
            out.setZ((mat._m[6] + mat._m[9]) / s);
        }
        else
        {
            double s = std::sqrt(1.0 + mat._m[10] - mat._m[0] - mat._m[5]) * 2.0;
            out.setW((mat._m[4] - mat._m[1]) / s);
            out.setX((mat._m[2] + mat._m[8]) / s);
            out.setY((mat._m[6] + mat._m[9]) / s);
            out.setZ(0.25 * s);
        }
    }

    void Mat4::getScale(const Mat4 &mat, Vec3 &out)
    {
        float scaleX = std::sqrt(mat._m[0] * mat._m[0] +
                                 mat._m[1] * mat._m[1] +
                                 mat._m[2] * mat._m[2]);

        float scaleY = std::sqrt(mat._m[4] * mat._m[4] +
                                 mat._m[5] * mat._m[5] +
                                 mat._m[6] * mat._m[6]);

        float scaleZ = std::sqrt(mat._m[8] * mat._m[8] +
                                 mat._m[9] * mat._m[9] +
                                 mat._m[10] * mat._m[10]);

        float det = mat._m[0] * (mat._m[5] * mat._m[10] - mat._m[6] * mat._m[9]) +
                    mat._m[1] * (mat._m[6] * mat._m[8] - mat._m[4] * mat._m[10]) +
                    mat._m[2] * (mat._m[4] * mat._m[9] - mat._m[5] * mat._m[8]);

        if (det < 0)
        {
            scaleX *= -1;
        }

        out.setX(scaleX);
        out.setY(scaleY);
        out.setZ(scaleZ);
    }

    void Mat4::ortho(Mat4 &out, float left, float right, float bottom, float top, float near, float far, float projectionSignY, int orientation) // float minClipZ,
    {
        // OpenGL 向上 1   Vulkan 向下 -1  Metal 向下 -1  * projectionSignY
        float lr = 1.0f / (right - left);
        float bt = 1.0f / (top - bottom);
        float nf = 1.0f / (far - near);

        float x = 2.0f * lr;
        float y = 2.0f * bt;
        float dx = -(right + left) * lr;
        float dy = -(top + bottom) * bt;

        out._m[0] = x;
        out._m[1] = 0;
        out._m[2] = 0;
        out._m[3] = 0;
        out._m[4] = 0;
        out._m[5] = y * projectionSignY;
        out._m[6] = 0;
        out._m[7] = 0;
        out._m[8] = 0;
        out._m[9] = 0;
        out._m[10] = nf;
        out._m[11] = 0;
        out._m[12] = dx;
        out._m[13] = dy;
        out._m[14] = -near * nf;
        out._m[15] = 1;

        // float preTransform[4] = {1.0f, 0.0f, 0.0f, 1.0f};
        // 暂时不处理屏幕旋转
        //  orientation 含义 preTransform
        // 0 不旋转 (IDENTITY) [1, 0, 0, 1]
        // 1 旋转90° (ROTATE_90) [0, 1, -1, 0]
        // 2 旋转180° (ROTATE_180) [-1, 0, 0, -1]
        // 3 旋转270° (ROTATE_270) [0, -1, 1, 0]
        //  if (orientation == 0)
        //  {
        //      preTransform[0] = 1; preTransform[1] = 0;
        //      preTransform[2] = 0; preTransform[3] = 1;
        //  }
        //  else if (orientation == 1)
        //  {
        //      preTransform[0] = 0; preTransform[1] = 1;
        //      preTransform[2] = -1; preTransform[3] = 0;
        //  }
        //  else if (orientation == 2)
        //  {
        //      preTransform[0] = -1; preTransform[1] = 0;
        //      preTransform[2] = 0; preTransform[3] = -1;
        //  }
        //  else
        //  {
        //      preTransform[0] = 0; preTransform[1] = -1;
        //      preTransform[2] = 1; preTransform[3] = 0;
        //  }
        // out._m[0] = x * preTransform[0];
        // out._m[1] = x * preTransform[1];
        // out._m[2] = 0;
        // out._m[3] = 0;
        // out._m[4] = y * preTransform[2];
        // out._m[5] = y * preTransform[3];
        // out._m[6] = 0;
        // out._m[7] = 0;
        // out._m[8] = 0;
        // out._m[9] = 0;
        // out._m[10] = nf * (1.0f - minClipZ);
        // out._m[11] = 0;
        // out._m[12] = dx * preTransform[0] + dy * preTransform[2];
        // out._m[13] = dx * preTransform[1] + dy * preTransform[3];
        // out._m[14] = (near - minClipZ * far) * nf;
        // out._m[15] = 1;
    }

    void Mat4::perspective(Mat4 &out, float fov, float aspect, float near, float far, float projectionSignY, int orientation) // bool isFOVY, float minClipZ,
    {
        float tanHalfFov = tan(fov * 0.5f);
        // 计算矩阵元素
        float m00 = 1.0f / (aspect * tanHalfFov);
        float m11 = 1.0f / tanHalfFov;
        float m22 = far / (near - far);
        float m23 = -1.0f; // 注意：这是用于生成深度值
        float m32 = -(far * near) / (far - near);

        out._m[0] = m00;
        out._m[1] = 0.0f;
        out._m[2] = 0.0f;
        out._m[3] = 0.0f;
        out._m[4] = 0.0f;
        out._m[5] = m11 * projectionSignY;
        out._m[6] = 0;
        out._m[7] = 0;
        out._m[8] = 0;
        out._m[9] = 0;
        out._m[10] = m22;
        out._m[11] = m23;
        out._m[12] = 0.0f;
        out._m[13] = 0.0f;
        out._m[14] = m32;
        out._m[15] = 0.0f;

        // float f = 1.0f / std::tan(fov / 2.0f);
        // float nf = 1.0f / (near - far);

        // float preTransform[4] = {1.0f, 0.0f, 0.0f, 1.0f};
        // 暂时不处理屏幕旋转
        //  if (orientation == 0)
        //  {
        //      // 不旋转
        //      preTransform[0] = 1; preTransform[1] = 0;
        //      preTransform[0] = 1; preTransform[1] = 0;
        //      preTransform[2] = 0; preTransform[3] = 1;
        //  }
        //  else if (orientation == 1)
        //  {
        //      // 旋转90°
        //      preTransform[0] = 0; preTransform[1] = 1;
        //      preTransform[2] = -1; preTransform[3] = 0;
        //  }
        //  else if (orientation == 2)
        //  {
        //      // 旋转180°
        //      preTransform[0] = -1; preTransform[1] = 0;
        //      preTransform[2] = 0; preTransform[3] = -1;
        //  }
        //  else
        //  {
        //      // 旋转270°
        //      preTransform[0] = 0; preTransform[1] = -1;
        //      preTransform[2] = 1; preTransform[3] = 0;
        //  }

        // float x = isFOVY ? f / aspect : f;
        // float y = (isFOVY ? f : f * aspect) * projectionSignY;

        // out._m[0] = x * preTransform[0];
        // out._m[1] = x * preTransform[1];
        // out._m[2] = 0;
        // out._m[3] = 0;
        // out._m[4] = y * preTransform[2];
        // out._m[5] = y * preTransform[3];
        // out._m[6] = 0;
        // out._m[7] = 0;
        // out._m[8] = 0;
        // out._m[9] = 0;
        // out._m[10] = (far - minClipZ * near) * nf;
        // out._m[11] = -1;
        // out._m[12] = 0;
        // out._m[13] = 0;
        // out._m[14] = far * near * nf * (1.0f - minClipZ);
        // out._m[15] = 0;
    }
    void Mat4::toSRT(const Mat4 &mat, Vec3 *outPos, Quat *outQuat, Vec3 *outScale)
    {
        // 提取位移 (列主序: m[col*4+row])
        if (outPos)
        {
            outPos->setX(mat._m[12]);
            outPos->setY(mat._m[13]);
            outPos->setZ(mat._m[14]);
        }

        // 提取缩放 - 计算每列向量的长度
        float sx = std::sqrt(mat._m[0] * mat._m[0] + mat._m[1] * mat._m[1] + mat._m[2] * mat._m[2]);
        float sy = std::sqrt(mat._m[4] * mat._m[4] + mat._m[5] * mat._m[5] + mat._m[6] * mat._m[6]);
        float sz = std::sqrt(mat._m[8] * mat._m[8] + mat._m[9] * mat._m[9] + mat._m[10] * mat._m[10]);

        if (outScale)
        {
            outScale->setX(sx);
            outScale->setY(sy);
            outScale->setZ(sz);
        }

        // 缩放太接近0，无法分解旋转
        if (sx < 0.00001f || sy < 0.00001f || sz < 0.00001f)
        {
            if (outQuat)
            {
                outQuat->set(0, 0, 0, 1); // identity quaternion
            }
            return;
        }

        // 构建3x3旋转矩阵 (去除缩放)
        // 列主序: m3[col*3+row]
        float m3[9];
        m3[0] = mat._m[0] / sx;  // m00
        m3[1] = mat._m[1] / sx;  // m01
        m3[2] = mat._m[2] / sx;  // m02
        m3[3] = mat._m[4] / sy;  // m10
        m3[4] = mat._m[5] / sy;  // m11
        m3[5] = mat._m[6] / sy;  // m12
        m3[6] = mat._m[8] / sz;  // m20
        m3[7] = mat._m[9] / sz;  // m21
        m3[8] = mat._m[10] / sz; // m22

        // 计算3x3矩阵的行列式
        float det = m3[0] * (m3[4] * m3[8] - m3[5] * m3[7]) -
                    m3[1] * (m3[3] * m3[8] - m3[5] * m3[6]) +
                    m3[2] * (m3[3] * m3[7] - m3[4] * m3[6]);

        // 如果行列式为负，反转X轴缩放
        if (det < 0)
        {
            if (outScale)
            {
                outScale->setX(-outScale->getX());
            }
            m3[0] *= -1;
            m3[1] *= -1;
            m3[2] *= -1;
        }

        // 从3x3旋转矩阵提取四元数
        if (outQuat)
        {
            Quat::fromMat3(m3, *outQuat);
        }
    }

    bool Mat4::rotate(const Mat4 &a, float rad, const Vec3 &axis, Mat4 &out)
    {
        float x = axis.getX();
        float y = axis.getY();
        float z = axis.getZ();

        float len = sqrt(x * x + y * y + z * z);

        const float EPSILON = 1e-6f;
        if (fabs(len) < EPSILON)
        {
            return false;
        }

        len = 1.0f / len;
        x *= len;
        y *= len;
        z *= len;

        // ref: `https://en.wikipedia.org/wiki/Rotation_matrix#Axis_and_angle`

        float s = sin(rad);
        float c = cos(rad);
        float t = 1 - c;

        float a00 = a._m[0];
        float a01 = a._m[1];
        float a02 = a._m[2];
        float a03 = a._m[3];
        float a10 = a._m[4];
        float a11 = a._m[5];
        float a12 = a._m[6];
        float a13 = a._m[7];
        float a20 = a._m[8];
        float a21 = a._m[9];
        float a22 = a._m[10];
        float a23 = a._m[11];

        // Construct the elements of the rotation matrix
        float b00 = x * x * t + c;
        float b01 = y * x * t + z * s;
        float b02 = z * x * t - y * s;
        float b10 = x * y * t - z * s;
        float b11 = y * y * t + c;
        float b12 = z * y * t + x * s;
        float b20 = x * z * t + y * s;
        float b21 = y * z * t - x * s;
        float b22 = z * z * t + c;

        // Perform rotation-specific matrix multiplication
        out._m[0] = a00 * b00 + a10 * b01 + a20 * b02;
        out._m[1] = a01 * b00 + a11 * b01 + a21 * b02;
        out._m[2] = a02 * b00 + a12 * b01 + a22 * b02;
        out._m[3] = a03 * b00 + a13 * b01 + a23 * b02;
        out._m[4] = a00 * b10 + a10 * b11 + a20 * b12;
        out._m[5] = a01 * b10 + a11 * b11 + a21 * b12;
        out._m[6] = a02 * b10 + a12 * b11 + a22 * b12;
        out._m[7] = a03 * b10 + a13 * b11 + a23 * b12;
        out._m[8] = a00 * b20 + a10 * b21 + a20 * b22;
        out._m[9] = a01 * b20 + a11 * b21 + a21 * b22;
        out._m[10] = a02 * b20 + a12 * b21 + a22 * b22;
        out._m[11] = a03 * b20 + a13 * b21 + a23 * b22;

        // If the source and destination differ, copy the unchanged last row
        if (&a != &out)
        {
            out._m[12] = a._m[12];
            out._m[13] = a._m[13];
            out._m[14] = a._m[14];
            out._m[15] = a._m[15];
        }

        return true;
    }

} // namespace Boo

// namespace Boo
// {

//     // void Mat4::translate(Mat4 &out, const Mat4 &mat, Vec3 &v)
//     // {
//     //     float x = v.getX();
//     //     float y = v.getY();
//     //     float z = v.getZ();

//     //     if (&out == &mat)
//     //     {
//     //         out._m[12] = mat._m[0] * x + mat._m[4] * y + mat._m[8] * z + mat._m[12];
//     //         out._m[13] = mat._m[1] * x + mat._m[5] * y + mat._m[9] * z + mat._m[13];
//     //         out._m[14] = mat._m[2] * x + mat._m[6] * y + mat._m[10] * z + mat._m[14];
//     //         out._m[15] = mat._m[3] * x + mat._m[7] * y + mat._m[11] * z + mat._m[15];
//     //     }
//     //     else
//     //     {
//     //         out._m[0] = mat._m[0];
//     //         out._m[1] = mat._m[1];
//     //         out._m[2] = mat._m[2];
//     //         out._m[3] = mat._m[3];
//     //         out._m[4] = mat._m[4];
//     //         out._m[5] = mat._m[5];
//     //         out._m[6] = mat._m[6];
//     //         out._m[7] = mat._m[7];
//     //         out._m[8] = mat._m[8];
//     //         out._m[9] = mat._m[9];
//     //         out._m[10] = mat._m[10];
//     //         out._m[11] = mat._m[11];
//     //         out._m[12] = mat._m[0] * x + mat._m[4] * y + mat._m[8] * z + mat._m[12];
//     //         out._m[13] = mat._m[1] * x + mat._m[5] * y + mat._m[9] * z + mat._m[13];
//     //         out._m[14] = mat._m[2] * x + mat._m[6] * y + mat._m[10] * z + mat._m[14];
//     //         out._m[15] = mat._m[3] * x + mat._m[7] * y + mat._m[11] * z + mat._m[15];
//     //     }
//     // }

//     // void Mat4::scale(Mat4 &out, const Mat4 &mat, Vec3 &v)
//     // {
//     //     float x = v.getX();
//     //     float y = v.getY();
//     //     float z = v.getZ();

//     //     out._m[0] = mat._m[0] * x;
//     //     out._m[1] = mat._m[1] * x;
//     //     out._m[2] = mat._m[2] * x;
//     //     out._m[3] = mat._m[3] * x;
//     //     out._m[4] = mat._m[4] * y;
//     //     out._m[5] = mat._m[5] * y;
//     //     out._m[6] = mat._m[6] * y;
//     //     out._m[7] = mat._m[7] * y;
//     //     out._m[8] = mat._m[8] * z;
//     //     out._m[9] = mat._m[9] * z;
//     //     out._m[10] = mat._m[10] * z;
//     //     out._m[11] = mat._m[11] * z;
//     //     out._m[12] = mat._m[12];
//     //     out._m[13] = mat._m[13];
//     //     out._m[14] = mat._m[14];
//     //     out._m[15] = mat._m[15];
//     // }

//     // void Mat4::rotateX(Mat4 &out, const Mat4 &mat, float rad)
//     // {
//     //     float s = std::sin(rad);
//     //     float c = std::cos(rad);
//     //     float a10 = mat._m[4];
//     //     float a11 = mat._m[5];
//     //     float a12 = mat._m[6];
//     //     float a13 = mat._m[7];
//     //     float a20 = mat._m[8];
//     //     float a21 = mat._m[9];
//     //     float a22 = mat._m[10];
//     //     float a23 = mat._m[11];

//     //     if (&out != &mat)
//     //     {
//     //         out._m[0] = mat._m[0];
//     //         out._m[1] = mat._m[1];
//     //         out._m[2] = mat._m[2];
//     //         out._m[3] = mat._m[3];
//     //         out._m[12] = mat._m[12];
//     //         out._m[13] = mat._m[13];
//     //         out._m[14] = mat._m[14];
//     //         out._m[15] = mat._m[15];
//     //     }

//     //     out._m[4] = a10 * c + a20 * s;
//     //     out._m[5] = a11 * c + a21 * s;
//     //     out._m[6] = a12 * c + a22 * s;
//     //     out._m[7] = a13 * c + a23 * s;
//     //     out._m[8] = a20 * c - a10 * s;
//     //     out._m[9] = a21 * c - a11 * s;
//     //     out._m[10] = a22 * c - a12 * s;
//     //     out._m[11] = a23 * c - a13 * s;
//     // }

//     // void Mat4::rotateY(Mat4 &out, const Mat4 &mat, float rad)
//     // {
//     //     float s = std::sin(rad);
//     //     float c = std::cos(rad);
//     //     float a00 = mat._m[0];
//     //     float a01 = mat._m[1];
//     //     float a02 = mat._m[2];
//     //     float a03 = mat._m[3];
//     //     float a20 = mat._m[8];
//     //     float a21 = mat._m[9];
//     //     float a22 = mat._m[10];
//     //     float a23 = mat._m[11];

//     //     if (&out != &mat)
//     //     {
//     //         out._m[4] = mat._m[4];
//     //         out._m[5] = mat._m[5];
//     //         out._m[6] = mat._m[6];
//     //         out._m[7] = mat._m[7];
//     //         out._m[12] = mat._m[12];
//     //         out._m[13] = mat._m[13];
//     //         out._m[14] = mat._m[14];
//     //         out._m[15] = mat._m[15];
//     //     }

//     //     out._m[0] = a00 * c - a20 * s;
//     //     out._m[1] = a01 * c - a21 * s;
//     //     out._m[2] = a02 * c - a22 * s;
//     //     out._m[3] = a03 * c - a23 * s;
//     //     out._m[8] = a00 * s + a20 * c;
//     //     out._m[9] = a01 * s + a21 * c;
//     //     out._m[10] = a02 * s + a22 * c;
//     //     out._m[11] = a03 * s + a23 * c;
//     // }

//     // void Mat4::rotateZ(Mat4 &out, const Mat4 &mat, float rad)
//     // {
//     //     float s = std::sin(rad);
//     //     float c = std::cos(rad);
//     //     float a00 = mat._m[0];
//     //     float a01 = mat._m[1];
//     //     float a02 = mat._m[2];
//     //     float a03 = mat._m[3];
//     //     float a10 = mat._m[4];
//     //     float a11 = mat._m[5];
//     //     float a12 = mat._m[6];
//     //     float a13 = mat._m[7];

//     //     if (&out != &mat)
//     //     {
//     //         out._m[4] = mat._m[4];
//     //         out._m[5] = mat._m[5];
//     //         out._m[6] = mat._m[6];
//     //         out._m[7] = mat._m[7];
//     //         out._m[8] = mat._m[8];
//     //         out._m[9] = mat._m[9];
//     //         out._m[10] = mat._m[10];
//     //         out._m[11] = mat._m[11];
//     //         out._m[12] = mat._m[12];
//     //         out._m[13] = mat._m[13];
//     //         out._m[14] = mat._m[14];
//     //         out._m[15] = mat._m[15];
//     //     }

//     //     out._m[0] = a00 * c + a10 * s;
//     //     out._m[1] = a01 * c + a11 * s;
//     //     out._m[2] = a02 * c + a12 * s;
//     //     out._m[3] = a03 * c + a13 * s;
//     //     out._m[4] = a10 * c - a00 * s;
//     //     out._m[5] = a11 * c - a01 * s;
//     //     out._m[6] = a12 * c - a02 * s;
//     //     out._m[7] = a13 * c - a03 * s;
//     // }

//     // void Mat4::rotate(Mat4 &out, const Mat4 &mat, float rad, Vec3 &axis)
//     // {
//     //     float x = axis.getX();
//     //     float y = axis.getY();
//     //     float z = axis.getZ();

//     //     float len = std::sqrt(x * x + y * y + z * z);

//     //     if (len < 0.00001f)
//     //     {
//     //         return;
//     //     }

//     //     len = 1.0f / len;
//     //     x *= len;
//     //     y *= len;
//     //     z *= len;

//     //     float s = std::sin(rad);
//     //     float c = std::cos(rad);
//     //     float t = 1 - c;

//     //     float a00 = mat._m[0];
//     //     float a01 = mat._m[1];
//     //     float a02 = mat._m[2];
//     //     float a03 = mat._m[3];
//     //     float a10 = mat._m[4];
//     //     float a11 = mat._m[5];
//     //     float a12 = mat._m[6];
//     //     float a13 = mat._m[7];
//     //     float a20 = mat._m[8];
//     //     float a21 = mat._m[9];
//     //     float a22 = mat._m[10];
//     //     float a23 = mat._m[11];

//     //     float b00 = x * x * t + c;
//     //     float b01 = y * x * t + z * s;
//     //     float b02 = z * x * t - y * s;
//     //     float b10 = x * y * t - z * s;
//     //     float b11 = y * y * t + c;
//     //     float b12 = z * y * t + x * s;
//     //     float b20 = x * z * t + y * s;
//     //     float b21 = y * z * t - x * s;
//     //     float b22 = z * z * t + c;

//     //     out._m[0] = a00 * b00 + a10 * b01 + a20 * b02;
//     //     out._m[1] = a01 * b00 + a11 * b01 + a21 * b02;
//     //     out._m[2] = a02 * b00 + a12 * b01 + a22 * b02;
//     //     out._m[3] = a03 * b00 + a13 * b01 + a23 * b02;
//     //     out._m[4] = a00 * b10 + a10 * b11 + a20 * b12;
//     //     out._m[5] = a01 * b10 + a11 * b11 + a21 * b12;
//     //     out._m[6] = a02 * b10 + a12 * b11 + a22 * b12;
//     //     out._m[7] = a03 * b10 + a13 * b11 + a23 * b12;
//     //     out._m[8] = a00 * b20 + a10 * b21 + a20 * b22;
//     //     out._m[9] = a01 * b20 + a11 * b21 + a21 * b22;
//     //     out._m[10] = a02 * b20 + a12 * b21 + a22 * b22;
//     //     out._m[11] = a03 * b20 + a13 * b21 + a23 * b22;

//     //     if (&out != &mat)
//     //     {
//     //         out._m[12] = mat._m[12];
//     //         out._m[13] = mat._m[13];
//     //         out._m[14] = mat._m[14];
//     //         out._m[15] = mat._m[15];
//     //     }
//     // }

// } // namespace Boo

// void Mat4::fromQuat(Mat4 &out, Quat &quat)
// {
//     float x = quat.getX();
//     float y = quat.getY();
//     float z = quat.getZ();
//     float w = quat.getW();

//     float x2 = x + x;
//     float y2 = y + y;
//     float z2 = z + z;

//     float xx = x * x2;
//     float yx = y * x2;
//     float yy = y * y2;
//     float zx = z * x2;
//     float zy = z * y2;
//     float zz = z * z2;
//     float wx = w * x2;
//     float wy = w * y2;
//     float wz = w * z2;

//     out._m[0] = 1 - yy - zz;
//     out._m[1] = yx + wz;
//     out._m[2] = zx - wy;
//     out._m[3] = 0;

//     out._m[4] = yx - wz;
//     out._m[5] = 1 - xx - zz;
//     out._m[6] = zy + wx;
//     out._m[7] = 0;

//     out._m[8] = zx + wy;
//     out._m[9] = zy - wx;
//     out._m[10] = 1 - xx - yy;
//     out._m[11] = 0;

//     out._m[12] = 0;
//     out._m[13] = 0;
//     out._m[14] = 0;
//     out._m[15] = 1;
// }
// void Mat4::inverseTranspose(const Mat4 &mat, Mat4 &out)
// {
//     Mat4 inv;
//     inverse(mat, inv);

//     // 转置: out[row][col] = inv[col][row]
//     // 列主序: out[col*4+row] = inv[row*4+col]
//     out._m[0] = inv._m[0];
//     out._m[1] = inv._m[4];
//     out._m[2] = inv._m[8];
//     out._m[3] = 0;

//     out._m[4] = inv._m[1];
//     out._m[5] = inv._m[5];
//     out._m[6] = inv._m[9];
//     out._m[7] = 0;

//     out._m[8] = inv._m[2];
//     out._m[9] = inv._m[6];
//     out._m[10] = inv._m[10];
//     out._m[11] = 0;

//     out._m[12] = 0;
//     out._m[13] = 0;
//     out._m[14] = 0;
//     out._m[15] = 1;
// }
