#pragma once
#include <string>
#include <vector>
#include <cstdint>
#include <codecvt>
#include <iostream>
#include <sstream>
#include <fstream>
#include <filesystem>
#include "core/util/json-util.h"


namespace Boo {

class FileUtil
{
public:
    static void saveAtlasAsPGM(const std::string &filename, const std::vector<uint8_t> &atlasData, int width, int height);
    static void saveAtlasAsPNG(const std::string &filename, const std::vector<uint8_t> &atlasData, int width, int height);
    static void saveJsonToBinary(const std::string &filename, const json &data);
    static void saveJsonToText(const std::string &filename, const json &data);
    static std::string readTextFile(const std::string &filename);
    static json readJsonFromBinary(const std::string &filename);
    static json readJsonFromText(const std::string &filename);
    static void copyFile(const std::string &src, const std::string &dst);
    static void removeFile(const std::string &path);
};

} // namespace Boo
