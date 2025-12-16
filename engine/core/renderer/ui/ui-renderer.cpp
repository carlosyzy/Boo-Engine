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
	this->_materialAsset = new MaterialAsset();
	this->_materialAsset->createTest();
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
	// 激活的时候创建渲染物体
	// GfxMgr::getInstance()->createUIObject(this->_uuid, this->_positions, this->_colors, this->_normals, this->_uvs, this->_indices);
	// this->_updateRendererState();
	// this->_updateModelMatrix();
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
	// GfxMgr::getInstance()->setObjectColor(this->_uuid, this->_color.getR(), this->_color.getG(), this->_color.getB(), this->_color.getA());
}

void UIRenderer::_setMaterial(MaterialAsset *mtl)
{
	// if (this->_materialAsset == nullptr)
	// {
	// 	return;
	// }
	// this->_materialAsset = mtl;
	// if (!this->_isEnabledInHierarchy)
	// 	return;
	// GfxMgr::getInstance()->setObjectPass(this->_uuid, this->_materialAsset->getPass());
	// GfxMgr::getInstance()->setObjectPipeline(this->_uuid, this->_materialAsset->getPipeline());
	// // std::string vert = std::filesystem::path("resources/shader/ui/ui.vert.spv").generic_string();
	// // std::string frag = std::filesystem::path("resources/shader/ui/ui.frag.spv").generic_string();
	// // std::string pipeline = "Blend:1|DepthTest:0|DepthWrite:0|vert:" + vert + "|frag:" + frag;
	// // GfxMgr::getInstance()->createPipeline("ui", pipeline);
	// // GfxMgr::getInstance()->setObjectPipeline(this->_uuid, "ui-mask.mtl");
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

// void UIRenderer::_updateRendererState()
// {
// 	// // 更新颜色状态
// 	// // GfxMgr::getInstance()->setObjectColor(this->_uuid, this->_color.getR(), this->_color.getG(), this->_color.getB(), this->_color.getA());
// 	// // 更新纹理状态
// 	// if (this->_textureAsset)
// 	// {
// 	// 	// GfxMgr::getInstance()->setObjectTexture(this->_uuid, this->_textureAsset->getUuid());
// 	// }
// 	// //
// 	// if (this->_materialAsset)
// 	// {
// 	// 	// GfxMgr::getInstance()->setObjectPass(this->_uuid, this->_materialAsset->getPass());
// 	// 	// GfxMgr::getInstance()->setObjectPipeline(this->_uuid, this->_materialAsset->getPipeline());
// 	// }
// }
// void UIRenderer::_updateModelMatrix()
// {
// 	// Node2D *node2D = dynamic_cast<Node2D *>(this->_node);
// 	// GfxMgr::getInstance()->setObjectModelMatrix(this->_uuid, node2D->uiWorldMatrix().data());
// }

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
	// if (this->_node->hasFrameTransformFlag())
	// {
	// 	this->_updateModelMatrix();
	// }
	if (camera == nullptr)
	{
		return; // 相机为空
	}
	if (this->_color.getA() <= 0)
	{
		return; // 颜色透明
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
	if (node2D->getScale().getX() <= 0 || node2D->getScale().getY() <= 0)
	{
		return; // 节点不可见
	}
	// 提交渲染对象
	this->_instanceData.clear();
	this->_instanceData.reserve(16 + 4);
	// 1. 先添加模型矩阵 (16个float)
	const auto &matrix = node2D->uiWorldMatrix().data();
	_instanceData.insert(_instanceData.end(), matrix.begin(), matrix.end());
	// 2. 再添加颜色 (4个float)
	_instanceData.push_back(_color.getR());
	_instanceData.push_back(_color.getG());
	_instanceData.push_back(_color.getB());
	_instanceData.push_back(_color.getA());
	
	GfxMgr::getInstance()->submitRenderObject(camera->getUuid(), this->_materialAsset->getGfxMaterial(), Gfx::uiTestMesh, this->_instanceData);
}

void UIRenderer::Disable()
{
	Component::Disable();
}
void UIRenderer::destroy()
{
	Component::destroy();
	// GfxMgr::getInstance()->destroyObject(this->_uuid);
	std::cout << "UIRenderer::destroy" << std::endl;
}

UIRenderer::~UIRenderer()
{
	std::cout << "UIRenderer::~destructor" << this->_node->getName() << std::endl;
}
