# GFX 图形管理器
## gfx-mgr 图形管理器
- 图形渲染管理：分别负责gfx-context（专门负责 Vulkan 环境的初始化、管理和生命周期）以及gfx-renderer（负责渲染相关的操作，如绘制、交换链管理等）
### gfx-context 图形上下文
- 负责 Vulkan 环境的初始化、管理和生命周期
- 提供 Vulkan 相关的资源管理，如设备、队列、交换链等
- 负责 Vulkan 实例的创建和销毁
### gfx-renderer 渲染器
- 管理所有物体渲染
- 管理gfx-shader，gfx-texture，gfx-pass，gfx-pipeline全局复用缓存

#### gfx-shader 着色器
- 通过着色器代码，动态进行读取，编译，缓存到全局复用缓存中
- default.vert 顶点着色器
- default.geom 几何着色器（暂时不支持）
- default.frag 像素着色器

#### gfx-pass 渲染通道
- 通过 pass 的决定性属性创建指定通道，缓存到全局复用缓存中
- 一个pass可以对应多个pipeline

#### gfx-pipeline 渲染管线
- 通过shader，pass以及管线的决定性属性创建指定管线，缓存到全局复用缓存中
- 一个pipeline可以对应多个pass

#### gfx-texture 纹理
- vulkan 渲染纹理，直接传入数据引用，不进行数据拷贝

投影举证设置为全局属性
<!-- 

#### gfx-material 材质
- 材质为自定义的结构体，包含 shader，pass，pipeline，texture 等信息
- 相同材质并且顶点结构相同的，理论上可以进行渲染的合批吧
**结构设计：整体暂定 json 格式**
```
{
    "name": "material-ui",
    "pipelines": [
        {
            "name": "default",
            "vertexShader": "default.vert",
            "fragmentShader": "default.frag",
            "pass": "default"
        }
    ],
    "textures": ""
}
```


- 1.顶点着色器关联
- 2.几何着色器关联（暂时不支持）
- 3.像素着色器关联
- 4.渲染通道关联
- 5.渲染管线关联
- 6.渲染贴图关联 -->


