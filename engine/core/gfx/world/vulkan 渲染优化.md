1. 实例化渲染（Instanced Rendering）
这是最高效的方法，专门为一次绘制多个相似对象设计：

cpp
// 创建顶点缓冲区时包含实例数据
struct InstanceData {
    glm::mat4 modelMatrix;
    glm::vec4 color;
    // 其他每实例数据...
};

// 设置顶点输入绑定描述
VkVertexInputBindingDescription bindings[2];
// 顶点数据绑定
bindings[0].binding = 0;
bindings[0].stride = sizeof(Vertex);
bindings[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

// 实例数据绑定
bindings[1].binding = 1;
bindings[1].stride = sizeof(InstanceData);
bindings[1].inputRate = VK_VERTEX_INPUT_RATE_INSTANCE;

// 设置顶点属性描述
VkVertexInputAttributeDescription attributes[5];
// 顶点属性
attributes[0] = {0, 0, VK_FORMAT_R32G32B32_SFLOAT, 0}; // 位置
attributes[1] = {1, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, normal)};
attributes[2] = {2, 0, VK_FORMAT_R32G32_SFLOAT, offsetof(Vertex, uv)};

// 实例属性
attributes[3] = {3, 1, VK_FORMAT_R32G32B32A32_SFLOAT, offsetof(InstanceData, modelMatrix)};
attributes[4] = {4, 1, VK_FORMAT_R32G32B32A32_SFLOAT, offsetof(InstanceData, modelMatrix) + 16};
// ... 继续添加模型矩阵的其他行

// 渲染时一次性绘制所有实例
VkDeviceSize instanceOffset = 0;
vkCmdBindVertexBuffers(commandBuffer, 0, 1, &vertexBuffer, &vertexOffset);
vkCmdBindVertexBuffers(commandBuffer, 1, 1, &instanceBuffer, &instanceOffset);
vkCmdBindIndexBuffer(commandBuffer, indexBuffer, 0, VK_INDEX_TYPE_UINT32);

// 一次调用绘制所有实例
vkCmdDrawIndexed(
    commandBuffer, 
    indexCount,        // 每个实例的索引数
    instanceCount,     // 实例数量
    0,                 // 第一个索引
    0,                 // 顶点偏移
    0                  // 第一个实例
);
2. 间接绘制（Indirect Drawing）
允许从GPU缓冲区读取绘制参数，非常适合GPU驱动的剔除：

cpp
// 间接绘制结构体
struct VkDrawIndexedIndirectCommand {
    uint32_t indexCount;
    uint32_t instanceCount;
    uint32_t firstIndex;
    int32_t  vertexOffset;
    uint32_t firstInstance;
};

// 创建间接绘制命令缓冲区
std::vector<VkDrawIndexedIndirectCommand> commands;
for (int i = 0; i < objectCount; ++i) {
    commands.push_back({
        .indexCount = indexCount,
        .instanceCount = 1,  // 每个对象1个实例
        .firstIndex = 0,
        .vertexOffset = 0,
        .firstInstance = static_cast<uint32_t>(i)  // 使用firstInstance传递实例ID
    });
}

// 在着色器中通过 gl_InstanceIndex 访问实例数据
vkCmdDrawIndexedIndirect(
    commandBuffer,
    indirectBuffer,    // 包含绘制命令的缓冲区
    0,                 // 偏移
    objectCount,       // 绘制命令数量
    sizeof(VkDrawIndexedIndirectCommand)
);
3. 合并顶点数据
将所有对象的顶点数据合并到单个缓冲区：

cpp
// 合并所有顶点数据
std::vector<Vertex> allVertices;
std::vector<uint32_t> allIndices;
std::vector<uint32_t> indexOffsets;
std::vector<uint32_t> vertexOffsets;

for (const auto& object : objects) {
    uint32_t vertexOffset = static_cast<uint32_t>(allVertices.size());
    uint32_t indexOffset = static_cast<uint32_t>(allIndices.size());
    
    vertexOffsets.push_back(vertexOffset);
    indexOffsets.push_back(indexOffset);
    
    // 添加顶点（需要调整索引）
    for (const auto& vertex : object.vertices) {
        allVertices.push_back(vertex);
    }
    
    // 添加索引（需要偏移）
    for (uint32_t index : object.indices) {
        allIndices.push_back(index + vertexOffset);
    }
}

// 创建合并的缓冲区
createBuffer(allVertices, allIndices);

// 一次绑定，多次绘制（伪代码）
vkCmdBindVertexBuffers(commandBuffer, 0, 1, &vertexBuffer, &zeroOffset);
vkCmdBindIndexBuffer(commandBuffer, indexBuffer, 0, VK_INDEX_TYPE_UINT32);

// 使用 push constants 传递每个对象的数据
for (int i = 0; i < objects.size(); ++i) {
    vkCmdPushConstants(commandBuffer, pipelineLayout, 
                       VK_SHADER_STAGE_VERTEX_BIT, 
                       0, sizeof(ObjectData), &objects[i].data);
    
    vkCmdDrawIndexed(commandBuffer, 
                     objects[i].indexCount,
                     1,  // 实例数
                     indexOffsets[i],  // 索引偏移
                     vertexOffsets[i], // 顶点偏移
                     0); // 实例偏移
}
4. 多绘制（Multi-Draw）扩展
使用 VK_EXT_multi_draw 扩展（如果可用）：

cpp
// 需要启用扩展
std::vector<VkMultiDrawIndexedInfoEXT> draws;
for (int i = 0; i < objectCount; ++i) {
    draws.push_back({
        .firstIndex = indexOffsets[i],
        .indexCount = indexCounts[i],
        .vertexOffset = vertexOffsets[i]
    });
}

// 使用扩展命令
vkCmdDrawMultiIndexedEXT(
    commandBuffer,
    draws.data(),
    static_cast<uint32_t>(draws.size()),
    1,  // 实例数
    0,  // 每个实例的数据偏移
    sizeof(VkMultiDrawIndexedInfoEXT),
    nullptr  // 顶点偏移数组
);
5. 使用几何着色器或网格着色器
几何着色器方法（性能较低）：
glsl
#version 460
layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

uniform mat4 instanceMatrices[100];  // 所有实例的矩阵

void main() {
    for (int i = 0; i < instanceCount; i++) {
        for (int j = 0; j < 3; j++) {
            gl_Position = projection * view * instanceMatrices[i] * gl_in[j].gl_Position;
            // 传递其他属性...
            EmitVertex();
        }
        EndPrimitive();
    }
}
网格着色器方法（Vulkan 1.3+，性能更好）：
cpp
// 启用 VK_EXT_mesh_shader 扩展
vkCmdDrawMeshTasksEXT(commandBuffer, groupCountX, groupCountY, groupCountZ);
最佳实践建议：
对于静态/半静态对象：
cpp
// 方案1：实例化 + 间接绘制（最优）
// 优点：CPU开销最小，支持GPU剔除
// 适用：草地、树木、重复建筑等

// 方案2：合并顶点数据 + push constants
// 优点：兼容性好，不需要特殊扩展
// 适用：UI元素、简单场景
性能对比表：
方法	CPU开销	GPU效率	灵活性	适用场景
实例化	极低	极高	中	大量相似对象
间接绘制	极低	极高	高	GPU驱动场景
合并顶点	中	高	低	少量对象
多次drawcall	高	中	极高	调试、原型
着色器示例（实例化）：
glsl
#version 460
layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inUV;
layout(location = 3) in mat4 inModelMatrix;  // 使用4个location
layout(location = 7) in vec4 inInstanceColor;

layout(push_constant) uniform Constants {
    mat4 viewProj;
} pc;

void main() {
    // 使用实例矩阵变换顶点
    vec4 worldPos = inModelMatrix * vec4(inPosition, 1.0);
    gl_Position = pc.viewProj * worldPos;
    
    // 可以访问 gl_InstanceIndex 获取实例ID
    uint instanceID = gl_InstanceIndex;
}
建议优先使用实例化渲染，这是最标准且性能最好的方法。如果对象数量动态变化或需要GPU剔除，则结合使用间接绘制。