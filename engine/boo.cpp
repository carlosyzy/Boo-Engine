#include "boo.h"
#include <iostream>
#include "../editor/editor.h"
#include "window/window.h"
#include "core/utils/time-util.h"

namespace Boo
{
	Game* game = nullptr;
	Window* window = nullptr;
	Platform platform = Platform::None;
}