#include "harmonyos.h"
#include "boo.h"
#include "log.h"
HarmonyOS::HarmonyOS()
{
}
const int HarmonyOS::getWidth()
{
    return this->_width;
}
const int HarmonyOS::getHeight()
{
    return this->_height;
}
#if defined(BOO_PLATFORM_HARMONYOS)
HarmonyOS::HarmonyOS(OHNativeWindow* window){
    this->_nativeWindow=window;
    OH_NativeWindow_NativeWindowHandleOpt(this->_nativeWindow,GET_BUFFER_GEOMETRY,&this->_width, &this->_height);
    LOGI("NativeWindow Size: %d x %d", this->_width, this->_height);
}
OHNativeWindow *HarmonyOS::getNativeWindow()
{
    return this->_nativeWindow;
}
#endif
void HarmonyOS::tick()
{
}