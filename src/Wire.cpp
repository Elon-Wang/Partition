#include "Wire.h"


bool Wire::isSameGroup(){
    int size = cellsOfWire.size();
    int flag = cellsOfWire[0]->group;
    for (int i=1; i<size; i++) {
        if (flag != cellsOfWire[i]->group) {
            return false;
        }
    }
    return true;
}

void Wire::drawWire(int groupIdx){
    int cnt = cellsOfWire.size();
    int ii, jj;
    int color = (4 + groupIdx % 7);
    setcolor(color);
    for (ii = 0; ii < cnt; ii++) {
        Node* node1 = cellsOfWire[ii]->loc;
        for (jj = ii + 1; jj < cnt; jj++) {
            Node* node2 = cellsOfWire[jj]->loc;
            drawline(7 + node1->x * 10, 7 + node1->y * 10 , 7 + node2->x * 10, 7 + node2->y * 10);
        }
    }
}