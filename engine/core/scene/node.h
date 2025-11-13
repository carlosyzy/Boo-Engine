#pragma once
#include <string>
#include <vector>
#include <cstdint>
#include <functional>
#include <iostream>
#include "../math/quat.h"
#include "../math/vec3.h"
#include "../math/mat4.h"
#include <unordered_map>

namespace NodeEvent
{
	const std::string ON_TRANSFORM_CHANGED = "node_transform_changed";
}
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

enum class NodeLayer
{
	Node,
	Node2D,
	Node3D,
	Scene,
};

class Component;

class Node
{
private:
	int _nodeEventId = 0;
	struct TransformListener
	{
		std::function<void(uint32_t)> callback;
		void *owner;
		// 唯一标识符
		uint64_t id;
		TransformListener(std::function<void(uint32_t)> cb, void *own, uint64_t id)
			: callback(cb), owner(own), id(id)
		{
		}
	};
	std::vector<TransformListener> _transformListeners;
	

protected:
	/**
	 * 当前节点类型
	 */
	NodeLayer _layer;
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
	Vec3 _worldScale;
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
	 * 当前节点本地矩阵
	 */
	Mat4 _localMatrix;
	Mat4 _worldMatrix;

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
	uint32_t _worldTransformFlag;
	uint32_t _frameTransformFlag;

	/**
	 *组件列表
	 */
	std::vector<Component *> _components;

	/*
	 * 更新世界transform 的更新flag
	 */
	void _updateWorldTransformFlag(NodeTransformFlag flag);
	void _updateNodesActiveInHierarchyState(bool isActiveInHierarch);
	/**
	 * @brief 触发事件
	 * @param eventName 事件名称
	 */
	void emitTransformChanged(uint32_t flag)
	{
		for (auto &listener : this->_transformListeners)
		{
			listener.callback(flag);
		}
	}
	virtual void _updateWorldTransform();

public:
	// 基础属性
	void setName(const std::string &name);
	std::string getName() const;

	void setActive(bool active, bool force = false);
	bool isActive() const;

	const NodeLayer getLayer() const;
	const std::string getUuid() const;

	/**
	 * 设置坐标
	 */
	void setPosition(float x, float y, float z);
	const Vec3 &getPosition() { return this->_position; }
	void setWorldPosition(float x, float y, float z);
	/**
	 * 获取世界位置
	 */
	const Vec3 &getWorldPosition()
	{
		this->_updateWorldTransform();
		return this->_worldPosition;
	}

	/**
	 * 设置四元素角度
	 */
	void setRotation(float x, float y, float z, float w);
	const Quat &getRotation() { return this->_rotation; }
	void setWorldRotation(float x, float y, float z, float w);
	/**
	 * 获取世界角度
	 */
	const Quat &getWorldRotation()
	{
		this->_updateWorldTransform();
		return this->_worldRotation;
	}

	/*
	 * 设置缩放
	 */
	void setScale(float x, float y, float z);
	const Vec3 &getScale() { return this->_scale; }
	void setWorldScale(float x, float y, float z);
	/**
	 * 获取世界缩放
	 */
	const Vec3 &getWorldScale()
	{
		this->_updateWorldTransform();
		return this->_worldScale;
	}

	/**
	 * 设置欧拉角
	 */
	void setEulerAngles(float x, float y, float z);
	const Vec3 &getEulerAngles() { return this->_eulerAngles; }

	/**
	 * 获取本地矩阵
	 */
	const Mat4 &getLocalMatrix() { return this->_localMatrix; }
	/**
	 * 获取世界矩阵
	 */
	const Mat4 &getWorldMatrix()
	{
		this->_updateWorldTransform();
		return this->_worldMatrix;
	}

	const bool hasWorldTransformFlag() { return (this->_worldTransformFlag != NodeTransformFlag::NONE_FLAG); }
	const bool hasFrameTransformFlag() { return (this->_frameTransformFlag != NodeTransformFlag::NONE_FLAG); }

	template <typename T, typename Func>
	uint64_t onTransformChange(Func func, T *instance)
	{
		uint64_t id = this->_nodeEventId++;
		auto callback = [instance, func]()
		{
			if (instance->*func != nullptr && func != nullptr)
			{
				(instance->*func)();
			}
		};
		this->_transformListeners.emplace_back(callback, dynamic_cast<void *>(instance), id);
		return id;
	}
	void offTransformChange(uint64_t id)
	{
		for (auto it = this->_transformListeners.begin(); it != this->_transformListeners.end(); ++it)
		{
			if (it->id == id)
			{
				this->_transformListeners.erase(it);
				return;
			}
		}
	}
	void offAll()
	{
		this->_transformListeners.clear();
	}

	Node *getParent() { return this->_parent; }

	void setParent(Node *node);

	/**
	 * 添加子节点
	 */
	void addChild(Node *node);
	/**
	 * 获取子节点列表
	 */
	std::vector<Node *> getChildren() { return this->_children; }
	/**
	 * 删除子节点
	 */
	void removeChild(Node *node);
	/*
	 *删除所有子节点
	 */
	void destroyAllChildren();
	/**
	 * 获取子节点
	 */
	Node *getChildByName(std::string name){
		for (auto &child : this->_children)
		{
			if (child->_name == name)
			{
				return child;
			}
		}
		return nullptr;
	}
	/**
	 * 添加组件
	 */
	virtual Component *addComponent(std::string name, std::string uuid = "");
	/*
	 * 获取组件
	 */
	virtual Component *getComponent(std::string name);

	// 虚函数，子类可以重写
	virtual void update(float deltaTime);
	virtual void lateUpdate(float deltaTime);
	virtual void render();
	virtual void clearNodeFrameFlag();
	virtual void destroy();
};