#pragma once
#include <string>
#include <iostream>
class EditorAssetsLayout;
class Node2D;
class EditorAssets
{
private:
    Node2D *_rootNode;
    EditorAssetsLayout *_layout;

public:
    EditorAssets(Node2D *root);
    void init();
    void setRoot(std::string root);
    void update(float dt);
    ~EditorAssets();
};
