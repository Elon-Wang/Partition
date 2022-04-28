#pragma once

struct Node{
    int idx;
    int x, y;
    bool isOccupied= false;
    int cellIdx = -1;
    Node(){};
    Node(int cnt, int xSize, int ySize): idx(cnt){
        x = idx % xSize;
        y = idx / xSize;
    }
};