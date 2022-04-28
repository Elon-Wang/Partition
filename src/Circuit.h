#pragma once
#include "Wire.h"
#include "Node.h"


struct Circuit{
    int numOfCell, numOfWire, xSize, ySize;
    Wire wires[10000];
    Cell cells[10000];
    Node NN[10000];
    
    std::vector<int> cellsGroup[2];

    // bool readFile(const char* testbench, int detail_level);
    bool readFile(const char* testbench);
    void construct();
    void partition();
    void moveCell(Cell* cells, int idx);
    void putCells();
    void draw(int detail);
};

float evalMove(Wire* wires, Cell* cells, int idx);
int calCost(Wire* wireList, int numOfWires);

