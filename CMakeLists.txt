cmake_minimum_required(VERSION 3.12)  # 设置CMake最低版本要求
project(app)                          # 项目名称（对应TARGET）

set(CMAKE_CXX_STANDARD 17)            # -std=c++17
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # 生成compile_commands.json
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    # GCC或Clang编译器
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")
elseif(MSVC)
    # MSVC编译器
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /permissive-")
endif()

set(ENGINE_SOURCES
    editor/boo-editor.cpp
    editor/cache/assets/editor-asset-util.cpp
    editor/cache/assets/editor-cache-assets-task.cpp
    editor/cache/assets/editor-cache-assets.cpp
    editor/cache/editor-cache.cpp
    editor/cache/scene/editor-cache-scene.cpp
    editor/cache/setting/editor-cache-setting.cpp
    editor/config/editor-config.cpp
    editor/editor-boot.cpp
    editor/editor.cpp
    editor/loading/editor-loading.cpp
    editor/main/assets/editor-assets-file-tree.cpp
    editor/main/assets/editor-assets-layout.cpp
    editor/main/assets/editor-assets.cpp
    editor/main/editor-layout.cpp
    editor/main/editor-main.cpp
    editor/main/hierarchy/editor-hierarchy-layout.cpp
    editor/main/hierarchy/editor-hierarchy-node-tree.cpp
    editor/main/hierarchy/editor-hierarchy.cpp
    editor/main/property/editor-property-layout.cpp
    editor/main/property/editor-property.cpp
    editor/main/scene/editor-scene-layout.cpp
    editor/main/scene/editor-scene.cpp
    engine/boo.cpp
    engine/core/alpha/alpha.cpp
    engine/core/assets/asset-cache.cpp
    engine/core/assets/asset-load.cpp
    engine/core/assets/asset-task.cpp
    engine/core/assets/asset.cpp
    engine/core/assets/assets-manager.cpp
    engine/core/assets/material-asset.cpp
    engine/core/assets/mesh-asset.cpp
    engine/core/assets/scene-asset.cpp
    engine/core/assets/shader-asset.cpp
    engine/core/assets/texture-asset.cpp
    engine/core/component/component-factory.cpp
    engine/core/component/component.cpp
    engine/core/component/ui/ui-layout-horizontal.cpp
    engine/core/component/ui/ui-layout-vertical.cpp
    engine/core/component/ui/ui-tree/file-tree.cpp
    engine/core/component/ui/ui-tree/ui-tree.cpp
    engine/core/component/ui/ui-widget.cpp
    engine/core/event/event.cpp
    engine/core/font/freetype-mgr.cpp
    engine/core/game.cpp
    engine/core/gfx/base/gfx-buffer-instance.cpp
    engine/core/gfx/base/gfx-buffer-ubo.cpp
    engine/core/gfx/base/gfx-buffer.cpp
    engine/core/gfx/base/gfx-material.cpp
    engine/core/gfx/base/gfx-mesh.cpp
    engine/core/gfx/base/gfx-pipeline.cpp
    engine/core/gfx/base/gfx-push-constants.cpp
    engine/core/gfx/base/gfx-render-pass.cpp
    engine/core/gfx/base/gfx-render-texture.cpp
    engine/core/gfx/base/gfx-shader.cpp
    engine/core/gfx/base/gfx-texture.cpp
    engine/core/gfx/builtin/gfx-batch-builtin.cpp
    engine/core/gfx/builtin/gfx-pipeline-builtin.cpp
    engine/core/gfx/builtin/gfx-queue-builtin.cpp
    engine/core/gfx/builtin/gfx-render-pass-builtin.cpp
    engine/core/gfx/builtin/gfx-renderer-builtin.cpp
    engine/core/gfx/default/gfx-pipeline-default.cpp
    engine/core/gfx/default/gfx-queue-default.cpp
    engine/core/gfx/default/gfx-render-pass-default.cpp
    engine/core/gfx/default/gfx-renderer-default.cpp
    engine/core/gfx/gfx-context.cpp
    engine/core/gfx/gfx-mgr.cpp
    engine/core/gfx/gfx-renderer.cpp
    engine/core/gfx/gfx.cpp
    engine/core/global/global.cpp
    engine/core/input/input.cpp
    engine/core/math/color.cpp
    engine/core/math/mat4.cpp
    engine/core/math/quat.cpp
    engine/core/math/size.cpp
    engine/core/math/vec2.cpp
    engine/core/math/vec3.cpp
    engine/core/renderer/camera.cpp
    engine/core/renderer/renderer.cpp
    engine/core/renderer/ui/ui-mask.cpp
    engine/core/renderer/ui/ui-renderer.cpp
    engine/core/renderer/ui/ui-sprite.cpp
    engine/core/renderer/ui/ui-text.cpp
    engine/core/scene/node-2d.cpp
    engine/core/scene/node-3d.cpp
    engine/core/scene/node.cpp
    engine/core/scene/scene.cpp
    engine/core/utils/dialog-util.cpp
    engine/core/utils/file-util.cpp
    engine/core/utils/fmt-util.cpp
    engine/core/utils/sdf-util.cpp
    engine/core/utils/time-util.cpp
    engine/core/utils/uuid-util.cpp
    engine/engine.cpp
    engine/window/window.cpp
)

add_executable(
    ${PROJECT_NAME} 
    ${ENGINE_SOURCES}
    app.cpp
)  

if(APPLE)
    # 查找GLFW库
    find_package(glfw3 REQUIRED)
    # 查找Vulkan库
    find_package(Vulkan REQUIRED)
    # # 查找freetype库
    find_package(Freetype REQUIRED)
     
    find_library(COCOA_LIBRARY Cocoa REQUIRED)
    find_library(FOUNDATION_LIBRARY Foundation REQUIRED)
    find_library(APPKIT_LIBRARY AppKit REQUIRED)
    

    # 头文件包含
    target_include_directories(${PROJECT_NAME} PRIVATE 
        ${CMAKE_CURRENT_SOURCE_DIR}/engine/libs/stb
        ${CMAKE_CURRENT_SOURCE_DIR}/engine/libs/nativefiledialog/
    )
    # 库链接
    target_link_libraries(${PROJECT_NAME} PRIVATE
        glfw
        Vulkan::Vulkan
        shaderc_shared
        Freetype::Freetype
        
        ${COCOA_LIBRARY}
        ${FOUNDATION_LIBRARY}
        ${APPKIT_LIBRARY}
        ${CMAKE_CURRENT_SOURCE_DIR}/engine/libs/nativefiledialog/mac/lib/libnfd.a
    )
else()
    message(FATAL_ERROR "Unsupported platform")
endif()
# CMAKE_CURRENT_BINARY_DIR：当前 CMakeLists.txt 对应的构建目录
# CMAKE_BINARY_DIR：整个项目的顶级构建目录
# CMAKE_CURRENT_SOURCE_DIR：当前 CMakeLists.txt 所在的源码目录
# CMAKE_SOURCE_DIR：整个项目的顶级源码目录
#编辑器
file(REMOVE_RECURSE ${CMAKE_CURRENT_BINARY_DIR}/res)
file(COPY editor/res/ DESTINATION res)
#内置资源
file(REMOVE_RECURSE ${CMAKE_CURRENT_BINARY_DIR}/resources)
file(COPY engine/resources/ DESTINATION resources)