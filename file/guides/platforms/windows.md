# Windows 平台开发指南

## 概述

Boo Engine 支持 Windows 平台开发，使用 Visual Studio 作为 IDE，CMake 作为构建系统。

---

## 一、创建项目

### 方式一：使用创建脚本

```bash
python3 create.py
```

按提示选择：
1. 选择平台：`1. Windows`
2. 输入项目目录（需要已存在）
3. 输入项目名称

脚本会自动拷贝模板并修改项目名称。

### 方式二：手动创建

1. 从 `template/Windows/` 目录拷贝模板
2. 将模板目录重命名为你的项目名称（如 `MyProject`）

```bash
cp -r template/Windows/ MyProject
```

---

## 二、构建引擎静态库

### 方式一：自动构建

```bash
python3 build.py
```

选择 `1. Windows`，脚本会自动完成构建和安装。

### 方式二：手动构建

```powershell
# 1. 在引擎根目录创建构建目录
mkdir build
cd build

# 2. 配置 CMake（指定 Visual Studio 生成器）
cmake -S . -B build -G "Visual Studio 17 2022" -A x64

# 3. 构建 Release 版本
cmake --build build --config Release

# 4. 安装到 dist 目录
cmake --install build --prefix ./dist --config Release
```

### 构建产物

```
dist/
├── include/
│   ├── engine/           # 引擎自身头文件
│   └── libs/             # 三方库头文件
├── libs/
│   └── BooEngine.lib      # 静态库
└── CMakeLists.txt        # 接入示例
```

---

## 三、项目目录结构

```
MyProject/
├── CMakeLists.txt        # CMake 构建配置
├── app.cpp              # 应用程序入口
├── assets/              # 资源文件目录
├── src/                 # 源代码目录
└── engine/              # 引擎目录（存放 BooEngine.lib）
```

### 主要目录说明

| 目录 | 说明 |
|------|------|
| `CMakeLists.txt` | CMake 构建配置 |
| `app.cpp` | 应用程序入口 |
| `assets/` | 资源文件目录 |
| `src/` | 源代码目录 |
| `engine/` | 引擎目录，存放静态库和头文件 |

---

## 四、项目运行

### 应用入口

`app.cpp` 是应用程序的入口文件，负责初始化引擎、创建窗口并启动游戏循环。

### 编译运行

```powershell
# 1. 配置 CMake
cmake -B build -DCMAKE_BUILD_TYPE=Debug

# 2. 构建
cmake --build build

# 3. 运行
./build/MyProject.exe
```
