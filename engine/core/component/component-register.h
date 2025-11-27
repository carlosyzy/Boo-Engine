#pragma once
#include <iostream> 
#include "component-factory.h"
/**
 * @brief 自动注册宏 - 在全局命名空间注册组件
 * @param Component 组件类名
 * @param Description 组件描述（可选）
 */
#define REGISTER_COMPONENT(ComponentClass,Description) \
    namespace { \
        class ComponentClass##Registrar { \
        public: \
            ComponentClass##Registrar() { \
                bool success = ComponentFactory::getInstance().registerComponent<ComponentClass>( \
                    #ComponentClass, \
                    Description \
                ); \
            } \
        }; \
        static ComponentClass##Registrar ComponentClass##_registrar; \
    }