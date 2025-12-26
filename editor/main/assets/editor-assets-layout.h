#pragma once
#include <string>

class Node;
class Node2D;
class Scene;
class UISprite;
class EditorAssetsFileTree;

class EditorAssetsLayout
{
private:
    Node2D *_root;
    Node2D *_content;
    void _initContent();
    void _initTitle();
    // 添加按钮
    Node2D *_ndOperate;
    Node2D *_ndAdd;
    UISprite *_spriteAdd;
    Node2D *_ndFold;
    UISprite *_spriteFold;
    Node2D *_ndSearchIcon;
    UISprite *_spriteSearchIcon;
    Node2D *_ndSearchBox;
    UISprite *_spriteSearchBox;
    // 折叠按钮
    void _initOperate();
    void _initOperateFold();
    void _initOperateAdd();
    void _initOperateSearch();
    UISprite *_spriteNodeTree;
    Node2D *_ndNodeTree;
    EditorAssetsFileTree *_fileTree;
    void _initNodeTree();
    void _onFileTreeSelectEvent(std::string uuid);
    
public:
    EditorAssetsLayout(Node2D *root);
    void init();
    void setRoot(std::string root);
    ~EditorAssetsLayout();
};
