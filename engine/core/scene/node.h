#pragma once
#include <string>
#include <vector>
#include <cstdint>
#include <functional>
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

class Node
{
private:
	int _nodeEventId = 0;
	struct Listener
	{
		std::function<void()> callback;
		void *owner;
		// 唯一标识符
		uint64_t id; 
		Listener(std::function<void()> cb, void *own, uint64_t id)
			: callback(cb), owner(own), id(id) {}
	};
	std::unordered_map<std::string, std::vector<Listener>> _listeners;

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

	void setActive(bool active, bool force = false);
	bool isActive() const;

	const NodeLayer getLayer() const;
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
		return this->_worldScale;
	}
	/**
	 * 获取世界角度
	 */
	const Quat &getWorldRotation()
	{
		this->_updateWorldTransform();
		return this->_worldRotation;
	}
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
	/**
	 * @brief 注册事件监听器
	 * @param eventName 事件名称
	 * @param callback 事件处理函数
	 * @param owner 事件处理函数所属类的指针
	 * @return uint64_t 监听器ID
	 */
	template <typename T, typename Func>
	uint64_t on(const std::string eventName, Func func, T *instance)
	{
		uint64_t id = this->_nodeEventId++;
		auto callback = [instance, func]()
		{
			(instance->*func)();
		};
		this->_listeners[eventName].emplace_back(callback, static_cast<void *>(instance), id);
		return id;
	}
	/**
	 * @brief 触发事件
	 * @param eventName 事件名称
	 */
	template <typename... Args>
	void emit(const std::string eventName)
	{
		auto it = this->_listeners.find(eventName);
		if (it != this->_listeners.end())
		{
			for (auto &listener : it->second)
			{
				listener.callback();
			}
		}
	}
	/**
	 * @brief 移除指定ID的事件监听器
	 * @param eventName 事件名称
	 * @param id 监听器ID
	 */
	void off(const std::string eventName, uint64_t id)
	{
		for (auto it = this->_listeners.begin(); it != this->_listeners.end(); ++it)
		{
			auto &listeners = it->second;
			for (auto listenerIt = listeners.begin(); listenerIt != listeners.end(); ++listenerIt)
			{
				if (listenerIt->id == id)
				{
					listeners.erase(listenerIt);
					return;
				}
			}
		}
	}
	/**
	 * @brief 移除所有事件监听器
	 */
	void offAll()
	{
		this->_listeners.clear();
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
	void destroy();

	// 虚函数，子类可以重写
	virtual void update(float deltaTime);
	virtual void lateUpdate(float deltaTime);
	virtual void render();
};