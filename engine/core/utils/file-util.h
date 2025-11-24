#pragma once
#include <string>
#include <vector>
#include <cstdint>
#include <codecvt>
#include <iostream>
#include <fstream>
#include "json-util.h"



class FileUtil
{
public:
    static void saveAtlasAsPGM(const std::string &filename, const std::vector<uint8_t> &atlasData, int width, int height);
    static void saveAtlasAsPNG(const std::string &filename, const std::vector<uint8_t> &atlasData, int width, int height);
    static void saveJsonToBinary(const std::string &filename, const json &data);
    static json loadJsonFromBinary(const std::string &filename);
};
