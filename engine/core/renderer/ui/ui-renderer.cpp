#include "ui-renderer.h"
#include <filesystem>
#include "../../gfx/gfx.h"
#include "../../gfx/gfx-mgr.h"
#include "../../gfx/base/gfx-mesh.h"
#include "../../boo.h"
#include "../../scene/node.h"
#include "../../scene/node-2d.h"
#include "../../assets/material-asset.h"
#include "../../assets/asset.h"
#include "../../assets/assets-manager.h"
#include "../../assets/texture-asset.h"
#include "../../renderer/camera.h"

UIRenderer::UIRenderer(std::string name, Node *node, std::string uuid) : Component(name, node, uuid)
{
	this->_layer = NodeLayer::Node2D;
}
/**
 * @brief 反序列化组件属性-配置
 * 反序列化成功
 */
void UIRenderer::_deserialized()
{
	Component::_deserialized();
}
void UIRenderer::Awake()
{
	Component::Awake();
}
void UIRenderer::Enable()
{
	Component::Enable();
}
const int UIRenderer::getVisibility()
{
	return this->_node->getVisibility();
}
void UIRenderer::_setColor(float r, float g, float b, float a)
{
	// 透明度会影响到子节点的透明度
	if (r == this->_color.getR() && g == this->_color.getG() && b == this->_color.getB() && a == this->_color.getA())
	{
		return;
	}
	this->_color.set(r, g, b, a);
	if (!this->_isEnabledInHierarchy)
		return;
}

void UIRenderer::_setMaterial(MaterialAsset *mtl)
{
}
void UIRenderer::_setTexture(TextureAsset *texture)
{
	if (texture == nullptr)
	{
		std::cout << "UIRenderer::setTexture: texture is nullptr" << std::endl;
		return;
	}
	if (this->_textureAsset != nullptr && this->_textureAsset->getUuid() == texture->getUuid())
	{
		return;
	}
	this->_textureAsset = texture;
	this->_materialAsset->setTexture(0, texture->getUuid());
}
void UIRenderer::Update(float deltaTime)
{
	Component::Update(deltaTime);
}
void UIRenderer::LateUpdate(float deltaTime)
{
	Component::LateUpdate(deltaTime);
}
void UIRenderer::Render(Camera *camera)
{
	if (camera == nullptr)
	{
		return; // 相机为空
	}
	Node2D *node2D = dynamic_cast<Node2D *>(this->_node);
	if (node2D == nullptr)
	{
		return; // 节点不是Node2D类型
	}
	if (node2D->getSize().getHeight() <= 0 || node2D->getSize().getWidth() <= 0)
	{
		return; // 节点不可见
	}
	std::cout << "UIRenderer::Render:" << node2D->getName() << std::endl;
	// 提交渲染对象
	this->_instanceData.clear();
	this->_instanceData.reserve(16 + 4);
	// 1. 先添加模型矩阵 (16个float)
	const auto &matrix = node2D->uiWorldMatrix().data();
	_instanceData.insert(_instanceData.end(), matrix.begin(), matrix.end());
	// 2. 再添加颜色 (4个float)
	_instanceData.insert(_instanceData.end(), {_color.getR(), _color.getG(), _color.getB(), _color.getA()});

	GfxMgr::getInstance()->submitRenderObject(camera->getUuid(), this->_materialAsset->getGfxMaterial(), Gfx::uiTestMesh, this->_instanceData);
}

void UIRenderer::Disable()
{
	Component::Disable();
}
void UIRenderer::destroy()
{
	Component::destroy();
	// std::cout << "UIRenderer::destroy" << std::endl;
}

UIRenderer::~UIRenderer()
{
	// std::cout << "UIRenderer::~destructor" << this->_node->getName() << std::endl;
}
