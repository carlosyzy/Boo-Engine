#include "gfx-material.h"
#include "../../../log.h"
#include "../gfx.h"
#include "../gfx-context.h"

GfxMaterial::GfxMaterial()
{
	this->_rendererState = {};
	this->_instanceBlocks = {};
	this->_propertyBlocks = {};
	this->_textures = {};
}
void GfxMaterial::create(GfxRendererLayer layer, GfxRendererType type, const std::string& vert, const std::string& frag)
{
	if (layer == GfxRendererLayer::_UI)
	{
		this->_rendererState.layer = GfxRendererLayer::_UI;
		this->_textures.resize(4);
		this->_initUIInstanceDatas();
	}
	else if (layer == GfxRendererLayer::_3D)
	{
		this->_rendererState.layer = GfxRendererLayer::_3D;
		this->_textures.resize(10);
		this->_init3DInstanceDatas();
	}
	else
	{
		this->_rendererState.geom = "nosupport";
	}
	this->_rendererState.vert = vert;
	this->_rendererState.frag = frag;
	this->_rendererState.type = type;
	this->_rendererStateKey = this->_rendererState.generateKey();
}
void GfxMaterial::_initUIInstanceDatas()
{
	// 世界矩阵
	GfxMaterialDataBlock worldMatrix = {};
	worldMatrix.type = GfxMaterialPropertyType::Mat4;
	worldMatrix.key = "worldMat4";
	worldMatrix.offset = 0;
	worldMatrix.size = sizeof(float) * 16;
	this->_instanceBlocks[worldMatrix.key] = worldMatrix;
	// 颜色
	GfxMaterialDataBlock color = {};
	color.type = GfxMaterialPropertyType::Vec4;
	color.key = "color";
	color.offset = sizeof(float) * 16;
	color.size = sizeof(float) * 4;
	this->_instanceBlocks[color.key] = color;

	size_t reservedSize = sizeof(float) * 16 + sizeof(float) * 4;
	this->_instanceData.resize(reservedSize);
	// LOGI("GfxMaterial::_initUIInstanceDatas: reservedSize = %d", (int)reservedSize);
	// memset 是 C/C++ 中的一个内存设置函数，用于将一块内存区域的所有字节设置为指定的值。
	memset(this->_instanceData.data(), 0, reservedSize);
	std::array<float, 16> _m = {
		 1.0f, 0.0f, 0.0f, 0.0f,
		 0.0f, 1.0f, 0.0f, 0.0f,
		 0.0f, 0.0f, 1.0f, 0.0f,
		 0.0f, 0.0f, 0.0f, 1.0f };
	this->setModelWorldMatrix(_m);
	this->setUIColor(1.0f, 1.0f, 1.0f, 1.0f);
}
void GfxMaterial::_init3DInstanceDatas()
{
	// 世界矩阵
	GfxMaterialDataBlock worldMatrix = {};
	worldMatrix.type = GfxMaterialPropertyType::Mat4;
	worldMatrix.key = "worldMat4";
	worldMatrix.offset = 0;
	worldMatrix.size = sizeof(float) * 16; // 16 个 float 用于模型矩阵，
	this->_instanceBlocks[worldMatrix.key] = worldMatrix;
	GfxMaterialDataBlock worldMatrixIT = {};
	worldMatrixIT.type = GfxMaterialPropertyType::Mat4;
	worldMatrixIT.key = "transposeWorldMat4";
	worldMatrixIT.offset = sizeof(float) * 16;
	worldMatrixIT.size = sizeof(float) * 16;
	this->_instanceBlocks[worldMatrixIT.key] = worldMatrixIT;
	size_t reservedSize = sizeof(float) * 16 + sizeof(float) * 16;
	this->_instanceData.resize(reservedSize);
	// LOGI("GfxMaterial::_init3DInstanceDatas: reservedSize = %d", (int)reservedSize);
	// memset 是 C/C++ 中的一个内存设置函数，用于将一块内存区域的所有字节设置为指定的值。
	memset(this->_instanceData.data(), 0, reservedSize);
	std::array<float, 16> _m = {
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f };
	this->setModelWorldMatrix(_m);
	this->setModelWorldMatrixIT(_m);
}
void GfxMaterial::setVertexShader(const std::string& vert)
{
	if (this->_rendererState.vert == vert)
	{
		return;
	}
	this->_rendererState.vert = vert;
	this->_rendererStateKey = this->_rendererState.generateKey();
}
void GfxMaterial::setFragmentShader(const std::string& frag)
{
	if (this->_rendererState.frag == frag)
	{
		return;
	}
	this->_rendererState.frag = frag;
	this->_rendererStateKey = this->_rendererState.generateKey();
}

void GfxMaterial::setProperties(std::vector<GfxMaterialDataBlock>& properties)
{
	size_t offset = 0;
	this->_propertySize = 0;
	this->_propertyKey = "t";
	for (auto& property : properties)
	{
		if (property.type == GfxMaterialPropertyType::Int)
		{
			this->_addBytes(this->_propertyData, property.data, property.size);
		}
		else if (property.type == GfxMaterialPropertyType::Float)
		{
			this->_addBytes(this->_propertyData, property.data, property.size);
		}
		else if (property.type == GfxMaterialPropertyType::Vec2)
		{
			this->_addBytes(this->_propertyData, property.data, property.size);
		}
		else if (property.type == GfxMaterialPropertyType::Vec3)
		{
			this->_addBytes(this->_propertyData, property.data, property.size);
		}
		else if (property.type == GfxMaterialPropertyType::Vec4)
		{
			property.size = sizeof(property.data);
			this->_addBytes(this->_propertyData, property.data, property.size);
		}
		offset += property.size;
		property.offset = offset;
		this->_propertyBlocks[property.key] = property;
		this->_propertySize += property.size;
		this->_propertyKey += std::string(property.key);
	}
	this->_rendererState.pushConstantKey = this->_propertyKey;
	this->_rendererState.pushConstantSize = this->_propertySize;
	this->_rendererStateKey = this->_rendererState.generateKey();
}
void GfxMaterial::setModelWorldMatrix(const std::array<float, 16>& modelWorldMatrix)
{
	memcpy(this->_instanceData.data(), modelWorldMatrix.data(), sizeof(modelWorldMatrix));
	this->_instanceWorldMatrixData = modelWorldMatrix;
}
/**
 * @brief 设置模型世界矩阵的逆转置矩阵
 * @param modelWorldMatrixIT 模型世界矩阵的逆转置矩阵
 */
void GfxMaterial::setModelWorldMatrixIT(const std::array<float, 16>& modelWorldMatrixIT)
{
	if (this->_rendererState.layer != GfxRendererLayer::_3D)
	{
		return;
	}
	memcpy(this->_instanceData.data() + sizeof(float) * 16, modelWorldMatrixIT.data(), sizeof(modelWorldMatrixIT));
}
void GfxMaterial::setUIColor(const float r, const float g, const float b, const float a)
{
	if (this->_rendererState.layer != GfxRendererLayer::_UI)
	{
		return;
	}
	float color[4] = { r, g, b, a };
	memcpy(this->_instanceData.data() + sizeof(float) * 16, color, sizeof(float) * 4);
}
void GfxMaterial::setTexture(const int index, const std::string& texture)
{
	if (index >= this->_textures.size())
	{
		return;
	}
	this->_textures[index] = texture;
}
void GfxMaterial::setPropertyInt(const std::string& key, const int value)
{
	memcpy(this->_propertyData.data() + this->_propertyBlocks[key].offset, &value, sizeof(int));
}
void GfxMaterial::setPropertyFloat(const std::string& key, const float value)
{
	memcpy(this->_propertyData.data() + this->_propertyBlocks[key].offset, &value, sizeof(float));
}
void GfxMaterial::setPropertyVec2(const std::string& key, const float x, const float y)
{
	float value[2] = { x, y };
	memcpy(this->_propertyData.data() + this->_propertyBlocks[key].offset, value, sizeof(float) * 2);
}
void GfxMaterial::setPropertyVec3(const std::string& key, const float x, const float y, const float z)
{
	float value[3] = { x, y, z };
	memcpy(this->_propertyData.data() + this->_propertyBlocks[key].offset, value, sizeof(float) * 3);
}
void GfxMaterial::setPropertyVec4(const std::string& key, const float x, const float y, const float z, const float w)
{
	float value[4] = { x, y, z, w };
	memcpy(this->_propertyData.data() + this->_propertyBlocks[key].offset, value, sizeof(float) * 4);
}

// 直接添加字节数据（最底层的方法）
void GfxMaterial::_addBytes(std::vector<char>& _originData, const void* data, size_t size)
{
	size_t oldSize = _originData.size();
	_originData.resize(oldSize + size);
	memcpy(_originData.data() + oldSize, data, size);
}
void GfxMaterial::setPolygonMode(GfxRendererStatePolygonMode polygonMode)
{
	if (this->_rendererState.polygonMode == polygonMode)
	{
		return;
	}
	this->_rendererState.polygonMode = polygonMode;
	this->_rendererStateKey = this->_rendererState.generateKey();
}
void GfxMaterial::setCullMode(GfxRendererStateCullMode cullMode)
{
	if (this->_rendererState.cullMode == cullMode)
	{
		return;
	}
	this->_rendererState.cullMode = cullMode;
	this->_rendererStateKey = this->_rendererState.generateKey();
}
void GfxMaterial::setDepthTest(int depthTest)
{
	if (this->_rendererState.depthTest == depthTest)
	{
		return;
	}
	this->_rendererState.depthTest = depthTest;
	this->_rendererStateKey = this->_rendererState.generateKey();
}
void GfxMaterial::setDepthWrite(int depthWrite)
{
	if (this->_rendererState.depthWrite == depthWrite)
	{
		return;
	}
	this->_rendererState.depthWrite = depthWrite;
	this->_rendererStateKey = this->_rendererState.generateKey();
}
void GfxMaterial::setDepthCompareOp(GfxRendererStateCompareOp depthCompareOp)
{
	if (this->_rendererState.depthCompareOp == depthCompareOp)
	{
		return;
	}
	this->_rendererState.depthCompareOp = depthCompareOp;
	this->_rendererStateKey = this->_rendererState.generateKey();
}
void GfxMaterial::setStencilTest(int stencilTest)
{
	if (this->_rendererState.stencilTest == stencilTest)
	{
		return;
	}
	this->_rendererState.stencilTest = stencilTest;
	this->_rendererStateKey = this->_rendererState.generateKey();
}
void GfxMaterial::setStencilFrontCompareOp(GfxRendererStateCompareOp stencilFrontCompareOp)
{
	if (this->_rendererState.stencilFrontCompareOp == stencilFrontCompareOp)
	{
		return;
	}
	this->_rendererState.stencilFrontCompareOp = stencilFrontCompareOp;
	this->_rendererStateKey = this->_rendererState.generateKey();
}
void GfxMaterial::setStencilFrontFailOp(GfxRendererStateStencilOp stencilFrontFailOp)
{
	if (this->_rendererState.stencilFrontFailOp == stencilFrontFailOp)
	{
		return;
	}
	this->_rendererState.stencilFrontFailOp = stencilFrontFailOp;
	this->_rendererStateKey = this->_rendererState.generateKey();
}
void GfxMaterial::setStencilFrontDepthFailOp(GfxRendererStateStencilOp stencilFrontDepthFailOp)
{
	if (this->_rendererState.stencilFrontDepthFailOp == stencilFrontDepthFailOp)
	{
		return;
	}
	this->_rendererState.stencilFrontDepthFailOp = stencilFrontDepthFailOp;
	this->_rendererStateKey = this->_rendererState.generateKey();
}
void GfxMaterial::setStencilFrontPassOp(GfxRendererStateStencilOp stencilFrontPassOp)
{
	if (this->_rendererState.stencilFrontPassOp == stencilFrontPassOp)
	{
		return;
	}
	this->_rendererState.stencilFrontPassOp = stencilFrontPassOp;
	this->_rendererStateKey = this->_rendererState.generateKey();
}
void GfxMaterial::setStencilFrontCompareMask(uint32_t _stencilFrontCompareMask)
{
	if (this->_rendererState.stencilFrontCompareMask == _stencilFrontCompareMask)
	{
		return;
	}
	this->_rendererState.stencilFrontCompareMask = _stencilFrontCompareMask;
	this->_rendererStateKey = this->_rendererState.generateKey();
}
void GfxMaterial::setStencilFrontWriteMask(uint32_t _stencilFrontWriteMask)
{
	if (this->_rendererState.stencilFrontWriteMask == _stencilFrontWriteMask)
	{
		return;
	}
	this->_rendererState.stencilFrontWriteMask = _stencilFrontWriteMask;
	this->_rendererStateKey = this->_rendererState.generateKey();
}
void GfxMaterial::setStencilFrontRreference(uint32_t _stencilFrontRreference)
{
	if (this->_rendererState.stencilFrontRreference == _stencilFrontRreference)
	{
		return;
	}
	this->_rendererState.stencilFrontRreference = _stencilFrontRreference;
	this->_rendererStateKey = this->_rendererState.generateKey();
}
void GfxMaterial::setStencilBackCompareOp(GfxRendererStateCompareOp stencilBackCompareOp)
{
	if (this->_rendererState.stencilBackCompareOp == stencilBackCompareOp)
	{
		return;
	}
	this->_rendererState.stencilBackCompareOp = stencilBackCompareOp;
	this->_rendererStateKey = this->_rendererState.generateKey();
}
void GfxMaterial::setStencilBackFailOp(GfxRendererStateStencilOp stencilBackFailOp)
{
	if (this->_rendererState.stencilBackFailOp == stencilBackFailOp)
	{
		return;
	}
	this->_rendererState.stencilBackFailOp = stencilBackFailOp;
	this->_rendererStateKey = this->_rendererState.generateKey();
}
void GfxMaterial::setStencilBackDepthFailOp(GfxRendererStateStencilOp stencilBackDepthFailOp)
{
	this->_rendererState.stencilBackDepthFailOp = stencilBackDepthFailOp;
}
void GfxMaterial::setStencilBackPassOp(GfxRendererStateStencilOp stencilBackPassOp)
{
	if (this->_rendererState.stencilBackPassOp == stencilBackPassOp)
	{
		return;
	}
	this->_rendererState.stencilBackPassOp = stencilBackPassOp;
	this->_rendererStateKey = this->_rendererState.generateKey();
}
void GfxMaterial::setStencilBackCompareMask(uint32_t _stencilBackCompareMask)
{
	this->_rendererState.stencilBackCompareMask = _stencilBackCompareMask;
}
void GfxMaterial::setStencilBackWriteMask(uint32_t _stencilBackWriteMask)
{
	if (this->_rendererState.stencilBackWriteMask == _stencilBackWriteMask)
	{
		return;
	}
	this->_rendererState.stencilBackWriteMask = _stencilBackWriteMask;
	this->_rendererStateKey = this->_rendererState.generateKey();
}
void GfxMaterial::setStencilBackRreference(uint32_t _stencilBackRreference)
{
	if (this->_rendererState.stencilBackRreference == _stencilBackRreference)
	{
		return;
	}
	this->_rendererState.stencilBackRreference = _stencilBackRreference;
	this->_rendererStateKey = this->_rendererState.generateKey();
}
void GfxMaterial::setColorBlend(int colorBlend)
{
	if (this->_rendererState.colorBlend == colorBlend)
	{
		return;
	}
	this->_rendererState.colorBlend = colorBlend;
	this->_rendererStateKey = this->_rendererState.generateKey();
}
void GfxMaterial::setSrcColorBlendFactor(GfxRendererStateColorBlendFactor srcColorBlendFactor)
{
	if (this->_rendererState.srcColorBlendFactor == srcColorBlendFactor)
	{
		return;
	}
	this->_rendererState.srcColorBlendFactor = srcColorBlendFactor;
	this->_rendererStateKey = this->_rendererState.generateKey();
}
void GfxMaterial::setDstColorBlendFactor(GfxRendererStateColorBlendFactor dstColorBlendFactor)
{
	if (this->_rendererState.dstColorBlendFactor == dstColorBlendFactor)
	{
		return;
	}
	this->_rendererState.dstColorBlendFactor = dstColorBlendFactor;
	this->_rendererStateKey = this->_rendererState.generateKey();
}
void GfxMaterial::setColorBlendOp(GfxRendererStateColorBlendOp colorBlendOp)
{
	if (this->_rendererState.colorBlendOp == colorBlendOp)
	{
		return;
	}
	this->_rendererState.colorBlendOp = colorBlendOp;
	this->_rendererStateKey = this->_rendererState.generateKey();
}
void GfxMaterial::setSrcAlphaBlendFactor(GfxRendererStateColorBlendFactor srcAlphaBlendFactor)
{
	this->_rendererState.srcAlphaBlendFactor = srcAlphaBlendFactor;
}
void GfxMaterial::setDstAlphaBlendFactor(GfxRendererStateColorBlendFactor dstAlphaBlendFactor)
{
	if (this->_rendererState.dstAlphaBlendFactor == dstAlphaBlendFactor)
	{
		return;
	}
	this->_rendererState.dstAlphaBlendFactor = dstAlphaBlendFactor;
	this->_rendererStateKey = this->_rendererState.generateKey();
}
void GfxMaterial::setAlphaBlendOp(GfxRendererStateColorBlendOp alphaBlendOp)
{
	if (this->_rendererState.alphaBlendOp == alphaBlendOp)
	{
		return;
	}
	this->_rendererState.alphaBlendOp = alphaBlendOp;
	this->_rendererStateKey = this->_rendererState.generateKey();
}
/**
 * @brief 比较材质是否相等
 *
 * @param material
 * @return true
 * @return false
 */
bool GfxMaterial::equals(const GfxMaterial* material) const
{
	if (material == nullptr)
	{
		return false;
	}
	if (this == material)
	{
		return true;
	}
	if (this->_rendererStateKey != material->_rendererStateKey)
	{
		return false;
	}
	if (this->_textures.size() != material->_textures.size())
	{
		return false;
	}
	for (int i = 0; i < this->_textures.size(); i++)
	{
		if (this->_textures[i] != material->_textures[i])
		{
			return false;
		}
	}
	return true;
}
const std::vector<std::string>& GfxMaterial::getTextures() const
{
	return this->_textures;
}
GfxRendererState& GfxMaterial::getRendererState()
{
	return this->_rendererState;
}
const std::vector<char>& GfxMaterial::getInstanceData() const
{
	return this->_instanceData;
}
std::string GfxMaterial::getRendererStateKey() const
{
	return this->_rendererStateKey;
}
const std::array<float, 16>& GfxMaterial::getInstanceWorldMatrixData() const
{
	return this->_instanceWorldMatrixData;
}
void GfxMaterial::destroy()
{
	
}
GfxMaterial::~GfxMaterial()
{
}