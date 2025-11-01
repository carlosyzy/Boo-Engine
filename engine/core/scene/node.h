#pragma once
#include "../math/quat.h"
#include "../math/vec3.h"
#include <string>
#include <vector>

enum class NodeType {
  Node,
  Node2D,
  Node3D,
  Scene,
};

class Node {
protected:
  /**
   * 节点名称
   */
  std::string _name;
  /**
   * 节点uuid
   */
  std::string _uuid;

  bool _active;
  NodeType _layer;

  // 通用变换属性
  Vec3 _position; // 位置
  Vec3 _worldPosition;
  Vec3 _scale; // 缩放
  Vec3 _woildScale;
  Vec3 _eulerAngles; // 欧拉角
  Quat _rotate;
  Quat _worldRotate;

  /**
   * 子节点
   */
  std::vector<Node *> _children;
  /**
   * 父节点
   */
  Node *_parent;

public:
  Node(const std::string name, const std::string uuid);
  virtual ~Node();

  // 基础属性
  void setName(const std::string &name);
  std::string getName() const;

  void setActive(bool active);
  bool isActive() const;

  NodeType getLayer() const;
  std::string getUuid() const;

  // 位置属性
  void setPosition(const Vec3 &position);
  const Vec3 &getPosition() const;

  // 缩放属性
  void setScale(const Vec3 &scale);
  const Vec3 &getScale() const;

  // 欧拉角属性
  void setEulerAngles(const Vec3 &eulerAngles);
  const Vec3 &getEulerAngles() const;

  Node* getParent();
  void setParent(Node* node);

  /**
   * 添加子节点
   */
  virtual void addChild(Node *node);
  /**
   * 删除子节点
   */
  void removeChild(Node *node);
  /*
   *删除所有子节点
   */
  void destroyAllChildren();

  // 虚函数，子类可以重写
  virtual void update(float deltaTime);
  virtual void lateUpdate(float deltaTime);
  virtual void render();
};