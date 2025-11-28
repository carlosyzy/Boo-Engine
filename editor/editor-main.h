#pragma once
#include <filesystem>

class EditorLayout;
class EditorScene;

class EditorMain
{
private :
    int _initDelayScheduleID = 0;
    void _startLoading();

    EditorLayout *_editorLayout = nullptr;
    void _launchEditor();

    EditorScene *_editorScene = nullptr;
     // EditorHierarchy *_hierarchy;
    // EditorAssets *_assets;
    // EditorProperty *_property;
    void _initEditorModules();
    void _initEditorRunScene();
    void _launchEditorScene();
public:
    EditorMain();
    void init();
    


    
    ~EditorMain();
};