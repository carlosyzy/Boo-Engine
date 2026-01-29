
#include "editor-scene.h"

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

EditorScene::EditorScene(Node2D *root)
{
    this->_rootNode = root;
    this->_uiSprite = dynamic_cast<UISprite *>(this->_rootNode->getComponent("UISprite"));
    this->_viewWidth = this->_rootNode->getSize().getWidth();
    this->_viewHeight = this->_rootNode->getSize().getHeight();
}
void EditorScene::init()
{
    this->_renderer = new EditorSceneRenderer();
    this->_renderer->init(this->_uiSprite);
    this->_renderer->resize(this->_viewWidth, this->_viewHeight);
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
            this->_renderer->resize(this->_viewWidth, this->_viewHeight);
        }
    }
    this->_renderer->render();
}

EditorScene::~EditorScene()
{
}