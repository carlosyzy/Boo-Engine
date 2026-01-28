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

private: // camera相关
    std::string _cameraUuid = "";
    GfxRenderTexture *_cameraRenderTexture = nullptr;
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

    /**
     * 渲染类型
     * 0：2D渲染
     * 1：3D渲染
     */
    int _renderType = 0;

    void _initRenderCamera();
    void _updateRenderCamera();

private:
    Mat4 _refLineWorldMat = Mat4::identity();
    // 参考线网格
    GfxMesh *_refLineGfxMesh = nullptr;
    // 参考线材质
    GfxMaterial *_refLineGfxMtl = nullptr;
    MaterialAsset *_refLineAssetMtl = nullptr;

    std::vector<float> _refLineInstanceData;

    void _initRefLineGfx();
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