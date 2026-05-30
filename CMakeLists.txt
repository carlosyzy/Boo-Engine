cmake_minimum_required(VERSION 3.15)
project(BooEnginePackage)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# 引入 engine 编译
add_subdirectory(engine)
# -----------------------------------------------
# 安装规则：cmake --install build --prefix ./dist
# -----------------------------------------------
set(ENGINE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/engine)

# 1. 静态库
install(TARGETS Engine
    ARCHIVE DESTINATION .
)

# 2. 引擎自身头文件 -> include/engine/
install(FILES
    ${ENGINE_DIR}/boo.h
    ${ENGINE_DIR}/engine.h
    ${ENGINE_DIR}/log.h
    DESTINATION include
)
install(DIRECTORY ${ENGINE_DIR}/core/
    DESTINATION include/core
    FILES_MATCHING PATTERN "*.h"
)
install(DIRECTORY ${ENGINE_DIR}/platforms/
    DESTINATION include/platforms
    FILES_MATCHING PATTERN "*.h"
)

# 3. 三方库头文件 -> include/libs/（开关控制）
# fastgltf 头文件
install(DIRECTORY ${ENGINE_DIR}/libs/fastgltf/include/
    DESTINATION include/libs/fastgltf
    # FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp"
)
# freetype2 头文件
install(DIRECTORY ${ENGINE_DIR}/libs/freetype2/include/
    DESTINATION include/libs/freetype2
)
# glfw 头文件
install(DIRECTORY ${ENGINE_DIR}/libs/glfw/include/
    DESTINATION include/libs/glfw
)
# miniaudio 头文件
install(DIRECTORY ${ENGINE_DIR}/libs/miniaudio/
    DESTINATION include/libs/miniaudio
)
# nlohmann 头文件
install(FILES
    ${ENGINE_DIR}/libs/nlohmann/json.hpp
    ${ENGINE_DIR}/libs/nlohmann/fifo_map.hpp
    DESTINATION include/libs/nlohmann
)
# simdjson 头文件
install(DIRECTORY ${ENGINE_DIR}/libs/simdjson/include/
    DESTINATION include/libs/simdjson
    # FILES_MATCHING PATTERN "*.h"
)
# stb 头文件
install(DIRECTORY ${ENGINE_DIR}/libs/stb/
    DESTINATION include/libs/stb
    # FILES_MATCHING PATTERN "*.h"
)
# vulkan 头文件
install(DIRECTORY ${ENGINE_DIR}/libs/vulkan/include/vulkan/
    DESTINATION include/libs/vulkan/vulkan
)
install(FILES ${ENGINE_DIR}/libs/vulkan/include/volk/volk.h
    DESTINATION include/libs/vulkan/volk
)
install(DIRECTORY ${ENGINE_DIR}/libs/vulkan/include/vk_video
    DESTINATION include/libs/vulkan/vk_video
)
install(DIRECTORY ${ENGINE_DIR}/libs/vulkan/include/shaderc
    DESTINATION include/libs/vulkan/shaderc
)

# 4. 预编译二进制三方库 —— 按平台复制
if(APPLE)
    # glfw
    install(DIRECTORY ${ENGINE_DIR}/libs/glfw/lib/mac/
        DESTINATION glfw/
    )
    install(DIRECTORY ${ENGINE_DIR}/libs/freetype2/lib/mac/
        DESTINATION freetype2/
    )
    # vulkan 动态库文件
    install(DIRECTORY ${ENGINE_DIR}/libs/vulkan/lib/mac/lib/
        DESTINATION vulkan/lib/
    )
    # vulkan 配置json文件-转换
    install(DIRECTORY ${ENGINE_DIR}/libs/vulkan/lib/mac/vulkan/
        DESTINATION vulkan/vulkan/
    )

elseif(WIN32)
    # glfw
    install(DIRECTORY ${ENGINE_DIR}/libs/glfw/lib/win/
        DESTINATION glfw
    )
    install(DIRECTORY ${ENGINE_DIR}/libs/freetype2/lib/win/
        DESTINATION freetype2
    )
     # vulkan 库文件
    install(DIRECTORY ${ENGINE_DIR}/libs/vulkan/lib/win/
        DESTINATION vulkan/
    )
else()
    message(FATAL_ERROR "Unsupported platform")
endif()

