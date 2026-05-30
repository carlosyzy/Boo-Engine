# Boo Engine 跨平台构建文档

## 📋 目录

1. **环境要求**
2. **macOS 构建步骤**
3. **Windows 构建步骤**
4. **构建产物说明**
5. **常见问题解决**

## 1. 环境要求
### 1.1平台
| 平台 | 必需工具 |
|------|----------|
| macOS | CMake 3.15+、Xcode 命令行工具 |
| Windows | CMake 3.15+、 Visual Studio |
### 1.2shaderc 库
- shaderc 库是 Vulkan 1.2 中新增的库，用于编译 SPIR-V 着色器。shaderc库源码已经包含在项目engine/libs/shaderc目录下;但是在构建打包之前还需手动执行utils的指定脚本去同步所需的依赖库。
```bash
# 进入项目根目录
cd engine/libs/shaderc/
# 执行同步脚本
./utils/git-sync-deps
```
## 2. macOS 构建步骤
```bash
# 创建构建目录
# 配置 CMake
cmake -B build -DCMAKE_BUILD_TYPE=Release
# 构建项目
cmake --build build

# 安装到 dist 目录
cmake --install build --prefix ./dist
```

## 3. Windows 构建步骤

### 3.1 Visual Studio 构建（生成 .lib 文件）
```powershell
# 创建构建目录
# mkdir build
# cd build
# 配置 CMake（指定 Visual Studio 生成器）
# Visual Studio 2022 示例
cmake -S . -B build -G "Visual Studio 17 2022" -A x64
# 构建项目
cmake --build build --config Release
# 安装到 dist 目录
cmake --install build --prefix ./dist --config Release
```


## 4. 构建产物说明

### 4.1 生成的文件结构
```
dist/
├── include/
│   ├── engine/           # 引擎自身头文件
│   │   ├── core/
│   │   ├── platforms/
│   │   ├── boo.h
│   │   ├── engine.h
│   │   └── log.h
│   └── libs/             # 三方库头文件（仅当 BOO_INSTALL_THIRDPARTY_HEADERS=ON 时）
│       ├── fastgltf/
│       ├── freetype2/
│       ├── glfw/
│       ├── glslang/
│       ├── nlohmann/
│       ├── simdjson/
│       ├── stb/
│       ├── volk/
│       └── vulkan/
├── libs/                 # 预编译库
│   ├── glfw/             # GLFW 库
│   ├── freetype2/        # FreeType 库
│   ├── glslang/          # GLSLang 库
│   └── libBooEngine.a    # MinGW 生成的静态库
│       或
│       BooEngine.lib      # Visual Studio 生成的静态库
└── CMakeLists.txt        # 示例 CMake 配置文件
```

### 4.2 平台特定文件
- **macOS**：`libBooEngine.a`（静态库）
- **Windows (Visual Studio)**：`BooEngine.lib`（静态库）

### 5. 接入项目
- 将 `dist/` 目录下的文件复制到项目根目录,建议将 `dist/` 目录重命名为 `engine/`。
- 配置 CMake 配置文件（如 `CMakeLists.txt`），添加以下内容：
```cmake
set(BOO_ENGINE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/engine)
add_library(BooEngine STATIC IMPORTED)
if(APPLE)
    set_target_properties(BooEngine PROPERTIES
        IMPORTED_LOCATION ${BOO_ENGINE_DIR}/libs/libBooEngine.a
    )
elseif(WIN32)
    set_target_properties(BooEngine PROPERTIES
        IMPORTED_LOCATION ${BOO_ENGINE_DIR}/libs/BooEngine/BooEngine.lib
    )
else()
    message(FATAL_ERROR "Unsupported platform")
endif()

# 包含 Engine 头文件目录（引擎和三方库头文件）
target_include_directories(BooEngine INTERFACE
    ${BOO_ENGINE_DIR}/include
    ${BOO_ENGINE_DIR}/include/libs/fastgltf
    ${BOO_ENGINE_DIR}/include/libs/freetype2
    ${BOO_ENGINE_DIR}/include/libs/glfw
    ${BOO_ENGINE_DIR}/include/libs/glslang
    ${BOO_ENGINE_DIR}/include/libs/nlohmann
    ${BOO_ENGINE_DIR}/include/libs/simdjson
    ${BOO_ENGINE_DIR}/include/libs/stb
    ${BOO_ENGINE_DIR}/include/libs/vulkan
    ${BOO_ENGINE_DIR}/include/libs/volk
)

# 平台相关系统库
if(APPLE)
    find_library(COCOA_LIBRARY     Cocoa      REQUIRED)
    find_library(FOUNDATION_LIBRARY Foundation REQUIRED)
    find_library(APPKIT_LIBRARY    AppKit     REQUIRED)
    find_library(IOKIT_LIBRARY     IOKit      REQUIRED)

    # freetype2 依赖的系统库
    find_package(ZLIB  REQUIRED)
    find_package(BZip2 REQUIRED)
    find_package(PNG   REQUIRED)

    target_link_libraries(BooEngine INTERFACE
        ZLIB::ZLIB
        BZip2::BZip2
        PNG::PNG
        ${IOKIT_LIBRARY}
        ${COCOA_LIBRARY}
        ${FOUNDATION_LIBRARY}
        ${APPKIT_LIBRARY}
        ${BOO_ENGINE_DIR}/libs/freetype2/libfreetype.a
        ${BOO_ENGINE_DIR}/libs/glfw/libglfw3.a
        ${BOO_ENGINE_DIR}/libs/glslang/libglslang.a
        ${BOO_ENGINE_DIR}/libs/glslang/libglslang-default-resource-limits.a
        ${BOO_ENGINE_DIR}/libs/glslang/libSPIRV.a
        ${BOO_ENGINE_DIR}/libs/glslang/libSPIRV-Tools.a
        ${BOO_ENGINE_DIR}/libs/glslang/libSPIRV-Tools-opt.a
    )
elseif(WIN32)
    target_link_libraries(BooEngine INTERFACE
        ${BOO_ENGINE_DIR}/libs/freetype2/lib/freetype.lib
        ${BOO_ENGINE_DIR}/libs/glfw/lib-vc2022/glfw3_mt.lib
        # glslang
        debug ${BOO_ENGINE_DIR}/libs/glslang/glslangd.lib
        optimized ${BOO_ENGINE_DIR}/libs/glslang/glslang.lib
        debug ${BOO_ENGINE_DIR}/libs/glslang/glslang-default-resource-limitsd.lib
        optimized ${BOO_ENGINE_DIR}/libs/glslang/glslang-default-resource-limits.lib
        debug ${BOO_ENGINE_DIR}/libs/glslang/SPIRVd.lib
        optimized ${BOO_ENGINE_DIR}/libs/glslang/SPIRV.lib
        debug ${BOO_ENGINE_DIR}/libs/glslang/SPIRV-Toolsd.lib
        optimized ${BOO_ENGINE_DIR}/libs/glslang/SPIRV-Tools.lib
        debug ${BOO_ENGINE_DIR}/libs/glslang/SPIRV-Tools-optd.lib
        optimized ${BOO_ENGINE_DIR}/libs/glslang/SPIRV-Tools-opt.lib
    )
else()
    message(FATAL_ERROR "Unsupported platform")
endif()

# add_executable(${PROJECT_NAME}  ) 后边添加 BooEngine 库链接
target_link_libraries(${PROJECT_NAME} PRIVATE BooEngine)~
```

