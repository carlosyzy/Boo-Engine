#include "assets.h"

#ifdef _WIN32
#include <windows.h>
#elif defined(__APPLE__)
#include <unistd.h>
#include <limits.h>
#include <mach-o/dyld.h>  // 必需的头文件
#endif
#include <iostream>

Assets::Assets()
{
}
void Assets::_initRoot()
{
#ifdef _WIN32
	char exePath[MAX_PATH];
	GetModuleFileNameA(NULL, exePath, MAX_PATH);
	this->_root = std::filesystem::path(exePath).parent_path().string();
#elif defined(__APPLE__)
	uint32_t size = 0;
	_NSGetExecutablePath(nullptr, &size);

	std::vector<char> buffer(size);
	if (_NSGetExecutablePath(buffer.data(), &size) == 0) {
		// 使用 filesystem 处理路径
		fs::path exePath = fs::canonical(buffer.data());
		this->_root = exePath.parent_path().string();
	}
	else {
		std::cerr << "Mac获取可执行文件路径失败" << std::endl;
	}
#endif
	std::cout << "Assets root:"<<this->_root << std::endl;
}



Assets::~Assets()
{
}
