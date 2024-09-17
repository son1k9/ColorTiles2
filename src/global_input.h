#pragma once
#include "raylib.h"

namespace GlobalInput {
    inline bool settings() {
        if (IsKeyDown(KEY_RIGHT_CONTROL) && IsKeyPressed(KEY_O)
            || IsKeyPressed(KEY_ESCAPE)) {
            return true;
        }
    }
}