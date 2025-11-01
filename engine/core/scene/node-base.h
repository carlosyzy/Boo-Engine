#pragma once
#include <string>
#include "../math/vec3.h"

enum class NodeType {
    Node,
    Node2D,
    Node3D,
    Scene,
};

class NodeBase {
protected:
    std::string _name;
    std::string _uuid;
    bool _active;
    NodeType _layer;
    
    // 通用变换属性
    Vec3 _position;    // 位置
    Vec3 _scale;       // 缩放
    Vec3 _eulerAngles; // 欧拉角

public:
    NodeBase(const std::string& name, const std::string& uuid);
    virtual ~NodeBase();

    // 基础属性
    void setName(const std::string& name);
    std::string getName() const;
    
    void setActive(bool active);
    bool isActive() const;
    
    NodeType getLayer() const;
    std::string getUuid() const;

    // 位置属性
    void setPosition(const Vec3& position);
    const Vec3& getPosition() const;

    // 缩放属性
    void setScale(const Vec3& scale);
    const Vec3& getScale() const;

    // 欧拉角属性
    void setEulerAngles(const Vec3& eulerAngles);
    const Vec3& getEulerAngles() const;

    // 虚函数，子类可以重写
    virtual void update();
    virtual void render();
};