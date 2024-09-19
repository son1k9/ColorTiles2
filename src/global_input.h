#pragma once
#include "raylib.h"

namespace GlobalInput {
    inline bool settings() {
        return IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_O);
    }
}