#pragma once
#include "ColorPicker.h"

class FloodFill
{
public:
    static void Fill(
        IntVector3 skin[ColorPicker::SKIN_SIZE][ColorPicker::SKIN_SIZE],
        int startX,
        int startY,
        IntVector3 target,
        IntVector3 replacement
    );
};
