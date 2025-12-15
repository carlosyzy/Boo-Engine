#pragma once
#include <filesystem>

class EditorLayout;
class EditorScene;

class EditorMain
{
private :
    long long _deltaTime;
    int _frameRate;

    EditorLayout *_editorLayout = nullptr;
    void _launchEditor();

    EditorScene *_editorScene = nullptr;
     // EditorHierarchy *_hierarchy;
    // EditorAssets *_assets;
    // EditorProperty *_property;
    // void _initEditorModules();
    // void _initEditorRunScene();
    // void _launchEditorScene();
public:
    EditorMain();
    void init();
    

    void tick();
    void update(float deltaTime);
    
    ~EditorMain();
};