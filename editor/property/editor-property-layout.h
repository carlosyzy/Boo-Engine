// #pragma once
// #include <string>
// #include <iostream>
// #include <functional>
// class Node;
// class Node2D;
// class Sprite;
// class Vec3;
// struct UITreeStructure;
// enum class NodeInput;
// struct NodeInputResult;
// class FileTree;

// class EditorPropertyLayout
// {
// public:
//     // class PropertySceneLayout
//     // {
//     // private:
//     // public:
//     //     PropertySceneLayout();
//     //     ~PropertySceneLayout();
//     // };
//     class PropertyNodeLayout
//     {
//     private:
//         EditorPropertyLayout *_layout;
//         Node *_ndBind;

//         void _initTitle();
//     public:
//         // node标题
//         // active 选择框   名字编辑框
//         // position  x y z
//         // scale  x y z
//         // rotation  x y z
//         // size width height
//         // anchor x y
//         //  Node *_node = nullptr;
//         //  void _initNodeOperate();
//         PropertyNodeLayout(EditorPropertyLayout *layout, Node *ndBind);
        
//         ~PropertyNodeLayout();
//     };
//     // class PropertyNodeImage
//     // {
//     // private:
//     // public:
//     //     PropertyNodeImage();
//     //     ~PropertyNodeImage();
//     // };

// private:
//     Node2D *_root;
//     int _contentTransformChangeCallID = 0;
//     Node2D *_content;
//     void _initContent();
//     void _onRootInputStartEvent(NodeInputResult &result);
//     std::function<void(NodeInputResult &result)> _onRootInputStartEventCallback;
//     void _initTitle();
//     // 内容布局
//     Node2D *_contentLayout;
//     void _initContentLayout();

//     // 滑动条
//     Node2D *_ndScrollBarBg;
//     Sprite *_spScrollBarBg;
//     float _scrollBarBgWidth;
//     float _scrollBarBgHeight;
//     float _scrollBarBgX;
//     float _scrollBarBgY;

//     Node2D *_ndScrollBar;
//     Sprite *_spScrollBar;
//     float _scrollBarWidth;
//     float _scrollBarHeight;
//     float _scrollBarX;
//     float _scrollBarY;
//     Vec3 *_scrollBarTouch;
//     float _scrollBarRate = 1.0f;
//     void _initScrollView();
//     // 滑动条touch 事件
//     bool _isScrollBarTouching = false;
//     void _onScrollBarTouchStartEvent(NodeInputResult &result);
//     void _onScrollBarTouchMoveEvent(NodeInputResult &result);
//     void _onScrollBarTouchEndEvent(NodeInputResult &result);

//     // 动态指针 _sceneLayout = std::make_unique<PropertySceneLayout>();
//     // PropertySceneLayout *_sceneLayou;
//     PropertyNodeLayout *_nodeLayout = nullptr;
//     // PropertyNodeImage *_imageLayou;
//     void _clear();

// public:
//     EditorPropertyLayout(Node2D *root);
//     void onRootInputEndEvent(std::function<void(NodeInputResult &result)> callback);
//     void setNode(Node *node);
//     void loseFocus();
//     Node2D *getContentLayout()
//     {
//         return this->_contentLayout;
//     }

//     ~EditorPropertyLayout();
// };
