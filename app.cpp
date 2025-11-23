#include <iostream>
#include "editor/editor.h"

int main()
{
    Editor *editor = new Editor();
    editor->init();
    editor->run();
    delete editor;
    editor = nullptr;
    return 0;
}
