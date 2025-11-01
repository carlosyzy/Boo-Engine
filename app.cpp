#include <iostream>
#include <filesystem>
#include <thread>
#include "engine/core/math/color.h"
#include "engine/core/math/vec3.h"
#include "engine/core/math/quat.h"
#include "engine/core/math/mat4.h"
#include "engine/core/scene/node-base.h"

int main()
{
    unsigned int n = std::thread::hardware_concurrency();
    std::cout << "Supported concurrent threads: " << n << "\n";
    std::cout << "close" << std::endl;
    Color color;
    std::cout << color.r() << std::endl;
    Vec3 vec;
    std::cout << vec.x() << std::endl;
    Quat quat;
    std::cout << quat.x() << std::endl;
    NodeBase node("","");
    return 0;
}
