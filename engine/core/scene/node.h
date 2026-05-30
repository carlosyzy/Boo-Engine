#pragma once
#include <string>
#include <vector>
#include <cstdint>
#include <functional>
#include <iostream>
#include <unordered_map>
#include "core/math/mat4.h"
#include "core/math/vec3.h"
#include "core/math/vec2.h"
#include "core/math/quat.h"
#include "core/math/size.h"
#include "core/math/color.h"
#include "core/scene/node-type.h"
#include "core/component/component-type.h"
namespace Boo
{
	class Component;
	class Node
	{
	private:
	protected:
		/**
		 * 节点组ID，通过摄像机的Groups确定其可见性
		 */
		int _groupID = 0;
		/**
		 * 当前节点类型
		 * 固定不变的,在创建时就已经确定了
		 */
		ENodeLayer _layer;
		/**
		 * 节点名称
		 */
		std::string _name;
		/**
		 * 节点uuid
		 */
		std::string _uuid;

	public:
		bool _isLocked = false;
		/**
		 * @brief 获取节点类型
		 * @return NodeLayer 节点类型 Node2D,Node3D,Scene,
		 */
		const ENodeLayer getLayer() const;
		/**
		 * @brief 获取节点uuid
		 * @return std::string 节点uuid
		 */
		const std::string &getUuid() const;
		/**
		 * @brief 设置节点uuid
		 * @param uuid 节点uuid
		 */
		void setUuid(const std::string &uuid);
		/**
		 * @brief 设置节点名称
		 * @param name 节点名称
		 */
		void setName(const std::string &name);
		/**
		 * @brief 获取节点名称
		 * @return std::string 节点名称
		 */
		const std::string &getName() const;
		/**
		 * @brief 获取节点可见性
		 * @return int 可见性
		 */
		int getGroupId() const;
		/**
		 * @brief 设置节点组ID
		 * @param groupID 节点组ID
		 */
		void setGroupId(int groupId);

	protected:
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
		 * 当前节点世界矩阵的逆转置矩阵
		 */
		Mat4 _worldMatrixIT;


		/*
		 *节点世界变化是否发生了变换
		 */
		uint32_t _worldTransformFlag = uint32_t(ENodeTransformFlag::NONE_FLAG);
		/**
		 * @brief 当前节点在一帧内是否发生了变换
		 */
		uint32_t _frameTransformFlag = uint32_t(ENodeTransformFlag::NONE_FLAG);

	protected:
		/*
		 * 更新世界transform 的更新flag
		 */
		void _updateWorldTransformFlag(ENodeTransformFlag flag);
		/**
		 * @brief 更新世界transform矩阵
		 *
		 */
		virtual void _updateWorldTransform();

	public:
		/**
		 * 设置坐标
		 */
		virtual void setPosition(float x, float y, float z);
		/**
		 * 获取本地位置
		 */
		const Vec3 &getPosition();
		/**
		 * 设置世界位置
		 */
		virtual void setWorldPosition(float x, float y, float z);
		/**
		 * 获取世界位置
		 */
		const Vec3 &getWorldPosition();
		/**
		 * 设置四元素角度
		 */
		virtual void setRotation(float x, float y, float z, float w);
		/**
		 * 获取本地角度
		 */
		const Quat &getRotation();
		/**
		 * 设置世界角度
		 */
		virtual void setWorldRotation(float x, float y, float z, float w);
		/**
		 * 获取世界角度
		 */
		const Quat &getWorldRotation();

		/*
		 * 设置缩放
		 */
		virtual void setScale(float x, float y, float z);
		/**
		 * 获取本地缩放
		 */
		const Vec3 &getScale();
		/**
		 * 设置世界缩放
		 */
		virtual void setWorldScale(float x, float y, float z);
		/**
		 * 获取世界缩放
		 */
		const Vec3 &getWorldScale();

		/**
		 * 设置欧拉角
		 */
		virtual void setEulerAngles(float x, float y, float z);
		/**
		 * 获取欧拉角
		 */
		const Vec3 &getEulerAngles();

		/**
		 * @brief 设置本地矩阵
		 * @param matrix 本地矩阵
		 */
		void setMatrix(const Mat4 &matrix);
		/**
		 * 获取本地矩阵
		 */
		const Mat4 &getLocalMatrix();
		/**
		 * 获取世界矩阵
		 */
		const Mat4 &getWorldMatrix();
		

		/**
		 * @brief 当前节点在当前帧帧内是否发生了变换
		 * @return true 发生了变换
		 * @return false 没有发生变换
		 */
		const bool hasFrameTransformFlag() const;
		/**
		 * @brief 清除当前节点在当前帧帧内的变换标志
		 */
		virtual void clearNodeFrameFlag();

	protected:
		/**
		 * 子节点
		 */
		std::vector<Node *> _children;
		/**
		 * 父节点
		 */
		Node *_parent;

	public:
		/**
		 * 添加子节点
		 */
		void addChild(Node *node);
		/**
		 * 删除子节点
		 */
		void removeChild(Node *node);
		/**
		 * @brief 从父节点中移除
		 */
		void removeFromParent();
		/**
		 * 获取子节点列表
		 */
		std::vector<Node *> &getChildren();
		/**
		 * 获取子节点
		 */
		Node *getChildByName(std::string name);
		/*
		 * @brief 设置父节点
		 * @param parent 父节点
		 */
		void setParent(Node *parent);
		/**
		 * 获取父节点
		 */
		Node *getParent();

		/*
		 *删除所有子节点
		 */
		void destroyAllChildren();
		/**
		 * @brief 销毁节点
		 */
		virtual void destroy();

	protected:
		/**
		 * 当前节点是否激活
		 */
		bool _active = false;
		/**
		 * 当前节点在整个世界节点结构中是否激活
		 */
		bool _isActiveInHierarchy = false;

	protected:
		/**
		 * @brief 更新节点激活结构状态
		 * @param isActiveInHierarch 是否激活在整个世界节点结构中
		 */
		void _updateActiveInHierarchyState(bool isActiveInHierarch);
		void _activeNodeInHierarchyState();
		void _deactiveNodeInHierarchyState();
		// 递归获取当前节点所有子节点,然后倒叙返回
		void _getAllChildrenOfRecursion(Node *node, std::vector<Node *> &children);

	public:
		/**
		 * @brief 设置节点是否激活
		 * @param active 是否激活
		 */
		virtual void setActive(bool active);
		/**
		 * @brief 获取节点是否激活
		 * @return true 激活
		 * @return false 未激活
		 */
		bool getActive() const;
		/**
		 * @brief 获取节点是否激活在整个世界节点结构中
		 * @return true 激活
		 * @return false 未激活
		 */
		const bool getActiveInHierarchy() const;

	protected:
		/**
		 *组件列表
		 */
		std::vector<Component *> _components;

	public:
		/**
		 * 添加组件
		 */
		virtual Component *addComponent(std::string name, std::string uuid = "");
		/**
		 * 获取组件
		 */
		Component *getComponent(const std::string &name);
		/**
		 * 获取所有组件
		 */
		std::vector<Component *> getComponents();
		/**
		 * 移除指定组件
		 */
		void removeComponent(Component *component);
		/**
		 * 销毁指定组件
		 */
		void destoryComponent(Component *component);

	public:
		/**
		 * @brief 更新节点
		 * @param deltaTime 时间间隔
		 */
		virtual void update(float deltaTime);
		/**
		 * @brief 晚更新节点
		 * @param deltaTime 时间间隔
		 */
		virtual void lateUpdate(float deltaTime);
	};

} // namespace Boo
