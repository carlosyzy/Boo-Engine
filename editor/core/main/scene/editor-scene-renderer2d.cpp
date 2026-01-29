#include "editor-scene-renderer2d.h"

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

EditorSceneRenderer2D::EditorSceneRenderer2D()
{
    this->_refLineWorldMat = Mat4::identity();
}
void EditorSceneRenderer2D::init()
{
    this->_initRefLineGfx();
}
void EditorSceneRenderer2D::_initRefLineGfx()
{
    this->_refLineGfxMesh = new GfxMesh("RefLine_GfxMesh");
    this->_refLineGfxMesh->setInputVertices({-0.5f, 0.5f, 0.0f, 0.0f, 0.0f,
                                             -0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
                                             0.5f, -0.5f, 0.0f, 1.0f, 1.0f,
                                             0.5f, 0.5f, 0.0f, 1.0f, 0.0f},
                                            {0, 1, 2, 0, 2, 3});
    this->_refLineAssetMtl = new MaterialAsset();
    this->_refLineAssetMtl->createUITest();
}
void EditorSceneRenderer2D::resize(int width, int height)
{
    std::cout << "EditorSceneRenderer2D::resize: width:" << width << " height:" << height << std::endl;
    this->_refLineWorldMat.setM00(width);
    this->_refLineWorldMat.setM11(height);
}
void EditorSceneRenderer2D::render(std::string cameraUuid)
{
    this->_renderRefLine(cameraUuid);
}
void EditorSceneRenderer2D::_renderRefLine(std::string cameraUuid)
{
    this->_refLineInstanceData.clear();
    this->_refLineInstanceData.reserve(16 + 4);
    const auto &matrix = this->_refLineWorldMat.data();
    this->_refLineInstanceData.insert(this->_refLineInstanceData.end(), matrix.begin(), matrix.end());
    this->_refLineInstanceData.insert(this->_refLineInstanceData.end(), {1.0f, 1.0f, 1.0f, 1.0f});
    GfxMgr::getInstance()->submitRenderObject(cameraUuid, this->_refLineAssetMtl->getGfxMaterial(), this->_refLineGfxMesh, this->_refLineInstanceData);
}

EditorSceneRenderer2D::~EditorSceneRenderer2D()
{
}