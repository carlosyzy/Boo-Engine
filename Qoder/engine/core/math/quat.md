# Quat 模块说明

## 概述
Quat类提供了四元数的表示和基本操作功能。四元数常用于3D图形学中的旋转计算，避免了万向节死锁问题。

## 功能特性

### 构造函数
- `Quat()`: 默认构造函数，创建单位四元数 (0, 0, 0, 1)
- `Quat(float x, float y, float z, float w)`: 使用指定的x、y、z、w值构造四元数

### 方法
- `void set(float x, float y, float z, float w)`: 设置四元数的x、y、z、w值
- `float x()`: 获取四元数的x分量值
- `float y()`: 获取四元数的y分量值
- `float z()`: 获取四元数的z分量值
- `float w()`: 获取四元数的w分量值

## 使用示例
```cpp
// 创建默认单位四元数
Quat quat1;

// 创建指定值的四元数
Quat quat2(0.0f, 0.0f, 0.0f, 1.0f);

// 设置四元数值
Quat quat3;
quat3.set(1.0f, 0.0f, 0.0f, 0.0f);
```

## 注意事项
1. 四元数通常用于表示3D旋转
2. 单位四元数表示无旋转状态
3. 在进行旋转运算时，应确保四元数已标准化