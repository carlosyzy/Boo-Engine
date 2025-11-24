#pragma once

class EditorLayout;

class EditorMain
{
private :
    int _initDelayScheduleID = 0;
    void _startLoading();

    EditorLayout *_editorLayout = nullptr;
    void _launchEditor();

public:
    EditorMain();
    void init();
    


    
    ~EditorMain();
};