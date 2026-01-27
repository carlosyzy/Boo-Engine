// #include "editor-camera.h"
// #include "../../engine/boo.h"
// #include "../../engine/core/gfx/gfx-mgr.h"
// #include "../../engine/core/gfx/base/gfx-render-texture.h"

// EditorCamera::EditorCamera(std::string name, Node *node, std::string uuid) : Camera(name, node, uuid)
// {
// }
// void EditorCamera::_deserialized()
// {
//     Camera::_deserialized();
// }
// void EditorCamera::Awake()
// {
//    Camera::Awake();
// }
// void EditorCamera::_createRenderPipeline()
// {
//     this->_renderTexture = new GfxRenderTexture(this->_uuid);
//     GfxMgr::getInstance()->initRenderQueue(this->_uuid, this->_renderTexture);
//     if (this->_width > 0 && this->_height > 0)
//     {
//         this->_renderTexture->resize(this->_width * 2.0f, this->_height * 2.0f);
//     }
// }
// void EditorCamera::Enable()
// {
//     Camera::Enable();
//     Boo::game->extractCamera(this);
// }
// void EditorCamera::Render()
// {
//     Camera::Render();
//     GfxMgr::getInstance()->submitRenderMat(this->_uuid, this->_matView.data(), this->_matProj.data());
// }
// void EditorCamera::Disable()
// {
//     Camera::Disable();
//     Boo::game->removeCamera(this);
// }
// void EditorCamera::destroy()
// {
//     Camera::destroy();
// }

// EditorCamera::~EditorCamera()
// {
// }
