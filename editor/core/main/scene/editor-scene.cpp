
#include "editor-scene.h"

#include "../../../../engine/core/scene/scene.h"
#include "../../../../engine/core/scene/node.h"
#include "../../../../engine/core/scene/node-2d.h"
#include "../../../../engine/core/gfx/gfx-mgr.h"
#include "../../../../engine/core/gfx/base/gfx-render-texture.h"

EditorScene::EditorScene(Node2D *root)
{
    this->_rootNode = root;
}
void EditorScene::init()
{
    this->_initRenderCamera();
}
void EditorScene::_initRenderCamera()
{
    this->_cameraUuid = "Editor_Scene_Camera";
    this->_cameraRenderTexture = new GfxRenderTexture(this->_cameraUuid);
    GfxMgr::getInstance()->initRenderQueue(this->_cameraUuid, this->_cameraRenderTexture);
    this->_cameraRenderTexture->resize(this->_cameraRenderTextureWidth, this->_cameraRenderTextureHeight);
}
void EditorScene::update(float deltaTime)
{
}

EditorScene::~EditorScene()
{
}