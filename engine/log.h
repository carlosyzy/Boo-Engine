#pragma once
#include <iostream>
#include <cstdio>
#include <ctime>
#include "platforms/platform.h"

#ifndef LOG_TAG_NAME
#define LOG_TAG_NAME "BooEngine"
#endif

// 定义LOG输出
#ifdef NDEBUG
#define LOGV(...) ((void)0)
#define LOGD(...) ((void)0)
#define LOGI(...) ((void)0)
#define LOGW(...) ((void)0)
#define LOGE(...) ((void)0)
#define LOGF(...) ((void)0)
#else
#define LOGV(...) ((void)0)
#define LOGD(...) ((void)0)
#if defined(BOO_PLATFORM_WINDOWS) || defined(BOO_PLATFORM_MACOS)
// Linux/Mac 颜色代码
#define LOG_COLOR_RESET "\033[0m"
#define LOG_COLOR_RED "\033[31m"
#define LOG_COLOR_GREEN "\033[32m"
#define LOG_COLOR_YELLOW "\033[33m"
#define LOG_COLOR_BLUE "\033[34m"
#define LOG_COLOR_MAGENTA "\033[35m"
#define LOG_COLOR_CYAN "\033[36m"
#define LOG_COLOR_WHITE "\033[37m"

enum LogLevel
{
    LOG_LEVEL_VERBOSE = 0,
    LOG_LEVEL_DEBUG,
    LOG_LEVEL_INFO,
    LOG_LEVEL_WARN,
    LOG_LEVEL_ERROR,
    LOG_LEVEL_FATAL
};

// ========== 获取级别字符串 ==========
inline const char *getLevelString(LogLevel level)
{
    switch (level)
    {
    case LOG_LEVEL_VERBOSE:
        return "V";
    case LOG_LEVEL_DEBUG:
        return "D";
    case LOG_LEVEL_INFO:
        return "I";
    case LOG_LEVEL_WARN:
        return "W";
    case LOG_LEVEL_ERROR:
        return "E";
    case LOG_LEVEL_FATAL:
        return "F";
    default:
        return "?";
    }
}
// ========== 获取级别颜色 ==========
inline const char *getLevelColor(LogLevel level)
{
    switch (level)
    {
    case LOG_LEVEL_VERBOSE:
        return LOG_COLOR_WHITE;
    case LOG_LEVEL_DEBUG:
        return LOG_COLOR_BLUE;
    case LOG_LEVEL_INFO:
        return LOG_COLOR_GREEN;
    case LOG_LEVEL_WARN:
        return LOG_COLOR_YELLOW;
    case LOG_LEVEL_ERROR:
        return LOG_COLOR_RED;
    case LOG_LEVEL_FATAL:
        return LOG_COLOR_MAGENTA;
    default:
        return LOG_COLOR_RESET;
    }
}

// ========== 获取当前时间字符串 ==========
inline void getCurrentTime(char *buffer, size_t size)
{
    time_t now = time(nullptr);
    struct tm *tm_info = localtime(&now);
    strftime(buffer, size, "%H:%M:%S", tm_info);
}

// Unix 平台的日志实现
#define __max_log_print(level, tag, ...)                                    \
    do                                                                      \
    {                                                                       \
        char time_str[32];                                                  \
        getCurrentTime(time_str, sizeof(time_str));                         \
        printf("%s[%s] [%s] [%s] " LOG_COLOR_RESET,                         \
               getLevelColor(level), time_str, getLevelString(level), tag); \
        printf(__VA_ARGS__);                                                \
        printf("\n");                                                       \
    } while (0)
// #define LOGV(...) __max_log_print(LOG_LEVEL_VERBOSE, LOG_TAG_NAME, __VA_ARGS__)
// #define LOGD(...) __max_log_print(LOG_LEVEL_DEBUG, LOG_TAG_NAME, __VA_ARGS__)
#define LOGI(...) __max_log_print(LOG_LEVEL_INFO, LOG_TAG_NAME, __VA_ARGS__)
#define LOGW(...) __max_log_print(LOG_LEVEL_WARN, LOG_TAG_NAME, __VA_ARGS__)
#define LOGE(...) __max_log_print(LOG_LEVEL_ERROR, LOG_TAG_NAME, __VA_ARGS__)
#define LOGF(...) __max_log_print(LOG_LEVEL_FATAL, LOG_TAG_NAME, __VA_ARGS__)
#elif defined(BOO_PLATFORM_ANDROID)
// 定义LOG输出
#include <android/log.h>
// #define LOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG_NAME, __VA_ARGS__)
// #define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG_NAME, __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG_NAME, __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN, LOG_TAG_NAME, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG_NAME, __VA_ARGS__)
#define LOGF(...) __android_log_print(ANDROID_LOG_FATAL, LOG_TAG_NAME, __VA_ARGS__)
#elif defined(BOO_PLATFORM_HARMONYOS)
    // 鸿蒙平台的日志输出
    #include "hilog/log.h"
    
    // 鸿蒙日志级别映射
    #undef LOG_DOMAIN
    #undef LOG_TAG
    #define LOG_DOMAIN 0x3200  // 你可以修改为你自己的领域ID
    #define LOG_TAG LOG_TAG_NAME
    
    // #define LOGV(...) OH_LOG_DEBUG(LOG_APP, __VA_ARGS__)  // 鸿蒙没有VERBOSE，用DEBUG代替
    // #define LOGD(...) OH_LOG_DEBUG(LOG_APP, __VA_ARGS__)
    #define LOGI(...) OH_LOG_INFO(LOG_APP, __VA_ARGS__)
    #define LOGW(...) OH_LOG_WARN(LOG_APP, __VA_ARGS__)
    #define LOGE(...) OH_LOG_ERROR(LOG_APP, __VA_ARGS__)
    #define LOGF(...) OH_LOG_FATAL(LOG_APP, __VA_ARGS__)
#else
// 其他平台的日志实现
#define LOGV(...) ((void)0)
#define LOGD(...) ((void)0)
#define LOGI(...) ((void)0)
#define LOGW(...) ((void)0)
#define LOGE(...) ((void)0)
#define LOGF(...) ((void)0)
#endif
#endif
