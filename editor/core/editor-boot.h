#pragma once
#include <filesystem>

class EditorLayout;
class EditorScene;

class EditorBoot
{
private :
    void _launchEditorLoading();
    void _launchEditorMain();
    
public:
    EditorBoot();
    void init();
    void tick(float deltaTime);
    ~EditorBoot();
};