#pragma once
#include "asset.h"
#include <string>
#include <iostream>
#include <fstream>
#include <map>
#include "core/util/json-util.h"
#include "core/gfx/base/gfx-material.h"
namespace Boo
{
    class TextureAsset;
    class ShaderAsset;
    
    class MaterialAsset : public Asset
    {
    private:
        json _originData;
        GfxMaterial *_gfxMaterial;
        ShaderAsset *_vertex;
        ShaderAsset *_fragment;
        std::vector<GfxMaterialDataBlock> _properties;
        std::map<std::string, FMaterialTextureBlock> _textures;
        void _parse();
        void _parseProperties();
        void _parseTextures();
        void _parseRendererState();

    public:
        MaterialAsset();
        MaterialAsset(std::string uuid);
        MaterialAsset(std::string uuid, std::string path, std::string name);

        void create(const json &materialData);
        void create(MaterialAsset *mtl);
        const json &getOriginData();
        void setVertexShader(ShaderAsset *vertex);
        void setFragmentShader(ShaderAsset *fragment);
        void setVertexShader(const std::string &vert);
        void setFragmentShader(const std::string &frag);

        /**
         * @brief 更新模型矩阵
         * @param modelMatrix 模型矩阵
         */
        void setModelWorldMatrix(const std::array<float, 16> &modelMatrix);
        /**
         * @brief 更新模型世界矩阵的逆转置矩阵
         * @param modelMatrixIT 模型世界矩阵的逆转置矩阵
         */
        void setModelWorldMatrixIT(const std::array<float, 16> &modelMatrixIT);
        void setUIColor(float r, float g, float b, float w);
        void setTexture(TextureAsset *texture);
        void setTexture(const std::string &key, TextureAsset *texture);

        /**
         * @brief 获取图形材质
         * @return GfxMaterial* 图形材质
         */
        GfxMaterial *getGfxMaterial();
        /**
         * @brief 创建图形资源
         * 参数 宏定义
         */
        virtual void destroy() override;
        ~MaterialAsset() {}
    };

} // namespace Boo
