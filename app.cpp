#include <iostream>
#include "engine/engine.h"

int main()
{
    Engine *engine = new Engine();
    engine->init();
    engine->run();
    delete engine;
    engine = nullptr;
    std::cout << "close" << std::endl;
    return 0;
}
