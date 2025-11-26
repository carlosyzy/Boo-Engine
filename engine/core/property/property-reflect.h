#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <filesystem>
#include "../component/component.h"
#include "../utils/json-util.h"
enum class PropertyType
{
    Int,
    Float,
    String,
    Bool,
    Vec2,
    Vec3,
    Color,
    Image,
    Material,
    ShaderV,
    ShaderF,
};

class PropertyReflectHelper
{
public:
    using Getter = std::function<json(void *)>;
    using Setter = std::function<void(void *, const json &)>;
    struct PropertyInfo
    {
        PropertyType type;
        std::string name;
        std::string displayName;
        Getter getter; // 返回字段值
        Setter setter; // 设置字段值
    };
    struct ClassInfo
    {
        std::string uuid;
        std::unordered_map<std::string, PropertyInfo> s_properties; // 类静态成员
    };

private:
    static std::unordered_map<std::string, ClassInfo> s_classes;

public:
    static void addField(const std::string &classUuid, PropertyType type, const std::string &name, const std::string &displayName, Getter getter, Setter setter)
    {
        if (s_classes.find(classUuid) == s_classes.end())
        {
            s_classes[classUuid] = {classUuid, {}};
        }
        s_classes[classUuid].s_properties[name] = {type, name, displayName, getter, setter};
    }
    // 修正：序列化为JSON对象（字典）
    static json serialize(Component *object)
    {
        json result = json::object();
        for (auto &[name, field] : s_classes[object->getUuid()].s_properties)
        {
            result[name] = {
                {"value", field.getter(object)}};
        }
        return result;
    }

    // // 修正：从JSON对象反序列化
    // static void deserialize(void* object, const json& data) {
    //     for (auto& [fieldName, fieldData] : data.items()) {
    //         auto it = getFields().find(fieldName);
    //         if (it != getFields().end()) {
    //             // 从对象中获取value字段
    //             if (fieldData.contains("value")) {
    //                 it->second.setter(object, fieldData["value"]);
    //             }
    //         } else {
    //             std::cout << "⚠️  未知字段: " << fieldName << std::endl;
    //         }
    //     }
    // }

    // static void renderEditor(void* object) {
    //     std::cout << "\n🎮 Inspector:\n";
    //     std::cout << "====================\n";
    //     for (auto& [name, field] : getFields()) {
    //         json value = field.getter(object);
    //         std::cout << "  " << field.displayName << ": " << value << "\n";
    //     }
    // }
};
