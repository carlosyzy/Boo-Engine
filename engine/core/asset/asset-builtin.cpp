#include "asset-builtin.h"
#include "boo.h"
#include "core/asset/texture-asset.h"
#include "core/asset/shader-asset.h"
#include "core/asset/material-asset.h"
#include "core/asset/mesh-asset.h"
#include "core/asset/bmfont-asset.h"

namespace Boo
{

    AssetBuiltin::AssetBuiltin()
    {
    }
    void AssetBuiltin::init()
    {
        this->_initDefaultTextureLogo();
        this->_initDefaultShaderUI();
        this->_initDefaultShaderUIMask();
        this->_initDefaultShaderUnlit();
        this->_initDefaultMaterialUI();
        this->_initDefaultMaterialUIMask();
        this->_initDefaultMaterialUnlit();
        this->_initDefaultMeshUI();
        this->_initDefaultFont();
    }
    void AssetBuiltin::_initDefaultTextureLogo()
    {
        TextureAsset *texture = new TextureAsset(AssetBuiltinTexture::Default, "", AssetBuiltinTexture::Default);
        texture->create(GfxTextureDefault, sizeof(GfxTextureDefault));
        assetManager->getAssetsCache()->addAsset(AssetBuiltinTexture::Default, texture);

        // 默认splash纹理
        TextureAsset *splash = new TextureAsset(AssetBuiltinTexture::Splash, "", AssetBuiltinTexture::Splash);
        splash->create(GfxTextureSplash, sizeof(GfxTextureSplash));
        assetManager->getAssetsCache()->addAsset(AssetBuiltinTexture::Splash, splash);
    }
    void AssetBuiltin::_initDefaultShaderUI()
    {
        ShaderAsset *vert = new ShaderAsset(AssetBuiltinShader::UIVert, "", AssetBuiltinShader::UIVert);
        // vert->create(ShaderVertexAsset, std::string(AssetBuiltinShaderUIVert()), {});
        std::vector<uint32_t> vertSPV(std::begin(AssetBuiltinShaderUIVert), std::end(AssetBuiltinShaderUIVert));
        vert->create(vertSPV);
        assetManager->getAssetsCache()->addAsset(AssetBuiltinShader::UIVert, vert);
        ShaderAsset *frag = new ShaderAsset(AssetBuiltinShader::UIFrag, "", AssetBuiltinShader::UIFrag);
        // frag->create(ShaderFragmentAsset, std::string(AssetBuiltinShaderUIFrag()), {});
        std::vector<uint32_t> fragSPV(std::begin(AssetBuiltinShaderUIFrag), std::end(AssetBuiltinShaderUIFrag));
        frag->create(fragSPV);
        assetManager->getAssetsCache()->addAsset(AssetBuiltinShader::UIFrag, frag);
    }
    void AssetBuiltin::_initDefaultShaderUIMask()
    {
        ShaderAsset *vert = new ShaderAsset(AssetBuiltinShader::UIMaskVert, "", AssetBuiltinShader::UIMaskVert);
        // vert->create(ShaderVertexAsset, std::string(AssetBuiltinShaderUIMaskVert()), {});
        std::vector<uint32_t> vertSPV(std::begin(AssetBuiltinShaderUIMaskVert), std::end(AssetBuiltinShaderUIMaskVert));
        vert->create(vertSPV);
        assetManager->getAssetsCache()->addAsset(AssetBuiltinShader::UIMaskVert, vert);
        ShaderAsset *frag = new ShaderAsset(AssetBuiltinShader::UIMaskFrag, "", AssetBuiltinShader::UIMaskFrag);
        // frag->create(ShaderFragmentAsset, std::string(AssetBuiltinShaderUIMaskFrag()), {});
        std::vector<uint32_t> fragSPV(std::begin(AssetBuiltinShaderUIMaskFrag), std::end(AssetBuiltinShaderUIMaskFrag));
        frag->create(fragSPV);
        assetManager->getAssetsCache()->addAsset(AssetBuiltinShader::UIMaskFrag, frag);
    }

    void AssetBuiltin::_initDefaultShaderUnlit()
    {
        ShaderAsset *vert = new ShaderAsset(AssetBuiltinShader::UnlitVert, "", AssetBuiltinShader::UnlitVert);
        // vert->create(ShaderVertexAsset, std::string(AssetBuiltinShaderUnlitVert()), {});
        std::vector<uint32_t> vertSPV(std::begin(AssetBuiltinShaderUnlitVert), std::end(AssetBuiltinShaderUnlitVert));
        vert->create(vertSPV);
        assetManager->getAssetsCache()->addAsset(AssetBuiltinShader::UnlitVert, vert);
        ShaderAsset *frag = new ShaderAsset(AssetBuiltinShader::UnlitFrag, "", AssetBuiltinShader::UnlitFrag);
        // frag->create(ShaderFragmentAsset, std::string(AssetBuiltinShaderUnlitFrag()), {});
        std::vector<uint32_t> fragSPV(std::begin(AssetBuiltinShaderUnlitFrag), std::end(AssetBuiltinShaderUnlitFrag));
        frag->create(fragSPV);
        assetManager->getAssetsCache()->addAsset(AssetBuiltinShader::UnlitFrag, frag);
    }
    void AssetBuiltin::_initDefaultMaterialUI()
    {
        MaterialAsset *materialUI = new MaterialAsset(AssetBuiltinMaterial::UI, "", AssetBuiltinMaterial::UI);
        json uiMaterial = json::parse(AssetBuiltinMaterialUI);
        materialUI->create(uiMaterial);
        assetManager->getAssetsCache()->addAsset(AssetBuiltinMaterial::UI, materialUI);
    }
    void AssetBuiltin::_initDefaultMaterialUIMask()
    {
        // 默认UI遮罩-add材质
        MaterialAsset *materialUIMaskAdd = new MaterialAsset(AssetBuiltinMaterial::UIMaskAdd, "", AssetBuiltinMaterial::UIMaskAdd);
        json uimaskAddMaterialAdd = json::parse(AssetBuiltinMaterialUIMaskAdd);
        materialUIMaskAdd->create(uimaskAddMaterialAdd);
        assetManager->getAssetsCache()->addAsset(AssetBuiltinMaterial::UIMaskAdd, materialUIMaskAdd);
        // 默认UI遮罩-sub材质
        MaterialAsset *materialUIMaskSub = new MaterialAsset(AssetBuiltinMaterial::UIMaskSub, "", AssetBuiltinMaterial::UIMaskSub);
        json uimaskSubMaterialSub = json::parse(AssetBuiltinMaterialUIMaskSub);
        materialUIMaskSub->create(uimaskSubMaterialSub);
        assetManager->getAssetsCache()->addAsset(AssetBuiltinMaterial::UIMaskSub, materialUIMaskSub);
    }
    void AssetBuiltin::_initDefaultMaterialUnlit()
    {
        MaterialAsset *materialUnlit = new MaterialAsset(AssetBuiltinMaterial::Unlit, "", AssetBuiltinMaterial::Unlit);
        json unlitMaterial = json::parse(AssetBuiltinMaterialUnlit);
        materialUnlit->create(unlitMaterial);
        assetManager->getAssetsCache()->addAsset(AssetBuiltinMaterial::Unlit, materialUnlit);
    }
    void AssetBuiltin::_initDefaultMeshUI()
    {
        MeshAsset *meshUI = new MeshAsset(AssetBuiltinMesh::UI, "", AssetBuiltinMesh::UI);
        FMeshPrimitive primitive;
        primitive.index = 0;
        primitive.mode = EMeshMode::UI;
        primitive._positions = {-0.5f, 0.5f, 0.0f,
                                -0.5f, -0.5f, 0.0f,
                                0.5f, -0.5f, 0.0f,
                                0.5f, 0.5f, 0.0f};
        primitive._uvs = {0.0f, 0.0f,
                          0.0f, 1.0f,
                          1.0f, 1.0f,
                          1.0f, 0.0f};
        primitive._indices = {0, 1, 2, 0, 2, 3};
        meshUI->create({primitive});
        assetManager->getAssetsCache()->addAsset(AssetBuiltinMesh::UI, meshUI);
    }
    void AssetBuiltin::_initDefaultFont()
    {
        // 默认字体图集纹理
        TextureAsset *fontAtlas = new TextureAsset(AssetBuiltinFont::FontAtlas, "", AssetBuiltinFont::FontAtlas);
        fontAtlas->create(GfxTextureFontAtlas, sizeof(GfxTextureFontAtlas));
        assetManager->getAssetsCache()->addAsset(AssetBuiltinFont::FontAtlas, fontAtlas);
        // 默认字体文件
        BMFontAsset *bmFont = new BMFontAsset(AssetBuiltinFont::Font);
        bmFont->create(AssetBuiltinFontContent);
        assetManager->getAssetsCache()->addAsset(AssetBuiltinFont::Font, bmFont);
    }

    AssetBuiltin::~AssetBuiltin()
    {
    }

} // namespace Boo