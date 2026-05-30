#include "mesh-asset.h"
#include "log.h"
#include "boo.h"
#include "core/gfx/gfx-manager.h"
#include "core/gfx/base/gfx-mesh.h"
namespace Boo
{
    MeshAsset::MeshAsset() : Asset()
    {
        this->_type = EAssetType::Mesh;
    }
    MeshAsset::MeshAsset(std::string uuid) : Asset(uuid)
    {
        this->_type = EAssetType::Mesh;
    }
    MeshAsset::MeshAsset(std::string uuid, std::string path, std::string name) : Asset(uuid, path, name)
    {
        this->_type = EAssetType::Mesh;
    }
    void MeshAsset::create(std::vector<FMeshPrimitive> primitives)
    {
        this->_createMesh(EMeshType::Static, primitives);
    }
    void MeshAsset::createDynamic(std::vector<FMeshPrimitive> primitives)
    {
        this->_createMesh(EMeshType::Dynamic, primitives);
    }
    void MeshAsset::_createMesh(EMeshType meshType, std::vector<FMeshPrimitive> &primitives)
    {
        this->_meshType = meshType;
        this->_primitives.clear();
        this->_gfxMeshes.resize(primitives.size(), nullptr);
        for (auto &primitive : primitives)
        {
            // 顶点数据
            std::vector<float> vertexData;
            this->_prepareVertexData(primitive, vertexData);
            // 顶点索引
            const std::vector<uint32_t> &_indices = primitive._indices;
            std::string gfxMeshUuid = this->_uuid + "_" + std::to_string(primitive.index);
            GfxMesh *gfxMesh = nullptr;
            // std::chrono::high_resolution_clock::time_point frameStart = std::chrono::high_resolution_clock::now();
            if (primitive.mode == EMeshMode::UI)
            {
                gfxMesh = GfxManager::getInstance()->createUIMesh(gfxMeshUuid, int(meshType), vertexData, _indices);
            }
            else if (primitive.mode == EMeshMode::Model)
            {
                gfxMesh = GfxManager::getInstance()->createMesh(gfxMeshUuid, int(meshType), vertexData, _indices);
            }
            else
            {
                LOGW("MeshAsset::_createMesh: primitive mode %d not supported", primitive.mode);
                return;
            }
            this->_gfxMeshes[primitive.index] = gfxMesh;
            // 复制网格原语
            FMeshPrimitive newPrimitive = primitive;
            newPrimitive.index = primitive.index;
            newPrimitive.mode = primitive.mode;
            this->_primitives.push_back(newPrimitive);
            // std::chrono::high_resolution_clock::time_point frameEnd = std::chrono::high_resolution_clock::now();
            // float frameDuration = std::chrono::duration_cast<std::chrono::nanoseconds>(frameEnd - frameStart).count() / 1000000.0f;
            // LOGI("[MeshAsset : create] :: create gfx mesh duration: %f", frameDuration);
        }
    }
    void MeshAsset::updateDynamicSubMesh(int index, FMeshPrimitive &data)
    {
        if (index < 0 || index >= this->_primitives.size())
        {
            LOGW("MeshAsset::updateDynamicSubMesh: index out of range");
            return;
        }
        if (this->_meshType != EMeshType::Dynamic)
        {
            LOGW("MeshAsset::updateDynamicSubMesh: only dynamic mesh can be updated");
            return;
        }
        FMeshPrimitive tempPrimitive = this->_primitives[index];
        tempPrimitive._positions = data._positions;
        tempPrimitive._uvs = data._uvs;
        tempPrimitive._indices = data._indices;
        if (tempPrimitive.mode == EMeshMode::Model)
        {
            tempPrimitive._normals = data._normals;
            tempPrimitive._uvs1 = data._uvs1;
            tempPrimitive._uvs2 = data._uvs2;
            tempPrimitive._colors = data._colors;
            tempPrimitive._tangents = data._tangents;
        }
        std::vector<float> vertexData;
        this->_prepareVertexData(tempPrimitive, vertexData);
        if (this->_gfxMeshes[index] != nullptr)
        {
            this->_gfxMeshes[index]->update(vertexData, tempPrimitive._indices);
        }
    }
    GfxMesh *MeshAsset::getGfxMesh(int index)
    {
        if (index < 0 || index >= this->_gfxMeshes.size())
        {
            LOGW("MeshAsset::getGfxMesh: index out of range");
            return nullptr;
        }
        return this->_gfxMeshes[index];
    }
    const size_t MeshAsset::getSubMeshCount()
    {
        return this->_primitives.size();
    }
    void MeshAsset::_prepareVertexData(FMeshPrimitive &primitive, std::vector<float> &vertexData)
    {
        vertexData.clear();
        // 顶点位置
        const std::vector<float> &_positions = primitive._positions;
        // 顶点法线
        const std::vector<float> &_normals = primitive._normals;
        // 顶点纹理坐标
        const std::vector<float> &_uvs = primitive._uvs;
        // 顶点纹理坐标
        const std::vector<float> &_uvs1 = primitive._uvs1;
        // 顶点纹理坐标
        const std::vector<float> &_uvs2 = primitive._uvs2;
        // 顶点颜色
        const std::vector<float> &_colors = primitive._colors;
        // 顶点切线
        const std::vector<float> &_tangents = primitive._tangents;
        // 准备顶点数据
        int vertexCount = _positions.size() / 3;
        if (primitive.mode == EMeshMode::UI)
        {
            vertexData.resize(5 * vertexCount);
        }
        else if (primitive.mode == EMeshMode::Model)
        {
            vertexData.resize(20 * vertexCount);
        }
        size_t vertexIndex = 0;
        for (int i = 0; i < vertexCount; i++)
        {
            // 顶点坐标
            // vertexData.push_back(_positions[i * 3]);
            // vertexData.push_back(_positions[i * 3 + 1]);
            // vertexData.push_back(_positions[i * 3 + 2]);
            vertexData[vertexIndex++] = _positions[i * 3];
            vertexData[vertexIndex++] = _positions[i * 3 + 1];
            vertexData[vertexIndex++] = _positions[i * 3 + 2];

            if (primitive.mode == EMeshMode::Model)
            {
                // 法线 - 越界填充0
                // vertexData.push_back((i * 3) < _normals.size() ? _normals[i * 3] : 0.0f);
                // vertexData.push_back((i * 3 + 1) < _normals.size() ? _normals[i * 3 + 1] : 0.0f);
                // vertexData.push_back((i * 3 + 2) < _normals.size() ? _normals[i * 3 + 2] : 0.0f);
                vertexData[vertexIndex++] = (i * 3) < _normals.size() ? _normals[i * 3] : 0.0f;
                vertexData[vertexIndex++] = (i * 3 + 1) < _normals.size() ? _normals[i * 3 + 1] : 0.0f;
                vertexData[vertexIndex++] = (i * 3 + 2) < _normals.size() ? _normals[i * 3 + 2] : 0.0f;
            }

            // 纹理坐标 - 越界填充0
            // vertexData.push_back((i * 2) < _uvs.size() ? _uvs[i * 2] : 0.0f);
            // vertexData.push_back((i * 2 + 1) < _uvs.size() ? _uvs[i * 2 + 1] : 0.0f);
            vertexData[vertexIndex++] = (i * 2) < _uvs.size() ? _uvs[i * 2] : 0.0f;
            vertexData[vertexIndex++] = (i * 2 + 1) < _uvs.size() ? _uvs[i * 2 + 1] : 0.0f;

            if (primitive.mode == EMeshMode::Model)
            {
                // 纹理坐标1 - 越界填充0
                // vertexData.push_back((i * 2) < _uvs1.size() ? _uvs1[i * 2] : 0.0f);
                // vertexData.push_back((i * 2 + 1) < _uvs1.size() ? _uvs1[i * 2 + 1] : 0.0f);
                vertexData[vertexIndex++] = (i * 2) < _uvs1.size() ? _uvs1[i * 2] : 0.0f;
                vertexData[vertexIndex++] = (i * 2 + 1) < _uvs1.size() ? _uvs1[i * 2 + 1] : 0.0f;

                // 纹理坐标2 - 越界填充0
                // vertexData.push_back((i * 2) < _uvs2.size() ? _uvs2[i * 2] : 0.0f);
                // vertexData.push_back((i * 2 + 1) < _uvs2.size() ? _uvs2[i * 2 + 1] : 0.0f);
                vertexData[vertexIndex++] = (i * 2) < _uvs2.size() ? _uvs2[i * 2] : 0.0f;
                vertexData[vertexIndex++] = (i * 2 + 1) < _uvs2.size() ? _uvs2[i * 2 + 1] : 0.0f;

                // 颜色 - 越界填充0，RGBA
                // vertexData.push_back((i * 4) < _colors.size() ? _colors[i * 4] : 0.0f);
                // vertexData.push_back((i * 4 + 1) < _colors.size() ? _colors[i * 4 + 1] : 0.0f);
                // vertexData.push_back((i * 4 + 2) < _colors.size() ? _colors[i * 4 + 2] : 0.0f);
                // vertexData.push_back((i * 4 + 3) < _colors.size() ? _colors[i * 4 + 3] : 1.0f); // Alpha默认1.0
                vertexData[vertexIndex++] = (i * 4) < _colors.size() ? _colors[i * 4] : 0.0f;
                vertexData[vertexIndex++] = (i * 4 + 1) < _colors.size() ? _colors[i * 4 + 1] : 0.0f;
                vertexData[vertexIndex++] = (i * 4 + 2) < _colors.size() ? _colors[i * 4 + 2] : 0.0f;
                vertexData[vertexIndex++] = (i * 4 + 3) < _colors.size() ? _colors[i * 4 + 3] : 1.0f;

                // 切线 - 越界填充0
                // vertexData.push_back((i * 4) < _tangents.size() ? _tangents[i * 4] : 0.0f);
                // vertexData.push_back((i * 4 + 1) < _tangents.size() ? _tangents[i * 4 + 1] : 0.0f);
                // vertexData.push_back((i * 4 + 2) < _tangents.size() ? _tangents[i * 4 + 2] : 0.0f);
                // vertexData.push_back((i * 4 + 3) < _tangents.size() ? _tangents[i * 4 + 3] : 1.0f); // 切线方向默认1
                vertexData[vertexIndex++] = (i * 4) < _tangents.size() ? _tangents[i * 4] : 0.0f;
                vertexData[vertexIndex++] = (i * 4 + 1) < _tangents.size() ? _tangents[i * 4 + 1] : 0.0f;
                vertexData[vertexIndex++] = (i * 4 + 2) < _tangents.size() ? _tangents[i * 4 + 2] : 0.0f;
                vertexData[vertexIndex++] = (i * 4 + 3) < _tangents.size() ? _tangents[i * 4 + 3] : 1.0f;
            }
        }
    }
    void MeshAsset::destroy()
    {
        for (auto &gfxMesh : this->_gfxMeshes)
        {
            GfxManager::getInstance()->destroyMesh(gfxMesh);
            gfxMesh = nullptr;
        }
        this->_gfxMeshes.clear();
    }
    MeshAsset::~MeshAsset()
    {
    }
} // namespace Boo