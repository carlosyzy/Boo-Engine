# Mat4 模块说明

## 概述
Mat4类提供了4x4矩阵的表示和操作功能，用于3D图形变换，包括平移、旋转、缩放等操作。

## 功能特性

### 构造函数
- `Mat4()`: 默认构造函数，创建单位矩阵
- `Mat4(float m00, float m01, ..., float m33)`: 使用16个值创建矩阵

### 访问器方法
- `float& operator()(int row, int col)`: 通过行列索引访问矩阵元素
- `const float& operator()(int row, int col) const`: 通过行列索引访问矩阵元素（只读）
- `float& at(int row, int col)`: 通过行列索引访问矩阵元素
- `const float& at(int row, int col) const`: 通过行列索引访问矩阵元素（只读）
- `const std::array<float, 16> &data() const`: 获取矩阵数据数组

### 基本变换方法
- `void translate(float x, float y, float z)`: 平移变换
- `void scale(float x, float y, float z)`: 缩放变换
- `void rotateX(float angle)`: 绕X轴旋转变换
- `void rotateY(float angle)`: 绕Y轴旋转变换
- `void rotateZ(float angle)`: 绕Z轴旋转变换

### 矩阵运算
- `Mat4 operator*(const Mat4 &other) const`: 矩阵乘法
- `Mat4& operator*=(const Mat4 &other)`: 矩阵乘法赋值
- `bool operator==(const Mat4 &other) const`: 矩阵相等比较
- `bool operator!=(const Mat4 &other) const`: 矩阵不等比较

### 矩阵操作
- `void transpose()`: 矩阵转置
- `float determinant() const`: 计算矩阵行列式

### 静态构造方法
- `static Mat4 identity()`: 创建单位矩阵
- `static Mat4 translation(float x, float y, float z)`: 创建平移矩阵
- `static Mat4 scaling(float x, float y, float z)`: 创建缩放矩阵
- `static Mat4 rotationX(float angle)`: 创建绕X轴旋转矩阵
- `static Mat4 rotationY(float angle)`: 创建绕Y轴旋转矩阵
- `static Mat4 rotationZ(float angle)`: 创建绕Z轴旋转矩阵
- `static Mat4 perspective(float fov, float aspect, float near, float far)`: 创建透视投影矩阵
- `static Mat4 orthographic(float left, float right, float bottom, float top, float near, float far)`: 创建正交投影矩阵

### 静态操作方法
- `static void inverse(Mat4 &mat, Mat4 &out)`: 计算矩阵的逆矩阵
- `static void multiplyVec3(Mat4 &mat, Vec3 &in, Vec3 &out)`: 矩阵与向量相乘
- `static void transpose(const Mat4 &mat, Mat4 &out)`: 矩阵转置

## 使用示例
```cpp
// 创建单位矩阵
Mat4 identity = Mat4::identity();

// 创建变换矩阵
Mat4 translation = Mat4::translation(10.0f, 20.0f, 30.0f);
Mat4 rotation = Mat4::rotationY(45.0f);
Mat4 scale = Mat4::scaling(2.0f, 2.0f, 2.0f);

// 组合变换
Mat4 transform = translation * rotation * scale;

// 应用变换到向量
Vec3 point(1.0f, 1.0f, 1.0f);
Vec3 result;
Mat4::multiplyVec3(transform, point, result);

// 访问矩阵元素
float value = transform(0, 0); // 访问第一行第一列元素
```

## 注意事项
1. 矩阵元素按行优先顺序存储
2. 角度参数使用度数而不是弧度
3. 矩阵乘法不满足交换律，注意操作顺序
4. 透视投影矩阵使用OpenGL风格的坐标系统