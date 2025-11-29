// #pragma once
// #include <iostream>
// #include <vector>
// #include <string>
// #include <unordered_map>
// #include <filesystem>
// #include "../component/component.h"
// #include "../utils/json-util.h"
// enum class PropertyType
// {
//     Int = 0,
//     Float = 1,
//     String = 2,
//     Bool = 3,
//     Vec2 = 4,
//     Vec3 = 5,
//     Color = 6,
//     Image = 7,
//     Material = 8,
//     ShaderV = 9,
//     ShaderF = 10,
// };
// // 属性描述
// struct PropertyInfo
// {
//     int type;
//     std::string name;
//     std::string displayName;
//     std::function<json(void *)> getter;
//     std::function<void(void *, const json &)> setter;
// };

// // 类反射信息
// class ClassReflection
// {
// public:
//     std::string className;
//     std::unordered_map<std::string, PropertyInfo> properties;

//     void addProperty(const PropertyInfo &prop)
//     {
//         properties[prop.name] = prop;
//     }

//     const PropertyInfo *getProperty(const std::string &name) const
//     {
//         auto it = properties.find(name);
//         return it != properties.end() ? &it->second : nullptr;
//     }

//     std::vector<std::string> getPropertyNames() const
//     {
//         std::vector<std::string> names;
//         for (const auto &[name, _] : properties)
//         {
//             names.push_back(name);
//         }
//         return names;
//     }
// };
// // 全局反射注册表
// class ReflectionRegistry
// {
// private:
//     std::unordered_map<std::string, ClassReflection> classRegistry;

// public:
//     static ReflectionRegistry &getInstance()
//     {
//         static ReflectionRegistry instance;
//         return instance;
//     }

//     ClassReflection &getOrCreateClassReflection(const std::string &className)
//     {
//         return classRegistry[className];
//     }

//     ClassReflection *getClassReflection(const std::string &className)
//     {
//         auto it = classRegistry.find(className);
//         return it != classRegistry.end() ? &it->second : nullptr;
//     }
// };

// class PropertyReflectHelper
// {
// public:
//     // using Getter = std::function<json(void *)>;
//     // using Setter = std::function<void(void *, const json &)>;
//     // struct PropertyInfo
//     // {
//     //     int type;
//     //     std::string name;
//     //     std::string displayName;
//     //     Getter getter; // 返回字段值
//     //     Setter setter; // 设置字段值
//     // };
//     // struct ClassInfo
//     // {
//     //     std::string uuid;
//     //     std::unordered_map<std::string, PropertyInfo> s_properties; // 类静态成员
//     // };

// private:
//     // static std::unordered_map<std::string, ClassInfo> s_classes;

// public:
//     template <typename ClassType, typename ValueType>
//     static void addField(const std::string &className, int type, const std::string &name, const std::string &displayName, ValueType ClassType::*propertyPtr)
//     {
//         PropertyInfo prop;
//         prop.type = type;
//         prop.name = name;
//         prop.displayName = displayName;
//         prop.getter = [propertyPtr](void *obj) -> json
//         {
//             ClassType *self = static_cast<ClassType *>(obj);
//             return self->*propertyPtr;
//         };
//         prop.setter = [propertyPtr](void *obj, const json &j)
//         {
//             ClassType *self = static_cast<ClassType *>(obj);
//             self->*propertyPtr = j.get<ValueType>();
//         };
//         auto &registry = ReflectionRegistry::getInstance();
//         auto &classReflection = registry.getOrCreateClassReflection(className);
//         classReflection.className = className;
//         classReflection.addProperty(prop);
//     }

//     // // 修正：序列化为JSON对象（字典）
//     // static json serialize(Component *object)
//     // {
//     //     json result = json::object();
//     //     for (auto &[name, field] : s_classes[object->getUuid()].s_properties)
//     //     {
//     //         result[name] = {
//     //             {"value", field.getter(object)}};
//     //     }
//     //     return result;
//     // }

//     // // 修正：从JSON对象反序列化
//     // static void deserialize(void* object, const json& data) {
//     //     for (auto& [fieldName, fieldData] : data.items()) {
//     //         auto it = getFields().find(fieldName);
//     //         if (it != getFields().end()) {
//     //             // 从对象中获取value字段
//     //             if (fieldData.contains("value")) {
//     //                 it->second.setter(object, fieldData["value"]);
//     //             }
//     //         } else {
//     //             std::cout << "⚠️  未知字段: " << fieldName << std::endl;
//     //         }
//     //     }
//     // }

//     // static void renderEditor(void* object) {
//     //     std::cout << "\n🎮 Inspector:\n";
//     //     std::cout << "====================\n";
//     //     for (auto& [name, field] : getFields()) {
//     //         json value = field.getter(object);
//     //         std::cout << "  " << field.displayName << ": " << value << "\n";
//     //     }
//     // }
// };


// class ReflectionHelper
// {
// public:
//     static std::vector<std::string> getPropertyNames(const std::string &className)
//     {
//         auto &registry = ReflectionRegistry::getInstance();
//         auto classReflection = registry.getClassReflection(className);
//         if (classReflection)
//         {
//             return classReflection->getPropertyNames();
//         }
//         return {};
//     }

//     static const PropertyInfo *getPropertyInfo(const std::string &className, const std::string &propertyName)
//     {
//         auto &registry = ReflectionRegistry::getInstance();
//         auto classReflection = registry.getClassReflection(className);
//         return classReflection ? classReflection->getProperty(propertyName) : nullptr;
//     }

//     template <typename T>
//     static T getPropertyValue(void *object, const std::string &className, const std::string &propertyName)
//     {
//         auto propInfo = getPropertyInfo(className, propertyName);
//         if (propInfo)
//         {
//             json value = propInfo->getter(object);
//             return value.get<T>();
//         }
//         return T{};
//     }

//     template <typename T>
//     static void setPropertyValue(void *object, const std::string &className,
//                                  const std::string &propertyName, const T &value)
//     {
//         auto propInfo = getPropertyInfo(className, propertyName);
//         if (propInfo)
//         {
//             propInfo->setter(object, json(value));
//         }
//     }

//     static json serializeToJson(void *object, const std::string &className)
//     {
//         json result;
//         auto &registry = ReflectionRegistry::getInstance();
//         auto classReflection = registry.getClassReflection(className);
//         if (classReflection)
//         {
//             for (const auto &[propName, propInfo] : classReflection->properties)
//             {
//                 result[propName] = propInfo.getter(object);
//             }
//         }
//         return result;
//     }

//     static void deserializeFromJson(void *object, const std::string &className, const json &j)
//     {
//         auto &registry = ReflectionRegistry::getInstance();
//         auto classReflection = registry.getClassReflection(className);
//         if (classReflection)
//         {
//             for (const auto &[propName, propInfo] : classReflection->properties)
//             {
//                 if (j.contains(propName))
//                 {
//                     propInfo.setter(object, j[propName]);
//                 }
//             }
//         }
//     }
// };
