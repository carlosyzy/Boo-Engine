# 资产缓存 assets.db
{
    path:{
        type:  资源类型
        name:  资源名称      - 所有资源都有,不带后缀
        uuid:  资源uuid     - 当次资源type 非复合型资源时携带唯一资源uuid
        extension:  资源扩展名  - 资源后缀名
        <!-- path:  资源路径      - 所有资源都有,包含后缀  相对于项目根目录 -->
        subAssets:[
            {
                name:  子资源名称  
                uuid:  子资源uuid 
                type:  子资源类型  
                extension:  子资源扩展名
                path:  子资源不写路径
            }
        ]
    }
}