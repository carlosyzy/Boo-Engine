
#include "editor-scene.h"

#include "../../../../engine/core/scene/scene.h"
#include "../../../../engine/core/scene/node.h"
#include "../../../../engine/core/scene/node-2d.h"

EditorScene::EditorScene(Node2D *root)
{
    this->_rootNode = root;
}
void EditorScene::init()
{

}
void EditorScene::update(float deltaTime)
{
    
}



EditorScene::~EditorScene()
{
}