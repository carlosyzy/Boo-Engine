#include "gfx-mesh.h"
#include "../gfx-mgr.h"
#include "../gfx.h"
#include "../gfx-context.h"

GfxMesh::GfxMesh(std::string uuid)
{
    this->_uuid = uuid;
}
void GfxMesh::setInputVertices(std::vector<float> vertices, std::vector<uint32_t> indices)
{
    this->_vertices = vertices;
    this->_indices = indices;
    this->_createVertexBuffers();
}
void GfxMesh::_createVertexBuffers()
{
    this->_clearVertexBuffers();

    // 顶点缓冲区
    GfxMgr::getInstance()->createBuffer(
        VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
        &this->_vertexBuffer,
        &this->_vertexMemory,
        this->_vertices.size() * sizeof(float), // 总字节数
        this->_vertices.data()                  // 数据指针
    );
    // 索引缓冲区（不变）
    GfxMgr::getInstance()->createBuffer(
        VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
        &this->_indexBuffer,
        &this->_indexMemory,
        this->_indices.size() * sizeof(uint32_t),
        this->_indices.data());
    this->_indexSize = this->_indices.size();
}
void GfxMesh::_clearVertexBuffers()
{
    if (this->_vertexBuffer != VK_NULL_HANDLE)
    {
        vkDestroyBuffer(Gfx::context->getVkDevice(), this->_vertexBuffer, nullptr);
        this->_vertexBuffer = VK_NULL_HANDLE;
    }
    if (_vertexMemory != VK_NULL_HANDLE)
    {
        vkFreeMemory(Gfx::context->getVkDevice(), this->_vertexMemory, nullptr);
        this->_vertexMemory = VK_NULL_HANDLE;
    }
    if (this->_indexBuffer != VK_NULL_HANDLE)
    {
        vkDestroyBuffer(Gfx::context->getVkDevice(), this->_indexBuffer, nullptr);
        this->_indexBuffer = VK_NULL_HANDLE;
    }
    if (this->_indexMemory != VK_NULL_HANDLE)
    {
        vkFreeMemory(Gfx::context->getVkDevice(), this->_indexMemory, nullptr);
        this->_indexMemory = VK_NULL_HANDLE;
    }
}
bool GfxMesh::equals(const GfxMesh *mesh) const
{
    if (mesh == nullptr)
    {
        return false;
    }
    if (this->_uuid.empty() || mesh->_uuid.empty())
    {
        return false;
    }
    if (this->_uuid != mesh->_uuid)
    {
        return false;
    }
    if (this->_vertices.empty() || mesh->_vertices.empty())
    {
        return false;
    }
    if (this->_vertices.size() != mesh->_vertices.size())
    {
        return false;
    }
    if (this->_indices.empty() || mesh->_indices.empty())
    {
        return false;
    }

    if (this->_indices.size() != mesh->_indices.size())
    {
        return false;
    }
    // 暂时不比较顶点数据
    return true;
}
VkBuffer GfxMesh::getVertexBuffer()
{
    return this->_vertexBuffer;
}
VkBuffer GfxMesh::getIndexBuffer()
{
    return this->_indexBuffer;
}
uint32_t GfxMesh::getIndexCount()
{
    return this->_indices.size();
}
int GfxMesh::getIndexSize()
{
    return this->_indexSize;
}
