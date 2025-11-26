#pragma once
#include <iostream> 
#include "component-factory.h"
/**
 * @brief 自动注册宏 - 在全局命名空间注册组件
 * @param ComponentClass 组件类名
 * @param TypeName 组件类型名称（字符串）
 * @param Description 组件描述（可选）
 */
#define REGISTER_COMPONENT(ComponentClass, TypeName, Description) \
    namespace { \
        class ComponentClass##Registrar { \
        public: \
            ComponentClass##Registrar() { \
                bool success = ComponentFactory::getInstance().registerComponent<ComponentClass>( \
                    TypeName, \
                    Description \
                ); \
                if (!success) { \
                    std::cerr << "❌ 注册组件 " << TypeName << " 失败" << std::endl; \
                } \
            } \
        }; \
        static ComponentClass##Registrar ComponentClass##_registrar; \
    }