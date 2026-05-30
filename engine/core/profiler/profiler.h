#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include "core/math/mat4.h"


class GfxRenderTexture;
namespace Boo
{
    class MaterialAsset;
    class MeshAsset;
    struct FontTexture;
    class BMFontAsset;
}
namespace Boo
{
    class Profiler
    {
    private:
        std::string _uuid = "";
        int _width = 0;
        int _height = 0;
        bool _isShowDebug = false;

        /**
         * 视图矩阵
         * 将3D世界空间 → 3D相机空间
         */
        Mat4 _matView = Mat4::identity();
        /**
         * 投影矩阵
         * 将3D观察空间 → 2D裁剪空间（NDC空间：-1到1）
         */
        Mat4 _matProj = Mat4::identity();

        bool _initialized = false;
        void _doInit();

        float _lastFrameTime = -1.0f; // 新增
        int _lastDrawCount = -1;      // 新增
        int _lastObjectCount = -1;    // 新增

    private:
        BMFontAsset *_bmFont = nullptr;
        int _fontSize = 20;
        int _objectCount = 0;
        void _createDefaultFont();
        MeshAsset *_defaultMeshAsset = nullptr;
        void _createDefaultMeshAsset();
        MeshAsset *_createMeshAsset();
        MaterialAsset *_createMaterialAsset();
        void _setText(std::string text, MeshAsset *meshAsset, MaterialAsset *materialAsset, int &width, int &height);
        void _setFontTransform(int x, int y, int width, int height, int fontSize, Mat4 &mat);
        void _submitRenderObject(MaterialAsset *materialAsset, MeshAsset *meshAsset, Mat4 &mat, float colorR, float colorG, float colorB, float colorA);

        MaterialAsset *_bgMltAsset = nullptr;
        Mat4 _bgMat = Mat4::identity();
        void _initBg();
        void _refreshBg();

        // FPS
        Mat4 _fpsMat = Mat4::identity();
        Mat4 _fpsNumMat = Mat4::identity();
        std::string _fpsTitle = "fps:";
        int _fpsNum = 0;
        MaterialAsset *_fpsMltAsset = nullptr;
        MaterialAsset *_fpsNumMltAsset = nullptr;
        MeshAsset *_fpsMeshAsset = nullptr;
        MeshAsset *_fpsMeshNumAsset = nullptr;
        void _initFps();
        void _refreshFps();
        // 上一帧渲染时间
        MeshAsset *_renderTitleMeshAsset = nullptr;
        MaterialAsset *_renderTitleMltAsset = nullptr;
        MeshAsset *_renderTimeMeshAsset = nullptr;
        MaterialAsset *_renderTimeMltAsset = nullptr;
        Mat4 _renderTitleMat = Mat4::identity();
        Mat4 _renderTimeMat = Mat4::identity();
        std::string _renderTimeTitle = "renderer:";
        void _initRenderTime();
        void _refreshRenderTime();
        // 上一帧渲染提交次数
        MaterialAsset *_renderCountTitleMltAsset = nullptr;
        MaterialAsset *_renderCountMltAsset = nullptr;
        MeshAsset *_renderCountTitleMeshAsset = nullptr;
        MeshAsset *_renderCountMeshAsset = nullptr;
        Mat4 _renderCountTitleMat = Mat4::identity();
        Mat4 _renderCountMat = Mat4::identity();
        std::string _renderCountTitle = "draw call:";
        void _initRenderCount();
        void _refreshRenderCount();

        // 当前帧渲染物体数量
        MaterialAsset *_objectCountTitleMltAsset = nullptr;
        MaterialAsset *_objectCountMltAsset = nullptr;
        MeshAsset *_objectCountTitleMeshAsset = nullptr;
        MeshAsset *_objectCountMeshAsset = nullptr;
        Mat4 _objectCountTitleMat = Mat4::identity();
        Mat4 _objectCountMat = Mat4::identity();
        std::string _objectCountTitle = "object count:";
        void _initObjectCount();
        void _refreshObjectCount();

    public:
        Profiler();
        void init();
        void openDebug();
        void closeDebug();
        void addObjectCount(int count);
        void render();
        ~Profiler();
    };
}