#pragma once
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "../../libs/nativefiledialog/nfd.h"

// const nfdchar_t *SceneFilterList[] = {
//     "Scene files", "scene",  // 第一组过滤器
//     "All files", "*",        // 第二组过滤器
//     NULL                     // 结束标记
// };
extern const nfdchar_t *SceneFilterList;

class DialogUtil
{
public:
    static void openFileDialog();
    static std::string saveFileDialog(const std::string &defaultPath, const nfdchar_t *filterList);
};
