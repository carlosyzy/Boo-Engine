
#include "editor-scene.h"
#include "../../../../engine/core/assets/material-asset.h"
#include "../../../../engine/core/scene/scene.h"
#include "../../../../engine/core/utils/time-util.h"
#include "../../../../engine/core/scene/node.h"
#include "../../../../engine/core/renderer/ui/ui-sprite.h"
#include "../../../../engine/core/scene/node-2d.h"
#include "../../../../engine/core/gfx/gfx-mgr.h"
#include "../../../../engine/core/gfx/base/gfx-mesh.h"
#include "../../../../engine/core/gfx/base/gfx-material.h"
#include "../../../../engine/core/gfx/base/gfx-render-texture.h"

EditorScene::EditorScene(Node2D *root)
{
    this->_rootNode = root;
    this->_uiSprite = dynamic_cast<UISprite *>(this->_rootNode->getComponent("UISprite"));
    this->_viewWidth = this->_rootNode->getSize().getWidth();
    this->_viewHeight = this->_rootNode->getSize().getHeight();
    this->_cameraMatView = Mat4::identity();
    this->_cameraMatProj = Mat4::identity();
    this->_refLineWorldMat = Mat4::identity();
}
void EditorScene::init()
{
    this->_initRenderCamera();
    this->_initRefLineGfx();
   
}
void EditorScene::_initRenderCamera()
{
    this->_cameraUuid = "Editor_Scene_Camera";
    this->_cameraRenderTexture = new GfxRenderTexture(this->_cameraUuid);
    GfxMgr::getInstance()->initRenderQueue(this->_cameraUuid, this->_cameraRenderTexture);
    this->_cameraRenderTextureWidth = this->_viewWidth;
    this->_cameraRenderTextureHeight = this->_viewHeight;
    this->_cameraRenderTexture->resize(this->_cameraRenderTextureWidth, this->_cameraRenderTextureHeight);
    this->_cameraMatProj.setM00(2.0f / (float)this->_cameraRenderTextureWidth);
    this->_cameraMatProj.setM11(2.0f / (float)this->_cameraRenderTextureHeight);
    //设置渲染rt到uiSprite
    this->_uiSprite->setRenderTexture(this->_cameraRenderTexture);
}
void EditorScene::_initRefLineGfx()
{
    //
    this->_refLineGfxMesh = new GfxMesh("RefLine_GfxMesh");
    this->_refLineGfxMesh->setInputVertices({-0.5f, 0.5f, 0.0f, 0.0f, 0.0f,
                                             -0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
                                             0.5f, -0.5f, 0.0f, 1.0f, 1.0f,
                                             0.5f, 0.5f, 0.0f, 1.0f, 0.0f},
                                            {0, 1, 2, 0, 2, 3});
    this->_refLineAssetMtl = new MaterialAsset();
    this->_refLineAssetMtl->createUITest();
    //
    this->_refLineWorldMat.setM00(this->_cameraRenderTextureWidth);
    this->_refLineWorldMat.setM11(this->_cameraRenderTextureHeight);
    //
    this->_refLineInstanceData.clear();
    this->_refLineInstanceData.reserve(16 + 4);
    const auto &matrix = this->_refLineWorldMat.data();
    this->_refLineInstanceData.insert(this->_refLineInstanceData.end(), matrix.begin(), matrix.end());
    this->_refLineInstanceData.insert(this->_refLineInstanceData.end(), {1.0f, 1.0f, 1.0f, 1.0f});
}
void EditorScene::update(float deltaTime)
{
    this->_checkViewSizeChange();
    this->_updateRenderCamera();
}
void EditorScene::_checkViewSizeChange()
{
    float viewWidth = this->_rootNode->getSize().getWidth();
    float viewHeight = this->_rootNode->getSize().getHeight();
    if (this->_viewWidth != viewWidth || this->_viewHeight != viewHeight)
    {
        this->_viewWidth = viewWidth;
        this->_viewHeight = viewHeight;
        this->_isViewChanged = true;
        this->_viewChangedTime = TimeUtil::nowTime();
    };
}
void EditorScene::_updateRenderCamera()
{
    if (this->_isViewChanged)
    {
        if (TimeUtil::nowTime() - this->_viewChangedTime >= 100)
        {
            this->_isViewChanged = false;
            this->_cameraRenderTextureWidth = this->_viewWidth;
            this->_cameraRenderTextureHeight = this->_viewHeight;
            this->_cameraRenderTexture->resize(this->_cameraRenderTextureWidth, this->_cameraRenderTextureHeight);
            this->_cameraMatProj.setM00(2.0f / (float)this->_cameraRenderTextureWidth);
            this->_cameraMatProj.setM11(2.0f / (float)this->_cameraRenderTextureHeight);
        }
    }
    GfxMgr::getInstance()->submitRenderData(this->_cameraUuid, this->_cameraMatView.data(), this->_cameraMatProj.data(), false);
    // 提交渲染数据
    GfxMgr::getInstance()->submitRenderObject(this->_cameraUuid, this->_refLineAssetMtl->getGfxMaterial(), this->_refLineGfxMesh, this->_refLineInstanceData);
    
}

EditorScene::~EditorScene()
{
}