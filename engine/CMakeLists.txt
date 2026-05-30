# -----------------------------------------------
# 收集源文件
# -----------------------------------------------
file(GLOB_RECURSE ENGINE_CORE_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/core/*.cpp"
)
file(GLOB_RECURSE ENGINE_PLATFORM_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/platforms/*.cpp"
)
file(GLOB_RECURSE ENGINE_LIB_NLOHMANN_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/libs/nlohmann/*.cpp"
)
file(GLOB_RECURSE ENGINE_LIB_SIMDJSON_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/libs/simdjson/*.cpp"
)
file(GLOB_RECURSE ENGINE_LIB_STB_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/libs/stb/*.cpp"
)
file(GLOB_RECURSE ENGINE_LIB_VOLK_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/libs/vulkan/include/volk/*.c"
)
file(GLOB_RECURSE ENGINE_LIB_TINYXML2_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/libs/tinyxml2/*.cpp"
)
file(GLOB_RECURSE ENGINE_LIB_FASTGLTF_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/libs/fastgltf/*.cpp"
)

# -----------------------------------------------
# 创建静态库
# -----------------------------------------------
set(ENGINE_SOURCES
    ${ENGINE_CORE_SOURCES}
    ${ENGINE_PLATFORM_SOURCES}
    ${ENGINE_LIB_SIMDJSON_SOURCES}
    ${ENGINE_LIB_NLOHMANN_SOURCES}
    ${ENGINE_LIB_STB_SOURCES}
    ${ENGINE_LIB_VOLK_SOURCES}
    ${ENGINE_LIB_TINYXML2_SOURCES}
    "${CMAKE_CURRENT_SOURCE_DIR}/boo.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/engine.cpp"
)
if(WIN32 OR ANDROID OR APPLE)
     list(APPEND ENGINE_SOURCES ${ENGINE_LIB_FASTGLTF_SOURCES})
endif()
add_library(Engine STATIC ${ENGINE_SOURCES})

# 自定义输出文件名：libBooEngine.a / BooEngine.lib
set_target_properties(Engine PROPERTIES OUTPUT_NAME "BooEngine")
# Debug 版本添加 d 后缀
set_target_properties(Engine PROPERTIES DEBUG_POSTFIX "d")

# -----------------------------------------------
# 引擎自身头文件路径
# -----------------------------------------------
target_include_directories(Engine PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

if(WIN32)
    # 收集当前目录的 .cpp 文件（不递归）
    file(GLOB ENGINE_SOURCES_ROOT
        "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp"
    )
    file(GLOB_RECURSE ENGINE_SOURCES_SUB 
        "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp"
    )

    set(ENGINE_ALL_SOURCES ${ENGINE_SOURCES_ROOT} ${ENGINE_SOURCES_SUB})
    # 【新增】将所有头文件添加到 target 用于显示
    target_sources(Engine PRIVATE ${ENGINE_ALL_SOURCES})
    # ========== 创建源文件分组，保持目录结构 ==========
    # 为每个源文件创建对应的VS过滤器（文件夹）
    foreach(FILE ${ENGINE_ALL_SOURCES})
        file(RELATIVE_PATH REL_PATH "${CMAKE_CURRENT_SOURCE_DIR}" "${FILE}")
        get_filename_component(FILE_DIR "${REL_PATH}" DIRECTORY)
        
        if(FILE_DIR)
            string(REPLACE "\\" "/" GROUP_NAME "${FILE_DIR}")
            source_group("${GROUP_NAME}" FILES "${FILE}")
        else()
            # 关键：使用空字符串，文件就会出现在项目根目录
            source_group("" FILES "${FILE}")
        endif()
    endforeach()
endif()

# -----------------------------------------------
# Volk 配置
# -----------------------------------------------
if(ANDROID)
    # Android 平台
    set(VOLK_STATIC_DEFINES VK_USE_PLATFORM_ANDROID_KHR)
    target_compile_definitions(Engine PRIVATE VK_USE_PLATFORM_ANDROID_KHR)
elseif(WIN32)
    set(VOLK_STATIC_DEFINES VK_USE_PLATFORM_WIN32_KHR)
    target_compile_definitions(Engine PRIVATE VK_USE_PLATFORM_WIN32_KHR)
elseif(UNIX AND NOT APPLE)
    set(VOLK_STATIC_DEFINES VK_USE_PLATFORM_XLIB_KHR)
elseif(APPLE)
    set(VOLK_STATIC_DEFINES VK_USE_PLATFORM_MACOS_MVK)
endif()

# -----------------------------------------------
# 三方库头文件路径（跨平台统一 include/）
# -----------------------------------------------
target_include_directories(Engine BEFORE PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/glfw/include
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/freetype2/include
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/nlohmann/include
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/simdjson/include
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/stb
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/vulkan/include
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/tinyxml2
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/miniaudio
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/fastgltf/include
)

# -----------------------------------------------
# 平台相关链接库
# -----------------------------------------------
if(APPLE)
    set(VULKAN_LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libs/vulkan/lib/mac)
    target_link_libraries(Engine PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/libs/glfw/lib/mac/libglfw3.a
        ${CMAKE_CURRENT_SOURCE_DIR}/libs/freetype2/lib/mac/libfreetype.a
        # shaderc
        ${VULKAN_LIB_DIR}/lib/libvulkan.1.dylib
        ${VULKAN_LIB_DIR}/lib/libshaderc_shared.1.dylib
    )
    # add_custom_command(TARGET Engine POST_BUILD
    #     COMMAND ${CMAKE_COMMAND} -E copy_directory
    #         ${VULKAN_LIB_DIR}
    #         ${CMAKE_BINARY_DIR}
    # )
elseif(WIN32)
    set(VULKAN_LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libs/vulkan/lib/win)
    target_link_libraries(Engine PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/libs/glfw/lib/win/lib-vc2022/glfw3.lib
        ${CMAKE_CURRENT_SOURCE_DIR}/libs/freetype2/lib/win/freetype.lib
        # vulkan
        ${VULKAN_LIB_DIR}/vulkan-1.lib
        debug ${VULKAN_LIB_DIR}/shaderc_sharedd.lib
        optimized ${VULKAN_LIB_DIR}/shaderc_shared.lib
    )
    # add_custom_command(TARGET Engine POST_BUILD
    #     COMMAND ${CMAKE_COMMAND} -E copy_directory
    #         "${VULKAN_LIB_DIR}"
    #         "$<TARGET_FILE_DIR:Engine>"
    #     )
elseif(ANDROID)
    # 查找必要的 Android 库
    find_library(log-lib log)
    find_library(android-lib android)
    # 链接库
    target_link_libraries(Engine
        ${log-lib}
        ${android-lib}
        ${CMAKE_CURRENT_SOURCE_DIR}/libs/freetype2/lib/android/${ANDROID_ABI}/libfreetype.a
        # shaderc
        ${CMAKE_CURRENT_SOURCE_DIR}/libs/shaderc/lib/android/c++_static/${ANDROID_ABI}/libshaderc.a
    )
elseif(OHOS OR DEFINED OHOS_PLATFORM)
    target_link_libraries(Engine
        libz.so
        libace_napi.z.so           # 提供 napi_module_register 等 NAPI 核心函数
        libace_ndk.z.so             # ✅ ArkUI/XComponent 原生接口库 (提供 OH_NativeXComponent_* API)
        libhilog_ndk.z.so          # 提供 HiLog 日志功能（如果需要）
        libnative_window.so
        # shaderc
        ${CMAKE_CURRENT_SOURCE_DIR}/libs/shaderc/lib/harmony/${OHOS_ARCH}/libshaderc_combined.a
    )
else()
    message(FATAL_ERROR "Unsupported platform")
endif()

message(STATUS "Engine sources collected: ${ENGINE_CORE_SOURCES}")