#pragma once
#include <string>
#include <vector>
#include <map>
#include <cstdint>
#include <codecvt>
#include <locale>

#include <iostream>
#include <ft2build.h>
#include FT_FREETYPE_H
#include "../math/vec2.h"
#include "../utils/fmt-util.h"
#include "../utils/file-util.h"

// 修改后的字符结构
struct TextTexture
{
    std::string uuid;
    uint32_t width;
    uint32_t height;
    uint32_t channels;
    std::vector<uint8_t> datas;
};

class FreetypeMgr
{
private:
    FreetypeMgr(const FreetypeMgr &) = delete;            // 禁用拷贝构造
    FreetypeMgr &operator=(const FreetypeMgr &) = delete; // 禁用赋值操作符

    std::string _fontPath;
    FT_Face _face;
    FT_Library _ft;

public:
    FreetypeMgr();
    void init();
    void crateFont(TextTexture &textTexture, const std::string &str, uint32_t fontSize, uint32_t lineHeight);
    void clearFont(std::string key);


    ~FreetypeMgr();
};
