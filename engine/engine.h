// https://chat.deepseek.com/a/chat/s/6acc0de2-2900-44a6-8cb8-4375f5e92b8e 结构调整参考
#pragma once
#include <iostream>
#include <memory> // 智能指针头文件
#include <thread> // 线程头文件
#include <string>
class Windows;
class Android;
class MacOS;
class HarmonyOS;
class Game;
namespace Boo
{
    enum class UIDesignFitMode;
    struct InitConfig;
}
class Engine
{

public:
    Engine();
    /**
     * window 平台下传入内置的Window封装
     * @param uiDesignWidth 设计宽度
     * @param uiDesignHeight 设计高度
     * @param fitMode 适配模式
     */
    void init(Windows *windows,Boo::InitConfig &config);
    /**
     * android 平台下传入内置的Android封装
     * @param android Android封装
     * @param uiDesignWidth 设计宽度
     * @param uiDesignHeight 设计高度
     * @param fitMode 适配模式
     * @param width 实际宽度
     * @param height 实际高度
     */
    void init(Android *android,Boo::InitConfig &config);
    void init(MacOS *macos,Boo::InitConfig &config);
    void init(HarmonyOS *harmonyos,Boo::InitConfig &config);

    void tick();

    ~Engine();
};
