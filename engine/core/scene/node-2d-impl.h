#pragma once
#include "node-2d.h"
#include "../../boo.h"
#include "../input/input.h"

template<typename T, typename Method>
int Node2D::onNodeInputEvent(NodeInput input, Method method, T* instance, bool useCapture) {
    return Boo::game->input()->onNodeInputEvent(this, input, method, instance, useCapture);
}