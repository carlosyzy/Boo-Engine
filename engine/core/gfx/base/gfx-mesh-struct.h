#pragma once
#include <vulkan/vulkan_core.h>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <vector>
#include <set>
#include <map>
#include <cstdint>
#include <array>
#include "gfx-struct.h"
#include "gfx-mesh.h"

// GfxMesh *uiMesh = new GfxMesh("789abcde-f012-34a5-b678-901234567890");
// uiMesh->setInputVertices({-0.5f, 0.5f, 0.0f, 0.0f, 0.0f,
//                           -0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
//                           0.5f, -0.5f, 0.0f, 1.0f, 1.0f,
//                           0.5f, 0.5f, 0.0f, 1.0f, 0.0f},
//                          {0, 1, 2, 0, 2, 3});