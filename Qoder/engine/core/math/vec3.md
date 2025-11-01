# Vec3 模块说明

## 概述
Vec3类提供了三维向量的表示和操作功能，包括基本的向量运算、长度计算、旋转等操作，是3D图形学和游戏引擎中的基础数学类。

## 功能特性

### 构造函数
- `Vec3()`: 默认构造函数，创建零向量 (0, 0, 0)
- `Vec3(float x, float y, float z)`: 使用指定的x、y、z值构造向量

### 方法
- `void set(float x, float y, float z)`: 设置向量的x、y、z值
- `void set1(Vec3 &v)`: 使用另一个向量设置当前向量值
- `void setX(float x)`: 设置向量的x分量值
- `void setY(float y)`: 设置向量的y分量值
- `void setZ(float z)`: 设置向量的z分量值
- `float x()`: 获取向量的x分量值
- `float y()`: 获取向量的y分量值
- `float z()`: 获取向量的z分量值
- `float len()`: 计算向量的长度
- `void add(Vec3 &v)`: 将指定向量加到当前向量上
- `void subtract(Vec3 &v)`: 从当前向量中减去指定向量
- `void min(Vec3 &v)`: 将当前向量的各分量设置为与指定向量对应分量的最小值
- `void max(Vec3 &v)`: 将当前向量的各分量设置为与指定向量对应分量的最大值
- `void rotate(Quat *q)`: 使用四元数旋转向量
- `void test()`: 测试方法

### 静态方法
- `static void add(Vec3 *out, Vec3 *a, Vec3 *b)`: 计算两个向量的和并存储在输出向量中
- `static void subtract(Vec3 *out, Vec3 *a, Vec3 *b)`: 计算两个向量的差并存储在输出向量中

## 使用示例
```cpp
// 创建默认零向量
Vec3 vec1;

// 创建指定值的向量
Vec3 vec2(1.0f, 2.0f, 3.0f);

// 设置向量值
Vec3 vec3;
vec3.set(4.0f, 5.0f, 6.0f);

// 向量加法
vec2.add(vec3);

// 向量长度计算
float length = vec2.len();
```

## 注意事项
1. 向量分量值可以为任意实数
2. rotate方法使用四元数进行旋转向量计算
3. test方法用于性能测试，实际使用中可能不需要