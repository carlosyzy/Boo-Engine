#pragma once
#include <string>
#include <iostream>

class FmtUtil
{
public:
    // UTF-8 转 std::wstring
    static std::wstring utf8_to_wstring(const std::string &utf8_str);
    // std::wstring 转 UTF-8
    static std::string wstring_to_utf8(const std::wstring &wstr);
};
