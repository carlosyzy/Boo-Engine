#include "ui-bmfont-text.h"
#include "boo.h"
#include "log.h"
#include "core/scene/node-2d.h"
#include "core/renderer/camera.h"
#include "core/asset/asset-manager.h"
#include "core/asset/texture-asset.h"
#include "core/asset/mesh-asset.h"
#include "core/asset/material-asset.h"
#include "core/asset/bmfont-asset.h"
#include "core/gfx/gfx-manager.h"
#include "core/gfx/base/gfx-mesh.h"
#include "core/gfx/base/gfx-material.h"
#include "core/util/fmt-util.h"
#include "core/font/bmfont-manager.h"

namespace Boo
{

	UIBMFontText::UIBMFontText(std::string name, Node *node, std::string uuid) : UIRenderer(name, node, uuid),
																				 _text(L"Boo"),
																				 _fontSize(40),
																				 _lineHeight(40),
																				 _textLen(0),
																				 _materialAsset(nullptr),
																				 _meshAsset(nullptr)
	{
		this->_createMaterial();
		this->_createBMFont();
	}
	void UIBMFontText::_createMaterial()
	{
		this->_materialAsset = new MaterialAsset(AssetBuiltinMaterial::UI, "", "");
		MaterialAsset *mtl = dynamic_cast<MaterialAsset *>(assetManager->getAsset(AssetBuiltinMaterial::UI));
		this->_materialAsset->create(mtl);
	}
	void UIBMFontText::_createBMFont()
	{
		Asset *asset = assetManager->getAsset(AssetBuiltinFont::Font);
		BMFontAsset *bmFont = dynamic_cast<BMFontAsset *>(asset);
		if (bmFont != nullptr)
		{
			this->_bmFont = bmFont;
		}
	}
	void UIBMFontText::OnAwake()
	{
		UIRenderer::OnAwake();
	}
	void UIBMFontText::setProperty(json &data)
	{
		UIRenderer::setProperty(data);
		// set text
		if (data.contains("text") && data["text"].is_string())
		{
			std::wstring wstr = FmtUtil::utf8_to_wstring(data["text"].get<std::string>());
			this->setText(wstr);
		}
		// set font size
		if (data.contains("fontSize") && data["fontSize"].is_number_integer())
		{
			this->setSize(data["fontSize"].get<int>());
		}
		// set line height
		if (data.contains("lineHeight") && data["lineHeight"].is_number_integer())
		{
			this->setLineHeight(data["lineHeight"].get<int>());
		}
		// set color
		if (data.contains("color") && data["color"].is_object())
		{
			float r = 1.0f;
			float g = 1.0f;
			float b = 1.0f;
			float a = 1.0f;
			if (data["color"].contains("r") && data["color"]["r"].is_number())
			{
				r = data["color"]["r"].get<float>();
			}
			if (data["color"].contains("g") && data["color"]["g"].is_number())
			{
				g = data["color"]["g"].get<float>();
			}
			if (data["color"].contains("b") && data["color"]["b"].is_number())
			{
				b = data["color"]["b"].get<float>();
			}
			if (data["color"].contains("a") && data["color"]["a"].is_number())
			{
				a = data["color"]["a"].get<float>();
			}
			this->setColor(r, g, b, a);
		}
	}

	void UIBMFontText::OnEnable()
	{
		UIRenderer::OnEnable();
		this->_isVertDirty = true;
		this->_isTextDirty = true;
		this->_isMBFontDirty = true;
	}
	void UIBMFontText::setBMFont(std::string path)
	{
		BMFontAsset *bmFont = dynamic_cast<BMFontAsset *>(assetManager->getAsset(path, true));
		if (bmFont == nullptr)
		{
			LOGW("[UIBMFontText]:setBMFont: path %s not found", path.c_str());
			return;
		}
		this->setBMFont(bmFont);
	}
	void UIBMFontText::setBMFont(BMFontAsset *bmFont)
	{
		if (bmFont == nullptr || this->_bmFont == bmFont)
		{
			return;
		}
		this->_bmFont = bmFont;
		this->_isMBFontDirty = true;
	}
	void UIBMFontText::setSize(int fontSize)
	{
		this->_fontSize = fontSize;
		this->_isTextDirty = true;
	}
	const int &UIBMFontText::getSize()
	{
		return this->_fontSize;
	}
	void UIBMFontText::setLineHeight(int lineHeight)
	{
		this->_lineHeight = lineHeight;
		this->_isTextDirty = true;
	}
	const int &UIBMFontText::getLineHeight()
	{
		return this->_lineHeight;
	}
	void UIBMFontText::setText(const std::string &text)
	{
		std::wstring wstr = FmtUtil::utf8_to_wstring(text);
		this->setText(wstr);
	}
	void UIBMFontText::setText(const std::wstring &text)
	{
		this->setText(text.c_str());
	}
	void UIBMFontText::setText(const wchar_t *text)
	{
		if (this->_text == text)
		{
			return;
		}
		this->_text = text;
		this->_isTextDirty = true;
		size_t textLen = this->_text.length();
		if (textLen > this->_textLen)
		{
			this->_textLen = textLen;
			this->_isVertDirty = true;
		}
	}
	const std::wstring &UIBMFontText::getText()
	{
		return this->_text;
	}

	void UIBMFontText::setColor(Color &color)
	{
		if (color.getR() == this->_color.getR() &&
			color.getG() == this->_color.getG() &&
			color.getB() == this->_color.getB() &&
			color.getA() == this->_color.getA())
		{
			return;
		}
		this->setColor(color.getR(), color.getG(), color.getB(), color.getA());
	}
	void UIBMFontText::setColor(std::string color)
	{
		if (color == this->_color.hexString())
		{
			return;
		}
		Color c(color);
		this->setColor(c.getR(), c.getG(), c.getB(), c.getA());
	}
	void UIBMFontText::setAlpha(float alpha)
	{
		if (alpha == this->_color.getA())
			return;
		Color c(this->_color);
		c.setA(alpha);
		this->setColor(c.getR(), c.getG(), c.getB(), c.getA());
	}
	void UIBMFontText::setColor(float r, float g, float b, float a)
	{
		if (r == this->_color.getR() && g == this->_color.getG() &&
			b == this->_color.getB() && a == this->_color.getA())
		{
			return;
		}
		this->_color = Color(r, g, b, a);
		this->_color.set(r, g, b, a);
		this->_materialAsset->setUIColor(r, g, b, a);
	}
	void UIBMFontText::setMaterial(std::string material)
	{
		MaterialAsset *mtl = dynamic_cast<MaterialAsset *>(assetManager->getAsset(material, true));
		if (mtl == nullptr)
		{
			LOGW("[UIBMFontText]:setMaterial: material %s not found", material.c_str());
			return;
		}
		this->setMaterial(mtl);
	}
	void UIBMFontText::setMaterial(MaterialAsset *material)
	{
		if (material == nullptr)
		{
			LOGW("[UIBMFontText]:setMaterial: material %s not found", material->getName().c_str());
			return;
		}
		this->_materialAsset->create(material);
	}

	void UIBMFontText::Update(float deltaTime)
	{
		UIRenderer::Update(deltaTime);
	}
	void UIBMFontText::LateUpdate(float deltaTime)
	{
		UIRenderer::LateUpdate(deltaTime);
	}
	void UIBMFontText::Render(Camera *camera)
	{
		if (!this->_canRenderer(camera))
		{
			return;
		}
		if (this->_text.size() <= 0)
		{
			return;
		}
		if (this->_bmFont == nullptr)
		{
			return;
		}
		if (this->_isVertDirty)
		{
			this->_createMesh();
			this->_isVertDirty = false;
		}
		if (this->_isTextDirty || this->_isMBFontDirty)
		{
			this->_updateBMFont();
			this->_isTextDirty = false;
			this->_isMBFontDirty = false;
		}
		if (this->_meshAsset == nullptr || this->_materialAsset == nullptr)
		{
			return;
		}
		UIRenderer::Render(camera);
		// 1. 先添加模型矩阵 (16个float)
		const Mat4 &matrix = this->_node2D->getUIWorldMatrix();
		Mat4::multiply(matrix, view->getFitMatrix(), this->_uiViewMatrix);
		// 更新世界矩阵
		this->_materialAsset->setModelWorldMatrix(this->_uiViewMatrix.data());
		GfxManager::getInstance()->submitRenderObject(camera->getUuid(), this->_materialAsset->getGfxMaterial(), this->_meshAsset->getGfxMesh(0));
		// 增加渲染物体数量
		profiler->addObjectCount(1);
	}
	void UIBMFontText::_createMesh()
	{
		if (this->_meshAsset != nullptr)
		{
			this->_meshAsset->destroy();
			this->_meshAsset = nullptr;
		}
		size_t textLen = this->_text.length();
		if (textLen <= 0)
		{
			return;
		}
		this->_meshAsset = new MeshAsset();
		std::vector<float> positions = {};
		std::vector<float> uvs = {};
		std::vector<uint32_t> indices = {};
		positions.reserve(textLen * 6 * 3); // 1800
		uvs.reserve(textLen * 6 * 2);		// 1200
		indices.reserve(textLen * 6);		// 600

		for (int i = 0; i < textLen; i++)
		{
			// 每个字符两个三角形,6个顶点
			// 第一个三角形
			// 顶点0 左上角
			positions.push_back(-0.5f);
			positions.push_back(0.5f);
			positions.push_back(0.0f);
			uvs.push_back(0.0f);
			uvs.push_back(0.0f);
			// 顶点1 左下角
			positions.push_back(-0.5f);
			positions.push_back(-0.5f);
			positions.push_back(0.0f);
			uvs.push_back(0.0f);
			uvs.push_back(1.0f);
			// 顶点2 右下角
			positions.push_back(0.5f);
			positions.push_back(-0.5f);
			positions.push_back(0.0f);
			uvs.push_back(1.0f);
			uvs.push_back(1.0f);
			// 第二个三角形
			// 顶点3 左上角
			positions.push_back(-0.5f);
			positions.push_back(0.5f);
			positions.push_back(0.0f);
			uvs.push_back(0.0f);
			uvs.push_back(0.0f);
			// 顶点4 右下角
			positions.push_back(0.5f);
			positions.push_back(-0.5f);
			positions.push_back(0.0f);
			uvs.push_back(1.0f);
			uvs.push_back(1.0f);
			// 顶点5 右上角
			positions.push_back(0.5f);
			positions.push_back(0.5f);
			positions.push_back(0.0f);
			uvs.push_back(1.0f);
			uvs.push_back(0.0f);
		}
		uint32_t vertexCount = positions.size() / 3;
		for (int i = 0; i < vertexCount; i++)
		{
			indices.push_back(i * 3);
			indices.push_back(i * 3 + 1);
			indices.push_back(i * 3 + 2);
		}
		FMeshPrimitive primitive;
		primitive.index = 0;
		primitive.mode = EMeshMode::UI;
		primitive._positions = positions;
		primitive._uvs = uvs;
		primitive._indices = indices;
		this->_meshAsset->createDynamic({primitive});
	}
	void UIBMFontText::_updateBMFont()
	{
		if (this->_bmFont == nullptr)
		{
			return;
		}
		if (this->_meshAsset == nullptr || this->_materialAsset == nullptr || this->_node2D == nullptr)
		{
			return;
		}
		if (this->_text.length() <= 0)
		{
			return;
		}
		if (this->_fontSize <= 0)
		{
			return;
		}
		BMFontLayoutResult result = {};
		BMFontManager::getInstance()->create(this->_bmFont, this, result);
		if (result.width <= 0 || result.height <= 0)
		{
			return;
		}
		this->_node2D->setSize(result.width, result.height);
		if (result.positions.empty() || result.uvs.empty() || result.indices.empty())
		{
			return;
		}
		if (result.texture == nullptr)
		{
			return;
		}
		FMeshPrimitive primitive;
		primitive._positions = result.positions;
		primitive._uvs = result.uvs;
		primitive._indices = result.indices;
		int vertexCount = result.positions.size() / 3;
		this->_meshAsset->updateDynamicSubMesh(0, primitive);
		this->_materialAsset->setTexture(result.texture);
	}
	void UIBMFontText::OnDisable()
	{
		UIRenderer::OnDisable();
	}

	void UIBMFontText::destroy()
	{
		UIRenderer::destroy();
	}
	UIBMFontText::~UIBMFontText()
	{
		if (this->_meshAsset != nullptr)
		{
			this->_meshAsset->destroy();
			delete this->_meshAsset;
		}
		if (this->_materialAsset != nullptr)
		{
			this->_materialAsset->destroy();
			delete this->_materialAsset;
		}
		this->_materialAsset = nullptr;
		this->_meshAsset = nullptr;
		this->_bmFont = nullptr;
	}

} // namespace Boo
