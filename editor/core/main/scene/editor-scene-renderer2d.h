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

class EditorSceneRenderer2D
{
private:
    GfxMesh *_refLineGfxMesh = nullptr;
    GfxMaterial *_refLineGfxMtl = nullptr;
    MaterialAsset *_refLineAssetMtl = nullptr;
    Mat4 _refLineWorldMat = Mat4::identity();
    std::vector<float> _refLineInstanceData;
    void _initRefLineGfx();
    void _renderRefLine(std::string cameraUuid);

public:
    EditorSceneRenderer2D();
    void init();
    void resize(int width, int height);
    void render(std::string cameraUuid);
    ~EditorSceneRenderer2D();
};
