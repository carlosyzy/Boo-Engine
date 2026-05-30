#pragma once
// #include <vulkan/vulkan_core.h>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <vector>
#include <set>
#include <map>
#include <unordered_map>
#include <cstdint>
#include <array>
#include "../gfx-struct.h"

enum class GfxMaterialPropertyType {
    Int,
    Float,
    Vec2,
    Vec3,
    Vec4,
    Mat4,
};
struct GfxMaterialDataBlock {
    GfxMaterialPropertyType type;
    std::string key;
    size_t offset;  // 相对于HEADER的偏移
    size_t size;
    void * data;
};
class GfxMaterial
{
private:
    GfxRendererState _rendererState;
    std::string _rendererStateKey;
    /**
     * 实例数据
     */
    std::unordered_map<std::string, GfxMaterialDataBlock> _instanceBlocks;
    std::vector<char> _instanceData;
    std::array<float, 16> _instanceWorldMatrixData;

    /**
     * @brief 自定义属性
     */
    std::unordered_map<std::string, GfxMaterialDataBlock> _propertyBlocks;
    std::vector<char> _propertyData;
    size_t _propertySize = 0;
    std::string _propertyKey="";
    /**
     * 纹理
     */
    std::vector<std::string> _textures;
    void _initUIInstanceDatas();
    void _init3DInstanceDatas();
    void _addBytes(std::vector<char> &_originData, const void *data, size_t size);

public:
    GfxMaterial();
    void create(GfxRendererLayer layer, GfxRendererType type, const std::string &vert, const std::string &frag);
    void setVertexShader(const std::string &vert);
    void setFragmentShader(const std::string &frag);
    void setProperties(std::vector<GfxMaterialDataBlock> &properties);
    void setPolygonMode(GfxRendererStatePolygonMode polygonMode);
    void setCullMode(GfxRendererStateCullMode cullMode);
    void setDepthTest(int depthTest);
    void setDepthWrite(int depthWrite);
    void setDepthCompareOp(GfxRendererStateCompareOp depthCompareOp);
    void setStencilTest(int stencilTest);
    void setStencilFrontCompareOp(GfxRendererStateCompareOp stencilFrontCompareOp);
    void setStencilFrontFailOp(GfxRendererStateStencilOp stencilFrontFailOp);
    void setStencilFrontDepthFailOp(GfxRendererStateStencilOp stencilFrontDepthFailOp);
    void setStencilFrontPassOp(GfxRendererStateStencilOp stencilFrontPassOp);
    void setStencilFrontCompareMask(uint32_t _stencilFrontCompareMask);
    void setStencilFrontWriteMask(uint32_t _stencilFrontWriteMask);
    void setStencilFrontRreference(uint32_t _stencilFrontRreference);
    void setStencilBackCompareOp(GfxRendererStateCompareOp stencilBackCompareOp);
    void setStencilBackFailOp(GfxRendererStateStencilOp stencilBackFailOp);
    void setStencilBackDepthFailOp(GfxRendererStateStencilOp stencilBackDepthFailOp);
    void setStencilBackPassOp(GfxRendererStateStencilOp stencilBackPassOp);
    void setStencilBackCompareMask(uint32_t _stencilBackCompareMask);
    void setStencilBackWriteMask(uint32_t _stencilBackWriteMask);
    void setStencilBackRreference(uint32_t _stencilBackRreference);
    void setColorBlend(int colorBlend);
    void setSrcColorBlendFactor(GfxRendererStateColorBlendFactor srcColorBlendFactor);
    void setDstColorBlendFactor(GfxRendererStateColorBlendFactor dstColorBlendFactor);
    void setColorBlendOp(GfxRendererStateColorBlendOp colorBlendOp);
    void setSrcAlphaBlendFactor(GfxRendererStateColorBlendFactor srcAlphaBlendFactor);
    void setDstAlphaBlendFactor(GfxRendererStateColorBlendFactor dstAlphaBlendFactor);
    void setAlphaBlendOp(GfxRendererStateColorBlendOp alphaBlendOp);

    void setTexture(const int index, const std::string &texture);
    void setModelWorldMatrix(const std::array<float, 16> &modelWorldMatrix);
    void setModelWorldMatrixIT(const std::array<float, 16> &modelWorldMatrixIT);
    void setUIColor(const float r, const float g, const float b, const float a);
    void setPropertyInt(const std::string &key, const int value);
    void setPropertyFloat(const std::string &key, const float value);
    void setPropertyVec2(const std::string &key, const float x, const float y);
    void setPropertyVec3(const std::string &key, const float x, const float y, const float z);
    void setPropertyVec4(const std::string &key, const float x, const float y, const float z, const float w);
     /**
     * @brief 比较材质是否相等
     *
     * @param material
     * @return true
     * @return false
     */
    bool equals(const GfxMaterial *material) const;
    GfxRendererState &getRendererState();
    std::string getRendererStateKey() const;
    const std::vector<char> &getInstanceData() const;
    const std::vector<std::string> &getTextures() const;
    const std::array<float, 16> &getInstanceWorldMatrixData() const;

    void destroy();

    ~GfxMaterial();
};