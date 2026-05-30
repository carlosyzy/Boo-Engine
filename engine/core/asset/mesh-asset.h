#pragma once
#include <string>
#include <vector>
#include "asset.h"

class GfxMesh;
namespace Boo
{
    /**
     * @brief 网格资产
     */
    class MeshAsset : public Asset
    {
    private:
        EMeshType _meshType;
        /**
         * @brief 网格原语
         * 创建了gfx网格后，原顶点数据不会保留
         */
        std::vector<FMeshPrimitive> _primitives;
        std::vector<GfxMesh *> _gfxMeshes;
        void _createMesh(EMeshType meshType, std::vector<FMeshPrimitive> &primitives);
        void _prepareVertexData(FMeshPrimitive &primitive,std::vector<float> &vertexData);
    public:
        MeshAsset();
        MeshAsset(std::string uuid);
        MeshAsset(std::string uuid, std::string path, std::string name);
        /**
         * @brief 创建网格
         *
         * @param primitives 网格原语
         */
        void create(std::vector<FMeshPrimitive> primitives);
        /**
         * @brief 创建动态网格
         *
         * @param primitives 网格原语
         */
        void createDynamic(std::vector<FMeshPrimitive> primitives);
        /**
         * @brief 更新动态网格
         *
         * @param index 索引
         * @param data 网格原语
         */
        void updateDynamicSubMesh(int index, FMeshPrimitive &data);
        const size_t getSubMeshCount();
        GfxMesh *getGfxMesh(int index);
        void destroy();
        ~MeshAsset();
    };

} // namespace Boo
