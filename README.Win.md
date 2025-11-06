# Boo-Engine

# 资源构建
项目属性 → 生成事件 → 后期生成事件 的命令行中：
编辑器res文件
if exist "$(TargetDir)res" rmdir /s /q "$(TargetDir)res"
if exist "$(ProjectDir)editor\res" (
    xcopy /Y /I /E "$(ProjectDir)editor\res\*" "$(TargetDir)res\"
)
公用基础resoruces文件
if exist "$(TargetDir)resources" rmdir /s /q "$(TargetDir)resources"
if exist "$(ProjectDir)engine\resources" (
    xcopy /Y /I /E "$(ProjectDir)engine\resources\*" "$(TargetDir)resources\"
)
# 编辑器/引擎分支同步
git restore --source=master -- editor
git restore --source=master -- engine 
git restore --source=master -- app.cpp
git restore --source=master -- README.md
