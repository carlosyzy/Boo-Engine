1. 基础标量类型
GLSL类型	大小	对齐	示例
bool	4字节	4字节	bool visible;
int	4字节	4字节	int objectId;
uint	4字节	4字节	uint flags;
float	4字节	4字节	float alpha;
double	8字节	8字节*	double precisionValue;
*注意：double需要Vulkan 1.1+或扩展VK_KHR_shader_float_controls

2. 向量类型
GLSL类型	大小	对齐	内存布局
vec2 / ivec2 / uvec2 / bvec2	8字节	8字节	连续存储
vec3 / ivec3 / uvec3 / bvec3	12字节	16字节**	有填充！
vec4 / ivec4 / uvec4 / bvec4	16字节	16字节	连续存储
dvec2 / dvec3 / dvec4	16/24/32字节	8/16/16字节	double向量
重要：vec3 在GLSL中占用16字节（有4字节填充），不是12字节！

3. 矩阵类型
GLSL类型	大小	对齐	内存布局
mat2	16字节	8字节*列	vec2[2]
mat2x3	32字节	16字节*列	vec3[2]
mat2x4	32字节	16字节*列	vec4[2]
mat3x2	24字节	8字节*列	vec2[3]
mat3	48字节	16字节*列	vec3[3]（有填充）
mat3x4	48字节	16字节*列	vec4[3]
mat4x2	32字节	8字节*列	vec2[4]
mat4x3	64字节	16字节*列	vec3[4]（有填充）
mat4	64字节	16字节*列	vec4[4]
*列对齐：矩阵的每一列都有自己的对齐要求

4. 数组类型
glsl
// 一维数组
layout(push_constant) uniform PC {
    float values[4];      // 16字节对齐，总大小16字节
    int ids[3];           // 16字节对齐，总大小16字节（有填充）
};

// 多维数组
layout(push_constant) uniform PC {
    mat2 matrices[2];     // 每元素16字节，总大小32字节
};
数组对齐规则：

基础类型数组：对齐 = 元素大小

向量/矩阵数组：对齐 = 元素对齐要求

5. 结构体类型
glsl
// GLSL结构体定义
struct Material {
    vec4 albedo;      // 16字节
    float roughness;  // 4字节
    float metallic;   // 4字节
    // 自动填充到16字节边界
};

layout(push_constant) uniform PC {
    Material mat;     // 32字节（不是24！）
} pushConstants;
结构体对齐 = 最大成员对齐要求

6. 完整示例
glsl
// 完整的push constant声明示例
layout(push_constant) uniform PushConstants {
    // 基础类型
    layout(offset = 0)   int frameId;        // 4字节 @0
    layout(offset = 4)   float time;         // 4字节 @4
    layout(offset = 8)   uint objectFlags;   // 4字节 @8
    
    // 向量（注意对齐！）
    layout(offset = 16)  vec2 uvOffset;      // 8字节 @16（不是12！）
    layout(offset = 24)  vec3 lightDir;      // 16字节 @24（不是24！vec3占16字节）
    
    // 矩阵
    layout(offset = 40)  mat4 modelView;     // 64字节 @40（不是32！）
    
    // 结构体
    struct PointLight {
        vec3 position;    // 16字节
        vec3 color;       // 16字节
        float intensity;  // 4字节
    };
    
    layout(offset = 104) PointLight light;   // 48字节 @104
    
    // 数组
    layout(offset = 152) float lodLevels[4]; // 16字节 @152
} pc;
对应的C++结构体：

cpp
struct PushConstants {
    // 基础类型段
    alignas(4)  int32_t frameId;        // offset: 0
    alignas(4)  float time;             // offset: 4
    alignas(4)  uint32_t objectFlags;   // offset: 8
    // padding: 12-15
    
    // 向量段（16字节对齐开始）
    alignas(8)  glm::vec2 uvOffset;     // offset: 16
    alignas(16) glm::vec3 lightDir;     // offset: 24
    // padding: 36-39
    
    // 矩阵段
    alignas(16) glm::mat4 modelView;    // offset: 40
    
    // 结构体
    struct PointLight {
        alignas(16) glm::vec3 position;
        alignas(16) glm::vec3 color;
        alignas(4)  float intensity;
        // padding: 44-47
    };
    
    alignas(16) PointLight light;       // offset: 104
    
    // 数组段
    alignas(16) float lodLevels[4];     // offset: 152
    
    // 总大小：168字节
};
static_assert(sizeof(PushConstants) == 168);
7. 特殊数据类型
glsl
// 原子计数器（需要扩展）
#extension GL_EXT_shader_atomic_float : enable
layout(push_constant) uniform PC {
    atomic_uint counter;  // 4字节
};

// 半精度浮点数（需要扩展）
#extension GL_EXT_shader_explicit_arithmetic_types_float16 : enable
layout(push_constant) uniform PC {
    f16vec2 halfVec;      // 4字节
    float16_t halfFloat;  // 2字节（但填充到4字节）
};
8. 不支持的数据类型
以下类型不能直接用于push constant：

glsl
// ❌ 不支持
layout(push_constant) uniform PC {
    sampler2D tex;           // 采样器 - 必须用描述符
    image2D img;             // 存储图像 - 必须用描述符
    samplerBuffer buf;       // 采样缓冲 - 必须用描述符
    atomic_int64_t counter;  // 64位原子 - 需要特殊支持
    // 接口块、子程序、函数等
};
9. 布局限定符
glsl
// 可以指定布局限定符
layout(push_constant, scalar) uniform PC {
    // scalar布局：所有成员按标量对齐
    vec3 position;  // 12字节（无填充）
};

layout(push_constant, row_major) uniform PC {
    // 行主序矩阵（默认是列主序）
    mat3 transform;  // 36字节（3x vec3）
};
10. 最佳实践
总是显式指定offset：避免自动布局的陷阱

使用std430布局：更紧凑，但需要显式管理

验证大小：使用glslangValidator验证

平台一致性：确保C++和GLSL布局一致

bash
# 验证GLSL布局
glslangValidator -V shader.frag --stdin -S frag -o /dev/null << 'EOF'
#version 450
layout(push_constant) uniform PC { vec4 a; float b; } pc;
void main() {}
EOF
关键要点
基础类型：int/float/uint/bool = 4字节

vec3陷阱：实际占用16字节，不是12字节！

矩阵存储：默认列主序，每列单独对齐

结构体填充：对齐到最大成员对齐要求

数组：元素连续，但有对齐要求

总大小限制：通常128-256字节，检查VkPhysicalDeviceLimits::maxPushConstantsSize