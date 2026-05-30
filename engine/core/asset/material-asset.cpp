#include "material-asset.h"
#include "texture-asset.h"
#include "shader-asset.h"
#include "log.h"
#include "boo.h"
#include "core/util/uuid-util.h"
#include "core/gfx/gfx-manager.h"

namespace Boo
{
    MaterialAsset::MaterialAsset() : Asset()
    {
        this->_type = EAssetType::Material;
        this->_uuid = UuidUtil::generateUUID();
        this->_gfxMaterial = new GfxMaterial();
        this->_originData = json::object();
    }
    MaterialAsset::MaterialAsset(std::string uuid) : Asset(uuid)
    {
        this->_type = EAssetType::Material;
        this->_gfxMaterial = new GfxMaterial();
        this->_originData = json::object();
    }
    MaterialAsset::MaterialAsset(std::string uuid, std::string path, std::string name) : Asset(uuid, path, name)
    {
        this->_type = EAssetType::Material;
        this->_gfxMaterial = new GfxMaterial();
        this->_originData = json::object();
    }
    void MaterialAsset::create(MaterialAsset *mtl)
    {
        if (mtl == nullptr)
        {
            LOGE("MaterialAsset::create(MaterialAsset *mtl) mtl is nullptr");
            return;
        }
        this->create(mtl->getOriginData());
    }
    void MaterialAsset::create(const json &materialData)
    {
        this->_originData = materialData;
        this->_parse();
    }
    void MaterialAsset::_parse()
    {
        if (!this->_originData.contains("layer"))
        {
            LOGW("MaterialAsset::_parse() layer is not found");
            return;
        }

        int layer = this->_originData["layer"].get<int>();
        if (layer != int(ERendererLayer::UI) && layer != int(ERendererLayer::Model))
        {
            LOGE("MaterialAsset::_parse(json &materialData) layer is not UI or 3D");
            return;
        }
        if (!this->_originData.contains("type"))
        {
            LOGW("MaterialAsset::_parse() type is not found");
            return;
        }
        int type = this->_originData["type"].get<int>();
        if (type != int(ERendererType::Opaque) && type != int(ERendererType::Transparent))
        {
            LOGE("MaterialAsset::_parse(json &materialData) type is not Opaque or Transparent");
            return;
        }
        std::string vert = "";
        if (this->_originData.contains("vert"))
        {
            vert = this->_originData["vert"].get<std::string>();
        }
        if (!vert.empty())
        {
            this->_vertex = dynamic_cast<ShaderAsset *>(assetManager->loadAsset(vert));
        }
        std::string frag = "";
        if (this->_originData.contains("frag"))
        {
            frag = this->_originData["frag"].get<std::string>();
        }
        if (!frag.empty())
        {
            this->_fragment = dynamic_cast<ShaderAsset *>(assetManager->loadAsset(frag));
        }

        this->_gfxMaterial->create(GfxRendererLayer(layer), GfxRendererType(type), vert, frag);
        this->_parseProperties();
        this->_parseTextures();
        this->_parseRendererState();
    }
    void MaterialAsset::_parseProperties()
    {
        if (!this->_originData.contains("properties"))
        {
            LOGW("MaterialAsset::_parseProperties() properties is not found");
            return;
        }
        this->_properties.clear();
        json &properties = this->_originData["properties"];
        if (properties.is_array())
        {
            for (int i = 0; i < properties.size(); i++)
            {
                json &param = properties[i];
                std::string name = param["name"];
                std::string type = param["type"];
                if (type == "int")
                {
                    int value = param["value"].get<int>();
                    GfxMaterialDataBlock block = {};
                    block.key = name;
                    block.offset = 0;
                    block.size = sizeof(int);
                    block.type = GfxMaterialPropertyType::Int;
                    block.data = new int(value);
                    this->_properties.push_back(block);
                }
                else if (type == "float")
                {
                    float value = param["value"].get<float>();
                    GfxMaterialDataBlock block = {};
                    block.key = name;
                    block.offset = 0;
                    block.size = sizeof(float);
                    block.type = GfxMaterialPropertyType::Float;
                    block.data = new float(value);
                    this->_properties.push_back(block);
                }
                else if (type == "vec2")
                {
                    float x = param["value"][0];
                    float y = param["value"][1];
                    float *value = new float[2]; // ✅ 动态分配数组
                    value[0] = x;
                    value[1] = y;
                    GfxMaterialDataBlock block = {};
                    block.key = name;
                    block.offset = 0;
                    block.size = sizeof(value);
                    block.type = GfxMaterialPropertyType::Vec2;
                    block.data = value;
                    this->_properties.push_back(block);
                }
                else if (type == "vec3")
                {
                    float x = param["value"][0];
                    float y = param["value"][1];
                    float z = param["value"][2];
                    float *value = new float[3]; // ✅ 动态分配数组
                    value[0] = x;
                    value[1] = y;
                    value[2] = z;
                    GfxMaterialDataBlock block = {};
                    block.key = name;
                    block.offset = 0;
                    block.size = sizeof(value);
                    block.type = GfxMaterialPropertyType::Vec3;
                    block.data = value;
                    this->_properties.push_back(block);
                }
                else if (type == "vec4")
                {
                    float x = param["value"][0];
                    float y = param["value"][1];
                    float z = param["value"][2];
                    float w = param["value"][3];
                    float *value = new float[4]; // ✅ 动态分配数组
                    value[0] = x;
                    value[1] = y;
                    value[2] = z;
                    value[3] = w;
                    GfxMaterialDataBlock block = {};
                    block.key = name;
                    block.offset = 0;
                    block.size = sizeof(value);
                    block.type = GfxMaterialPropertyType::Vec4;
                    block.data = value;
                    this->_properties.push_back(block);
                }
            }
        }
        this->_gfxMaterial->setProperties(this->_properties);
    }
    void MaterialAsset::_parseTextures()
    {
        if (!this->_originData.contains("textures"))
        {
            return;
        }
        json &textures = this->_originData["textures"];
        if (textures.is_object())
        {
            for (auto &texture : textures.items()) // items() 返回键值对迭代器
            {
                std::string key = texture.key(); // ✅ 正确
                json &data = texture.value();    // ✅ 正确
                if (!data.contains("binding") || !data.contains("path"))
                {
                    LOGW("MaterialAsset::_parseTextures() textures %s is not found binding or path", key.c_str());
                    continue;
                }
                int binding = data["binding"];
                std::string path = data["path"];
                // 加载关联图片资产
                Asset *textureAsset = assetManager->loadAsset(path);
                if (binding <= 0)
                {
                    LOGW("MaterialAsset::_parseTextures() textures %s binding is invalid", key.c_str());
                    continue;
                }
                FMaterialTextureBlock textureBlock = {};
                textureBlock.binding = binding;
                textureBlock.key = key;
                textureBlock.path = path;
                this->_textures[key] = textureBlock;
                this->_gfxMaterial->setTexture(binding - 1, path);
            }
        }
    }
    void MaterialAsset::_parseRendererState()
    {
        // 多边形模式
        if (this->_originData.contains("polygonMode"))
        {
            GfxRendererStatePolygonMode polygonMode = (GfxRendererStatePolygonMode)this->_originData["polygonMode"].get<int>();
            this->_gfxMaterial->setPolygonMode(polygonMode);
        }
        // 剔除模式
        if (this->_originData.contains("cullMode"))
        {
            GfxRendererStateCullMode cullMode = (GfxRendererStateCullMode)this->_originData["cullMode"].get<int>();
            this->_gfxMaterial->setCullMode(cullMode);
        }
        // 深度测试
        if (this->_originData.contains("depthTest"))
        {
            this->_gfxMaterial->setDepthTest(this->_originData["depthTest"].get<int>());
        }
        // 深度写入
        if (this->_originData.contains("depthWrite"))
        {
            this->_gfxMaterial->setDepthWrite(this->_originData["depthWrite"].get<int>());
        }
        // 深度比较操作
        if (this->_originData.contains("depthCompareOp"))
        {
            GfxRendererStateCompareOp compareOp = (GfxRendererStateCompareOp)this->_originData["depthCompareOp"].get<int>();
            this->_gfxMaterial->setDepthCompareOp(compareOp);
        }
        // 模板测试
        if (this->_originData.contains("stencilTest"))
        {
            this->_gfxMaterial->setStencilTest(this->_originData["stencilTest"].get<int>());
        }
        // 正面三角形（逆时针）的Stencil操作
        if (this->_originData.contains("stencilFrontCompareOp"))
        {
            GfxRendererStateCompareOp compareOp = (GfxRendererStateCompareOp)this->_originData["stencilFrontCompareOp"].get<int>();
            this->_gfxMaterial->setStencilFrontCompareOp(compareOp);
        }
        // 正面三角形（逆时针）的Stencil操作
        if (this->_originData.contains("stencilFrontFailOp"))
        {
            GfxRendererStateStencilOp failOp = (GfxRendererStateStencilOp)this->_originData["stencilFrontFailOp"].get<int>();
            this->_gfxMaterial->setStencilFrontFailOp(failOp);
        }
        // 正面三角形（逆时针）的Stencil操作
        if (this->_originData.contains("stencilFrontDepthFailOp"))
        {
            GfxRendererStateStencilOp depthFailOp = (GfxRendererStateStencilOp)this->_originData["stencilFrontDepthFailOp"].get<int>();
            this->_gfxMaterial->setStencilFrontDepthFailOp(depthFailOp);
        }
        // 正面三角形（逆时针）的Stencil操作
        if (this->_originData.contains("stencilFrontPassOp"))
        {
            GfxRendererStateStencilOp passOp = (GfxRendererStateStencilOp)this->_originData["stencilFrontPassOp"].get<int>();
            this->_gfxMaterial->setStencilFrontPassOp(passOp);
        }
        // // 正面三角形（逆时针）的Stencil操作
        // if(originData.contains("stencilFrontCompareMask")){
        //     this->_rendererState.stencilFrontCompareMask = originData["stencilFrontCompareMask"];
        // }else{
        //     this->_rendererState.stencilFrontCompareMask = 0xFFFFFFFF;
        // }
        // // 正面三角形（逆时针）的Stencil操作
        // if(originData.contains("stencilFrontWriteMask")){
        //     this->_rendererState.stencilFrontWriteMask = originData["stencilFrontWriteMask"];
        // }else{
        //     this->_rendererState.stencilFrontWriteMask = 0xFFFFFFFF;
        // }
        // // 正面三角形（逆时针）的Stencil操作
        // if(originData.contains("stencilFrontRreference")){
        //     this->_rendererState.stencilFrontRreference = originData["stencilFrontRreference"];
        // }else{
        //     this->_rendererState.stencilFrontRreference = 0;
        // }
        // 反面三角形（顺时针）的Stencil操作
        if (this->_originData.contains("stencilBackCompareOp"))
        {
            GfxRendererStateCompareOp compareOp = (GfxRendererStateCompareOp)this->_originData["stencilBackCompareOp"].get<int>();
            this->_gfxMaterial->setStencilBackCompareOp(compareOp);
        }
        // 反面三角形（顺时针）的Stencil操作
        if (this->_originData.contains("stencilBackFailOp"))
        {
            GfxRendererStateStencilOp failOp = (GfxRendererStateStencilOp)this->_originData["stencilBackFailOp"].get<int>();
            this->_gfxMaterial->setStencilBackFailOp(failOp);
        }
        // 反面三角形（顺时针）的Stencil操作
        if (this->_originData.contains("stencilBackDepthFailOp"))
        {
            GfxRendererStateStencilOp depthFailOp = (GfxRendererStateStencilOp)this->_originData["stencilBackDepthFailOp"].get<int>();
            this->_gfxMaterial->setStencilBackDepthFailOp(depthFailOp);
        }
        // 正面三角形（逆时针）的Stencil操作
        if (this->_originData.contains("stencilBackPassOp"))
        {
            GfxRendererStateStencilOp passOp = (GfxRendererStateStencilOp)this->_originData["stencilBackPassOp"].get<int>();
            this->_gfxMaterial->setStencilBackPassOp(passOp);
        }
        // 反面三角形（顺时针）的Stencil操作
        // if(originData.contains("stencilBackCompareMask")){
        //     this->_rendererState.stencilBackCompareMask = originData["stencilBackCompareMask"];
        // }else{
        //     this->_rendererState.stencilBackCompareMask = 0xFFFFFFFF;
        // }
        // // 反面三角形（顺时针）的Stencil操作
        // if(originData.contains("stencilBackWriteMask")){
        //     this->_rendererState.stencilBackWriteMask = originData["stencilBackWriteMask"];
        // }else{
        //     this->_rendererState.stencilBackWriteMask = 0xFFFFFFFF;
        // }
        // // 反面三角形（顺时针）的Stencil操作
        // if(originData.contains("stencilBackRreference")){
        //     this->_rendererState.stencilBackRreference = originData["stencilBackRreference"];
        // }else{
        //     this->_rendererState.stencilBackRreference = 0;
        // }

        // 颜色缓和
        // 开关
        if (this->_originData.contains("colorBlend"))
        {
            this->_gfxMaterial->setColorBlend((int)this->_originData["colorBlend"]);
        }
        // 源颜色混合因子
        if (this->_originData.contains("srcColorBlendFactor"))
        {
            GfxRendererStateColorBlendFactor srcColorBlendFactor = (GfxRendererStateColorBlendFactor)this->_originData["srcColorBlendFactor"].get<int>();
            this->_gfxMaterial->setSrcColorBlendFactor(srcColorBlendFactor);
        }
        // 目标颜色混合因子
        if (this->_originData.contains("dstColorBlendFactor"))
        {
            GfxRendererStateColorBlendFactor dstColorBlendFactor = (GfxRendererStateColorBlendFactor)this->_originData["dstColorBlendFactor"].get<int>();
            this->_gfxMaterial->setDstColorBlendFactor(dstColorBlendFactor);
        }
        // 颜色混合操作
        if (this->_originData.contains("colorBlendOp"))
        {
            GfxRendererStateColorBlendOp colorBlendOp = (GfxRendererStateColorBlendOp)this->_originData["colorBlendOp"].get<int>();
            this->_gfxMaterial->setColorBlendOp(colorBlendOp);
        }
        // 源alpha混合因子
        if (this->_originData.contains("srcAlphaBlendFactor"))
        {
            GfxRendererStateColorBlendFactor srcAlphaBlendFactor = (GfxRendererStateColorBlendFactor)this->_originData["srcAlphaBlendFactor"].get<int>();
            this->_gfxMaterial->setSrcAlphaBlendFactor(srcAlphaBlendFactor);
        }
        // 目标alpha混合因子
        if (this->_originData.contains("dstAlphaBlendFactor"))
        {
            GfxRendererStateColorBlendFactor dstAlphaBlendFactor = (GfxRendererStateColorBlendFactor)this->_originData["dstAlphaBlendFactor"].get<int>();
            this->_gfxMaterial->setDstAlphaBlendFactor(dstAlphaBlendFactor);
        }
        // alpha混合操作
        if (this->_originData.contains("alphaBlendOp"))
        {
            GfxRendererStateColorBlendOp alphaBlendOp = (GfxRendererStateColorBlendOp)this->_originData["alphaBlendOp"].get<int>();
            this->_gfxMaterial->setAlphaBlendOp(alphaBlendOp);
        }
    }

    void MaterialAsset::setVertexShader(const std::string &vert)
    {
        if (vert.empty())
        {
            LOGE("MaterialAsset::setVertexShader(const std::string &vert) vert is empty");
            return;
        }
        ShaderAsset *vertex = dynamic_cast<ShaderAsset *>(assetManager->getAsset(vert, true));
        if (vertex == nullptr)
        {
            LOGE("MaterialAsset::setVertexShader(const std::string &vert) vertex is nullptr");
            return;
        }
        this->setVertexShader(vertex);
    }
    void MaterialAsset::setFragmentShader(const std::string &frag)
    {
        if (frag.empty())
        {
            LOGE("MaterialAsset::setFragmentShader(const std::string &frag) frag is empty");
            return;
        }
        ShaderAsset *fragment = dynamic_cast<ShaderAsset *>(assetManager->getAsset(frag, true));
        if (fragment == nullptr)
        {
            LOGE("MaterialAsset::setFragmentShader(const std::string &frag) fragment is nullptr");
            return;
        }
        this->setFragmentShader(fragment);
    }
    void MaterialAsset::setVertexShader(ShaderAsset *vertex)
    {
        if (vertex == nullptr)
        {
            LOGE("MaterialAsset::setVertexShader(ShaderAsset *vertex) vertex is nullptr");
            return;
        }
        if (this->_gfxMaterial == nullptr)
        {
            LOGE("MaterialAsset::setVertexShader(ShaderAsset *vertex) gfxMaterial is nullptr");
            return;
        }
        this->_vertex = vertex;
        this->_originData["vert"] = vertex->getUuid();
        this->_gfxMaterial->setVertexShader(vertex->getUuid());
    }
    void MaterialAsset::setFragmentShader(ShaderAsset *fragment)
    {
        if (fragment == nullptr)
        {
            LOGE("MaterialAsset::setFragmentShader(ShaderAsset *fragment) fragment is nullptr");
            return;
        }
        if (this->_gfxMaterial == nullptr)
        {
            LOGE("MaterialAsset::setFragmentShader(ShaderAsset *fragment) gfxMaterial is nullptr");
            return;
        }
        this->_fragment = fragment;
        this->_originData["frag"] = fragment->getUuid();
        this->_gfxMaterial->setFragmentShader(fragment->getUuid());
    }

    void MaterialAsset::setModelWorldMatrix(const std::array<float, 16> &modelMatrix)
    {
        this->_gfxMaterial->setModelWorldMatrix(modelMatrix);
    }
    /**
     * @brief 设置模型世界矩阵的逆转置矩阵
     * @param modelMatrixIT 模型世界矩阵的逆转置矩阵
     */
    void MaterialAsset::setModelWorldMatrixIT(const std::array<float, 16> &modelMatrixIT)
    {
        this->_gfxMaterial->setModelWorldMatrixIT(modelMatrixIT);
    }
    void MaterialAsset::setUIColor(float r, float g, float b, float w)
    {
        this->_gfxMaterial->setUIColor(r, g, b, w);
    }
    void MaterialAsset::setTexture(TextureAsset *texture)
    {
        if (texture == nullptr)
        {
            LOGE("MaterialAsset::setTexture(const std::string &key, TextureAsset *texture) texture is nullptr");
            return;
        }
        for (auto &[key, textureBlock] : this->_textures)
        {
            if (textureBlock.binding == 1)
            {
                this->_gfxMaterial->setTexture(0, texture->getUuid());
            }
        }
    }
    void MaterialAsset::setTexture(const std::string &key, TextureAsset *texture)
    {
        if (texture == nullptr)
        {
            LOGE("MaterialAsset::setTexture(const std::string &key, TextureAsset *texture) texture is nullptr");
            return;
        }
        if (this->_textures.find(key) == this->_textures.end())
        {
            LOGW("MaterialAsset::setTexture(const std::string &key, TextureAsset *texture) key %s is not found", key.c_str());
            return;
        }
        FMaterialTextureBlock &textureBlock = this->_textures[key];
        int binding = textureBlock.binding;
        this->_gfxMaterial->setTexture(binding - 1, texture->getUuid());
    }

    const json &MaterialAsset::getOriginData()
    {
        return this->_originData;
    }
    GfxMaterial *MaterialAsset::getGfxMaterial()
    {
        return this->_gfxMaterial;
    }
    void MaterialAsset::destroy()
    {
        // 销毁渲染材质
        this->_gfxMaterial->destroy();
        delete this->_gfxMaterial;
        this->_gfxMaterial = nullptr;
        // 销毁着色器 不去销毁,只清空指针
        this->_vertex = nullptr;
        this->_fragment = nullptr;
        // 销毁纹理 不去销毁,只清空指针
        this->_textures.clear();
        // 销毁属性 数据
        for (auto &block : this->_properties)
        {
            if (block.data == nullptr)
                continue;
            if (block.type == GfxMaterialPropertyType::Vec2 ||
                block.type == GfxMaterialPropertyType::Vec3)
            {
                delete[] static_cast<float *>(block.data);
            }
            else if (block.type == GfxMaterialPropertyType::Int)
            {
                delete static_cast<int *>(block.data);
            }
            else
            {
                delete static_cast<float *>(block.data);
            }
            block.data = nullptr;
        }
        this->_properties.clear();
    }

} // namespace Boo