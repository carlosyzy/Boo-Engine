#pragma once
#include <string>
#include <vector>
#include <cstdint>
#include "../math/quat.h"
#include "../math/vec3.h"

enum NodeTransformFlag : uint32_t
{
	NONE_FLAG = 0,
	POSITION_FLAG = 1 << 0,
	ROTATION_FLAG = 1 << 1,
	SCALE_FLAG = 1 << 2,
	SIZE_FLAG = 1 << 3,
	ANCHOR_FLAG = 1 << 4,
	ALL_FLAG = POSITION_FLAG | ROTATION_FLAG | SCALE_FLAG | SIZE_FLAG | ANCHOR_FLAG,
};

enum class NodeType
{
	Node,
	Node2D,
	Node3D,
	Scene,
};

class Node
{
protected:
	/**
	 * 当前节点类型
	 */
	NodeType _layer;
	/**
	 * 节点名称
	 */
	std::string _name;
	/**
	 * 节点uuid
	 */
	std::string _uuid;
	/**
	 * 当前节点是否激活
	 */
	bool _active;
	/**
	 * 当前节点在整个世界节点结构中是否激活
	 */
	bool _isActiveInHierarchy = true;
	/*
	 * 当前节点本地位置
	 */
	Vec3 _position;
	/*
	 * 当前节点世界位置
	 */
	Vec3 _worldPosition;
	/*
	 * 当前节点本地缩放
	 */
	Vec3 _scale; // 缩放
	/*
	 * 当前节点世界缩放
	 */
	Vec3 _woildScale;
	/*
	 * 欧拉角
	 */
	Vec3 _eulerAngles;
	/*
	 * 当前节点本地角度
	 */
	Quat _rotation;
	/*
	 * 当前节点世界角度
	 */
	Quat _worldRotation;

	/**
	 * 子节点
	 */
	std::vector<Node *> _children;
	/**
	 * 父节点
	 */
	Node *_parent;
	/*
	 *节点世界变化是否发生了变换，自身或父节点影响
	 */
	uint32_t _worldTransformFlag; //
	/*
	 * 更新世界transform 的更新flag
	 */
	void _updateWorldTransformFlag(NodeTransformFlag flag);
	void _updateNodesActiveInHierarchyState(bool isActiveInHierarch);
	virtual void _updateWorldTransform();

public:
	Node(const std::string name, const std::string uuid);
	virtual ~Node();

	// 基础属性
	void setName(const std::string &name);
	std::string getName() const;

	void setActive(bool active);
	bool isActive() const;

	const NodeType getLayer() const;
	const std::string getUuid() const;

	/**
	 * 设置坐标
	 */
	void setPosition(float x, float y, float z);
	void setWorldPosition(float x, float y, float z);
	/**
	 * 设置四元素角度
	 */
	void setRotation(float x, float y, float z, float w);
	void setWorldRotation(float x, float y, float z, float w);
	/*
	 * 设置缩放
	 */
	void setScale(float x, float y, float z);
	void setWorldScale(float x, float y, float z);
	/**
	 * 设置欧拉角
	 */
	void setEulerAngles(float x, float y, float z);

	const Vec3 &getPosition() { return this->_position; }
	const Quat &getRotation() { return this->_rotation; }
	const Vec3 &getScale() { return this->_scale; }

	/**
	 * 获取世界位置
	 */
	const Vec3 &getWorldPosition()
	{
		this->_updateWorldTransform();
		return this->_worldPosition;
	}
	/**
	 * 获取世界缩放
	 */
	const Vec3 &getWorldScale()
	{
		this->_updateWorldTransform();
		return this->_woildScale;
	}
	/**
	 * 获取世界角度
	 */
	const Quat &getWorldRotation()
	{
		this->_updateWorldTransform();
		return this->_worldRotation;
	}

	Node *getParent();
	void setParent(Node *node);

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