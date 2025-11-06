// #pragma once
// #include <string>
// #include <vector>
// #include <map>
// #include <cstdint>
// #include <codecvt>
// #include <locale>

// #include <iostream>
// #include <ft2build.h>
// #include FT_FREETYPE_H
// #include "../math/vec2.h"
// #include "../utils/sdf-util.h"
// #include "../utils/file-util.h"

// // 修改后的字符结构
// struct TextTexture
// {
//     uint32_t width;
//     uint32_t height;
//     std::string texture;
// };



// class FreetypeMgr
// {
// private:
//     FreetypeMgr() = default;
//     ~FreetypeMgr() = default;
//     FreetypeMgr(const FreetypeMgr &) = delete;            // 禁用拷贝构造
//     FreetypeMgr &operator=(const FreetypeMgr &) = delete; // 禁用赋值操作符

//     std::string _fontPath;
//     FT_Face _face;
//     FT_Library _ft;
//     std::wstring _utf8_to_wstring(const std::string &str);
// public:
//     static FreetypeMgr *getInstance();
//     void init();
//     TextTexture crateFont(const std::string &str, uint32_t fontSize, uint32_t lineHeight);
//     void clearFont(std::string key);
//     void destroy();

   
// };
