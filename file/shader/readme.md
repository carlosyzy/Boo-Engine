并将顶点着色器存储在名为 shader.vert 的文件中，将片段着色器存储在该目录中名为 shader.frag 的文件中。 GLSL Shader没有官方的扩展名，但这两个通常用来区分它们。

在这个编译器还有很多可以挖掘的地方。这两个命令告诉编译器读取 GLSL 源文件并使用 -o（输出）标志输出一个 SPIR-V 字节码文件。 如果您的Shader包含语法错误，那么编译器会按照您的预期告诉您行号和问题。例如，尝试省略分号并再次运行编译脚本。还可以尝试不带任何参数运行编译器，以查看它支持哪些类型的标志。例如，它还可以将字节码输出为人类可读的格式，这样您就可以准确地看到Shader正在做什么以及在此阶段应用的任何优化。 在命令行上编译Shader是最直接的选项之一，也是我们将在本教程中使用的选项，但也可以直接从您自己的代码编译Shader。

// 可以通过如下两个命令，将顶点着色器和像素着色器编译为.spv格式。
glslangValidator -V -S vert ui.vert -o ui.vert.spv
glslangValidator -V -S geom ui.geom -o ui.geom.spv
glslangValidator -V -S frag ui.frag -o ui.frag.spv




# 自定义名称输出