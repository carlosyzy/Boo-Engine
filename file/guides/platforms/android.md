# Android 平台开发指南

## 概述

Boo Engine 支持 Android 平台开发，使用 Android Studio 作为 IDE。

---

## 一、创建项目

### 方式一：使用创建脚本

```bash
python3 create.py
```

按提示选择：
1. 选择平台：`3. Android`
2. 输入项目目录（需要已存在）
3. 输入项目名称
4. 输入 Android 包名

脚本会自动拷贝模板并修改各项配置。

### 方式二：手动创建

1. 从 `template/Android/` 目录拷贝模板
2. 将模板目录重命名为你的项目名称（如 `MyProject`）
3. 手动修改各项配置（见下文）

### 目录结构

```
MyProject/
├── app/
│   ├── build.gradle
│   └── src/main/
│       ├── AndroidManifest.xml
│       ├── cpp/
│       │   ├── CMakeLists.txt
│       │   ├── engine/         # 引擎源码
│       │   └── boo-engine-jni.cpp
│       ├── java/
│       │   └── com/example/myapp/
│       │       └── MainActivity.java
│       └── res/
├── build.gradle
├── settings.gradle
└── gradle/
```

---

## 二、Android Studio 打开运行

### 导入项目

1. 打开 Android Studio
2. 选择 `Open` 或 `Import Project`
3. 选择项目根目录

### 运行项目

1. 连接 Android 设备或启动模拟器
2. 点击 Android Studio 工具栏的 `Run` 按钮（绿色三角形）
3. 选择目标设备
4. 应用将自动安装并运行

### 构建 APK

通过 Android Studio 菜单 `Build > Build Bundle(s) / APK(s) > Build APK(s)` 构建 APK。
