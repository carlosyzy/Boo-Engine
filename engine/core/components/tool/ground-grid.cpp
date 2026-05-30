#include "ground-grid.h"
#include "boo.h"
#include "log.h"
#include "core/asset/mesh-asset.h"
#include "core/asset/material-asset.h"
#include "core/asset/shader-asset.h"
#include "core/scene/node-3d.h"
#include "core/renderer/3d/mesh-renderer.h"
namespace Boo
{
    GroundGrid::GroundGrid(std::string name, Node *node, std::string uuid)
        : Component(name, node, uuid)
    {
        this->_layer = EComponentLayer::Layer3D;
        this->_node = dynamic_cast<Node3D *>(node);
        this->_meshAsset = nullptr;
        this->_materialAsset = nullptr;
    }
    void GroundGrid::OnAwake()
    {
        this->_createMesh();
        this->_createMaterial();
        this->_createShader();
        this->_createMeshRenderer();
    }
    void GroundGrid::setProperty(json &data)
    {
        Component::setProperty(data);
    }

    void GroundGrid::_createMesh()
    {
        this->_meshAsset = new MeshAsset();
        FMeshPrimitive primitive{};
        primitive.mode = EMeshMode::Model;
        primitive._positions = {
            -0.5f, 0, -0.5f,
            -0.5f, 0, 0.5f,
            0.5f, 0, 0.5f,
            0.5f, 0, -0.5f};
        primitive._colors = {1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f};
        primitive._uvs = {0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f};
        primitive._normals = {0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f};
        primitive._indices = {0, 1, 2, 0, 2, 3};
        this->_meshAsset->create({primitive});
    }
    void GroundGrid::_createMaterial()
    {
        const std::string groundGridMaterial = R"({
            "layer": 1,
            "type":1,
            "vert": "",
            "frag": "",
            "polygonMode": 0,
            "cullMode": 0,
            "depthTest": 1,
            "depthWrite": 0,
            "depthCompareOp": 1,
            "stencilTest": 0,
            "stencilFrontCompareOp": 5,
            "stencilFrontFailOp": 0,
            "stencilFrontDepthFailOp": 0,
            "stencilFrontPassOp": 0,
            "stencilBackCompareOp": 5,
            "stencilBackFailOp": 0,
            "stencilBackDepthFailOp": 0,
            "stencilBackPassOp": 0,
            "colorBlend": 1,
            "srcColorBlendFactor": 4,
            "dstColorBlendFactor": 6,
            "colorBlendOp": 0,
            "srcAlphaBlendFactor": 0,
            "dstAlphaBlendFactor": 6,
            "alphaBlendOp": 0,
            "properties":[
                {
                    "name":"color",
                    "type":"vec4",
                    "value":[1.0,1.0,1.0,1.0]
                }],
            "textures":{
                "mainTexture": {
                    "binding": 1,
                    "path": ""
                }
            }
        })";
        this->_materialAsset = new MaterialAsset();
        json materialJson = json::parse(groundGridMaterial);
        this->_materialAsset->create(materialJson);
    }
    void GroundGrid::_createShader()
    {
        // 顶点着色器
        std::string vertShader = R"(
        #version 450
        #extension GL_ARB_separate_shader_objects : enable
        precision highp float;

        layout(binding = 0) uniform UniformBufferObject {
            mat4 view;
            mat4 proj;
            vec4 cameraPos;
            vec4 mainLitDir;
            vec4 mainLitColor;
            float time;
        } ubo;
        //cpu输入属性
        layout (location = 0) in vec3 l_position; // 输入本地坐标
        layout (location = 1) in vec3 l_normal; // 输入本地法线
        layout (location = 2) in vec2 l_texCoord; // 输入uv坐标
        layout (location = 3) in vec2 l_texCoord_1; // 输入uv坐标_1
        layout (location = 4) in vec2 l_texCoord_2; // 输入uv坐标_2
        layout (location = 5) in vec4 l_color; // 输入颜色
        layout (location = 6) in vec4 l_tangent; // 输入切线
        layout (location = 7) in mat4 w_matrix; // 输入模型世界矩阵
        layout (location = 11) in mat4 w_matrix_it; // 输入模型逆转置矩阵

        // 输出至片段着色器的属性
        layout (location = 0) out vec2 v_uv;    // 输出uv坐标
        layout (location = 1) out vec3 v_pos;     // 输出世界坐标
        layout (location = 2) out vec3 v_cameraPos;     // 输出相机坐标
        layout (location = 3) out float v_gridSize;     // 输出网格大小

        void main() {
            float gridSize = 100000.0f;
            // 不用传进来的矩阵,自定义一个与摄像机位置一样,缩放为100,100,1的矩阵
            mat4 worldMatrixC ={
            vec4(gridSize, 0.0f, 0.0f,0.0f),
            vec4(0.0f, 1.0f, 0.0f, 0.0f),
            vec4(0.0f, 0.0f, gridSize, 0.0f),
            vec4(0.0f, 0.0f, 0.0f, 1.0f)
            };
            //列主序矩阵存储在GPU中，列主序应该用（列向量右乘矩阵,向量写在矩阵右边）
            //计算世界坐标
            vec4 worldPosition = worldMatrixC*vec4(l_position, 1.0f) ;
            v_pos = worldPosition.xyz;
            // 视图矩阵将相机位置从世界空间转换到观察空间
            vec4 viewPos = ubo.view*worldPosition;
            // 转换为NDC坐标
            vec4 clipPos = ubo.proj*viewPos;
            v_uv = l_texCoord;
            v_cameraPos = ubo.cameraPos.xyz;
            v_gridSize = gridSize;
            gl_Position = clipPos;
        }
        )";
        // 片段着色器
        std::string fragShader = R"(
        #version 450
        #extension GL_ARB_separate_shader_objects : enable
        precision highp float;  // 添加明确的精度声明
        //采样器
        layout(binding = 1) uniform sampler2D mainTexture; // 主纹理
        //顶点输出属性
        layout (location = 0) in vec2 v_uv;    // 输出uv坐标
        layout (location = 1) in vec3 v_pos;     // 输出世界坐标
        layout (location = 2) in vec3 v_cameraPos;     // 输出相机坐标
        layout (location = 3) in float v_size;     // 输出网格大小
        //输出属性
        layout(location = 0) out vec4 outColor;

        vec4 drawCenterLines(vec2 uv, float v_size){
            float cameraDis=max(1.0,distance(v_pos,v_cameraPos));  //0-1    1000-0
            float t = cameraDis / 1000.0;
            float widthRote = 1.0 + pow(t, 1.5) *50.0;

            //计算x轴网格坐标
            float rote=1.0/v_size;
            float xGrid = abs(uv.x-0.5);
            xGrid=0.5-xGrid;
            xGrid*=2.0;
            float xAlpha = smoothstep(1.0-rote*widthRote,1.0, xGrid);
            vec4 xGridColor = vec4(0.0, 0.0, 1.0, xAlpha);
            // lineColor = mix(lineColor, xGridColor, xAlpha);
            //计算y轴网格坐标
            float yGrid = abs(uv.y-0.5);
            yGrid=0.5-yGrid;
            yGrid*=2.0;
            float yAlpha = smoothstep(1.0-rote*widthRote,1.0, yGrid);
            vec4 yGridColor = vec4(1.0, 0.0, 0.0, yAlpha);
            vec4 gridColor = vec4(0.0, 0.0, 0.0, 0.0);
            gridColor = mix(gridColor, xGridColor, xAlpha);
            gridColor = mix(gridColor, yGridColor, yAlpha);
            return gridColor;
        }
        vec4 drawGridLines(vec2 uv, float v_size){
            float rote=1.0/v_size;
            // 相机到网格的距离,计算网格等级
            float dis=max(1.0, abs(v_cameraPos.y));
            float cameraDis=max(1.0,distance(v_pos,v_cameraPos));  //0-1    1000-0
            float t = cameraDis / 1000.0;
            float widthRote = 0.3 + pow(t, 1.5) * 75.0;

            float power = floor(log(dis)/log(10.0f));  // 获取数量级  0-10:1  10-100:10 100-1000:100
            // 计算当前网格大小和下一个网格大小
            float lower = pow(10.0, power);      // 当前网格大小
            float upper = pow(10.0, power + 1.0); // 下一个网格大小
            float ratio = (dis - lower) / (upper - lower); // 计算当前网格距离下一个网格的比例
           
            vec2 grideUV=uv-vec2(0.5, 0.5);
            grideUV.x=abs(grideUV.x*2.0);
            grideUV.y=abs(grideUV.y*2.0);
            //当前网格
            float lowerGridNum = v_size/lower;
            float lowerWidth=rote*lowerGridNum*widthRote;
            vec2 lowerUV = uv*lowerGridNum;
            lowerUV=fract(lowerUV);
            lowerUV-=vec2(0.5, 0.5);
            lowerUV.x=abs(lowerUV.x*2.0);
            lowerUV.y=abs(lowerUV.y*2.0);
            float lowerAlphaX = smoothstep(1.0-lowerWidth,1.0, lowerUV.x);
            float lowerAlphaY = smoothstep(1.0-lowerWidth,1.0, lowerUV.y);
            float lowerAlpha = max(lowerAlphaY, lowerAlphaX);
            vec4 lowerColor = vec4(0.5, 0.5, 0.5, lowerAlpha);

            //下一个网格
            float upperGridNum = v_size/upper;
            float upperWidth=rote*upperGridNum*widthRote;
            vec2 upperUV = uv*upperGridNum;
            upperUV=fract(upperUV);
            upperUV-=vec2(0.5, 0.5);
            upperUV.x=abs(upperUV.x*2.0);
            upperUV.y=abs(upperUV.y*2.0);
            float upperAlphaX = smoothstep(1.0-upperWidth,1.0, upperUV.x);
            float upperAlphaY = smoothstep(1.0-upperWidth,1.0, upperUV.y);
            float upperAlpha = max(upperAlphaY, upperAlphaX);
            vec4 upperColor = vec4(0.5, 0.5, 0.5, upperAlpha);

            vec4 gridColor = mix(lowerColor, upperColor, ratio);
            return gridColor;
        }

        vec4 alphaBlend(vec4 bottom, vec4 top) {
            // 标准混合公式：result = top * top.alpha + bottom * (1.0 - top.alpha)
            vec3 blendedRGB = top.rgb * top.a + bottom.rgb * (1.0 - top.a);
            float blendedAlpha = top.a + bottom.a * (1.0 - top.a);
            return vec4(blendedRGB, blendedAlpha);
        }
        void main() {
            vec4 baseColor = vec4(0.5, 0.5, 0.5, 1.0);
            vec2 uv = v_uv;
            vec4 lineColor = drawCenterLines(uv, v_size);
            vec4 gridColor = drawGridLines(uv, v_size);
            baseColor = alphaBlend(gridColor, lineColor);
            float dis=distance(v_pos,v_cameraPos);
            float alpha=1.0-smoothstep(0.0f, 1.0f, dis/1000.0f);
            baseColor.a*=alpha;
            outColor=baseColor;
        }
        )";
        // 顶点着色器资产
        this->_vertShaderAsset = new ShaderAsset("AssetBuiltinMesh::GroundGridVert");
        this->_vertShaderAsset->create(ShaderVertexAsset, vertShader, {});
        this->_materialAsset->setVertexShader(this->_vertShaderAsset);
        // 片段着色器资产
        this->_fragShaderAsset = new ShaderAsset("AssetBuiltinMesh::GroundGridFrag");
        this->_fragShaderAsset->create(ShaderFragmentAsset, fragShader, {});
        this->_materialAsset->setFragmentShader(this->_fragShaderAsset);
    }
    void GroundGrid::_createMeshRenderer()
    {
        this->_meshRenderer = dynamic_cast<MeshRenderer *>(this->_node->addComponent("MeshRenderer"));
        if (this->_meshRenderer == nullptr)
        {
            LOGW("GroundGrid::_createMeshRenderer: meshRenderer is nullptr");
            return;
        }
        std::cout << "GroundGrid::_createMeshRenderer: meshRenderer created1" << std::endl;
        this->_meshRenderer->setMesh(this->_meshAsset);
        this->_meshRenderer->setMaterial(this->_materialAsset);
        std::cout << "GroundGrid::_createMeshRenderer: meshRenderer created2" << std::endl;
    }
    void GroundGrid::OnEnable()
    {
    }
    void GroundGrid::Update(float deltaTime)
    {
    }
    void GroundGrid::LateUpdate(float deltaTime)
    {
    }

    void GroundGrid::OnDisable()
    {
    }
    void GroundGrid::destroy()
    {
        Component::destroy();
    }
    GroundGrid::~GroundGrid()
    {
        if (this->_vertShaderAsset != nullptr)
        {
            this->_vertShaderAsset->destroy();
            delete this->_vertShaderAsset;
        }
        if (this->_fragShaderAsset != nullptr)
        {
            this->_fragShaderAsset->destroy();
            delete this->_fragShaderAsset;
        }
        if (this->_materialAsset != nullptr)
        {
            this->_materialAsset->destroy();
            delete this->_materialAsset;
        }
        if (this->_meshAsset != nullptr)
        {
            this->_meshAsset->destroy();
            delete this->_meshAsset;
        }
        this->_meshAsset = nullptr;
        this->_materialAsset = nullptr;
        this->_vertShaderAsset = nullptr;
        this->_fragShaderAsset = nullptr;
        this->_meshRenderer = nullptr;
    }
}