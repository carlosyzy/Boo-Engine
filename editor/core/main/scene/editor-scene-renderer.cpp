#include "editor-scene-renderer.h"

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

EditorSceneRenderer::EditorSceneRenderer()
{
    this->_cameraMatView = Mat4::identity();
    this->_cameraMatProj = Mat4::identity();
}
void EditorSceneRenderer::init(UISprite *target)
{
    this->_target = target;
    this->_initRenderCamera();
}
void EditorSceneRenderer::_initRenderCamera()
{
    this->_cameraUuid = "Editor_Scene_Camera";
    this->_cameraRenderTexture = new GfxRenderTexture(this->_cameraUuid);
    GfxMgr::getInstance()->initRenderQueue(this->_cameraUuid, this->_cameraRenderTexture);
    this->_target->setRenderTexture(this->_cameraRenderTexture);
}
void EditorSceneRenderer::resize(int width, int height)
{
    this->_cameraRenderTextureWidth = width;
    this->_cameraRenderTextureHeight = height;
    this->_cameraRenderTexture->resize(this->_cameraRenderTextureWidth, this->_cameraRenderTextureHeight);
    this->_cameraMatProj.setM00(2.0f / (float)this->_cameraRenderTextureWidth);
    this->_cameraMatProj.setM11(2.0f / (float)this->_cameraRenderTextureHeight);
}
void EditorSceneRenderer::render()
{
    GfxMgr::getInstance()->submitRenderData(this->_cameraUuid, this->_cameraMatView.data(), this->_cameraMatProj.data(), false);
    // // 提交渲染数据
    //  GfxMgr::getInstance()->submitRenderObject(this->_cameraUuid, this->_refLineAssetMtl->getGfxMaterial(), this->_refLineGfxMesh, this->_refLineInstanceData);
}
EditorSceneRenderer::~EditorSceneRenderer()
{
}