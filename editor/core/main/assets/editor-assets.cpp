#include "editor-assets.h"
#include "editor-assets-layout.h"

#include "../../../../engine/core/scene/scene.h"
#include "../../../../engine/core/scene/node.h"
#include "../../../../engine/core/scene/node-2d.h"

EditorAssets::EditorAssets(Node2D *root)
{
    this->_rootNode = root;
}
void EditorAssets::init()
{
    this->_layout = new EditorAssetsLayout(this->_rootNode);
    this->_layout->init();
}
void EditorAssets::setRoot(std::string root)
{
    this->_layout->setRoot(root);
}

void EditorAssets::update(float dt)
{
    
}



EditorAssets::~EditorAssets()
{
}