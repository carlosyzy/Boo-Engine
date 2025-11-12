// #include "editor-assets.h"
// #include "editor-assets-layout.h"

// EditorAssets::EditorAssets(Node2D *root)
// {
//     this->_layout = new EditorAssetsLayout(root);
//     EditorIpc::getInstance()->on(IpcEvent::FOCUS, &EditorAssets::_onFocusEvent, this);
// }
// void EditorAssets::setRoot(std::string root)
// {
//     this->_layout->setRoot(root);
// }
// void EditorAssets::_onFocusEvent(const EventValue& value)
// {
//     if (std::holds_alternative<std::string>(value))
//     {
//         std::string focus = std::get<std::string>(value);
//         std::cout << "EditorAssets Focus: " << focus << std::endl;
//         if (focus != "EditorAssets")
//         {
//             this->_layout->loseFocus();
//         }
//     }
// }
// void EditorAssets::update(float dt)
// {
    
// }



// EditorAssets::~EditorAssets()
// {
// }