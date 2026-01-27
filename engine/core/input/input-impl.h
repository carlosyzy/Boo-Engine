#pragma once
#include "input.h"
#include "../scene/node.h"
#include "../scene/node-2d.h"

template <typename T, typename Func>
int Input::onNodeInputEvent(Node2D *node, NodeInput input, Func func, T *instance, bool isIntercept)
{

    int id = this->_nodeInputCallId++;
    if (this->_nodeInputMap.find(node->getUuid()) == this->_nodeInputMap.end())
    {
        NodeInputStruct nodeInputStruct;
        nodeInputStruct.node = node;
        nodeInputStruct.isIntercept = isIntercept;
        nodeInputStruct.touchResult.node = node;
        nodeInputStruct.status = 0;
        this->_nodeInputMap.emplace(node->getUuid(), nodeInputStruct);
    }
    auto callback = [func, instance](NodeInputResult &result)
    {
        if (instance != nullptr && func != nullptr)
        {
            (instance->*func)(result);
        }
    };
    if (input == NodeInput::TOUCH_START)
    {
        this->_nodeInputMap[node->getUuid()].touchStarts.emplace(id, callback);
    }
    else if (input == NodeInput::TOUCH_END)
    {
        this->_nodeInputMap[node->getUuid()].touchEnds.emplace(id, callback);
    }
    else if (input == NodeInput::TOUCH_MOVE)
    {
        this->_nodeInputMap[node->getUuid()].touchMoves.emplace(id, callback);
    }
    else if (input == NodeInput::TOUCH_CANCEL)
    {
        this->_nodeInputMap[node->getUuid()].touchCancels.emplace(id, callback);
    }
    else if (input == NodeInput::CURSOR_HOVER)
    {
        this->_nodeInputMap[node->getUuid()].cursorHovers.emplace(id, callback);
    }
    else if (input == NodeInput::CURSOR_HOVER_OUT)
    {
        this->_nodeInputMap[node->getUuid()].cursorHoverOuts.emplace(id, callback);
    }
    return id;
}