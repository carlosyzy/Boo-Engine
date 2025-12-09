明白了！你想让用户/玩家在运行时自定义数据结构，然后动态创建对应的 UBO。这在静态类型的 C++/Vulkan
  中是个挑战，但有几种实用方案。

  方案 1: 字节缓冲区 + 元数据描述（最灵活）

  #include <string>
  #include <vector>
  #include <map>
  #include <cstring>

  // 支持的数据类型
  enum class FieldType {
      Float,      // 4 字节
      Vec2,       // 8 字节
      Vec3,       // 12 字节 (对齐到 16)
      Vec4,       // 16 字节
      Mat4,       // 64 字节
      Int,        // 4 字节
      Bool        // 4 字节 (GPU 中 bool 是 4 字节)
  };

  // 字段元数据
  struct FieldMetadata {
      std::string name;
      FieldType type;
      size_t offset;      // 在缓冲区中的偏移
      size_t size;        // 字节大小
      size_t alignment;   // 对齐要求
  };

  // 动态 UBO 构建器
  class DynamicUBOBuilder {
  private:
      std::vector<FieldMetadata> fields;
      size_t currentOffset = 0;
      size_t totalSize = 0;

      // 获取类型信息
      static size_t getTypeSize(FieldType type) {
          switch (type) {
              case FieldType::Float:  return 4;
              case FieldType::Vec2:   return 8;
              case FieldType::Vec3:   return 12;
              case FieldType::Vec4:   return 16;
              case FieldType::Mat4:   return 64;
              case FieldType::Int:    return 4;
              case FieldType::Bool:   return 4;
              default: return 0;
          }
      }

      static size_t getTypeAlignment(FieldType type) {
          switch (type) {
              case FieldType::Float:  return 4;
              case FieldType::Vec2:   return 8;
              case FieldType::Vec3:   return 16;  // Vec3 对齐到 16 字节
              case FieldType::Vec4:   return 16;
              case FieldType::Mat4:   return 16;  // Mat4 列对齐到 16 字节
              case FieldType::Int:    return 4;
              case FieldType::Bool:   return 4;
              default: return 4;
          }
      }

      // 计算对齐后的偏移
      size_t alignOffset(size_t offset, size_t alignment) {
          return (offset + alignment - 1) & ~(alignment - 1);
      }

  public:
      // 添加字段
      DynamicUBOBuilder& addField(const std::string& name, FieldType type) {
          FieldMetadata field;
          field.name = name;
          field.type = type;
          field.size = getTypeSize(type);
          field.alignment = getTypeAlignment(type);

          // 计算对齐后的偏移
          currentOffset = alignOffset(currentOffset, field.alignment);
          field.offset = currentOffset;

          fields.push_back(field);

          currentOffset += field.size;

          std::cout << "Added field: " << name
                    << " (offset: " << field.offset
                    << ", size: " << field.size << ")" << std::endl;

          return *this;
      }

      // 构建完成，计算总大小
      size_t finalize() {
          // UBO 需要对齐到 16 字节
          totalSize = alignOffset(currentOffset, 16);

          std::cout << "✓ UBO structure finalized:" << std::endl;
          std::cout << "  Fields: " << fields.size() << std::endl;
          std::cout << "  Total size: " << totalSize << " bytes" << std::endl;

          return totalSize;
      }

      const std::vector<FieldMetadata>& getFields() const {
          return fields;
      }

      size_t getTotalSize() const {
          return totalSize;
      }

      // 生成 GLSL 代码
      std::string generateGLSL(const std::string& uboName = "DynamicUBO") const {
          std::string glsl = "layout(set = 0, binding = 0) uniform " + uboName + " {\n";

          for (const auto& field : fields) {
              std::string typeStr;
              switch (field.type) {
                  case FieldType::Float: typeStr = "float"; break;
                  case FieldType::Vec2:  typeStr = "vec2"; break;
                  case FieldType::Vec3:  typeStr = "vec3"; break;
                  case FieldType::Vec4:  typeStr = "vec4"; break;
                  case FieldType::Mat4:  typeStr = "mat4"; break;
                  case FieldType::Int:   typeStr = "int"; break;
                  case FieldType::Bool:  typeStr = "bool"; break;
              }

              glsl += "    " + typeStr + " " + field.name + ";\n";
          }

          glsl += "} " + uboName[0] + std::string(1, std::tolower(uboName[0])) + ";\n";

          return glsl;
      }
  };

  // 动态 UBO 数据容器
  class DynamicUBOData {
  private:
      std::vector<uint8_t> buffer;
      const DynamicUBOBuilder& builder;
      std::map<std::string, size_t> fieldOffsets;

  public:
      DynamicUBOData(const DynamicUBOBuilder& builder)
          : builder(builder) {
          buffer.resize(builder.getTotalSize(), 0);

          // 构建字段名到偏移的映射
          for (const auto& field : builder.getFields()) {
              fieldOffsets[field.name] = field.offset;
          }
      }

      // 设置字段值
      void setFloat(const std::string& name, float value) {
          size_t offset = fieldOffsets[name];
          memcpy(buffer.data() + offset, &value, sizeof(float));
      }

      void setVec2(const std::string& name, const glm::vec2& value) {
          size_t offset = fieldOffsets[name];
          memcpy(buffer.data() + offset, &value, sizeof(glm::vec2));
      }

      void setVec3(const std::string& name, const glm::vec3& value) {
          size_t offset = fieldOffsets[name];
          memcpy(buffer.data() + offset, &value, sizeof(glm::vec3));
      }

      void setVec4(const std::string& name, const glm::vec4& value) {
          size_t offset = fieldOffsets[name];
          memcpy(buffer.data() + offset, &value, sizeof(glm::vec4));
      }

      void setMat4(const std::string& name, const glm::mat4& value) {
          size_t offset = fieldOffsets[name];
          memcpy(buffer.data() + offset, &value, sizeof(glm::mat4));
      }

      void setInt(const std::string& name, int value) {
          size_t offset = fieldOffsets[name];
          memcpy(buffer.data() + offset, &value, sizeof(int));
      }

      void setBool(const std::string& name, bool value) {
          size_t offset = fieldOffsets[name];
          int intValue = value ? 1 : 0;  // GPU 中 bool 是 4 字节
          memcpy(buffer.data() + offset, &intValue, sizeof(int));
      }

      // 获取原始数据
      const void* data() const {
          return buffer.data();
      }

      size_t size() const {
          return buffer.size();
      }

      // 上传到 GPU
      void uploadToGPU(void* mappedMemory) const {
          memcpy(mappedMemory, buffer.data(), buffer.size());
      }
  };

  使用示例

  // 用户定义 UBO 结构
  void userDefinesStructure() {
      // 玩家/用户可以在运行时定义结构
      DynamicUBOBuilder builder;

      // 假设用户通过 UI 或配置文件定义了这些字段
      builder.addField("viewMatrix", FieldType::Mat4)
             .addField("projectionMatrix", FieldType::Mat4)
             .addField("cameraPosition", FieldType::Vec3)
             .addField("time", FieldType::Float)
             .addField("enableLighting", FieldType::Bool)
             .addField("lightColor", FieldType::Vec3)
             .addField("lightIntensity", FieldType::Float);

      size_t uboSize = builder.finalize();

      // 生成对应的 GLSL 代码
      std::string glslCode = builder.generateGLSL("CustomUBO");
      std::cout << "\nGenerated GLSL:\n" << glslCode << std::endl;

      // 创建 GPU 缓冲区
      VkBuffer ubo;
      VkDeviceMemory uboMemory;
      void* uboMapped;

      createBuffer(uboSize,
                  VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                  VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                  VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                  ubo, uboMemory);
      vkMapMemory(device, uboMemory, 0, uboSize, 0, &uboMapped);

      // 创建数据容器
      DynamicUBOData uboData(builder);

      // 设置数据
      uboData.setMat4("viewMatrix", camera.getViewMatrix());
      uboData.setMat4("projectionMatrix", camera.getProjectionMatrix());
      uboData.setVec3("cameraPosition", camera.position);
      uboData.setFloat("time", currentTime);
      uboData.setBool("enableLighting", true);
      uboData.setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
      uboData.setFloat("lightIntensity", 100.0f);

      // 上传到 GPU
      uboData.uploadToGPU(uboMapped);

      std::cout << "✓ Uploaded " << uboSize << " bytes to GPU" << std::endl;
  }

  方案 2: 从配置文件加载结构定义

  #include <json/json.h>  // 使用 JSON 库

  class UBODefinitionLoader {
  public:
      // 从 JSON 加载 UBO 定义
      static DynamicUBOBuilder loadFromJSON(const std::string& jsonStr) {
          Json::Value root;
          Json::Reader reader;

          if (!reader.parse(jsonStr, root)) {
              throw std::runtime_error("Failed to parse JSON");
          }

          DynamicUBOBuilder builder;

          const Json::Value fields = root["fields"];
          for (const auto& field : fields) {
              std::string name = field["name"].asString();
              std::string typeStr = field["type"].asString();

              FieldType type = parseFieldType(typeStr);
              builder.addField(name, type);
          }

          builder.finalize();
          return builder;
      }

  private:
      static FieldType parseFieldType(const std::string& typeStr) {
          if (typeStr == "float") return FieldType::Float;
          if (typeStr == "vec2") return FieldType::Vec2;
          if (typeStr == "vec3") return FieldType::Vec3;
          if (typeStr == "vec4") return FieldType::Vec4;
          if (typeStr == "mat4") return FieldType::Mat4;
          if (typeStr == "int") return FieldType::Int;
          if (typeStr == "bool") return FieldType::Bool;

          throw std::runtime_error("Unknown field type: " + typeStr);
      }
  };

  // 使用示例
  void loadUserDefinedStructure() {
      // 用户提供的 JSON 配置
      std::string jsonConfig = R"(
      {
          "fields": [
              {"name": "viewMatrix", "type": "mat4"},
              {"name": "projectionMatrix", "type": "mat4"},
              {"name": "cameraPosition", "type": "vec3"},
              {"name": "time", "type": "float"},
              {"name": "enableLighting", "type": "bool"}
          ]
      }
      )";

      // 加载并构建 UBO
      DynamicUBOBuilder builder = UBODefinitionLoader::loadFromJSON(jsonConfig);

      // 生成 GLSL
      std::string glsl = builder.generateGLSL();
      std::cout << glsl << std::endl;

      // 创建数据容器
      DynamicUBOData data(builder);

      // ... 使用
  }

  方案 3: 运行时编译 Shader

  #include <shaderc/shaderc.hpp>

  class RuntimeShaderCompiler {
  public:
      // 根据 UBO 定义动态生成并编译 shader
      static std::vector<uint32_t> compileShader(
          const DynamicUBOBuilder& uboBuilder,
          const std::string& shaderCode,
          shaderc_shader_kind shaderKind) {

          // 生成 UBO 定义
          std::string uboDefinition = uboBuilder.generateGLSL("DynamicUBO");

          // 完整的 shader 代码
          std::string fullShaderCode =
              "#version 450\n\n" +
              uboDefinition + "\n" +
              shaderCode;

          std::cout << "Compiling shader with dynamic UBO:\n"
                    << fullShaderCode << std::endl;

          // 使用 shaderc 编译
          shaderc::Compiler compiler;
          shaderc::CompileOptions options;

          auto result = compiler.CompileGlslToSpv(
              fullShaderCode,
              shaderKind,
              "dynamic_shader.glsl",
              options
          );

          if (result.GetCompilationStatus() != shaderc_compilation_status_success) {
              throw std::runtime_error("Shader compilation failed: " +
                                     result.GetErrorMessage());
          }

          return {result.cbegin(), result.cend()};
      }
  };

  // 使用示例
  void dynamicShaderPipeline() {
      // 1. 用户定义 UBO 结构
      DynamicUBOBuilder builder;
      builder.addField("modelMatrix", FieldType::Mat4)
             .addField("viewMatrix", FieldType::Mat4)
             .addField("projectionMatrix", FieldType::Mat4)
             .addField("color", FieldType::Vec4);
      builder.finalize();

      // 2. 顶点着色器代码（不包含 UBO 定义）
      std::string vertexShaderCode = R"(
          layout(location = 0) in vec3 inPosition;
          layout(location = 0) out vec4 fragColor;

          void main() {
              gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(inPosition, 1.0);
              fragColor = color;
          }
      )";

      // 3. 动态编译
      std::vector<uint32_t> spirv = RuntimeShaderCompiler::compileShader(
          builder,
          vertexShaderCode,
          shaderc_vertex_shader
      );

      // 4. 创建 shader module
      VkShaderModuleCreateInfo createInfo{};
      createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
      createInfo.codeSize = spirv.size() * sizeof(uint32_t);
      createInfo.pCode = spirv.data();

      VkShaderModule shaderModule;
      vkCreateShaderModule(device, &createInfo, nullptr, &shaderModule);

      std::cout << "✓ Created shader module with dynamic UBO" << std::endl;
  }

  方案 4: 使用 SSBO（更灵活）

  // SSBO 可以存储任意结构的数据
  class FlexibleSSBOManager {
  private:
      struct FieldDescriptor {
          std::string name;
          FieldType type;
          size_t offset;
      };

      std::vector<FieldDescriptor> fields;
      std::vector<uint8_t> buffer;

  public:
      void defineStructure(const std::vector<std::pair<std::string, FieldType>>& fieldDefs) {
          size_t offset = 0;

          for (const auto& [name, type] : fieldDefs) {
              FieldDescriptor desc;
              desc.name = name;
              desc.type = type;
              desc.offset = offset;

              fields.push_back(desc);
              offset += DynamicUBOBuilder::getTypeSize(type);
          }

          buffer.resize(offset);
      }

      // 在 shader 中
      /*
      layout(set = 0, binding = 0) readonly buffer FlexibleSSBO {
          // 作为字节数组
          uint data[];
      } ssbo;

      // 通过偏移访问
      mat4 viewMatrix = mat4(
          ssbo.data[0], ssbo.data[1], ssbo.data[2], ssbo.data[3],
          ssbo.data[4], ssbo.data[5], ssbo.data[6], ssbo.data[7],
          ssbo.data[8], ssbo.data[9], ssbo.data[10], ssbo.data[11],
          ssbo.data[12], ssbo.data[13], ssbo.data[14], ssbo.data[15]
      );
      */
  };

  完整使用流程

  class DynamicRenderSystem {
  private:
      DynamicUBOBuilder uboBuilder;
      VkBuffer uboBuffer;
      VkDeviceMemory uboMemory;
      void* uboMapped;

  public:
      // 1. 初始化：用户定义结构
      void initialize(const std::vector<std::pair<std::string, FieldType>>& userFields) {
          // 根据用户定义构建 UBO
          for (const auto& [name, type] : userFields) {
              uboBuilder.addField(name, type);
          }

          size_t uboSize = uboBuilder.finalize();

          // 创建 GPU 缓冲区
          createBuffer(uboSize,
                      VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                      VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                      VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                      uboBuffer, uboMemory);
          vkMapMemory(device, uboMemory, 0, uboSize, 0, &uboMapped);

          // 生成并编译 shader
          std::string glsl = uboBuilder.generateGLSL();
          std::cout << "Generated UBO structure:\n" << glsl << std::endl;
      }

      // 2. 每帧更新
      void updateFrame(const std::map<std::string, std::any>& values) {
          DynamicUBOData data(uboBuilder);

          // 根据用户提供的值更新
          for (const auto& [name, value] : values) {
              // 根据类型设置值
              if (value.type() == typeid(float)) {
                  data.setFloat(name, std::any_cast<float>(value));
              }
              else if (value.type() == typeid(glm::vec3)) {
                  data.setVec3(name, std::any_cast<glm::vec3>(value));
              }
              else if (value.type() == typeid(glm::mat4)) {
                  data.setMat4(name, std::any_cast<glm::mat4>(value));
              }
              // ... 其他类型
          }

          // 上传到 GPU
          data.uploadToGPU(uboMapped);
      }
  };

  // 使用
  void main() {
      DynamicRenderSystem renderSystem;

      // 用户定义结构
      std::vector<std::pair<std::string, FieldType>> userDefinedStructure = {
          {"viewMatrix", FieldType::Mat4},
          {"projMatrix", FieldType::Mat4},
          {"cameraPos", FieldType::Vec3},
          {"time", FieldType::Float},
          {"enableFog", FieldType::Bool}
      };

      renderSystem.initialize(userDefinedStructure);

      // 渲染循环
      while (running) {
          // 用户提供值
          std::map<std::string, std::any> values;
          values["viewMatrix"] = camera.getViewMatrix();
          values["projMatrix"] = camera.getProjectionMatrix();
          values["cameraPos"] = camera.position;
          values["time"] = (float)glfwGetTime();
          values["enableFog"] = true;

          renderSystem.updateFrame(values);

          // ... 渲染
      }
  }

  总结

  | 方案           | 灵活性   | 性能   | 复杂度 | 推荐度   |
  |--------------|-------|------|-----|-------|
  | 字节缓冲区 + 元数据  | ⭐⭐⭐⭐⭐ | ⭐⭐⭐⭐ | 中   | ⭐⭐⭐⭐⭐ |
  | JSON 配置      | ⭐⭐⭐⭐⭐ | ⭐⭐⭐⭐ | 中   | ⭐⭐⭐⭐  |
  | 运行时编译 Shader | ⭐⭐⭐⭐⭐ | ⭐⭐⭐  | 高   | ⭐⭐⭐   |
  | SSBO         | ⭐⭐⭐⭐  | ⭐⭐⭐⭐ | 低   | ⭐⭐⭐⭐  |

  推荐：使用方案 1（字节缓冲区 + 元数据），它提供了最好的平衡！