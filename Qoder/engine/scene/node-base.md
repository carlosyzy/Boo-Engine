# NodeBase 模块说明

## 概述
NodeBase类是所有场景节点的基类，提供了节点的基本属性和功能。所有的2D和3D节点都继承自这个基类。NodeBase使用Vec3类型来表示位置、缩放和欧拉角，所有节点都具有这些变换属性。

## 功能特性

### 构造函数
- `NodeBase()`: 默认构造函数，创建名为"Node"的节点，自动生成UUID
- `NodeBase(const std::string& name, const std::string& uuid = "")`: 使用指定名称创建节点，可选择性提供UUID

### 基础属性
- `void setName(const std::string& name)`: 设置节点名称
- `std::string getName() const`: 获取节点名称
- `void setActive(bool active)`: 设置节点激活状态
- `bool isActive() const`: 获取节点激活状态
- `void setLayer(int layer)`: 设置节点层
- `int getLayer() const`: 获取节点层
- `void setUuid(const std::string& uuid)`: 设置节点UUID
- `std::string getUuid() const`: 获取节点UUID

### 变换属性
- `void setPosition(const Vec3& position)`: 设置节点位置
- `const Vec3& getPosition() const`: 获取节点位置
- `void setScale(const Vec3& scale)`: 设置节点缩放
- `const Vec3& getScale() const`: 获取节点缩放
- `void setEulerAngles(const Vec3& eulerAngles)`: 设置节点欧拉角
- `const Vec3& getEulerAngles() const`: 获取节点欧拉角

### 虚函数方法
- `virtual void update()`: 更新节点状态
- `virtual void render()`: 渲染节点

## 使用示例
```cpp
// 创建基础节点
NodeBase node("MyNode");

// 设置节点属性
node.setActive(true);
node.setLayer(1);

// 设置变换属性
Vec3 position(10.0f, 20.0f, 0.0f);
Vec3 scale(2.0f, 2.0f, 1.0f);
Vec3 rotation(0.0f, 0.0f, 45.0f);

node.setPosition(position);
node.setScale(scale);
node.setEulerAngles(rotation);

// 更新和渲染节点
node.update();
node.render();

// 获取节点UUID
std::string uuid = node.getUuid();
```

## 注意事项
1. NodeBase是一个虚基类，通常不会直接实例化使用
2. 所有变换属性都使用Vec3类型表示
3. update()和render()方法是虚函数，子类可以重写实现
4. 节点的激活状态会影响渲染行为
5. 每个节点都有唯一的UUID标识符