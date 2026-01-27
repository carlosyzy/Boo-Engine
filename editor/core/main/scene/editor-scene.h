#pragma once
class Scene;

class EditorScene
{
private:
    /* data */
public:
    EditorScene(/* args */);
    void init();
    void update(float deltaTime);
    // void openScene(Scene *scene);

    ~EditorScene();
};



/**
 * 有两个摄像机
 * 一个是2D_Editor摄像机，用于编辑器渲染2D元素
 * 一个是3D_Editor摄像机，用于编辑器渲染3D元素
 */