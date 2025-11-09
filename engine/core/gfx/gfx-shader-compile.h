#pragma once
#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <vulkan/vulkan_core.h>
#include <shaderc/shaderc.hpp>
class GfxContext;

class GfxShaderCompile
{
private:
    GfxShaderCompile(const GfxShaderCompile &) = delete;            /* // 禁用拷贝构造 */
    GfxShaderCompile &operator=(const GfxShaderCompile &) = delete; /* // 禁用赋值操作符 */
    GfxShaderCompile();
    ~GfxShaderCompile();

    shaderc::Compiler _compiler;
public:
    static GfxShaderCompile *getInstance();
    /**
     * @brief 初始化
     * 
     */
    void init();
    /**
     * @brief 编译着色器
     * @param path 着色器路径
     */
    std::vector<uint32_t> compile(const std::string &type,const std::string &cacheKey,const std::string &source,const std::map<std::string, std::string> &macros);
};