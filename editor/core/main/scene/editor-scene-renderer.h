#pragma once
#include <iostream>
#include "../../../../engine/core/math/mat4.h"

class GfxRenderTexture;
class UISprite;

class EditorSceneRenderer
{
private:
    UISprite *_target = nullptr;

    std::string _cameraUuid = "";
    /**
     * 相机渲染目标宽度
     */
    int _cameraRenderTextureWidth = 0;
    /**
     * 相机渲染目标高度
     */
    int _cameraRenderTextureHeight = 0;
    /**
     * 视图矩阵
     * 将3D世界空间 → 3D相机空间
     */
    Mat4 _cameraMatView = Mat4::identity();
    /**
     * 投影矩阵
     * 将3D观察空间 → 2D裁剪空间（NDC空间：-1到1）
     */
    Mat4 _cameraMatProj = Mat4::identity();

    GfxRenderTexture *_cameraRenderTexture = nullptr;

    void _initRenderCamera();
    void _updateRenderCamera();

protected:
public:
    EditorSceneRenderer();
    void init(UISprite *target);
    void resize(int width, int height);
    void render();
    ~EditorSceneRenderer();
};