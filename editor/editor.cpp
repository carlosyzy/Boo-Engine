#include "editor.h"
#include "../engine/engine.h"
#include "editor-layout.h"


//#include "../engine/core/renderer/scene.h"
//#include "../engine/core/renderer/ui/sprite.h"
//#include "../engine/core/global/event.h"
//#include "../engine/core/global/global.h"
//#include "hierarchy/editor-hierarchy.h"
//#include "assets/editor-assets.h"
//#include "property/editor-property.h"

Editor::Editor()
{
}
Editor::~Editor()
{
}
Editor *Editor::getInstance()
{
    static Editor instance;
    return &instance;
}

void Editor::init()
{
    this->_editorLayout = new EditorLayout();
    /*this->_initHierarchy();
    this->_initAssets();
    this->_initProperty();*/
    
  /*  EditorIpc::getInstance()->on(IpcEvent::UPDATE_HIERARCHY_ROOT, &Editor::_onHierarchyRootUpdate, this);
    EditorIpc::getInstance()->send(IpcEvent::UPDATE_HIERARCHY_ROOT,0);*/
}
//void Editor::_initHierarchy()
//{
//    Node2D *layout = this->_editorLayout->getHierarchy();
//    this->_hierarchy = new EditorHierarchy(layout);
//}
//void Editor::_initAssets()
//{
//    Node2D *layout = this->_editorLayout->getAssets();
//    this->_assets = new EditorAssets(layout);
//}
//void Editor::_initProperty()
//{
//    Node2D *layout = this->_editorLayout->getProperty();
//    this->_property = new EditorProperty(layout);
//}
//void Editor::_onHierarchyRootUpdate(const EventValue &value)
//{
//    if (this->_hierarchy != nullptr)
//    {
//        std::cout << "EditorHierarchy Update Tree" << std::endl;
//        this->_hierarchy->updateRoot();
//    }
//}
//void Editor::setHierarchyRoot(Scene *scene)
//{
//    if (this->_hierarchy != nullptr)
//    {
//        this->_hierarchy->setScene(scene);
//    }
//}
//void Editor::setHierarchyNode(Node *node)
//{
//    if (this->_hierarchy != nullptr)
//    {
//        this->_hierarchy->setNode(node);
//    }
//}
//void Editor::setAssetsRoot(std::string root)
//{
//    if (this->_assets != nullptr)
//    {
//        this->_assets->setRoot(root);
//    }
//}



void Editor::update(float dt)
{
    this->_editorLayout->update(dt);
   /* this->_hierarchy->update(dt);
    this->_assets->update(dt);
    this->_property->update(dt);*/
}