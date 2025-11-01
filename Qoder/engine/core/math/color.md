# Color 模块说明

## 概述
Color类提供了颜色处理的基本功能，支持RGBA颜色值的表示和操作。

## 功能特性

### 构造函数
- `Color()`: 默认构造函数，创建黑色不透明颜色 (0, 0, 0, 1)
- `Color(float r, float g, float b, float a)`: 使用指定的RGBA值构造颜色

### 方法
- `void set(float r, float g, float b, float a)`: 设置颜色的RGBA值
- `void set(std::string color)`: 通过十六进制字符串设置颜色值
  - 支持格式: #RRGGBB 或 #RRGGBBAA
- `float r()`: 获取红色分量值
- `float g()`: 获取绿色分量值
- `float b()`: 获取蓝色分量值
- `float a()`: 获取透明度分量值

## 使用示例
```cpp
// 创建默认颜色(黑色)
Color color1;

// 创建指定颜色(红色)
Color color2(1.0f, 0.0f, 0.0f, 1.0f);

// 通过十六进制字符串设置颜色
Color color3;
color3.set("#00FF00"); // 绿色
```

## 注意事项
1. 颜色分量值范围为 0.0f 到 1.0f
2. 十六进制颜色字符串必须以 # 开头
3. 无效的颜色格式会输出错误信息