#include <iostream>
#include "engine/engine.h"

int main()
{
    Engine *engine = new Engine();
    engine->init();
    engine->tick();
    delete engine;
    std::cout << "close" << std::endl;
    return 0;
}
