#include "gfx-mesh.h"
#include "../gfx.h"
#include "../gfx-context.h"
#include "../gfx-manager.h"
#include <cmath>

GfxMesh::GfxMesh(std::string uuid)
{
    this->_uuid = uuid;
}
void GfxMesh::createUIMesh(int meshType, const std::vector<float> &vertices, const std::vector<uint32_t> &indices)
{
    this->_isVisible = false;
    this->_meshMode = 0;
    this->_meshType = meshType;
    if (vertices.size() <= 0)
    {
        LOGE("GfxMesh::createUIMesh: vertices.size() <=0");
        return;
    }
    if (indices.size() <= 0)
    {
        LOGE("GfxMesh::createUIMesh: indices.size() <=0");
        return;
    }
    this->_vertices = vertices;
    this->_indices = indices;
    if (this->_meshType == 0)
    {
        this->_createStaticBuffers();
    }
    else
    {
        this->_createDynamicBuffers();
    }
}
void GfxMesh::createMesh(int meshType, const std::vector<float> &vertices, const std::vector<uint32_t> &indices)
{
    this->_isVisible = false;
    this->_meshMode = 1;
    this->_meshType = meshType;
    if (vertices.size() <= 0 || indices.size() <= 0)
    {
        LOGE("GfxMesh::createMesh: vertices.size() <=0 || indices.size() <=0");
        return;
    }
    this->_vertices = vertices;
    this->_indices = indices;
    if (this->_meshType == 0)
    {
        this->_createStaticBuffers();
    }
    else
    {
        this->_createDynamicBuffers();
    }
}

void GfxMesh::_createStaticBuffers()
{
    VkDevice device = Gfx::_context->getVkDevice();
    VkDeviceSize vertexDataSize = this->_vertices.size() * sizeof(float);
    VkDeviceSize indexDataSize = this->_indices.size() * sizeof(uint32_t);
    // ========== 第一步：创建 Staging Buffers（CPU 可见） ==========
    // Staging buffers（临时，CPU 可见）
    VkBuffer stagingVertexBuffer, stagingIndexBuffer;
    VkDeviceMemory stagingVertexMemory, stagingIndexMemory;
    VkResult result = GfxManager::getInstance()->createBuffer(
        VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        &stagingVertexBuffer, &stagingVertexMemory,
        vertexDataSize, this->_vertices.data());
    if (result != VK_SUCCESS)
    {
        // 错误处理
        LOGE("GfxMesh::_createStaticBuffers: create staging vertex buffer failed");
        return;
    }
    result = GfxManager::getInstance()->createBuffer(
        VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        &stagingIndexBuffer, &stagingIndexMemory,
        indexDataSize, this->_indices.data());
    if (result != VK_SUCCESS)
    {
        // 错误处理
        LOGE("GfxMesh::_createStaticBuffers: create staging index buffer failed");
        vkDestroyBuffer(device, stagingVertexBuffer, nullptr);
        vkFreeMemory(device, stagingVertexMemory, nullptr);
        return;
    }
    // ========== 第二步：创建目标 Buffers（GPU 显存） ==========
    // 目标 buffers（GPU 显存）
    GfxManager::getInstance()->createBuffer(
        VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
        &this->_vertexBuffer, &this->_vertexMemory,
        vertexDataSize, nullptr);
    if (result != VK_SUCCESS)
    {
        // 错误处理
        LOGE("GfxMesh::_createStaticBuffers: create vertex buffer failed");
        vkDestroyBuffer(device, stagingVertexBuffer, nullptr);
        vkFreeMemory(device, stagingVertexMemory, nullptr);
        vkDestroyBuffer(device, stagingIndexBuffer, nullptr);
        vkFreeMemory(device, stagingIndexMemory, nullptr);
        return;
    }
    result = GfxManager::getInstance()->createBuffer(
        VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
        &this->_indexBuffer, &this->_indexMemory,
        indexDataSize, nullptr);
    if (result != VK_SUCCESS)
    {
        // 错误处理
        LOGE("GfxMesh::_createStaticBuffers: create vertex buffer failed");
        vkDestroyBuffer(device, this->_vertexBuffer, nullptr);
        vkFreeMemory(device, this->_vertexMemory, nullptr);
        vkDestroyBuffer(device, stagingVertexBuffer, nullptr);
        vkFreeMemory(device, stagingVertexMemory, nullptr);
        vkDestroyBuffer(device, stagingIndexBuffer, nullptr);
        vkFreeMemory(device, stagingIndexMemory, nullptr);
        return;
    }
    // ========== 第三步：通过 GPU 复制数据 ==========
    // 通过命令缓冲区从 staging 复制到 GPU
    GfxManager::getInstance()->copyBuffer(stagingVertexBuffer, this->_vertexBuffer, vertexDataSize);
    GfxManager::getInstance()->copyBuffer(stagingIndexBuffer, this->_indexBuffer, indexDataSize);
    // ========== 第四步：释放 Staging Buffers ==========
    // 释放 staging 资源
    vkDestroyBuffer(Gfx::_context->getVkDevice(), stagingVertexBuffer, nullptr);
    vkFreeMemory(Gfx::_context->getVkDevice(), stagingVertexMemory, nullptr);
    vkDestroyBuffer(Gfx::_context->getVkDevice(), stagingIndexBuffer, nullptr);
    vkFreeMemory(Gfx::_context->getVkDevice(), stagingIndexMemory, nullptr);

    this->_isVisible = true;
    this->_indexSize = this->_indices.size();
    // 计算哈希值
    this->_calculateHash();
}
void GfxMesh::_createDynamicBuffers()
{
    VkDeviceSize vertexDataSize = this->_vertices.size() * sizeof(float);
    VkDeviceSize indexDataSize = this->_indices.size() * sizeof(uint32_t);
    VkResult result = GfxManager::getInstance()->createBuffer(
        VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        &this->_vertexBuffer,
        &this->_vertexMemory,
        vertexDataSize,
        this->_vertices.data());
    if (result != VK_SUCCESS)
    {
        // 错误处理
        return;
    }
    // 索引缓冲区
    result = GfxManager::getInstance()->createBuffer(
        VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        &this->_indexBuffer,
        &this->_indexMemory,
        indexDataSize,
        this->_indices.data());
    if (result != VK_SUCCESS)
    {
        // 错误处理
        return;
    }
    this->_isVisible = true;
    this->_indexSize = this->_indices.size();
    // 计算哈希值
    this->_calculateHash();
}
void GfxMesh::update(const std::vector<float> &vertices, const std::vector<uint32_t> &indices)
{
    if (this->_meshType == 0)
    {
        return;
    }
    if (vertices.empty())
    {
        return;
    }
    if (indices.empty())
    {
        return;
    }
    if (vertices.size() > this->_vertices.size())
    {
        LOGW("GfxMesh::update: the vertex size is greater than the initial size");
        for (int i = 0; i < this->_vertices.size(); i++)
        {
            this->_vertices[i] = vertices[i];
        }
    }
    else
    {
        for (int i = 0; i < vertices.size(); i++)
        {
            this->_vertices[i] = vertices[i];
        }
    }
    if (indices.size() > this->_indices.size())
    {
        LOGW("GfxMesh::update: the index size is greater than the initial size");
        for (int i = 0; i < this->_indices.size(); i++)
        {
            this->_indices[i] = indices[i];
        }
        this->_indexSize = this->_indices.size();
    }
    else
    {
        for (int i = 0; i < indices.size(); i++)
        {
            this->_indices[i] = indices[i];
        }
        this->_indexSize = indices.size();
    }

    // 更新数据
    VkDevice device = Gfx::_context->getVkDevice();
    // 更新顶点数据
    VkDeviceSize vertexDataSize = this->_vertices.size() * sizeof(float);
    void *vertexMapped = nullptr;
    vkMapMemory(device, this->_vertexMemory, 0, vertexDataSize, 0, &vertexMapped);
    memcpy(vertexMapped, this->_vertices.data(), vertexDataSize);
    vkUnmapMemory(device, this->_vertexMemory);

    // 更新索引数据
    VkDeviceSize indexDataSize = this->_indices.size() * sizeof(uint32_t);
    void *indexMapped = nullptr;
    vkMapMemory(device, this->_indexMemory, 0, indexDataSize, 0, &indexMapped);
    memcpy(indexMapped, this->_indices.data(), indexDataSize);
    vkUnmapMemory(device, this->_indexMemory);
    // 计算哈希值
    this->_calculateHash();
}

const uint32_t GfxMesh::getIndexCount()
{
    return this->_indexSize;
}
void GfxMesh::_calculateHash()
{
    size_t vertexNum = this->_vertices.size() > 1000 ? 1000 : this->_vertices.size();
    this->_vertexHash = this->_vertices.size();
    for (int i = 0; i < vertexNum; i++)
    {
        size_t h = std::hash<float>()(this->_vertices[i]);
        this->_vertexHash ^= h + 0x9e3779b9 + (this->_vertexHash << 6) + (this->_vertexHash >> 2);
    }
    size_t indexNum = this->_indices.size() > 1000 ? 1000 : this->_indices.size();
    this->_indexHash = this->_indices.size();
    for (int i = 0; i < indexNum; i++)
    {
        size_t h = std::hash<uint32_t>()(this->_indices[i]);
        this->_indexHash ^= h + 0x9e3779b9 + (this->_indexHash << 6) + (this->_indexHash >> 2);
    }
}
bool GfxMesh::equals(const GfxMesh *mesh) const
{
    if (mesh == nullptr)
    {
        return false;
    }
    if (this == mesh)
    {
        return true;
    }
    // 顶点数据不一样，直接返回false
    if (this->_vertices.size() != mesh->_vertices.size())
    {
        return false;
    }
    // 索引数据不一样，直接返回false
    if (this->_indices.size() != mesh->_indices.size())
    {
        return false;
    }
    if (this->_indexSize != mesh->_indexSize)
    {
        return false;
    }
    if (this->_indexHash != mesh->_indexHash)
    {
        return false;
    }
    if (this->_vertexHash != mesh->_vertexHash)
    {
        return false;
    }

    size_t indicesStep = ceil(this->_indices.size()/500.0);
    for (int i = 0; i < this->_indices.size(); i+=indicesStep)
    {
        if (this->_indices[i] != mesh->_indices[i])
        {
            return false;
        }
    }
    size_t vertexStep = ceil(this->_vertices.size()/500.0);
    for (int i = 0; i < this->_vertices.size(); i+=vertexStep)
    {
        if (this->_vertices[i] != mesh->_vertices[i])
        {
            return false;
        }
    }
    return true;
}
void GfxMesh::_clearBuffers()
{
    if (this->_vertexBuffer != VK_NULL_HANDLE)
    {
        vkDestroyBuffer(Gfx::_context->getVkDevice(), this->_vertexBuffer, nullptr);
        this->_vertexBuffer = VK_NULL_HANDLE;
    }
    if (_vertexMemory != VK_NULL_HANDLE)
    {
        vkFreeMemory(Gfx::_context->getVkDevice(), this->_vertexMemory, nullptr);
        this->_vertexMemory = VK_NULL_HANDLE;
    }
    if (this->_indexBuffer != VK_NULL_HANDLE)
    {
        vkDestroyBuffer(Gfx::_context->getVkDevice(), this->_indexBuffer, nullptr);
        this->_indexBuffer = VK_NULL_HANDLE;
    }
    if (this->_indexMemory != VK_NULL_HANDLE)
    {
        vkFreeMemory(Gfx::_context->getVkDevice(), this->_indexMemory, nullptr);
        this->_indexMemory = VK_NULL_HANDLE;
    }

    this->_isVisible = false;
}
bool GfxMesh::isVisible()
{
    return this->_isVisible;
}


const std::string &GfxMesh::getUuid()
{
    return this->_uuid;
}
VkBuffer GfxMesh::getVertexBuffer()
{
    return this->_vertexBuffer;
}
VkBuffer GfxMesh::getIndexBuffer()
{
    return this->_indexBuffer;
}
void GfxMesh::destroy()
{
    this->_clearBuffers();
}

// void GfxMesh::createUIMesh(const std::vector<float> &_positions, const std::vector<float> &_uvs, const std::vector<int> &_indices)
// {
//     this->_rendererCategory = GfxRendererCategory::_UI;
//     this->_isVisible = false;
//     this->_vertices.clear();
//     this->_indices.clear();
//     if (_positions.size() <= 0 || _uvs.size() <= 0 || _indices.size() <= 0)
//     {
//         LOGE("GfxMesh::createUIMesh: _positions.size() <=0 || _uvs.size() <=0 || _indices.size() <=0");
//         return;
//     }
//     // 准备顶点数据
//     int vertexCount = _positions.size() / 3;
//     for (int i = 0; i < vertexCount; i++)
//     {
//         // 顶点坐标
//         this->_vertices.push_back(_positions[i * 3]);
//         this->_vertices.push_back(_positions[i * 3 + 1]);
//         this->_vertices.push_back(_positions[i * 3 + 2]);
//         // 纹理坐标
//         this->_vertices.push_back(_uvs[i * 2]);
//         this->_vertices.push_back(_uvs[i * 2 + 1]);
//     }
//     // 准备索引数据
//     for (int i = 0; i < _indices.size(); i++)
//     {
//         this->_indices.push_back(_indices[i]);
//     }

//     this->_createVertexBuffers();
// }
// void GfxMesh::createMesh(const std::vector<float> &_positions, const std::vector<float> &_normals, const std::vector<float> &_uvs, const std::vector<float> &_uvs1, const std::vector<float> &_uvs2, const std::vector<float> &_colors, const std::vector<float> &_tangents, const std::vector<int> &_indices)
// {
//     this->_rendererCategory = GfxRendererCategory::_3D;
//     // this->update(_positions, _normals, _uvs, _uvs1, _uvs2, _colors, _tangents, _indices);
//     this->_isVisible = false;
//     this->_vertices.clear();
//     this->_indices.clear();
//     if (_positions.size() <= 0 || _normals.size() <= 0 || _uvs.size() <= 0 || _uvs1.size() <= 0 || _uvs2.size() <= 0 || _colors.size() <= 0 || _tangents.size() <= 0 || _indices.size() <= 0)
//     {
//         LOGE("GfxMesh::createMesh: _positions.size() <=0 || _normals.size() <=0 || _uvs.size() <=0 || _uvs1.size() <=0 || _uvs2.size() <=0 || _colors.size() <=0 || _tangents.size() <=0 || _indices.size() <=0");
//         return;
//     }
//     // 准备顶点数据
//     int vertexCount = _positions.size() / 3;
//     for (int i = 0; i < vertexCount; i++)
//     {
//         // 顶点坐标
//         this->_vertices.push_back(_positions[i * 3]);
//         this->_vertices.push_back(_positions[i * 3 + 1]);
//         this->_vertices.push_back(_positions[i * 3 + 2]);
//         // 法线
//         this->_vertices.push_back(_normals[i * 3]);
//         this->_vertices.push_back(_normals[i * 3 + 1]);
//         this->_vertices.push_back(_normals[i * 3 + 2]);
//         // 纹理坐标
//         this->_vertices.push_back(_uvs[i * 2]);
//         this->_vertices.push_back(_uvs[i * 2 + 1]);
//         // 纹理坐标1
//         this->_vertices.push_back(_uvs1[i * 2]);
//         this->_vertices.push_back(_uvs1[i * 2 + 1]);
//         // 纹理坐标2
//         this->_vertices.push_back(_uvs2[i * 2]);
//         this->_vertices.push_back(_uvs2[i * 2 + 1]);
//         // 颜色
//         this->_vertices.push_back(_colors[i * 4]);
//         this->_vertices.push_back(_colors[i * 4 + 1]);
//         this->_vertices.push_back(_colors[i * 4 + 2]);
//         this->_vertices.push_back(_colors[i * 4 + 3]);
//         // 切线
//         this->_vertices.push_back(_tangents[i * 4]);
//         this->_vertices.push_back(_tangents[i * 4 + 1]);
//         this->_vertices.push_back(_tangents[i * 4 + 2]);
//         this->_vertices.push_back(_tangents[i * 4 + 3]);
//     }
//     // 准备索引数据
//     for (int i = 0; i < _indices.size(); i++)
//     {
//         this->_indices.push_back(_indices[i]);
//     }
//     this->_createVertexBuffers();
// }

// //动态网格
//   GfxManager::getInstance()->createBuffer(
//       VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,           // 去掉 TRANSFER_DST
//       VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
//       &this->_vertexBuffer,
//       &this->_vertexMemory,
//       this->_vertices.size() * sizeof(float),
//       this->_vertices.data()
//   );
//   // 初始化时创建一次（容量足够大）
//   void GfxMesh::_createDynamicVertexBuffer()
//   {
//       VkDeviceSize maxSize = MAX_VERTICES * sizeof(Vertex);

//       GfxManager::getInstance()->createBuffer(
//           VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
//           VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
//           &this->_vertexBuffer,
//           &this->_vertexMemory,
//           maxSize,
//           nullptr  // 先不传数据
//       );
//   }

//   // 每帧更新数据（不重建 buffer）
//   void GfxMesh::updateVertices(const std::vector<Vertex>& newVertices)
//   {
//       VkDevice device = Gfx::_context->getVkDevice();
//       VkDeviceSize dataSize = newVertices.size() * sizeof(Vertex);

//       void* mapped;
//       vkMapMemory(device, this->_vertexMemory, 0, dataSize, 0, &mapped);
//       memcpy(mapped, newVertices.data(), dataSize);
//       vkUnmapMemory(device, this->_vertexMemory);

//       this->_vertexCount = newVertices.size();
//   }

//   // 渲染时
//   vkCmdBindVertexBuffers(cmd, 0, 1, &this->_vertexBuffer, &offset);
//   vkCmdDraw(cmd, this->_vertexCount, 1, 0, 0);

// 进阶优化：Persistent Mapping

//   如果每帧都要更新，连 map/unmap 都省掉：

//   // 初始化时 map 一次，保持映射
//   void GfxMesh::_createPersistentBuffer()
//   {
//       GfxManager::getInstance()->createBuffer(...);

//       vkMapMemory(device, this->_vertexMemory, 0, VK_WHOLE_SIZE, 0, &this->_mappedPtr);
//       // 不 unmap，一直保持映射状态
//   }

//   // 每帧直接写
//   void GfxMesh::updateVertices(const std::vector<Vertex>& newVertices)
//   {
//       memcpy(this->_mappedPtr, newVertices.data(), newVertices.size() * sizeof(Vertex));
//       // 如果不是 HOST_COHERENT，这里需要 vkFlushMappedMemoryRanges
//   }

//   // 析构时才 unmap
//   ~GfxMesh()
//   {
//       vkUnmapMemory(device, this->_vertexMemory);
//   }

//   ---
//   实际例子

//   粒子系统（1000 个粒子，每帧更新位置）

//   // 初始化
//   ParticleSystem::init()
//   {
//       createBuffer(..., MAX_PARTICLES * sizeof(Particle), nullptr);
//       vkMapMemory(..., &_mappedParticles);  // 常驻映射
//   }

//   // 每帧
//   ParticleSystem::update(float dt)
//   {
//       for (auto& p : particles) {
//           p.position += p.velocity * dt;  // CPU 计算
//       }
//       memcpy(_mappedParticles, particles.data(), ...);  // 直接写入
//   }

//   // 渲染
//   vkCmdBindVertexBuffers(..., _particleBuffer, ...);
//   vkCmdDraw(..., particles.size(), ...);

//   ---