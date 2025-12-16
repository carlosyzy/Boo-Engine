#pragma once
#include <filesystem>

class EditorLayout;
class EditorScene;

class EditorBoot
{
private :
    long long _deltaTime;
    int _frameRate;
    void _launchEditorLoading();
    void _launchEditorMain();
    
public:
    EditorBoot();
    void init();
    

    void tick();
    void update(float deltaTime);
    
    ~EditorBoot();
};