#pragma once
#include <vector>
namespace Boo
{
    /**
     * @brief 资源类型
     */
    enum class EAssetType
    {
        None,
        Texture,
        Audio,
        Font,
        Shader,
        Material,
        Scene,
        GLTF,
        Mesh,
        BMFont,
    };
    /**
     * @brief 网格模式
     */
    enum class EMeshMode
    {

        /**
         * @brief ui网格
         */
        UI = 0,
        /**
         * @brief 模型网格
         */
        Model = 1,
    };
    /**
     * @brief 网格模式
     */
    enum class EMeshType
    {
        /**
         * @brief 静态网格
         */
        Static,
        /**
         * @brief 动态网格
         */
        Dynamic,
    };
    /**
     * @brief 纹理类型
     */
    enum class ETextureType
    {
        /**
         * @brief 静态网格
         */
        Static = 0,
        /**
         * @brief 动态网格
         */
        Dynamic = 1,
    };

    struct FMeshPrimitive
    {
        /**
         * @brief 索引
         */
        int index;
        /**
         * @brief 类型
         * 0:ui网格 1:模型网格
         */
        EMeshMode mode;
        /**
         * @brief 顶点位置
         */
        std::vector<float> _positions;
        /**
         * @brief 顶点法线
         */
        std::vector<float> _normals;
        /**
         * @brief 顶点纹理坐标
         */
        std::vector<float> _uvs;
        /**
         * @brief 顶点纹理坐标
         */
        std::vector<float> _uvs1;
        /**
         * @brief 顶点纹理坐标
         */
        std::vector<float> _uvs2;
        /**
         * @brief 顶点颜色
         */
        std::vector<float> _colors;
        /**
         * @brief 顶点切线
         */
        std::vector<float> _tangents;
        /**
         * @brief 顶点索引
         */
        std::vector<uint32_t> _indices;
    };
    /**
     * @brief 渲染器层
     */
    enum class ERendererLayer
    {
        /**
         * @brief ui渲染器层
         */
        UI = 0,
        /**
         * @brief 模型渲染器层
         */
        Model = 1,
    };
    /**
     * @brief 渲染器类型
     */
    enum class ERendererType
    {
        /**
         * @brief 不透明渲染器类型
         */
        Opaque = 0,
        /**
         * @brief 透明渲染器类型
         */
        Transparent = 1,
    };
    /**
     * @brief 材质纹理块
     */
    struct FMaterialTextureBlock
    {
        int binding;
        std::string key;
        std::string path;
    };
} // namespace Boo