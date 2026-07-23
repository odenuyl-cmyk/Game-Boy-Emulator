#pragma once

#include "../../include/keyboard_mouse_inputs.h"

namespace Input {
    inline double mouseXPos = 0;
    inline double mouseYPos = 0;
    bool mouseHeld(Mouse button);
    bool keyHeld(Key key);
}