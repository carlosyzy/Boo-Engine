#include "ui-renderer.h"
#include <filesystem>
#include "boo.h"
#include "log.h"
#include "core/gfx/gfx.h"
#include "core/gfx/gfx-manager.h"
#include "core/scene/node.h"
#include "core/scene/node-2d.h"
#include "core/gfx/base/gfx-mesh.h"
#include "core/gfx/base/gfx-material.h"

namespace Boo
{

	UIRenderer::UIRenderer(std::string name, Node *node, std::string uuid) : Component(name, node, uuid),
																			 _uiViewMatrix(Mat4::identity())
	{
		this->_layer = EComponentLayer::Layer2D;
		this->_node2D = dynamic_cast<Node2D *>(node);
	}
	void UIRenderer::OnAwake()
	{
		Component::OnAwake();
	}
	void UIRenderer::setProperty(json &data)
	{
	}
	void UIRenderer::OnEnable()
	{
		Component::OnEnable();
	}
	void UIRenderer::Update(float deltaTime)
	{
		Component::Update(deltaTime);
	}
	void UIRenderer::LateUpdate(float deltaTime)
	{
		Component::LateUpdate(deltaTime);
	}
	bool UIRenderer::_canRenderer(Camera *camera)
	{
		if (camera == nullptr)
		{
			return false; // 相机为空
		}
		if (this->_node2D == nullptr)
		{
			return false; // 节点不是Node2D类型
		}
		if (this->_node2D->getSize().getHeight() <= 0 || this->_node2D->getSize().getWidth() <= 0)
		{
			return false; // 节点不可见
		}
		if (this->_node->getWorldScale().getX() <= 0 || this->_node->getWorldScale().getY() <= 0)
		{
			return false; // 节点不可见
		}
		return true;
	}
	void UIRenderer::Render(Camera *camera)
	{
	}

	void UIRenderer::OnDisable()
	{
		Component::OnDisable();
	}
	void UIRenderer::destroy()
	{
		Component::destroy();
	}

	UIRenderer::~UIRenderer()
	{
		this->_node2D = nullptr;
	}

} // namespace Boo