#pragma once
#include <bits/stdc++.h>
#include "Node.h"
#include "draw.h"

struct Cell{
    int idx = -1;
    int group = -1;
    bool locked = false;
    Node* loc;
    std::vector<int> wireIdx;

    void setLoc(Node* a);
    void drawCell();
};