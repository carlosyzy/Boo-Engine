// #pragma once
// #include <string>
// #include "property-reflect-helper.h"
// #include "../math/vec2.h"
// #include "../math/vec3.h"
// #include "../math/color.h"
// /**
//  * std::string 类型
//  */
// #define REGISTER_PROPERTY_STRING(ClassName, property, displayName) \
// public:                                                      \
//     std::string property;                                        \
// private:                                                     \
//     static inline auto  _field_##property= []() { \
//         PropertyReflectHelper::addField( \
//             #ClassName, \
//             2, \
//             #property, \
//             displayName, \
//             &ClassName::property \
//         ); \
//         return true; }()
