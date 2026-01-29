#pragma once
#include <iostream>
#include "../../../../engine/core/math/mat4.h"

class Scene;
class Node2D;
class GfxRenderTexture;
class MaterialAsset;
class GfxMesh;
class GfxMaterial;
class UISprite;
class EditorSceneRenderer;

class EditorScene
{
private: // view
    Node2D *_rootNode;
    UISprite *_uiSprite = nullptr;
    int _viewWidth = 0;
    int _viewHeight = 0;
    bool _isViewChanged = false;
    long long _viewChangedTime = 0;
    void _checkViewSizeChange();
    void _updateRenderCamera();

private:
    EditorSceneRenderer *_renderer = nullptr;
public:
    EditorScene(Node2D *root);
    void init();
    void update(float deltaTime);
    // void openScene(Scene *scene);

    ~EditorScene();
};

/**
 * 有两个摄像机
 * 一个是2D_Editor摄像机，用于编辑器渲染2D元素
 * 一个是3D_Editor摄像机，用于编辑器渲染3D元素
 */