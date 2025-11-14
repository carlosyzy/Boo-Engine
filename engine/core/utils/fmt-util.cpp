#include "fmt-util.h"

// UTF-8 转 std::wstring
std::wstring FmtUtil::utf8_to_wstring(const std::string & utf8_str)
{
    std::wstring result;

    for (size_t i = 0; i < utf8_str.size();) {
        uint32_t code_point = 0;
        unsigned char c = static_cast<unsigned char>(utf8_str[i]);

        // 解码 UTF-8 序列
        if ((c & 0x80) == 0x00) {
            // 1字节: 0xxxxxxx
            code_point = c;
            i += 1;
        }
        else if ((c & 0xE0) == 0xC0) {
            // 2字节: 110xxxxx 10xxxxxx
            if (i + 1 >= utf8_str.size()) {
                throw std::runtime_error("Invalid UTF-8: truncated 2-byte sequence");
            }
            unsigned char c2 = static_cast<unsigned char>(utf8_str[i + 1]);
            if ((c2 & 0xC0) != 0x80) {
                throw std::runtime_error("Invalid UTF-8: invalid continuation byte");
            }
            code_point = ((c & 0x1F) << 6) | (c2 & 0x3F);
            i += 2;
        }
        else if ((c & 0xF0) == 0xE0) {
            // 3字节: 1110xxxx 10xxxxxx 10xxxxxx
            if (i + 2 >= utf8_str.size()) {
                throw std::runtime_error("Invalid UTF-8: truncated 3-byte sequence");
            }
            unsigned char c2 = static_cast<unsigned char>(utf8_str[i + 1]);
            unsigned char c3 = static_cast<unsigned char>(utf8_str[i + 2]);
            if ((c2 & 0xC0) != 0x80 || (c3 & 0xC0) != 0x80) {
                throw std::runtime_error("Invalid UTF-8: invalid continuation bytes");
            }
            code_point = ((c & 0x0F) << 12) | ((c2 & 0x3F) << 6) | (c3 & 0x3F);
            i += 3;
        }
        else if ((c & 0xF8) == 0xF0) {
            // 4字节: 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
            if (i + 3 >= utf8_str.size()) {
                throw std::runtime_error("Invalid UTF-8: truncated 4-byte sequence");
            }
            unsigned char c2 = static_cast<unsigned char>(utf8_str[i + 1]);
            unsigned char c3 = static_cast<unsigned char>(utf8_str[i + 2]);
            unsigned char c4 = static_cast<unsigned char>(utf8_str[i + 3]);
            if ((c2 & 0xC0) != 0x80 || (c3 & 0xC0) != 0x80 || (c4 & 0xC0) != 0x80) {
                throw std::runtime_error("Invalid UTF-8: invalid continuation bytes");
            }
            code_point = ((c & 0x07) << 18) | ((c2 & 0x3F) << 12) |
                ((c3 & 0x3F) << 6) | (c4 & 0x3F);
            i += 4;
        }
        else {
            throw std::runtime_error("Invalid UTF-8: invalid starting byte");
        }

        // 添加到结果字符串
#ifdef _WIN32
            // Windows: wchar_t 是 16位，需要处理代理对
        if (code_point <= 0xFFFF) {
            result += static_cast<wchar_t>(code_point);
        }
        else if (code_point <= 0x10FFFF) {
            // 转换为 UTF-16 代理对
            code_point -= 0x10000;
            wchar_t high_surrogate = static_cast<wchar_t>(0xD800 | (code_point >> 10));
            wchar_t low_surrogate = static_cast<wchar_t>(0xDC00 | (code_point & 0x3FF));
            result += high_surrogate;
            result += low_surrogate;
        }
        else {
            throw std::runtime_error("Invalid Unicode code point");
        }
#else
            // Linux/macOS: wchar_t 是 32位，直接存储
        result += static_cast<wchar_t>(code_point);
#endif
    }

    return result;
}

// std::wstring 转 UTF-8
std::string FmtUtil::wstring_to_utf8(const std::wstring & wstr)
{
    std::string result;

    for (size_t i = 0; i < wstr.size();) {
        uint32_t code_point = 0;

#ifdef _WIN32
        // Windows: 处理 UTF-16 代理对
        wchar_t wc1 = wstr[i];
        if (wc1 >= 0xD800 && wc1 <= 0xDBFF && i + 1 < wstr.size()) {
            // 高代理项
            wchar_t wc2 = wstr[i + 1];
            if (wc2 >= 0xDC00 && wc2 <= 0xDFFF) {
                // 有效的代理对
                code_point = 0x10000 + (((wc1 & 0x3FF) << 10) | (wc2 & 0x3FF));
                i += 2;
            }
            else {
                // 无效的代理对，当作普通字符
                code_point = static_cast<uint32_t>(wc1);
                i += 1;
            }
        }
        else {
            code_point = static_cast<uint32_t>(wc1);
            i += 1;
        }
#else
        // Linux/macOS: 直接转换
        code_point = static_cast<uint32_t>(wstr[i]);
        i += 1;
#endif

        // 编码为 UTF-8
        if (code_point <= 0x7F) {
            // 1字节
            result += static_cast<char>(code_point);
        }
        else if (code_point <= 0x7FF) {
            // 2字节
            result += static_cast<char>(0xC0 | (code_point >> 6));
            result += static_cast<char>(0x80 | (code_point & 0x3F));
        }
        else if (code_point <= 0xFFFF) {
            // 3字节
            result += static_cast<char>(0xE0 | (code_point >> 12));
            result += static_cast<char>(0x80 | ((code_point >> 6) & 0x3F));
            result += static_cast<char>(0x80 | (code_point & 0x3F));
        }
        else if (code_point <= 0x10FFFF) {
            // 4字节
            result += static_cast<char>(0xF0 | (code_point >> 18));
            result += static_cast<char>(0x80 | ((code_point >> 12) & 0x3F));
            result += static_cast<char>(0x80 | ((code_point >> 6) & 0x3F));
            result += static_cast<char>(0x80 | (code_point & 0x3F));
        }
        else {
            throw std::runtime_error("Invalid Unicode code point");
        }
    }

    return result;
}

// // 验证 UTF-8 字符串
// static bool is_valid_utf8(const std::string& str) {
//     try {
//         auto wide = utf8_to_wstring(str);
//         auto back = wstring_to_utf8(wide);
//         return str == back;
//     }
//     catch (...) {
//         return false;
//     }
// }

// // 安全转换（遇到错误返回空字符串）
// static std::wstring safe_utf8_to_wstring(const std::string& utf8_str) {
//     try {
//         return utf8_to_wstring(utf8_str);
//     }
//     catch (const std::exception& e) {
//         // 输出错误信息（可选）
//         // std::cerr << "UTF-8 conversion error: " << e.what() << std::endl;
//         return L"";
//     }
// }

// // 获取字符串长度（字符数，不是字节数）
// static size_t character_count(const std::string& utf8_str) {
//     try {
//         auto wide_str = utf8_to_wstring(utf8_str);
//         return wide_str.length();
//     }
//     catch (...) {
//         return 0;
//     }
// }