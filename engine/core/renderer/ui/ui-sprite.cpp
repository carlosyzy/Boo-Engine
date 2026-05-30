#include "ui-sprite.h"
#include "boo.h"
#include "log.h"
#include "core/scene/node-2d.h"
#include "core/renderer/camera.h"
#include "core/asset/asset-manager.h"
#include "core/asset/texture-asset.h"
#include "core/asset/mesh-asset.h"
#include "core/asset/material-asset.h"
#include "core/gfx/gfx.h"
#include "core/gfx/gfx-manager.h"
#include "core/gfx/base/gfx-mesh.h"
#include "core/gfx/base/gfx-material.h"

namespace Boo
{

    UISprite::UISprite(std::string name, Node *node, std::string uuid) : UIRenderer(name, node, uuid)
    {
        this->sizeMode = ESizeMode::Custom;
        // 默认纹理
        this->_textureAsset = nullptr;
        this->_materialAsset = nullptr;
        this->_meshAsset = nullptr;
        // 默认UI材质
        this->_createDefaultMaterial();
        // 默认UI网格
        this->_createDefaultMesh();
    }
    void UISprite::_createDefaultMaterial()
    {
        this->_materialAsset = new MaterialAsset();
        MaterialAsset *mtl = dynamic_cast<MaterialAsset *>(assetManager->getAsset(AssetBuiltinMaterial::UI));
        this->_materialAsset->create(mtl);
    }

    void UISprite::_createDefaultMesh()
    {
        this->_meshAsset = dynamic_cast<MeshAsset *>(assetManager->getAsset(AssetBuiltinMesh::UI));
    }

    void UISprite::OnAwake()
    {
        UIRenderer::OnAwake();
    }
    void UISprite::setProperty(json &data)
    {
        UIRenderer::setProperty(data);
        // set texture asset
        if (data.contains("texture") && data["texture"].is_string())
        {
            this->setTexture(data["texture"].get<std::string>());
        }
        // set material asset
        if (data.contains("material") && data["material"].is_string())
        {
            this->setMaterial(data["material"].get<std::string>());
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
        // set alpha
        if (data.contains("alpha") && data["alpha"].is_number())
        {
            this->setAlpha(data["alpha"].get<float>());
        }
        // set size mode
        if (data.contains("sizeMode") && data["sizeMode"].is_number_integer())
        {
            ESizeMode sizeMode = ESizeMode(data["sizeMode"].get<int>());
            this->setSizeMode(sizeMode);
        }
    }

    void UISprite::OnEnable()
    {
        UIRenderer::OnEnable();
    }

    void UISprite::setColor(Color &color)
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
    void UISprite::setColor(std::string color)
    {
        if (color == this->_color.hexString())
        {
            return;
        }
        Color c(color);
        this->setColor(c.getR(), c.getG(), c.getB(), c.getA());
    }
    void UISprite::setAlpha(float alpha)
    {
        if (alpha == this->_color.getA())
            return;
        Color c(this->_color);
        c.setA(alpha);
        this->setColor(c.getR(), c.getG(), c.getB(), c.getA());
    }
    void UISprite::setColor(float r, float g, float b, float a)
    {
        if (r == this->_color.getR() &&
            g == this->_color.getG() &&
            b == this->_color.getB() &&
            a == this->_color.getA())
        {
            return;
        }
        this->_color.set(r, g, b, a);
        this->_materialAsset->setUIColor(r, g, b, a);
    }

    void UISprite::setMaterial(std::string material)
    {
        MaterialAsset *mtl = dynamic_cast<MaterialAsset *>(assetManager->getAsset(material, true));
        if (mtl == nullptr)
        {
            LOGW("[UISprite]:setMaterial: material %s not found", material.c_str());
            return;
        }
        this->setMaterial(mtl);
    }
    void UISprite::setMaterial(MaterialAsset *material)
    {
        if (material == nullptr)
        {
            LOGW("[UISprite]:setMaterial: material %s not found", material->getName().c_str());
            return;
        }
        this->_materialAsset->create(material);
    }

    void UISprite::setTexture(std::string path)
    {
        TextureAsset *texture = dynamic_cast<TextureAsset *>(assetManager->getAsset(path, true));
        if (texture == nullptr)
        {
            LOGW("[UISprite]:setTexture: texture %s not found", path.c_str());
            return;
        }
        this->setTexture(texture);
    }
    void UISprite::setTexture(TextureAsset *texture)
    {
        if (this->_textureAsset == texture)
        {
            return;
        }
        this->_textureAsset = texture;
        if (this->_textureAsset == nullptr)
        {
            return;
        }
        if (this->sizeMode == ESizeMode::Raw)
        {
            float width = this->_textureAsset->getWidth();
            float height = this->_textureAsset->getHeight();
            this->_node2D->removeSizeLock(ENode2DSizeLock::SpriteRaw);
            this->_node2D->setSize(width, height);
            this->_node2D->addSizeLock(ENode2DSizeLock::SpriteRaw);
        }
        if (this->_materialAsset == nullptr)
        {
            return;
        }
        this->_materialAsset->setTexture(texture);
    }

    void UISprite::setSizeMode(ESizeMode sizeMode)
    {
        if (this->sizeMode == sizeMode)
        {
            return;
        }
        this->sizeMode = sizeMode;
        if (this->sizeMode == ESizeMode::Raw)
        {
            if (this->_textureAsset != nullptr)
            {
                float width = this->_textureAsset->getWidth();
                float height = this->_textureAsset->getHeight();
                this->_node2D->removeSizeLock(ENode2DSizeLock::SpriteRaw);
                this->_node2D->setSize(width, height);
            }
            this->_node2D->addSizeLock(ENode2DSizeLock::SpriteRaw);
        }
        else
        {
            this->_node2D->removeSizeLock(ENode2DSizeLock::SpriteRaw);
        }
    }

    void UISprite::Update(float deltaTime)
    {
        UIRenderer::Update(deltaTime);
    }
    void UISprite::LateUpdate(float deltaTime)
    {
        UIRenderer::LateUpdate(deltaTime);
    }
    void UISprite::Render(Camera *camera)
    {
        if (!this->_canRenderer(camera))
        {
            return;
        }
        if (this->_materialAsset == nullptr)
        {
            return;
        }
        if (this->_meshAsset == nullptr)
        {
            return;
        }
        UIRenderer::Render(camera);
        // 后续添加节点层级alpha测试
        const Mat4 &matrix = this->_node2D->getUIWorldMatrix();
        Mat4::multiply(matrix, view->getFitMatrix(), this->_uiViewMatrix);
        // 更新世界矩阵
        this->_materialAsset->setModelWorldMatrix(this->_uiViewMatrix.data());
        GfxManager::getInstance()->submitRenderObject(camera->getUuid(), this->_materialAsset->getGfxMaterial(), this->_meshAsset->getGfxMesh(0));
        // 增加渲染物体数量
        profiler->addObjectCount(1);
    }
    void UISprite::OnDisable()
    {
        UIRenderer::OnDisable();
    }

    void UISprite::destroy()
    {
        UIRenderer::destroy();
    }
    UISprite::~UISprite()
    {
        if (this->_materialAsset != nullptr)
        {
            this->_materialAsset->destroy();
            delete this->_materialAsset;
        }
        this->_materialAsset = nullptr;
        this->_meshAsset = nullptr;
        this->_textureAsset=nullptr;
    }

} // namespace Boo