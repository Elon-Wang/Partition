#pragma once
#include "Cell.h"
// extern "C" {
//     #include "graphics.h"
// }

struct Wire{
    std::vector<Cell*> cellsOfWire;
    bool isSameGroup();
    void drawWire(int groupIdx);
};