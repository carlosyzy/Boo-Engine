#include "editor-hierarchy.h"
#include "editor-hierarchy-layout.h"

#include "../../../../engine/core/scene/scene.h"
#include "../../../../engine/core/scene/node.h"
#include "../../../../engine/core/scene/node-2d.h"

EditorHierarchy::EditorHierarchy(Node2D *root)
{
    this->_rootNode = root;
}
void EditorHierarchy::init()
{
    this->_layout = new EditorHierarchyLayout(this->_rootNode);
    this->_layout->init();
}
void EditorHierarchy::setScene(Scene *scene)
{
    if (this->_layout != nullptr)
    {
        this->_layout->setScene(scene);
    }
}
void EditorHierarchy::setNode(Node *node)
{
}

void EditorHierarchy::update(float dt)
{
}

EditorHierarchy::~EditorHierarchy()
{
    this->_rootNode = nullptr;
}