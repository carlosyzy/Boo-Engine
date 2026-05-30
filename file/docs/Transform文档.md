# 游戏引擎节点变换核心知识手册

> 本文档记录了游戏引擎中节点（Node）变换属性的核心知识点，包括矩阵计算、坐标转换、四元数操作等。

---

## 目录

1. [从 TRS 属性计算本地矩阵](#1-从-trs-属性计算本地矩阵)
2. [矩阵变换顺序：T × R × S](#2-矩阵变换顺序t--r--s)
3. [世界位置转本地位置](#3-世界位置转本地位置)
4. [世界缩放转本地缩放](#4-世界缩放转本地缩放)
5. [从矩阵提取缩放因子](#5-从矩阵提取缩放因子)
6. [欧拉角转四元数与旋转顺序](#6-欧拉角转四元数与旋转顺序)
7. [世界旋转转本地旋转](#7-世界旋转转本地旋转)

---

## 1. 从 TRS 属性计算本地矩阵

### 1.1 原理

给定节点的三个基本变换属性：
- `position`：位移向量 (px, py, pz)
- `rotation`：旋转四元数 (x, y, z, w)
- `scale`：缩放向量 (sx, sy, sz)

通过 T × R × S 顺序计算本地变换矩阵。

### 1.2 数学公式

```
LocalMatrix = TranslationMatrix × RotationMatrix × ScaleMatrix
```

### 1.3 实现代码（行主序）

```cpp
Matrix4 calculateTRSMatrix(const Vector3& position,
                           const Quaternion& rotation,
                           const Vector3& scale) {
    Matrix4 result;
    
    float x = rotation.x;
    float y = rotation.y;
    float z = rotation.z;
    float w = rotation.w;
    
    // ===== 1. 计算旋转矩阵 R =====
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
    
    // R 矩阵 (行主序)
    float r00 = 1 - (yy + zz);
    float r01 = xy + wz;
    float r02 = xz - wy;
    
    float r10 = xy - wz;
    float r11 = 1 - (xx + zz);
    float r12 = yz + wx;
    
    float r20 = xz + wy;
    float r21 = yz - wx;
    float r22 = 1 - (xx + yy);
    
    // ===== 2. 应用缩放: R × S =====
    result.m[0]  = r00 * scale.x;
    result.m[1]  = r01 * scale.x;
    result.m[2]  = r02 * scale.x;
    result.m[3]  = 0;
    
    result.m[4]  = r10 * scale.y;
    result.m[5]  = r11 * scale.y;
    result.m[6]  = 12 * scale.y;
    result.m[7]  = 0;
    
    result.m[8]  = r20 * scale.z;
    result.m[9]  = r21 * scale.z;
    result.m[10] = r22 * scale.z;
    result.m[11] = 0;
    
    // ===== 3. 应用位移: T + =====
    result.m[12] = position.x;
    result.m[13] = position.y;
    result.m[14] = position.z;
    result.m[15] = 1;
    
    return result;
}
```

### 1.4 矩阵结构（行主序）

```
| X轴方向×scale.x   X轴方向×scale.x   X轴方向×scale.x   0 |
| Y轴方向×scale.y   Y轴方向×scale.y   Y轴方向×scale.y   0 |
| Z轴方向×scale.z   Z轴方向×scale.z   Z轴方向×scale.z   0 |
|     pos.x              pos.y              pos.z        1 |

        ↓        ↓        ↓        ↓
       缩放     旋转     旋转     旋转      位移
```

---

## 2. 矩阵变换顺序：T × R × S

### 2.1 关键理解

**矩阵乘法是从右向左结合的！**

```
T × R × S
   ↓
先计算 R × S
再计算 T × (R × S)
```

**所以实际的变换执行顺序是：先缩放 → 再旋转 → 最后位移**

### 2.2 为什么叫 TRS？

TRS 只是**命名约定**，不是运算顺序：
- TRS 表示"有位移、有旋转、有缩放"这三个分量
- 矩阵顺序表示"运算"顺序

### 2.3 均匀缩放 vs 非均匀缩放

| 类型 | S × R vs R × S |
|------|----------------|
| 均匀缩放 (2,2,2) | ✅ 结果相同 |
| 非均匀缩放 (2,1,1) | ❌ 结果不同 |

### 2.4 推荐顺序

| 引擎 | 顺序 |
|------|------|
| Unity | T × R × S |
| Cocos2d-x | T × R × S |
| Three.js | T × R × S |

**推荐使用 T × R × S（先缩放，再旋转，最后位移）**

---

## 3. 世界位置转本地位置

### 3.1 原理

```
世界位置 = 父节点世界矩阵 × 本地位置
本地位置 = 父节点世界矩阵⁻¹ × 世界位置
```

### 3.2 实现代码

```cpp
void Node::setWorldPosition(const Vec3& worldPos) {
    if (!_parent) {
        _position = worldPos;
    } else {
        Mat4 parentWorldMatrixInv;
        Mat4::inverse(_parent->getWorldMatrix(), parentWorldMatrixInv);
        Mat4::multiplyVec3(parentWorldMatrixInv, worldPos, _position);
    }
    
    _localMatrixDirty = true;
    markWorldDirty();
}
```

### 3.3 几何解释

```
世界位置 → 父节点世界矩阵的逆 → 本地位置

举例：
父节点在世界位置 (10, 0, 0)
子节点在世界位置 (15, 0, 0)

子节点的本地位置 = (15-10, 0-0, 0-0) = (5, 0, 0)
```

---

## 4. 世界缩放转本地缩放

### 4.1 原理

```
世界缩放 = 父节点世界缩放 × 本地缩放
本地缩放 = 世界缩放 ÷ 父节点世界缩放
```

### 4.2 实现代码

```cpp
Vec3 extractScale(const Mat4& matrix) {
    Vec3 scale;
    
    // X轴缩放 = 第0行向量的长度
    scale.x = sqrt(matrix.m[0]*matrix.m[0] + 
                   matrix.m[1]*matrix.m[1] + 
                   matrix.m[2]*matrix.m[2]);
    
    // Y轴缩放 = 第1行向量的长度
    scale.y = sqrt(matrix.m[4]*matrix.m[4] + 
                   matrix.m[5]*matrix.m[5] + 
                   matrix.m[6]*matrix.m[6]);
    
    // Z轴缩放 = 第2行向量的长度
    scale.z = sqrt(matrix.m[8]*matrix.m[8] + 
                   matrix.m[9]*matrix.m[9] + 
                   matrix.m[10]*matrix.m[10]);
    
    return scale;
}

void Node::setWorldScale(const Vec3& worldScale) {
    if (!_parent) {
        _scale = worldScale;
    } else {
        Mat4 parentWorldInv = _parent->getWorldMatrix().inverse();
        Mat4 worldScaleMatrix;
        worldScaleMatrix.setScale(worldScale.x, worldScale.y, worldScale.z);
        
        Mat4 localMatrix = parentWorldInv * worldScaleMatrix;
        _scale = extractScale(localMatrix);
    }
    
    _localMatrixDirty = true;
    markWorldDirty();
}
```

---

## 5. 从矩阵提取缩放因子

### 5.1 原理

对于 T × R × S 结构的矩阵，每行向量的长度等于对应轴的缩放因子。

```
| r00*sx  r01*sx  r02*sx   tx |
| r10*sy  r11*sy  r12*sy   ty |
| r20*sz  r21*sz  r22*sz   tz |
|   0       0       0      1 |
```

| 行 | 含义 | 长度 = 缩放 |
|----|------|-------------|
| 第0行 | X轴方向 | √(m0² + m1² + m2²) = scale.x |
| 第1行 | Y轴方向 | √(m4² + m5² + m6²) = scale.y |
| 第2行 | Z轴方向 | √(m8² + m9² + m10²) = scale.z |

### 5.2 实现代码（行主序）

```cpp
void Mat4::getScale(const Mat4 &mat, Vec3 &out) {
    // X轴缩放 = 第0行向量的长度
    float scaleX = sqrt(mat.getM00() * mat.getM00() +
                        mat.getM01() * mat.getM01() +
                        mat.getM02() * mat.getM02());

    // Y轴缩放 = 第1行向量的长度
    float scaleY = sqrt(mat.getM10() * mat.getM10() +
                        mat.getM11() * mat.getM11() +
                        mat.getM12() * mat.getM12());

    // Z轴缩放 = 第2行向量的长度
    float scaleZ = sqrt(mat.getM20() * mat.getM20() +
                        mat.getM21() * mat.getM21() +
                        mat.getM22() * mat.getM22());

    // 处理反射（行列式为负时）
    float det = mat.getM00() * (mat.getM11() * mat.getM22() - mat.getM12() * mat.getM21()) 
              + mat.getM01() * (-mat.getM22() * mat.getM10() + mat.getM12() * mat.getM20()) 
              + mat.getM02() * (mat.getM21() * mat.getM10() - mat.getM11() * mat.getM20());
    
    if (det < 0) {
        scaleX *= -1;
    }
    
    out.setX(scaleX);
    out.setY(scaleY);
    out.setZ(scaleZ);
}
```

### 5.3 关键点

**为什么向量长度 = 缩放因子？**

旋转矩阵 R 是正交矩阵（每行都是单位向量，长度 = 1）。

```
加入缩放后：|r00*sx|   |sx|
            |r01*sx| = |0 |  ← 方向不变，长度 = sx
            |r02*sx|   |0 |
```

---

## 6. 欧拉角转四元数与旋转顺序

### 6.1 为什么有旋转顺序问题？

**矩阵乘法不满足交换律！**

```
A × B ≠ B × A
```

所以：
- 先绕 X 轴旋转 90°，再绕 Y 轴旋转 90° → 效果 A
- 先绕 Y 轴旋转 90°，再绕 X 轴旋转 90° → 效果 B
- **A ≠ B！**

### 6.2 旋转顺序

| 顺序 | 说明 |
|------|------|
| XYZ | 先X，再Y，最后Z |
| YZX | 先Y，再Z，最后X（代码默认） |
| ZYX | 先Z，再Y，最后X |
| ... | 其他组合 |

### 6.3 实现代码

```cpp
void Quat::fromEuler(float x, float y, float z, RotationOrder order = RotationOrder::YZX) {
    float halfToRad = 0.5f * 3.14159265358979323846f / 180.0f;
    x *= halfToRad;
    y *= halfToRad;
    z *= halfToRad;
    
    float sx = sin(x);
    float cx = cos(x);
    float sy = sin(y);
    float cy = cos(y);
    float sz = sin(z);
    float cz = cos(z);
    
    switch (order) {
        case RotationOrder::XYZ:
            this->_x = sx * cy * cz - cx * sy * sz;
            this->_y = cx * sy * cz + sx * cy * sz;
            this->_z = cx * cy * sz - sx * sy * cz;
            this->_w = cx * cy * cz + sx * sy * sz;
            break;
        // ... 其他顺序
    }
}
```

### 6.4 重要结论

| 问题 | 答案 |
|------|------|
| 不同顺序四元数相同吗？ | ❌ 不同 |
| 不同顺序矩阵相同吗？ | ❌ 不同 |
| GPU 渲染结果相同吗？ | ❌ **完全不同！** |
| 应该选哪个？ | 选择你想要的旋转效果，保持一致 |

### 6.5 业界推荐

| 引擎 | 默认顺序 |
|------|----------|
| Unity | YXZ |
| Unreal | YXZ |
| Three.js | YXZ |
| Cocos | YXZ |

---

## 7. 世界旋转转本地旋转

### 7.1 原理

```
世界旋转 = 父节点世界旋转 × 本地旋转
本地旋转 = 父节点世界旋转⁻¹ × 世界旋转
```

**对于单位四元数，四元数的逆 = 四元数的共轭**

```
q⁻¹ = q* = (-x, -y, -z, w)
```

### 7.2 四元数共轭

```cpp
void Quat::conjugate(const Quat &q, Quat &out) {
    out.set(-q._x, -q._y, -q._z, q._w);
}
```

### 7.3 四元数乘法

```cpp
void Quat::multiply(const Quat &q1, const Quat &q2, Quat &out) {
    float x = q1.getX() * q2.getW() + q1.getW() * q2.getX() + q1.getY() * q2.getZ() - q1.getZ() * q2.getY();
    float y = q1.getY() * q2.getW() + q1.getW() * q2.getY() + q1.getZ() * q2.getX() - q1.getX() * q2.getZ();
    float z = q1.getZ() * q2.getW() + q1.getW() * q2.getZ() + q1.getX() * q2.getY() - q1.getY() * q2.getX();
    float w = q1.getW() * q2.getW() - q1.getX() * q2.getX() - q1.getY() * q2.getY() - q1.getZ() * q2.getZ();
    out.setX(x);
    out.setY(y);
    out.setZ(z);
    out.setW(w);
}
```

### 7.4 setWorldRotation 实现

```cpp
void Node::setWorldRotation(float x, float y, float z, float w) {
    if (this->_parent == nullptr) return;
    
    const Quat &parentWorldQuat = this->_parent->getWorldRotation();
    Quat conjugateQuat;
    Quat::conjugate(parentWorldQuat, conjugateQuat);
    
    Quat localQuat{0,0,0,1};
    Quat worldQuat{x, y, z, w};
    Quat::multiply(conjugateQuat, worldQuat, localQuat);
    
    this->setRotation(localQuat.getX(), localQuat.getY(), localQuat.getZ(), localQuat.getW());
}
```

### 7.5 几何验证

```
场景：
父节点绕Y轴旋转 90°
子节点世界旋转：绕Y轴旋转 180°

计算：
conjugateQuat = -90°  (父节点共轭)
worldQuat = 180°
localQuat = (-90°) × 180° = 90°

验证：
90° × 180° = 180° ✓（父节点本地旋转合成世界旋转）
```

---

## 附录：常见问题 FAQ

### Q1: 为什么要用四元数而不是欧拉角？

1. 避免万向锁
2. 插值平滑（Slerp）
3. 存储紧凑（4个浮点数）

### Q2: 为什么要用脏标记（Dirty Flag）？

避免每帧都重新计算矩阵，只有属性变化时才计算，提升性能。

### Q3: 矩阵为什么要用行主序还是列主序？

- OpenGL：列主序
- DirectX：行主序
- 对 GPU 最终渲染无影响（只是内存布局不同）

### Q4: 非均匀缩放有什么问题？

会导致矩阵产生 skew（倾斜），不再是纯仿射变换，后续旋转会表现异常。

---

## 参考资料

- 《3D数学基础：图形与游戏开发》
- Unity Transform 文档
- Cocos Creator 引擎源码
- Three.js 引擎源码

---

> 文档创建日期：2026-03-30
> 作者：基于与 AI 助手的讨论整理
