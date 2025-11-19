#include "ui-renderer.h"
#include <filesystem>
#include "../../gfx/gfx-mgr.h"
#include "../../boo.h"

#include "../../scene/node.h"
#include "../../scene/node-2d.h"
#include "../../assets/material.h"
#include "../../assets/asset.h"
#include "../../assets/assets-manager.h"
#include "../../assets/texture.h"

UIRenderer::UIRenderer(Node *node, std::string uuid) : Component(node, uuid)
{
	this->_flag = static_cast<uint32_t>(UIFlag::UI_ALL);
	this->_layer = NodeLayer::Node2D;
	// 创建渲染物体
	GfxMgr::getInstance()->createObject(this->_uuid, "ui", this->_positions, this->_colors, this->_normals, this->_uvs, this->_indices);
	this->updateModelMatrix();
}

void UIRenderer::setColor(float r, float g, float b, float a)
{
	this->_color.set(r, g, b, a);
	this->_flag |= static_cast<uint32_t>(UIFlag::UI_COLOR);
	GfxMgr::getInstance()->setObjectColor(this->_uuid, this->_color.getR(), this->_color.getG(), this->_color.getB(), this->_color.getA());
}
void UIRenderer::setColor(std::string color)
{
	this->_color.set(color);
	this->_flag |= static_cast<uint32_t>(UIFlag::UI_COLOR);
	GfxMgr::getInstance()->setObjectColor(this->_uuid, this->_color.getR(), this->_color.getG(), this->_color.getB(), this->_color.getA());
}
void UIRenderer::setAlpha(float alpha)
{
	if (alpha == this->_color.getA())
		return;
	this->_color.setA(alpha);
	this->_flag |= static_cast<uint32_t>(UIFlag::UI_COLOR);
	GfxMgr::getInstance()->setObjectColor(this->_uuid, this->_color.getR(), this->_color.getG(), this->_color.getB(), this->_color.getA());
}
void UIRenderer::setMaterial(Material *mtl)
{
	this->_material = mtl;
	// std::string vert = std::filesystem::path("resources/shader/ui/ui.vert.spv").generic_string();
	// std::string frag = std::filesystem::path("resources/shader/ui/ui.frag.spv").generic_string();
	// std::string pipeline = "Blend:1|DepthTest:0|DepthWrite:0|vert:" + vert + "|frag:" + frag;
	// GfxMgr::getInstance()->createPipeline("ui", pipeline);
	GfxMgr::getInstance()->setObjectPipeline(this->_uuid, "ui.mtl");
}
void UIRenderer::setTexture(Texture *texture)
{
	if (texture == nullptr)
	{
		std::cout << "UIRenderer::setTexture: texture is nullptr" << std::endl;
		return;
	}
	if (this->_texture == texture)
	{
		return;
	}
	this->_texture = texture;
	GfxMgr::getInstance()->setObjectTexture(this->_uuid, this->_texture->getUuid());
}
void UIRenderer::setTexture(std::string texture)
{
	Asset *tex = Boo::game->assetsManager()->get(texture);
	this->setTexture(dynamic_cast<Texture *>(tex));
}
void UIRenderer::updateModelMatrix()
{
	Node2D *node2D = dynamic_cast<Node2D *>(this->_node);
	GfxMgr::getInstance()->setObjectModelMatrix(this->_uuid, node2D->uiWorldMatrix().data());
}
void UIRenderer::update(float deltaTime)
{
	Component::update(deltaTime);
}
void UIRenderer::lateUpdate(float deltaTime)
{
	Component::lateUpdate(deltaTime);
}
void UIRenderer::render()
{
	Component::render();

	if (!this->isEnabled())
	{
		return; // 组件未激活
	}

	if (this->_texture == nullptr)
	{
		return;
	}
	if (this->_color.getA() <= 0)
	{
		// std::cout << "UIRenderer::render: color alpha is 0" << std::endl;
		return; // 颜色透明
	}
	if (this->_modelMatrix.getM00() <= 0 || this->_modelMatrix.getM11() <= 0)
	{
		// 模型矩阵缩放为0
		// std::cout << "UIRenderer::render: modelMatrix scale to 0" << std::endl;
		return;
	}
	// 提交渲染对象
	GfxMgr::getInstance()->submitObjectRender(this->_uuid);
}
void UIRenderer::destroy()
{
	Component::destroy();
	std::cout << "UIRenderer::destroy" << std::endl;
	GfxMgr::getInstance()->destroyObject(this->_uuid);
}

UIRenderer::~UIRenderer()
{
	std::cout << "UIRenderer::~destructor" << std::endl;
}
