#include "gfx-shader-compile.h"

GfxShaderCompile::GfxShaderCompile()
{
}
GfxShaderCompile::~GfxShaderCompile()
{
}
GfxShaderCompile* GfxShaderCompile::getInstance()
{
    static GfxShaderCompile _instance;
    return &_instance;
}
void GfxShaderCompile::init()
{

}
std::vector<uint32_t> GfxShaderCompile::compile(const std::string &type, const std::string &cacheKey, const std::string &source, const std::map<std::string, std::string> &macros)
{

    // 配置编译选项
    shaderc::CompileOptions compileOptions;
    // 设置目标环境
    compileOptions.SetTargetEnvironment(
        shaderc_target_env_vulkan,
        shaderc_env_version_vulkan_1_0);
    // 优化级别
    compileOptions.SetOptimizationLevel(shaderc_optimization_level_performance);
    // 生成调试信息
    compileOptions.SetGenerateDebugInfo();
    // 添加宏定义
    // 添加通用宏定义
    compileOptions.AddMacroDefinition("GL_SPIRV", "1");
    compileOptions.AddMacroDefinition("VULKAN", "100");
    for (const auto &[key, value] : macros)
    {
        compileOptions.AddMacroDefinition(key, value);
    }
    // compileOptions.AddMacroDefinition("GL_SPIRV", "1");
    // compileOptions.AddMacroDefinition("VULKAN", "100");

    shaderc::SpvCompilationResult result;
    if (type == "Vert")
    {
        result = this->_compiler.CompileGlslToSpv(
            source.c_str(), shaderc_vertex_shader, cacheKey.c_str(), compileOptions);
    }
    else if (type == "Frag")
    {
        result = this->_compiler.CompileGlslToSpv(
            source.c_str(), shaderc_fragment_shader, cacheKey.c_str(), compileOptions);
    }
    else
    {
        throw std::runtime_error("Shader type not supported");
    }
    shaderc_compilation_status status = result.GetCompilationStatus();
    if (status != shaderc_compilation_status_success)
    {
        std::string errorMsg = "Shader compilation failed:\n";
        errorMsg += "File: " + cacheKey + "\n";
        errorMsg += "Error: " + result.GetErrorMessage();
        errorMsg += "Status: " + std::to_string(status);

        throw std::runtime_error(errorMsg);
    }
    // 输出警告信息
    if (result.GetNumWarnings() > 0)
    {
        std::cout << "Shader compilation warnings for " << cacheKey << ":\n"
                  << result.GetErrorMessage() << std::endl;
    }
    std::vector<uint32_t> spirvCode(result.cbegin(), result.cend());
    std::cout << "Successfully compiled " << cacheKey
              << " (" << spirvCode.size() << " SPIR-V words)" << std::endl;
    return spirvCode;
}
