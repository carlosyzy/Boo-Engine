# 引擎部分

# resources 资源目录
项目属性 → 生成事件 → 后期生成事件 的命令行中：
if exist "$(TargetDir)resources" rmdir /s /q "$(TargetDir)resources"
if exist "$(ProjectDir)engine\resources" (
    xcopy /Y /I /E "$(ProjectDir)engine\resources\*" "$(TargetDir)resources\"
)
