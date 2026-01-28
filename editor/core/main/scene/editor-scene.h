#pragma once
#include <iostream>
#include "../../../../engine/core/math/mat4.h"

class Scene;
class Node2D;
class GfxRenderTexture;
class EditorScene
{
private: //view
    Node2D *_rootNode;
    int _viewWidth = 0;
    int _viewHeight = 0;


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

    void _initRenderCamera();

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