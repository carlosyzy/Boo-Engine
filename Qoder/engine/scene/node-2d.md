# Node2D 模块说明

## 概述
Node2D类是2D场景节点类，继承自NodeBase基类。提供了2D空间中的位置、旋转和缩放等属性，适用于2D游戏对象。Node2D继承了NodeBase的所有变换属性，并提供了2D特定的便捷方法。

## 功能特性

### 构造函数
- `Node2D()`: 默认构造函数，创建2D节点，自动生成UUID
- `Node2D(const std::string& name, const std::string& uuid = "")`: 使用指定名称创建2D节点，可选择性提供UUID

### 2D特定位置属性
- `void setPosition(float x, float y)`: 设置节点2D位置
- `void setX(float x)`: 设置节点X坐标
- `void setY(float y)`: 设置节点Y坐标
- `float getX() const`: 获取节点X坐标
- `float getY() const`: 获取节点Y坐标

### 2D特定旋转属性
- `void setRotation(float rotation)`: 设置节点2D旋转角度（绕Z轴）
- `float getRotation() const`: 获取节点2D旋转角度

### 2D特定缩放属性
- `void setScale(float scale)`: 统一设置节点2D缩放
- `void setScale(float scaleX, float scaleY)`: 分别设置节点X、Y轴缩放
- `void setScaleX(float scaleX)`: 设置节点X轴缩放
- `void setScaleY(float scaleY)`: 设置节点Y轴缩放
- `float getScaleX() const`: 获取节点X轴缩放
- `float getScaleY() const`: 获取节点Y轴缩放

### 继承的属性
Node2D继承了NodeBase的所有属性：
- `void setName(const std::string& name)`: 设置节点名称
- `std::string getName() const`: 获取节点名称
- `void setActive(bool active)`: 设置节点激活状态
- `bool isActive() const`: 获取节点激活状态
- `void setLayer(int layer)`: 设置节点层
- `int getLayer() const`: 获取节点层
- `void setUuid(const std::string& uuid)`: 设置节点UUID
- `std::string getUuid() const`: 获取节点UUID

### 继承的变换属性
Node2D继承了NodeBase的所有变换属性：
- `void setPosition(const Vec3& position)`: 设置节点位置
- `const Vec3& getPosition() const`: 获取节点位置
- `void setScale(const Vec3& scale)`: 设置节点缩放
- `const Vec3& getScale() const`: 获取节点缩放
- `void setEulerAngles(const Vec3& eulerAngles)`: 设置节点欧拉角
- `const Vec3& getEulerAngles() const`: 获取节点欧拉角

### 虚函数方法
- `virtual void update() override`: 更新2D节点状态
- `virtual void render() override`: 渲染2D节点

## 使用示例
```cpp
// 创建2D节点
Node2D node2d("Player");

// 使用2D特定方法设置属性
node2d.setPosition(100.0f, 200.0f);
node2d.setRotation(45.0f);
node2d.setScale(2.0f);

// 也可以使用继承的方法
Vec3 position(100.0f, 200.0f, 0.0f);
Vec3 scale(2.0f, 2.0f, 1.0f);
Vec3 rotation(0.0f, 0.0f, 45.0f);

node2d.setPosition(position);
node2d.setScale(scale);
node2d.setEulerAngles(rotation);

// 设置其他属性
node2d.setActive(true);
node2d.setLayer(2);

// 更新和渲染节点
node2d.update();
node2d.render();

// 获取节点UUID
std::string uuid = node2d.getUuid();
```

## 注意事项
1. Node2D继承自NodeBase，具有基类的所有功能
2. 坐标系统为标准的2D笛卡尔坐标系
3. 旋转角度以度为单位
4. 缩放值为倍数，1.0表示原始大小
5. Z坐标和绕X、Y轴的旋转在2D中通常为0
6. 每个节点都有唯一的UUID标识符