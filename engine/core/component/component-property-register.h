#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include "component-property-reflect.h"
/**
 * int 类型
 */
#define REGISTER_PROPERTY_INT(ComponentClass, property, displayName) \
public:                                                              \
    int property;                                                    \
                                                                     \
private:                                                             \
    static inline auto _field_##property = []() { \
        ReflectionRegistry::getInstance().addField( \
            #ComponentClass, \
            0, \
            #property, \
            displayName, \
            &ComponentClass::property \
        ); \
        return true; }()

/**
 * float 类型
 */
#define REGISTER_PROPERTY_FLOAT(ComponentClass, property, displayName) \
public:                                                                \
    float property;                                                    \
                                                                       \
private:                                                               \
    static inline auto _field_##property = []() { \
        ReflectionRegistry::getInstance().addField( \
            #ComponentClass, \
            1, \
            #property, \
            displayName, \
            &ComponentClass::property \
        ); \
        return true; }()

/**
 * std::string 类型
 * 字符串类型的属性，用于存储文本等信息
 */
#define REGISTER_PROPERTY_STRING(ComponentClass, property, displayName) \
public:                                                                 \
    std::string property;                                               \
                                                                        \
private:                                                                \
    static inline auto _field_##property = []() { \
        ReflectionRegistry::getInstance().addField( \
            #ComponentClass, \
            2, \
            #property, \
            displayName, \
            &ComponentClass::property \
        ); \
        return true; }()