
# 组件工具
## 地面网格参考:ground-grid
```
glslangValidator -V -S vert grid.vert -o grid-vert.spv
glslangValidator -V -S frag grid.frag -o grid-frag.spv
glslangValidator -V -S geom grid.geom -o grid-geom.spv
```
## 编辑器相机参考:editor-camera
模拟编辑器相机组件，快捷进行物体预览，视角切换
