#include "node-input-event-intercept.h"
#include "core/scene/node-2d.h"
#include "boo.h"
#include "log.h"

namespace Boo
{
    NodeInputEventIntercept::NodeInputEventIntercept(std::string name, Node *node, std::string uuid)
        : Component(name, node, uuid)
    {
        _layer = EComponentLayer::Layer2D;
    }

    void NodeInputEventIntercept::OnAwake()
    {
        _node2d = dynamic_cast<Node2D *>(_node);
    }

    void NodeInputEventIntercept::OnEnable()
    {
        if (_node2d)
        {
            _touchStartID = input->onNodeTouchStart(_node2d, [this](FNodeTouchEvent &event)
                                                   { this->_onTouchStart(event); }, true);

            _touchMoveID = input->onNodeTouchMove(_node2d, [this](FNodeTouchEvent &event)
                                                 { this->_onTouchMove(event); }, true);

            _touchEndID = input->onNodeTouchEnd(_node2d, [this](FNodeTouchEvent &event)
                                               { this->_onTouchEnd(event); }, true);
        }
    }

    void NodeInputEventIntercept::Update(float deltaTime)
    {
    }

    void NodeInputEventIntercept::LateUpdate(float deltaTime)
    {
    }

    void NodeInputEventIntercept::OnDisable()
    {
        if (_node2d)
        {
            if (_touchStartID != 0)
            {
                input->offNodeTouch(_node2d, _touchStartID);
                _touchStartID = 0;
            }

            if (_touchMoveID != 0)
            {
                input->offNodeTouch(_node2d, _touchMoveID);
                _touchMoveID = 0;
            }

            if (_touchEndID != 0)
            {
                input->offNodeTouch(_node2d, _touchEndID);
                _touchEndID = 0;
            }
        }
    }

    void NodeInputEventIntercept::OnDestroy()
    {
        OnDisable();
    }

    void NodeInputEventIntercept::_onTouchStart(FNodeTouchEvent &event)
    {
        LOGI("NodeInputEventIntercept::_onTouchStart");
    }

    void NodeInputEventIntercept::_onTouchMove(FNodeTouchEvent &event)
    {
        LOGI("NodeInputEventIntercept::_onTouchMove");
    }

    void NodeInputEventIntercept::_onTouchEnd(FNodeTouchEvent &event)
    {
        LOGI("NodeInputEventIntercept::_onTouchEnd");
    }

    NodeInputEventIntercept::~NodeInputEventIntercept(){
        this->_node2d=nullptr;
    }
}