#include "scene.h"
#include <iostream>
#include "boo.h"
#include "log.h"
#include "core/util/uuid-util.h"

namespace Boo
{

	Scene::Scene(const std::string name, const std::string uuid)
	{
		this->_groupID = 0;
		this->_layer = ENodeLayer::Scene;
		this->_name = name;
		this->_uuid = uuid.empty() ? UuidUtil::generateUUID() : uuid;
		this->_isLocked = false;
		this->_active = true;
		this->_isActiveInHierarchy = false;
		this->_position.set(0.0f, 0.0f, 0.0f);
		this->_scale.set(1.0f, 1.0f, 1.0f);
		this->_eulerAngles.set(0.0f, 0.0f, 0.0f);
		this->_rotation.set(0.0f, 0.0f, 0.0f, 1.0f);
		this->_worldPosition.set(0.0f, 0.0f, 0.0f);
		this->_worldScale.set(1.0f, 1.0f, 1.0f);
		this->_worldRotation.set(0.0f, 0.0f, 0.0f, 1.0f);
		this->_localMatrix = Mat4::identity();
		this->_worldMatrix = Mat4::identity();
		this->_worldTransformFlag = static_cast<uint32_t>(ENodeTransformFlag::ALL_FLAG);
		this->_frameTransformFlag = static_cast<uint32_t>(ENodeTransformFlag::ALL_FLAG);
		this->_root2D = nullptr;
		this->_root3D = nullptr;
		this->_parent = nullptr;
		this->createRoot2D();
		this->createRoot3D();
	}
	void Scene::createRoot2D()
	{
		if (this->_root2D != nullptr)
		{
			return;
		}
		this->_root2D = new Node2D("root2D");
		this->_root2D->setGroupId(uint32_t(ENodeGroup::Node2D));
		// 跟节点上锁
		this->_root2D->_isLocked = true;
		this->addChild(this->_root2D);
		this->_root2D->setSize(view->getDesignWidth(), view->getDesignHeight());
	}
	void Scene::createRoot3D()
	{
		if (this->_root3D != nullptr)
		{
			return;
		}
		this->_root3D = new Node3D("root3D");
		this->_root3D->_isLocked = true;
		this->_root3D->setGroupId(uint32_t(ENodeGroup::Node3D));
		this->addChild(this->_root3D);
		// 跟节点上锁
		
	}
	Node3D *Scene::getRoot3D()
	{
		if (this->_root3D == nullptr)
		{
			LOGW("[Scene]:getRoot3D:: root3D is null");
			return nullptr;
		}
		return this->_root3D;
	}
	Node2D *Scene::getRoot2D()
	{
		if (this->_root2D == nullptr)
		{
			LOGW("[Scene]:getRoot2D:: root2D is null");
			return nullptr;
		}
		return this->_root2D;
	}
	void Scene::launch()
	{
		LOGI("[Scene]:launch::launch %s", this->getName().c_str());
		this->_updateActiveInHierarchyState(true);
	}

	void Scene::setPosition(float x, float y, float z)
	{
		LOGW("[Scene]:setPosition:: scene not support set position");
	}
	void Scene::setWorldPosition(float x, float y, float z)
	{
		LOGW("[Scene]:setWorldPosition:: scene not support set world position");
	}
	void Scene::setScale(float x, float y, float z)
	{
		LOGW("[Scene]:setScale:: scene not support set scale");
	}
	void Scene::setWorldScale(float x, float y, float z)
	{
		LOGW("[Scene]:setWorldScale:: scene not support set world scale");
	}
	void Scene::setEulerAngles(float x, float y, float z)
	{
		LOGW("[Scene]:setEulerAngles:: scene not support set euler angles");
	}
	void Scene::setRotation(float x, float y, float z, float w)
	{
		LOGW("[Scene]:setRotation:: scene not support set rotation");
	}
	void Scene::setWorldRotation(float x, float y, float z, float w)
	{
		LOGW("[Scene]:setWorldRotation:: scene not support set world rotation");
	}
	void Scene::_updateWorldTransform()
	{
	}
	void Scene::setActive(bool active)
	{
		this->_active = true;
		LOGW("[Scene]:setActive:: scene not support set active");
	}
	Component *Scene::addComponent(std::string name, std::string uuid)
	{
		LOGW("[Scene]:addComponent:: scene not support add component");
		return nullptr;
	}
	void Scene::update(float dt)
	{
		Node::update(dt);
	}
	void Scene::lateUpdate(float dt)
	{
		Node::lateUpdate(dt);
	}
	void Scene::clearNodeFrameFlag()
	{
		Node::clearNodeFrameFlag();
	}
	void Scene::destroy()
	{
		Node::destroy();
	}
	Scene::~Scene()
	{
		this->_root2D = nullptr;
		this->_root3D = nullptr;
		std::cout << "Scene::~destructor" << std::endl;
	}

} // namespace Boo