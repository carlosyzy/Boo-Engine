# 资产系统
所有的资产都是通过相对于assets的路径来访问的,也就是路径就是资产的唯一id
## 材质系统（MaterialAsset）
### 数据格式（JSON）
#### UI
- renderer: 0 渲染类型,0为UI,1为3D
- vert: 顶点着色器路径
- frag: 片源着色器 
// 纹理 ui中，除了默认的texture以外，还预留了三个空白为槽，
- textures: {
    "texture": {
        "binding": 1,
        "path": "builtin::default.png",
    },
    "textureSample0": {
        "binding": 2,
        "path": "builtin::default.png",
    },
    "textureSample1": {
        "binding": 3,
        "path": "builtin::default.png",
    },
    "textureSample2": {
        "binding": 4,
        "path": "builtin::default.png",
    },
}
<!-- - macro: {
    "USE_TEXTURE": 1,   //宏开关名称 定义在着色器中的宏,用于判断是否使用纹理
    "USE_TEXTURE_SAMPLE0": 1,             // 宏开关值,0为不使用,1为使用
    "USE_TEXTURE_SAMPLE1": 1,
    "USE_TEXTURE_SAMPLE2": 1,
} -->
- properties:[
    {
        name:"color",
        type:"vec4",
        value:[1.0,1.0,1.0,1.0],
    },
    {
        name:"width",
        type:"float",
        value:1920.0,
    },
    {
        name:"height",
        type:"float",
        value:1080.0,
    },
    {
        name:"pos",
        type:"vec4",
        value:[0.0,0.0,0.0,0.0],
    },
]

### 3D
- name: 材质的名称
- type: 材质的类型,必须为material
- shader: 材质使用的着色器
- uniforms: 材质的uniform变量

