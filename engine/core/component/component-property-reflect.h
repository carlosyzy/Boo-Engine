#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <functional>
#include <unordered_map>
#include "../utils/json-util.h"

enum class PropertyType
{
    Int = 0,
    Float = 1,
    String = 2,
    Bool = 3,
    Vec2 = 4,
    Vec3 = 5,
    Color = 6,
    Texture = 7,
    Material = 8,
    ShaderV = 9,
    ShaderF = 10,
};
// 属性描述
struct PropertyInfo
{
    int type;
    std::string name;
    std::string displayName;
    std::function<json(void *)> getter;
    std::function<void(void *, const json &)> setter;
};

// 类反射信息
class ClassReflection
{
public:
    std::string className;
    std::unordered_map<std::string, PropertyInfo> properties;

    void addProperty(const PropertyInfo &prop)
    {
        properties[prop.name] = prop;
    }

    const PropertyInfo *getProperty(const std::string &name) const
    {
        auto it = properties.find(name);
        return it != properties.end() ? &it->second : nullptr;
    }

    std::vector<std::string> getPropertyNames() const
    {
        std::vector<std::string> names;
        for (const auto &[name, _] : properties)
        {
            names.push_back(name);
        }
        return names;
    }
};
// 全局反射注册表
class ReflectionRegistry
{
private:
    std::unordered_map<std::string, ClassReflection> _classRegistry;

public:
    static ReflectionRegistry &getInstance()
    {
        static ReflectionRegistry instance;
        return instance;
    }
    template <typename ClassType, typename ValueType>
    void addField(const std::string &className, int type, const std::string &name, const std::string &displayName, ValueType ClassType::*propertyPtr)
    {
        PropertyInfo prop;
        prop.type = type;
        prop.name = name;
        prop.displayName = displayName;
        prop.getter = [propertyPtr](void *obj) -> json
        {
            ClassType *self = static_cast<ClassType *>(obj);
            return self->*propertyPtr;
        };
        prop.setter = [propertyPtr](void *obj, const json &j)
        {
            ClassType *self = static_cast<ClassType *>(obj);
            self->*propertyPtr = j.get<ValueType>();
        };
        auto &classReflection = getOrCreateClassReflection(className);
        classReflection.className = className;
        classReflection.addProperty(prop);
    }
    /**
     * @brief 获取类的所有属性名
     *
     * @param className 类名
     * @return std::vector<std::string> 属性名列表
     */
    std::vector<std::string> getPropertyNames(const std::string &className)
    {
        auto classReflection = getClassReflection(className);
        if (classReflection)
        {
            return classReflection->getPropertyNames();
        }
        return {};
    }
    /**
     * @brief 获取类的属性信息
     *
     * @param className 类名
     * @param propertyName 属性名
     * @return const PropertyInfo* 属性信息指针
     */
    const PropertyInfo *getPropertyInfo(const std::string &className, const std::string &propertyName)
    {
        auto classReflection = getClassReflection(className);
        return classReflection ? classReflection->getProperty(propertyName) : nullptr;
    }
    /**
     * @brief 获取类的属性值
     *
     * @tparam T 属性值类型
     * @param object 类实例指针
     * @param className 类名
     * @param propertyName 属性名
     * @return T 属性值
     */
    template <typename T>
    T getPropertyValue(void *object, const std::string &className, const std::string &propertyName)
    {
        auto propInfo = getPropertyInfo(className, propertyName);
        if (propInfo)
        {
            json value = propInfo->getter(object);
            return value.get<T>();
        }
        return T{};
    }
    /**
     * @brief 设置类的属性值
     *
     * @tparam T 属性值类型
     * @param object 类实例指针
     * @param className 类名
     * @param propertyName 属性名
     * @param value 属性值
     */
    template <typename T>
    void setPropertyValue(void *object, const std::string &className,
                                 const std::string &propertyName, const T &value)
    {
        auto propInfo = getPropertyInfo(className, propertyName);
        if (propInfo)
        {
            propInfo->setter(object, json(value));
        }
    }
    /**
     * @brief 将类实例序列化为 JSON 对象
     *
     * @param object 类实例指针
     * @param className 类名
     * @return json 序列化后的 JSON 对象
     */
    json serializeToJson(void *object, const std::string &className)
    {
        json results=json::array();
        auto &registry = ReflectionRegistry::getInstance();
        auto classReflection = registry.getClassReflection(className);
        if (classReflection)
        {
            for (const auto &[propName, propInfo] : classReflection->properties)
            {
                json result=json::object();
                result["_name"] = propName;
                result["_type"] = propInfo.type;
                result["_value"] = propInfo.getter(object);
                results.push_back(result);
            }
        }
        return results;
    }
    /**
     * @brief 从 JSON 对象反序列化类实例
     *
     * @param object 类实例指针
     * @param className 类名
     * @param j JSON 对象
     */
    void deserializeFromJson(void *object, const std::string &className, const json &j)
    {
        auto &registry = ReflectionRegistry::getInstance();
        auto classReflection = registry.getClassReflection(className);
        if (classReflection)
        {
            for (const auto &[propName, propInfo] : classReflection->properties)
            {
                if (j.contains(propName))
                {
                    propInfo.setter(object, j[propName]);
                }
            }
        }
    }

    ClassReflection &getOrCreateClassReflection(const std::string &className)
    {
        return _classRegistry[className];
    }

    ClassReflection *getClassReflection(const std::string &className)
    {
        auto it = _classRegistry.find(className);
        return it != _classRegistry.end() ? &it->second : nullptr;
    }
};