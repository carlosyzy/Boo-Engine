// #pragma once
// #include <iostream>
// #include <functional>
// #include <string>
// #include "../../engine/core/math/vec3.h"
// class Node2D;
// struct UITreeStructure;
// enum class NodeInput;
// struct NodeInputResult;
// class FileTree;
// class Sprite;


// class EditorPropertyNodeLayout
// {
// private:
//     bool _isActive = false;
//     Node2D *_root;
//     Node2D *_content;
//     void _initContent();
//     void _initTitle();
//     std::function<void(NodeInputResult &result)> _onRootInputEndEventCallback;
//     float _topLen = 0.0f;

//      // 内容布局
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
//     Vec3 _scrollBarTouch{};
//     float _scrollBarRate = 1.0f;
//     void _initScrollView();
//     // 滑动条touch 事件
//     bool _isScrollBarTouching = false;
//     void _onScrollBarTouchStartEvent(NodeInputResult &result);
//     void _onScrollBarTouchMoveEvent(NodeInputResult &result);
//     void _onScrollBarTouchEndEvent(NodeInputResult &result);

   
    
//     // float _top = 0.0f;
//      //node标题
//     //active 选择框   名字编辑框
//     //position  x y z
//     //scale  x y z
//     //rotation  x y z
//     //size width height
//     //anchor x y
//     // Node *_node = nullptr;
//     // void _initNodeOperate();
   
    

//     // void setNode(Node *node);

//     /**
//      * 消失,切换的时候调用
//      */
//     void _clear();
// public:
//     EditorPropertyNodeLayout(Node2D *root);
//     void loseFocus();
//     void setActive(bool active);
//     void update(float dt);
    
//     void onRootInputEndEvent(std::function<void(NodeInputResult &result)> callback);
//     ~EditorPropertyNodeLayout();
// };
