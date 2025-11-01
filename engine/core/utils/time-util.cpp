#include "time-util.h"

long long TimeUtil::nowTime()
{
    auto now = std::chrono::system_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch());
    // 调用.count() 获取 long 值
    
    return ms.count();  
};