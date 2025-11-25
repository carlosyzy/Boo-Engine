#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
class EditorSceneCache
{
public:
    EditorSceneCache();
    void init();
    void update();
    void clear();
    ~EditorSceneCache();
};