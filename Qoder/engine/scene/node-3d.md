# Node3D 模块说明

Node3D类是3D场景节点类，继承自Node基类。提供了3D空间中的位置、旋转和缩放等属性，适用于3D游戏对象。Node3D继承了Node的所有变换属性，并提供了3D特定的便捷方法。

## 类关系

Node3D继承了Node的所有属性：

- `_name`: 节点名称
- `_uuid`: 节点唯一标识符
- `_active`: 节点激活状态
- `_layer`: 节点层级类型

Node3D继承了Node的所有变换属性：

- `_position`: 节点位置(Vec3)
- `_scale`: 节点缩放(Vec3)
- `_eulerAngles`: 节点欧拉角(Vec3)
- `_rotate`: 节点旋转(Quat)

## 构造函数

- `Node3D(const std::string& name, const std::string& uuid = "")`: 使用指定名称创建3D节点，可选择性提供UUID

## 方法

Node3D重写了Node的以下虚方法：

- `virtual void update() override`: 3D节点更新逻辑
- `virtual void render() override`: 3D节点渲染逻辑

## 使用示例

```cpp
Node3D node3d("My3DNode");
```

## 注意事项

1. Node3D继承自Node，具有基类的所有功能
2. 在3D场景中使用Node3D比直接使用Node更合适
3. Node3D的变换属性在3D空间中使用
