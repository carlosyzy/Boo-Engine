#include "node.h"
#include "log.h"
#include "boo.h"
#include "core/component/component.h"

namespace Boo
{
	const ENodeLayer Node::getLayer() const
	{
		return this->_layer;
	}

	const std::string &Node::getUuid() const
	{
		return this->_uuid;
	}
	void Node::setUuid(const std::string &uuid)
	{
		this->_uuid = uuid;
	}
	void Node::setName(const std::string &name)
	{
		this->_name = name;
	}
	const std::string &Node::getName() const
	{
		return this->_name;
	}
	int Node::getGroupId() const
	{
		return this->_groupID;
	}
	void Node::setGroupId(int groupID)
	{
		this->_groupID = groupID;
	}

	void Node::setPosition(float x, float y, float z)
	{
		if (this->_isLocked)
			return;
		if (this->_position.getX() == x && this->_position.getY() == y && this->_position.getZ() == z)
		{
			return;
		}
		this->_position.set(x, y, z);
		this->_updateWorldTransformFlag(ENodeTransformFlag::POSITION_FLAG);
	}
	/**
	 * 获取本地位置
	 */
	const Vec3 &Node::getPosition()
	{
		return this->_position;
	}
	/**
	 * 设置世界位置
	 */
	void Node::setWorldPosition(float x, float y, float z)
	{
		if (this->_parent == nullptr)
			return;
		if (this->_isLocked)
			return;
		if (this->_worldPosition.getX() == x && this->_worldPosition.getY() == y && this->_worldPosition.getZ() == z)
		{
			return;
		}
		Mat4 parentMatInv{};
		Vec3 worldPos(x, y, z);
		Mat4::inverse(this->_parent->getWorldMatrix(), parentMatInv);
		Vec3 localPos{};
		Mat4::multiplyVec3(parentMatInv, worldPos, localPos);
		this->setPosition(localPos.getX(), localPos.getY(), localPos.getZ());
	}
	/**
	 * 获取世界位置
	 */
	const Vec3 &Node::getWorldPosition()
	{
		this->_updateWorldTransform();
		return this->_worldPosition;
	}
	/**
	 * 设置欧拉角
	 */
	void Node::setEulerAngles(float x, float y, float z)
	{
		if (this->_isLocked)
			return;
		this->_eulerAngles.set(x, y, z);
		Quat rotation;
		Quat::fromEuler(x, y, z, RotationOrder::YZX, rotation);
		this->setRotation(rotation.getX(), rotation.getY(), rotation.getZ(), rotation.getW());
	}
	/**
	 * 获取欧拉角
	 */
	const Vec3 &Node::getEulerAngles()
	{
		return this->_eulerAngles;
	}
	/**
	 * 设置四元素角度
	 */
	void Node::setRotation(float x, float y, float z, float w)
	{
		if (this->_isLocked)
			return;
		if (this->_rotation.getX() == x && this->_rotation.getY() == y && this->_rotation.getZ() == z && this->_rotation.getW() == w)
		{
			return;
		}
		this->_rotation.set(x, y, z, w);
		Quat::toEuler(this->_rotation, false, this->_eulerAngles);
		this->_updateWorldTransformFlag(ENodeTransformFlag::ROTATION_FLAG);
	}
	/**
	 * 获取本地角度
	 */
	const Quat &Node::getRotation()
	{
		return this->_rotation;
	}
	/**
	 * 设置世界角度
	 */
	void Node::setWorldRotation(float x, float y, float z, float w)
	{
		if (this->_parent == nullptr)
			return;
		if (this->_isLocked)
			return;
		if (this->_worldRotation.getX() == x && this->_worldRotation.getY() == y && this->_worldRotation.getZ() == z && this->_worldRotation.getW() == w)
		{
			return;
		}
		const Quat &parentWorldQuat = this->_parent->getWorldRotation();
		Quat conjugateQuat;
		Quat::conjugate(parentWorldQuat, conjugateQuat);
		Quat localQuat{0, 0, 0, 1};
		Quat worldQuat{x, y, z, w};
		Quat::multiply(conjugateQuat, worldQuat, localQuat);
		this->setRotation(localQuat.getX(), localQuat.getY(), localQuat.getZ(), localQuat.getW());
	}
	const Quat &Node::getWorldRotation()
	{
		this->_updateWorldTransform();
		return this->_worldRotation;
	}

	/**
	 * 设置本地缩放
	 */
	void Node::setScale(float x, float y, float z)
	{
		if (this->_isLocked)
			return;
		if (this->_scale.getX() == x && this->_scale.getY() == y && this->_scale.getZ() == z)
		{
			return;
		}
		this->_scale.set(x, y, z);
		this->_updateWorldTransformFlag(ENodeTransformFlag::SCALE_FLAG);
	}
	/**
	 * 获取本地缩放
	 */
	const Vec3 &Node::getScale()
	{
		return this->_scale;
	}
	/**
	 * 设置世界缩放
	 */
	void Node::setWorldScale(float x, float y, float z)
	{
		if (this->_parent == nullptr)
			return;
		if (this->_isLocked)
			return;
		if (this->_worldScale.getX() == x && this->_worldScale.getY() == y && this->_worldScale.getZ() == z)
		{
			return;
		}
		Mat4 parentMatInv{};
		Mat4::inverse(this->_parent->getWorldMatrix(), parentMatInv);
		// 1. 构建一个"只有缩放"的世界矩阵
		Mat4 _worldScaleMatrix{};
		_worldScaleMatrix.setM0(x);
		_worldScaleMatrix.setM5(y);
		_worldScaleMatrix.setM10(z);
		// 2. 用世界矩阵的逆矩阵乘以"只有缩放"的世界矩阵,得到"只有缩放"的本地矩阵
		Mat4 localMatrix{};
		Mat4::multiply(parentMatInv, _worldScaleMatrix, localMatrix);
		// 3. 从本地矩阵中提取缩放
		Vec3 localScale{};
		Mat4::getScale(localMatrix, localScale);
		this->setScale(localScale.getX(), localScale.getY(), localScale.getZ());
	}
	/**
	 * @brief 设置本地矩阵
	 * @param matrix 本地矩阵
	 */
	void Node::setMatrix(const Mat4 &matrix)
	{
		Mat4::toSRT(matrix, &this->_position, &this->_rotation, &this->_scale);
		Quat::toEuler(this->_rotation, false, this->_eulerAngles);
		this->_updateWorldTransformFlag(ENodeTransformFlag::ALL_FLAG);
	}

	const Mat4 &Node::getLocalMatrix()
	{
		return this->_localMatrix;
	}
	/**
	 * 获取世界缩放
	 */
	const Vec3 &Node::getWorldScale()
	{
		this->_updateWorldTransform();
		return this->_worldScale;
	}
	void Node::_updateWorldTransformFlag(ENodeTransformFlag flag)
	{
		this->_worldTransformFlag |= static_cast<uint32_t>(flag);
		this->_frameTransformFlag |= static_cast<uint32_t>(flag);
		// 更新渲染节点的模型矩阵
		for (auto &child : this->_children)
		{
			child->_updateWorldTransformFlag(flag);
		}
	}
	void Node::_updateWorldTransform()
	{
		// 通过位移,旋转,缩放计算本地矩阵
		this->_localMatrix.fromTRS(this->_position, this->_rotation, this->_scale);
		if (this->_parent)
		{
			Mat4::multiply(this->_localMatrix, this->_parent->getWorldMatrix(), this->_worldMatrix);
		}
		else
		{
			this->_worldMatrix = this->_localMatrix;
		}
		Mat4::getPosition(this->_worldMatrix, this->_worldPosition);
		Mat4::getScale(this->_worldMatrix, this->_worldScale);
		Mat4::getRotation(this->_worldMatrix, this->_worldRotation);
		this->_worldTransformFlag = ENodeTransformFlag::NONE_FLAG;
	}
	/**
	 * @brief 当前节点在当前帧帧内是否发生了变换
	 * @return true 发生了变换
	 * @return false 没有发生变换
	 */
	const bool Node::hasFrameTransformFlag() const
	{
		return (this->_frameTransformFlag != ENodeTransformFlag::NONE_FLAG);
	}
	/**
	 * @brief 获取世界矩阵
	 * @return Mat4 世界矩阵
	 */
	const Mat4 &Node::getWorldMatrix()
	{
		this->_updateWorldTransform();
		return this->_worldMatrix;
	}
	/**
	 * 添加子节点
	 */
	void Node::addChild(Node *child)
	{
		if (nullptr == child)
		{
			LOGW("[Node]:addChild:: child is nullptr");
			return;
		}
		child->removeFromParent();
		child->setParent(this);
	}
	/**
	 * 从父节点中移除子节点
	 */
	void Node::removeChild(Node *node)
	{
		if (nullptr == node)
		{
			LOGW("[Node]:removeChild:: node is nullptr");
			return;
		}
		// 使用标准算法更安全
		auto it = std::find(this->_children.begin(), this->_children.end(), node);
		if (it != this->_children.end())
		{
			this->_children.erase(it);
		}
		// 更新节点激活结构
		node->_updateActiveInHierarchyState(false);
	}
	/**
	 * @brief 从父节点中移除
	 */
	void Node::removeFromParent()
	{
		if (this->_parent == nullptr)
		{
			// LOGW("[Node]:removeFromParent:: %s is not in parent", this->getName().c_str());
			return;
		}
		this->_parent->removeChild(this);
		this->_parent = nullptr;
	}

	void Node::setParent(Node *parent)
	{
		if (nullptr == parent)
		{
			LOGW("[Node]:setParent:: parent is nullptr");
			return;
		}
		if (this->_parent == parent)
		{
			LOGW("[Node]:setParent:: %s already in parent %s", this->getName().c_str(), this->_parent->getName().c_str());
			return;
		}
		if (this->_parent != nullptr)
		{
			this->removeFromParent();
		}

		this->_parent = parent;
		this->_parent->_children.push_back(this);
		// 更新节点结构激活状态
		bool _isActiveInHierarchy = this->_parent->_isActiveInHierarchy && this->_active;
		this->_updateActiveInHierarchyState(_isActiveInHierarchy);
	}
	std::vector<Node *> &Node::getChildren()
	{
		return this->_children;
	}
	Node *Node::getChildByName(std::string name)
	{
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
	 * @brief 获取父节点
	 * @return Node* 父节点
	 */
	Node *Node::getParent()
	{
		return this->_parent;
	}

	void Node::setActive(bool active)
	{
		if (this->_active == active)
		{
			return;
		}
		this->_active = active;
		if (this->_parent == nullptr)
		{
			return;
		}
		// 子节点是否激活 = 父节点是否激活 && 子节点是否激活
		bool _isActiveInHierarchy = this->_parent->_isActiveInHierarchy && this->_active;
		this->_updateActiveInHierarchyState(_isActiveInHierarchy);
	}

	bool Node::getActive() const
	{
		return this->_active;
	}
	/**
	 * @brief 获取节点是否激活在整个世界节点结构中
	 * @return true 激活
	 * @return false 未激活
	 */
	const bool Node::getActiveInHierarchy() const
	{
		return this->_isActiveInHierarchy;
	}

	void Node::_updateActiveInHierarchyState(bool isActiveInHierarch)
	{
		if (this->_isActiveInHierarchy == isActiveInHierarch)
		{
			return;
		}

		// LOGW("[Node]:addChild:: _upda teActiveInHierarchyState %d ", isActiveInHierarch);
		if (isActiveInHierarch)
		{
			this->_isActiveInHierarchy = true;
			this->_activeNodeInHierarchyState();
		}
		else
		{
			this->_isActiveInHierarchy = false;
			this->_deactiveNodeInHierarchyState();
		}
	}
	void Node::_activeNodeInHierarchyState()
	{
		// 递归激活的节点
		// LOGW("[Node]:addChild:: _activeNodeInHierarchyState %s ", this->getName().c_str());
		for (auto *component : this->_components)
		{
			component->setNodeActiveInHierarchy(true);
		}
		for (auto &child : this->_children)
		{
			if (child->_active)
			{
				child->_isActiveInHierarchy = true;
				child->_activeNodeInHierarchyState();
			}
		}
	}
	void Node::_deactiveNodeInHierarchyState()
	{
		std::vector<Node *> children;
		this->_getAllChildrenOfRecursion(this, children);
		// 从后往前遍历，确保子节点先被处理
		for (int i = children.size() - 1; i >= 0; --i)
		{
			Node *child = children[i];
			child->_isActiveInHierarchy = false;
			// 更新组件生命周期
			for (auto &component : child->_components)
			{
				component->setNodeActiveInHierarchy(false);
			}
		}
	}
	/**
	 * 递归获取当前节点所有子节点,然后倒叙返回
	 */
	void Node::_getAllChildrenOfRecursion(Node *node, std::vector<Node *> &children)
	{
		children.push_back(node);
		for (auto &child : node->_children)
		{
			this->_getAllChildrenOfRecursion(child, children);
		}
	}

	void Node::destroyAllChildren()
	{
		auto childrenCopy = this->_children;
		this->_children.clear(); // 先清空，防止递归调用时的修改
		for (auto &child : childrenCopy)
		{
			child->destroy(); // 安全销毁
		}
	}
	void Node::destroy()
	{
		// 从父节点中移除-此时所有组件，包括子节点都置为未激活状态
		this->removeFromParent();
		// 销毁当前节点所有组件
		for (auto &component : this->_components)
		{
			component->OnDestroy();
			component->destroy();
		}
		this->_components.clear();
		// 递归销毁所有子节点
		this->destroyAllChildren();

		game->addNodeClearCaches(this);
	}

	/**
	 * 添加组件
	 */
	Component *Node::addComponent(std::string name, std::string uuid)
	{
		return nullptr;
	}
	/*
	 * 获取组件
	 */
	Component *Node::getComponent(const std::string &name)
	{
		for (auto component : this->_components)
		{
			if (component != nullptr && component->getName() == name)
			{
				return component;
			}
		}
		return nullptr;
	}
	/**
	 * 获取所有组件
	 */
	std::vector<Component *> Node::getComponents()
	{
		return this->_components;
	}
	/**
	 * 移除指定组件
	 */
	void Node::removeComponent(Component *component)
	{
		if (component == nullptr)
		{
			return;
		}
		auto comp = std::find(this->_components.begin(), this->_components.end(), component);
		if (comp != this->_components.end())
		{
			this->_components.erase(comp);
			component->setNodeActiveInHierarchy(false);
		}
	}
	/**
	 * 销毁指定组件
	 */
	void Node::destoryComponent(Component *component)
	{
		if (component == nullptr)
		{
			return;
		}
		auto comp = std::find(this->_components.begin(), this->_components.end(), component);
		if (comp != this->_components.end())
		{
			this->_components.erase(comp);
			component->setNodeActiveInHierarchy(false);
			component->OnDestroy();
			component->destroy();
		}
	}

	void Node::update(float dt)
	{
		// 更新组件
		for (auto &component : this->_components)
		{
			if (!component->isEnabledInHierarchy())
				continue;
			component->Update(dt);
		}
		// 更新子节点
		for (auto &child : this->_children)
		{
			child->update(dt);
		}
	}
	void Node::lateUpdate(float dt)
	{
		// 更新组件
		for (auto &component : this->_components)
		{
			if (!component->isEnabledInHierarchy())
				continue;
			component->LateUpdate(dt);
		}
		for (auto &child : this->_children)
		{
			child->lateUpdate(dt);
		}
	}
	void Node::clearNodeFrameFlag()
	{
		this->_frameTransformFlag = ENodeTransformFlag::NONE_FLAG;
		for (auto &child : this->_children)
		{
			child->clearNodeFrameFlag();
		}
	}

} // namespace Boo
