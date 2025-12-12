如果你想通过JSON动态配置PushConstants数据，你需要一个灵活的机制来处理不同类型的常量。以下是完整的解决方案：

1. 定义PushConstant数据描述
cpp
// PushConstant数据类型枚举
enum class PushConstantType {
    FLOAT,
    VEC2,
    VEC3,
    VEC4,
    INT,
    IVEC2,
    IVEC3,
    IVEC4,
    MAT3,
    MAT4,
    BOOL
};

// 单个PushConstant项的描述
struct PushConstantItem {
    std::string name;           // 常量名称
    PushConstantType type;      // 数据类型
    size_t offset;              // 在PushConstants块中的偏移量
    size_t size;                // 数据大小
    VkShaderStageFlags stages;  // 着色器阶段
};

// PushConstants布局描述
struct PushConstantsLayout {
    std::vector<PushConstantItem> items;
    size_t totalSize = 0;
    
    // 添加新的常量项
    void addItem(const std::string& name, PushConstantType type, 
                 VkShaderStageFlags stages = VK_SHADER_STAGE_ALL) {
        PushConstantItem item;
        item.name = name;
        item.type = type;
        item.stages = stages;
        
        // 根据类型计算大小和对齐
        switch (type) {
            case PushConstantType::FLOAT: item.size = 4; break;
            case PushConstantType::VEC2:  item.size = 8; break;
            case PushConstantType::VEC3:  item.size = 12; break;
            case PushConstantType::VEC4:  item.size = 16; break;
            case PushConstantType::INT:   item.size = 4; break;
            case PushConstantType::IVEC2: item.size = 8; break;
            case PushConstantType::IVEC3: item.size = 12; break;
            case PushConstantType::IVEC4: item.size = 16; break;
            case PushConstantType::MAT3:  item.size = 48; break;  // 注意：mat3占用48字节（3×vec4）
            case PushConstantType::MAT4:  item.size = 64; break;
            case PushConstantType::BOOL:  item.size = 4; break;   // bool在GLSL中是4字节
        }
        
        // 16字节对齐（Vulkan PushConstants通常需要16字节对齐）
        totalSize = (totalSize + 15) & ~15;
        item.offset = totalSize;
        totalSize += item.size;
        
        items.push_back(item);
    }
    
    // 获取常量项
    const PushConstantItem* getItem(const std::string& name) const {
        for (const auto& item : items) {
            if (item.name == name) return &item;
        }
        return nullptr;
    }
};
2. JSON配置解析
json
// push_constants.json
{
    "push_constants": [
        {
            "name": "defaultColor",
            "type": "vec4",
            "stages": "fragment",
            "value": [1.0, 0.5, 0.0, 1.0]
        },
        {
            "name": "modelMatrix",
            "type": "mat4",
            "stages": "vertex",
            "value": [
                1.0, 0.0, 0.0, 0.0,
                0.0, 1.0, 0.0, 0.0,
                0.0, 0.0, 1.0, 0.0,
                0.0, 0.0, 0.0, 1.0
            ]
        },
        {
            "name": "materialID",
            "type": "int",
            "stages": "fragment",
            "value": 1
        },
        {
            "name": "roughness",
            "type": "float",
            "stages": "fragment",
            "value": 0.5
        },
        {
            "name": "maskRect",
            "type": "vec4",
            "stages": "fragment",
            "value": [0.0, 0.0, 1.0, 1.0]
        }
    ]
}
3. PushConstants管理器类
cpp
class DynamicPushConstants {
private:
    PushConstantsLayout _layout;
    std::vector<uint8_t> _data;  // 原始数据存储
    std::unordered_map<std::string, void*> _valuePointers;  // 快速访问指针
    
public:
    // 从JSON文件初始化
    bool initFromJSON(const std::string& jsonFile) {
        // 读取JSON文件
        std::ifstream file(jsonFile);
        if (!file.is_open()) {
            std::cerr << "Failed to open JSON file: " << jsonFile << std::endl;
            return false;
        }
        
        nlohmann::json config;
        file >> config;
        
        // 解析PushConstants配置
        auto& constants = config["push_constants"];
        for (const auto& constant : constants) {
            std::string name = constant["name"];
            std::string typeStr = constant["type"];
            std::string stagesStr = constant["stages"];
            
            // 转换类型
            PushConstantType type;
            if (typeStr == "float") type = PushConstantType::FLOAT;
            else if (typeStr == "vec2") type = PushConstantType::VEC2;
            else if (typeStr == "vec3") type = PushConstantType::VEC3;
            else if (typeStr == "vec4") type = PushConstantType::VEC4;
            else if (typeStr == "int") type = PushConstantType::INT;
            else if (typeStr == "mat4") type = PushConstantType::MAT4;
            else {
                std::cerr << "Unknown type: " << typeStr << std::endl;
                continue;
            }
            
            // 转换着色器阶段
            VkShaderStageFlags stages = 0;
            if (stagesStr == "vertex") stages = VK_SHADER_STAGE_VERTEX_BIT;
            else if (stagesStr == "fragment") stages = VK_SHADER_STAGE_FRAGMENT_BIT;
            else if (stagesStr == "all") stages = VK_SHADER_STAGE_ALL;
            
            // 添加到布局
            _layout.addItem(name, type, stages);
            
            // 解析初始值
            parseAndStoreValue(name, type, constant["value"]);
        }
        
        // 分配数据缓冲区
        _data.resize(_layout.totalSize, 0);
        
        // 设置值指针
        for (const auto& item : _layout.items) {
            _valuePointers[item.name] = _data.data() + item.offset;
        }
        
        return true;
    }
    
    // 解析并存储值
    void parseAndStoreValue(const std::string& name, PushConstantType type, 
                           const nlohmann::json& valueJson) {
        const auto* item = _layout.getItem(name);
        if (!item) return;
        
        uint8_t* dataPtr = _data.data() + item->offset;
        
        switch (type) {
            case PushConstantType::FLOAT:
                *reinterpret_cast<float*>(dataPtr) = valueJson.get<float>();
                break;
            case PushConstantType::VEC4: {
                auto vec = valueJson.get<std::vector<float>>();
                if (vec.size() >= 4) {
                    memcpy(dataPtr, vec.data(), 4 * sizeof(float));
                }
                break;
            }
            case PushConstantType::INT:
                *reinterpret_cast<int*>(dataPtr) = valueJson.get<int>();
                break;
            case PushConstantType::MAT4: {
                auto matrix = valueJson.get<std::vector<float>>();
                if (matrix.size() >= 16) {
                    memcpy(dataPtr, matrix.data(), 16 * sizeof(float));
                }
                break;
            }
            // 添加其他类型的处理...
        }
    }
    
    // 设置值（运行时更新）
    template<typename T>
    void setValue(const std::string& name, const T& value) {
        auto it = _valuePointers.find(name);
        if (it != _valuePointers.end()) {
            *reinterpret_cast<T*>(it->second) = value;
        }
    }
    
    // 获取值
    template<typename T>
    T getValue(const std::string& name) const {
        auto it = _valuePointers.find(name);
        if (it != _valuePointers.end()) {
            return *reinterpret_cast<T*>(it->second);
        }
        return T{};
    }
    
    // 推送PushConstants到命令缓冲区
    void pushToCommandBuffer(VkCommandBuffer cmdBuffer, 
                            VkPipelineLayout pipelineLayout) {
        // 可能需要分多个vkCmdPushConstants调用（如果不同阶段有不同的偏移）
        std::unordered_map<VkShaderStageFlags, std::pair<size_t, size_t>> stageRanges;
        
        // 计算每个着色器阶段的数据范围
        for (const auto& item : _layout.items) {
            auto& range = stageRanges[item.stages];
            if (range.second == 0) {
                range.first = item.offset;
                range.second = item.offset + item.size;
            } else {
                range.first = std::min(range.first, item.offset);
                range.second = std::max(range.second, item.offset + item.size);
            }
        }
        
        // 为每个阶段推送数据
        for (const auto& [stage, range] : stageRanges) {
            size_t size = range.second - range.first;
            vkCmdPushConstants(
                cmdBuffer,
                pipelineLayout,
                stage,
                static_cast<uint32_t>(range.first),  // 偏移
                static_cast<uint32_t>(size),         // 大小
                _data.data() + range.first           // 数据
            );
        }
    }
    
    // 获取总大小
    size_t getTotalSize() const { return _layout.totalSize; }
    
    // 获取布局描述
    const PushConstantsLayout& getLayout() const { return _layout; }
    
    // 获取原始数据（用于调试）
    const uint8_t* getData() const { return _data.data(); }
};