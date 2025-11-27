#pragma once
#include <string>
#include "property-reflect.h"
#include "../math/vec2.h"
#include "../math/vec3.h"
#include "../math/color.h"

/**
 * int 类型
 */
#define SERIALIZE_FIELD_INT(name, displayName, uuid) \
    int name;                                  \
    static inline auto _field_##name = []() { \
        PropertyReflectHelper::addField( \
            uuid,  /* 使用 this->getUuid() 获取UUID */ \
            PropertyType::Int, \
            #name, displayName,  \
            [](void* obj) -> json { \
                return static_cast<std::decay_t<decltype(*this)>*>(obj)->name; \
            }, [](void* obj, const json& j) { \
                static_cast<std::decay_t<decltype(*this)>*>(obj)->name = j.get<int>(); \
            } \
        ); \
        return true; }()

/**
 * float 类型
 */
#define SERIALIZE_FIELD_FLOAT(name, displayName, uuid) \
    float name;                                  \
    static inline auto _field_##name = []() { \
        PropertyReflectHelper::addField( \
            uuid,  /* 使用 this->getUuid() 获取UUID */ \
            PropertyType::Float, \
            #name, displayName,  \
            [](void* obj) -> json { \
                return static_cast<std::decay_t<decltype(*this)>*>(obj)->name; \
            }, [](void* obj, const json& j) { \
                static_cast<std::decay_t<decltype(*this)>*>(obj)->name = j.get<float>(); \
            } \
        ); \
        return true; }()
/**
 * std::string 类型
 */
#define SERIALIZE_FIELD_STRING(name, displayName, uuid) \
    std::string name;                                   \
    static inline auto _field_##name = []() { \
        PropertyReflectHelper::addField( \
            uuid,  \
            PropertyType::String, \
            #name,   \
            displayName,  \
            [](void* obj) -> json { \
                return static_cast<std::decay_t<decltype(*this)>*>(obj)->name; \
            }, [](void* obj, const json& j) { \
                static_cast<std::decay_t<decltype(*this)>*>(obj)->name = j.get<std::string>(); \
            } \
        ); \
        return true; }()
/**
 * bool 类型
 */
#define SERIALIZE_FIELD_BOOL(name, displayName, uuid) \
    bool name;                                  \
    static inline auto _field_##name = []() { \
        PropertyReflectHelper::addField( \
            uuid,  /* 使用 this->getUuid() 获取UUID */ \
            PropertyType::Bool, \
            #name, displayName,  \
            [](void* obj) -> json { \
                return static_cast<std::decay_t<decltype(*this)>*>(obj)->name; \
            }, [](void* obj, const json& j) { \
                static_cast<std::decay_t<decltype(*this)>*>(obj)->name = j.get<bool>(); \
            } \
        ); \
        return true; }()
/**
 * Vec2 类型
 */
#define SERIALIZE_FIELD_VEC2(name, displayName, uuid) \
    Vec2 name;                                  \
    static inline auto _field_##name = []() { \
        PropertyReflectHelper::addField( \
            uuid,  /* 使用 this->getUuid() 获取UUID */ \
            PropertyType::Vec2, \
            #name, displayName,  \
            [](void* obj) -> json { \
                return static_cast<std::decay_t<decltype(*this)>*>(obj)->name; \
            }, [](void* obj, const json& j) { \
                static_cast<std::decay_t<decltype(*this)>*>(obj)->name = j.get<Vec2>(); \
            } \
        ); \
        return true; }()
/**
 * Vec3 类型
 */
#define SERIALIZE_FIELD_VEC3(name, displayName, uuid) \
    Vec3 name;                                  \
    static inline auto _field_##name = []() { \
        PropertyReflectHelper::addField( \
            uuid,  /* 使用 this->getUuid() 获取UUID */ \
            PropertyType::Vec3, \
            #name, displayName,  \
            [](void* obj) -> json { \
                return static_cast<std::decay_t<decltype(*this)>*>(obj)->name; \
            }, [](void* obj, const json& j) { \
                static_cast<std::decay_t<decltype(*this)>*>(obj)->name = j.get<Vec3>(); \
            } \
        ); \
        return true; }()
/**
 * Color 类型
 */
#define SERIALIZE_FIELD_COLOR(name, displayName, uuid) \
    Color name;                                  \
    static inline auto _field_##name = []() { \
        PropertyReflectHelper::addField( \
            uuid,  /* 使用 this->getUuid() 获取UUID */ \
            PropertyType::Color, \
            #name, displayName,  \
            [](void* obj) -> json { \
                return static_cast<std::decay_t<decltype(*this)>*>(obj)->name; \
            }, [](void* obj, const json& j) { \
                static_cast<std::decay_t<decltype(*this)>*>(obj)->name = j.get<Color>(); \
            } \
        ); \
        return true; }()
/**
 * Image 类型
 */
#define SERIALIZE_FIELD_IMAGE(name, displayName, uuid) \
    std::string name;                            \
    static inline auto _field_##name = []() { \
        PropertyReflectHelper::addField( \
            uuid,  /* 使用 this->getUuid() 获取UUID */ \
            PropertyType::Image, \
            #name, displayName,  \
            [](void* obj) -> json { \
                return static_cast<std::decay_t<decltype(*this)>*>(obj)->name; \
            }, [](void* obj, const json& j) { \
                static_cast<std::decay_t<decltype(*this)>*>(obj)->name = j.get<Image>(); \
            } \
        ); \
        return true; }()

/*

class GameObject {
public:
    SERIALIZE_FIELD(std::string, licenseKey, "许可证");
    // 宏展开后相当于：
    std::string licenseKey;
    static inline auto _field_licenseKey = []() {
        // 这个Lambda在GameObject的上下文中执行！
        SerializeHelper::addField("licenseKey", "许可证",
            // Getter Lambda
            [](void* obj) -> json {
                // 这里 *this 指的是 GameObject
                return static_cast<std::decay_t<decltype(*this)>*>(obj)->licenseKey;
            },
            // Setter Lambda
            [](void* obj, const json& j) {
                static_cast<std::decay_t<decltype(*this)>*>(obj)->licenseKey = j.get<std::string>();
            }
        );
        return true;
    }()
};
*/