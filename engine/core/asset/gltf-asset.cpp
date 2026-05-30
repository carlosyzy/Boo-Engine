#include "gltf-asset.h"
#include "log.h"
#include "boo.h"
#include "core/asset/mesh-asset.h"

namespace Boo
{
    GLTFAsset::GLTFAsset()
    {
        this->_type = EAssetType::GLTF;
        // this->_meshAssets.clear();
    }
    GLTFAsset::GLTFAsset(std::string uuid) : Asset(uuid)
    {
        this->_type = EAssetType::GLTF;
        // this->_meshAssets.clear();
    }
    GLTFAsset::GLTFAsset(std::string uuid, std::string path, std::string name) : Asset(uuid, path, name)
    {
        this->_type = EAssetType::GLTF;
        // this->_meshAssets.clear();
    }
    // void GLTFAsset::create(std::unique_ptr<fastgltf::Asset> m_asset)
    // {
    //     // std::chrono::high_resolution_clock::time_point frameStart = std::chrono::high_resolution_clock::now();
    //     this->m_asset = std::move(m_asset);
    //     this->_parseScenes();
    //     this->m_asset.reset();
    //     // std::chrono::high_resolution_clock::time_point frameEnd = std::chrono::high_resolution_clock::now();
    //     // float frameDuration = std::chrono::duration_cast<std::chrono::nanoseconds>(frameEnd - frameStart).count() / 1000000.0f;
    //     // LOGI("[GLTFAsset : create] :: parseScenes duration: %f", frameDuration);
    // }
    // void GLTFAsset::_parseScenes()
    // {
    //     // 检查是否有场景
    //     if (m_asset->scenes.empty())
    //     {
    //         LOGW("No scenes in asset");
    //         return;
    //     }
    //     this->_root = {};
    //     this->_root.name = this->_name.empty() ? "gltf_root" : this->_name;
    //     this->_root.local = Mat4::identity();
    //     this->_root.meshUuid = "";
    //     this->_root.path = this->_name;
    //     // 解析场景
    //     // 为每个场景加载数据
    //     for (size_t i = 0; i < m_asset->scenes.size(); ++i)
    //     {
    //         const auto &scene = m_asset->scenes[i];
    //         for (size_t nodeIdx : scene.nodeIndices)
    //         {
    //             this->_parseNode(this->_root, nodeIdx);
    //         }
    //     }
    //     // std::cout << "GLTFAsset::_parseScenes:root name=" << this->_root.name << " child count=" << this->_root.children.size() << std::endl;
    // }
    // void GLTFAsset::_parseNode(GLTFNode &parent, size_t nodeIdx)
    // {
    //     if (nodeIdx >= m_asset->nodes.size())
    //     {
    //         return;
    //     }
    //     const fastgltf::Node &node = m_asset->nodes[nodeIdx];
    //     std::string nodeName = std::string(node.name);
    //     GLTFNode nodeData{};
    //     nodeData.name = nodeName;
    //     nodeData.local = Mat4::identity();
    //     nodeData.meshUuid = "";
    //     nodeData.path = parent.path + "/" + nodeData.name;
    //     fastgltf::math::fmat4x4 matrix = fastgltf::getTransformMatrix(node);
    //     this->_fmatToMat4(nodeData.local, matrix);
    //     this->_parseMeshes(node, nodeData);
    //     // std::cout << "GLTFAsset::_parseNode:node name=" << nodeData.name << " child count=" << nodeData.children.size() << std::endl;
    //     for (size_t childIdx : node.children)
    //     {
    //         this->_parseNode(nodeData, childIdx);
    //     }
    //     parent.children.push_back(nodeData);
    // }
    // /**
    //  * @brief 解析网格
    //  *
    //  * @param meshIndex 网格索引
    //  * @param nodeData 节点数据
    //  */
    // void GLTFAsset::_parseMeshes(const fastgltf::Node &node, GLTFNode &nodeData)
    // {
    //     if (!node.meshIndex.has_value())
    //     {
    //         nodeData.meshUuid = "";
    //         return;
    //     }
    //     const size_t meshIndex = *node.meshIndex;
    //     if (meshIndex >= m_asset->meshes.size())
    //     {
    //         nodeData.meshUuid = "";
    //         return;
    //     }
    //     if (this->_meshIndexMap.find(meshIndex) != this->_meshIndexMap.end())
    //     {
    //         nodeData.meshUuid = this->_meshIndexMap[meshIndex];
    //         return;
    //     }
    //     // std::chrono::high_resolution_clock::time_point frameStart = std::chrono::high_resolution_clock::now();
    //     const fastgltf::Mesh &mesh = m_asset->meshes[meshIndex];
    //     const std::string meshName = std::string(mesh.name);
    //     // std::cout << "Mesh name: " << meshName <<"   mesh size:"<< mesh.primitives.size() << std::endl;
    //     std::string meshUuid = this->_uuid + ":" + ":" + meshName;
    //     nodeData.meshUuid = meshUuid;
    //     // 解析所有子网格数据
    //     std::vector<FMeshPrimitive> primitives;
    //     int index = 0;
    //     for (const auto &primitive : mesh.primitives)
    //     {
    //         FMeshPrimitive primitiveData{};
    //         primitiveData.index = index;
    //         primitiveData.mode = EMeshMode::Model;
    //         this->_parsePrimitive(index, primitive, primitiveData);
    //         if (primitiveData._indices.empty())
    //         {
    //             continue;
    //         }
    //         index++;
    //         primitives.push_back(primitiveData);
    //     }
    //     // std::cout << "Mesh uuid: " << meshName << "   uuid:" << meshUuid <<"aaaa:"<< primitives.size() << std::endl;
    //     this->_meshIndexMap[meshIndex] = meshUuid;
    //     // 创建网格资产
    //     MeshAsset *meshAsset = new MeshAsset(meshUuid, "", meshName);
    //     meshAsset->create(primitives);
    //     assetManager->getAssetsCache()->addAsset(meshUuid, meshAsset);
    //     this->_meshAssets[meshName] = meshAsset;
    //     // std::chrono::high_resolution_clock::time_point frameEnd = std::chrono::high_resolution_clock::now();
    //     // float frameDuration = std::chrono::duration_cast<std::chrono::nanoseconds>(frameEnd - frameStart).count() / 1000000.0f;
    //     // LOGI("[GLTFAsset : create] :: parseMeshes %s duration: %f", meshName.c_str(), frameDuration);
    // }
    // /**
    //  * @brief 解析子网格
    //  *
    //  * @param primitive 子网格数据
    //  * @param primitiveData 子网格数据
    //  */
    // void GLTFAsset::_parsePrimitive(int index, const fastgltf::Primitive &primitive, FMeshPrimitive &primitiveData)
    // {
    //     // std::chrono::high_resolution_clock::time_point frameStart = std::chrono::high_resolution_clock::now();
    //     primitiveData.index = index;
    //     auto *positionAttribute = primitive.findAttribute("POSITION");
    //     if (positionAttribute == primitive.attributes.end())
    //     {
    //         LOGW("No position attribute in primitive");
    //         return;
    //     }
    //     // 1. 读取位置数据
    //     const auto &positionAccessor = m_asset->accessors[positionAttribute->accessorIndex];
    //     auto positionIterable = fastgltf::iterateAccessor<fastgltf::math::fvec3>(*m_asset, positionAccessor);
    //     size_t vertexCount = positionAccessor.count;
    //     primitiveData._positions.resize(vertexCount * 3);
    //     primitiveData._normals.resize(vertexCount * 3);
    //     primitiveData._uvs.resize(vertexCount * 2);
    //     primitiveData._uvs1.resize(vertexCount * 2);
    //     primitiveData._uvs2.resize(vertexCount * 2);
    //     primitiveData._colors.resize(vertexCount * 4, 1.0f);
    //     primitiveData._tangents.resize(vertexCount * 4);
    //     size_t pIdx = 0;
    //     for (const auto &position : positionIterable)
    //     {
    //         // 3个顶点float坐标
    //         primitiveData._positions[pIdx++] = position.x();
    //         primitiveData._positions[pIdx++] = position.y();
    //         primitiveData._positions[pIdx++] = position.z();
    //     }

    //     // 2. 读取法线数据（如果存在）
    //     auto normIter = primitive.findAttribute("NORMAL");

    //     if (normIter != primitive.attributes.end())
    //     {
    //         const auto &normAccessor = m_asset->accessors[normIter->accessorIndex];
    //         auto normals = fastgltf::iterateAccessor<fastgltf::math::fvec3>(
    //             *m_asset, normAccessor);

    //         size_t nIdx = 0;
    //         for (auto norm : normals)
    //         {
    //             primitiveData._normals[nIdx++] = norm.x();
    //             primitiveData._normals[nIdx++] = norm.y();
    //             primitiveData._normals[nIdx++] = norm.z();
    //         }
    //     }
    //     // 3. 读取纹理坐标（如果存在）
    //     auto texIter = primitive.findAttribute("TEXCOORD_0");
    //     if (texIter != primitive.attributes.end())
    //     {
    //         const auto &texAccessor = m_asset->accessors[texIter->accessorIndex];
    //         auto texCoords = fastgltf::iterateAccessor<fastgltf::math::fvec2>(
    //             *m_asset, texAccessor);

    //         size_t tIdx = 0;
    //         for (auto tex : texCoords)
    //         {
    //             primitiveData._uvs[tIdx++] = tex.x();
    //             primitiveData._uvs[tIdx++] = tex.y();
    //         }
    //     }
    //     // 4. 读取切线数据（如果存在）
    //     auto tanIter = primitive.findAttribute("TANGENT");
    //     if (tanIter != primitive.attributes.end())
    //     {
    //         const auto &tanAccessor = m_asset->accessors[tanIter->accessorIndex];
    //         auto tangents = fastgltf::iterateAccessor<fastgltf::math::fvec4>(
    //             *m_asset, tanAccessor);

    //         size_t tanIdx = 0;
    //         for (auto tan : tangents)
    //         {
    //             primitiveData._tangents[tanIdx++] = tan.x();
    //             primitiveData._tangents[tanIdx++] = tan.y();
    //             primitiveData._tangents[tanIdx++] = tan.z();
    //             primitiveData._tangents[tanIdx++] = tan.w(); // w 分量不变
    //         }
    //     }
    //     // 6. 读取索引数据（重要！）
    //     if (primitive.indicesAccessor.has_value())
    //     {
    //         size_t idxAccessorIdx = primitive.indicesAccessor.value();
    //         if (idxAccessorIdx < m_asset->accessors.size())
    //         {
    //             const auto &idxAccessor = m_asset->accessors[idxAccessorIdx];
    //             primitiveData._indices.resize(idxAccessor.count);
    //             switch (idxAccessor.componentType)
    //             {
    //             case fastgltf::ComponentType::UnsignedByte:
    //             {
    //                 auto indices = fastgltf::iterateAccessor<uint8_t>(*m_asset, idxAccessor);
    //                 // size_t iIdx = 0;
    //                 // for (auto index : indices)
    //                 // {
    //                 //     // primitiveData._indices.push_back(index);
    //                 //     primitiveData._indices[iIdx++] = index;
    //                 // }
    //                 std::copy(indices.begin(), indices.end(), primitiveData._indices.begin());
    //                 break;
    //             }
    //             case fastgltf::ComponentType::UnsignedShort:
    //             {
    //                 auto indices = fastgltf::iterateAccessor<uint16_t>(*m_asset, idxAccessor);
    //                 // size_t iIdx = 0;
    //                 // for (auto index : indices)
    //                 // {
    //                 //     // primitiveData._indices.push_back(index);
    //                 //     primitiveData._indices[iIdx++] = index;
    //                 // }
    //                 std::copy(indices.begin(), indices.end(), primitiveData._indices.begin());
    //                 break;
    //             }
    //             case fastgltf::ComponentType::UnsignedInt:
    //             {
    //                 auto indices = fastgltf::iterateAccessor<uint32_t>(*m_asset, idxAccessor);
    //                 // size_t iIdx = 0;
    //                 // for (auto index : indices)
    //                 // {
    //                 //     // primitiveData._indices.push_back(index);
    //                 //     primitiveData._indices[iIdx++] = index;
    //                 // }
    //                 std::copy(indices.begin(), indices.end(), primitiveData._indices.begin());
    //                 break;
    //             }
    //             default:
    //                 LOGW("Unsupported index type");
    //                 break;
    //             }
    //         }
    //     }
    //     else
    //     {
    //         // 没有索引数据，生成顺序索引（三角形列表）
    //         size_t vertexCount = positionAccessor.count;
    //         primitiveData._indices.resize(vertexCount * 3);
    //         size_t iIdx = 0;
    //         for (size_t i = 0; i < vertexCount; i += 3)
    //         {
    //             primitiveData._indices[iIdx++] = i;
    //             primitiveData._indices[iIdx++] = i + 1;
    //             primitiveData._indices[iIdx++] = i + 2;
    //         }
    //     }
    //     // std::chrono::high_resolution_clock::time_point frameEnd = std::chrono::high_resolution_clock::now();
    //     // float frameDuration = std::chrono::duration_cast<std::chrono::nanoseconds>(frameEnd - frameStart).count() / 1000000.0f;
    //     // LOGI("[GLTFAsset : create] :: parsePrimitive triangle count: %d duration: %f", primitiveData._indices.size() / 3, frameDuration);
    // }

    // void GLTFAsset::_fmatToMat4(Mat4 &mat, const fastgltf::math::fmat4x4 &fmat)
    // {
    //     std::array<float, 16> result;
    //     for (int i = 0; i < 4; ++i)
    //     {
    //         result[i * 4 + 0] = fmat[i][0];
    //         result[i * 4 + 1] = fmat[i][1];
    //         result[i * 4 + 2] = fmat[i][2];
    //         result[i * 4 + 3] = fmat[i][3];
    //     }
    //     mat.set(result);
    // }
    const GLTFNode &GLTFAsset::getRoot()
    {
        return this->_root;
    }
    std::map<std::string,MeshAsset*> &GLTFAsset::getMeshs()
    {
        return this->_meshAssets;
    }
    MeshAsset *GLTFAsset::getMesh(std::string meshName)
    {
        if(this->_meshAssets.find(meshName)==this->_meshAssets.end())
        {
            return nullptr;
        }
        return this->_meshAssets[meshName];
    }
    void GLTFAsset::destroy()
    {
    }
    GLTFAsset::~GLTFAsset()
    {
    }
}
