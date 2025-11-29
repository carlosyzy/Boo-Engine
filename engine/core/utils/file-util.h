#pragma once
#include <string>
#include <vector>
#include <cstdint>
#include <codecvt>
#include <iostream>
#include <fstream>
#include <filesystem>
#include "json-util.h"



class FileUtil
{
public:
    static void saveAtlasAsPGM(const std::string &filename, const std::vector<uint8_t> &atlasData, int width, int height);
    static void saveAtlasAsPNG(const std::string &filename, const std::vector<uint8_t> &atlasData, int width, int height);
    static void saveJsonToBinary(const std::string &filename, const json &data);
    static void saveJsonToText(const std::string &filename, const json &data);
    static json readJsonFromBinary(const std::string &filename);
    static void copyFile(const std::string &src, const std::string &dst);
    static void removeFile(const std::string &path);
};
