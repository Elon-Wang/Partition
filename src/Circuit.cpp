#include "Circuit.h"

bool Circuit::readFile(const char* testbench){
    if (!freopen(testbench, "r", stdin)) {
        freopen("/dev/tty", "r", stdin);
        printf("Error opening file\n"); 
        return false;
    }
    // if (detail_level >=2 ) { 
    //     printf("reading file\n");
    // }
    scanf("%d %d %d %d", &numOfCell, &numOfWire, &xSize, &ySize);
    
    for (int i=0;i<numOfCell;i++) {
        cells[i].idx = i;
    }

    for (int i=0; i < numOfWire; i++) {
        int cellsInWire;
        int cellIdx;
        scanf("%d", &cellsInWire);
        for (int j = 0; j < cellsInWire; j++) {
            scanf("%d", &cellIdx);
            wires[i].cellsOfWire.push_back(&cells[cellIdx]);
            cells[cellIdx].wireIdx.push_back(i);
        }
    }
    return true;
}

void Circuit::construct(){
    //random initial partition
    srand( (unsigned)time( NULL ) );
    int slotOfGroup0 = (numOfCell + 1) / 2;
    int slotOfGroup1 = (numOfCell + 1) / 2;
    for (int i = 0; i < numOfCell; i++) {
        cells[i].idx = i;
        int random = rand() % (slotOfGroup0 + slotOfGroup1);
        if (random <slotOfGroup0) {
            cells[i].group = 0;
            cellsGroup[0].push_back(i);
            slotOfGroup0 --;
        } else {
            cells[i].group = 1;
            cellsGroup[1].push_back(i);
            slotOfGroup1 --;
        }
    }
}

void Circuit::partition(){
    //K&L algorithm
    // judge the initial group for movement
    printf("group0Size:%ld, group1Size:%ld\n",cellsGroup[0].size(),cellsGroup[1].size());
    int nextGroup = (cellsGroup[0].size()>cellsGroup[1].size())?0:1;
    int moveList[numOfCell];
    int minCost=calCost(wires,numOfWire);
    int moveCnt=-2;
    // printf("refresh status\n");
    for(int i=0;i<numOfCell;i++) {
        cells[i].locked = false;
    }
    // printf("forward move\n");
    for(int i=0;i<numOfCell;i++) {
        float max_potential = -numOfWire;
        int next_move = -1;

        // judge locked, desired group
        // calculate potential gain and choose the highest one
        // move & lock & record the best cut.
        for(int j=0; j<numOfCell;j++) {
            if(cells[j].locked == false && cells[j].group == nextGroup) {
                float potential = evalMove(wires, cells, j);
                if (potential > max_potential) {
                    max_potential = potential;
                    next_move = j;
                }
                // printf("cells[%d] potential gain:%f\n",j, potential);
            }
        }

        moveCell(cells, next_move); //move & lock
        nextGroup ^= 1;

        // prepare for roll back
        moveList[i] = next_move;
        int cost = calCost(wires,numOfWire);
        if(cost <= minCost) { //TODO: test the < and <=
            minCost = cost;
            moveCnt = i/2*2;
            // printf("round:%d  totalCost:%d lastMovedCell:%d\n",i,calCost(wires,numOfWire),next_move);
        }
    }

    // printf("\n\nRollback Process, Round:%d\n",moveCnt);
    //roll back process
    for (int i= numOfCell - 1; i>moveCnt+1;i--) {
        moveCell(cells, moveList[i]);
        // printf("movedCell:%d,  costs:%d\n",moveList[i],calCost(wires,numOfWire));
    }
    printf("cost:%d\n",calCost(wires,numOfWire));
}

void Circuit::moveCell(Cell* cells, int idx){
    if( idx<0 ){
        printf("Wrong idx, check the idx\n");
        return;
    }
    int origin_group = cells[idx].group;
    int new_group = origin_group^1;
    cells[idx].group = new_group;
    cells[idx].locked = true;
    cellsGroup[new_group].push_back(idx);
    cellsGroup[origin_group].erase(remove(cellsGroup[origin_group].begin(),cellsGroup[origin_group].end(),idx));
}

void Circuit::putCells() {
    for (int cnt = 0; cnt < xSize * ySize; cnt++){
        NN[cnt] = Node(cnt, xSize, ySize);
    }

    int frontIdx =0;
    int size0 = cellsGroup[0].size();
    for (int i=0;i<size0;i++) {
        cells[cellsGroup[0][i]].setLoc(&NN[frontIdx]);
        frontIdx++;
    }

    int backIdx = xSize * ySize-1;
    int size1 = cellsGroup[1].size();
    for (int i=0;i<size1;i++) {
        cells[cellsGroup[1][i]].setLoc(&NN[backIdx]);
        backIdx--;
    }
}

void Circuit::draw(int detail){
    char txt[10] = "Partition";
    init_graphics(txt);
    init_world(0,0,5+xSize*10, 5+ySize*10);
    setlinewidth(2);
    setlinestyle(SOLID);
    setcolor(BLACK);
    clearscreen();
    for(int i=0;i<=xSize;i++){
        drawline(2+i*10,2,2+i*10, 2+ySize*10);
    }
    for(int j=0;j<=ySize;j++){
        drawline(2,2+j*10,2+xSize*10,2+j*10);
    }
    for (int i= 0; i < numOfCell; i++) {
        cells[i].drawCell();
    }
    for(int i = 0; i < numOfWire; i++) {
        wires[i].drawWire(i);
        if (detail== 3){
            event_loop(button_press,drawscreen);
        }
    }
    if (detail>=2){
        event_loop(button_press,drawscreen);
    }
}

int calCost(Wire* wireList, int numOfWires) {
    int cost =0;
    for (int cnt=0; cnt<numOfWires; cnt++) {
        if (!wireList[cnt].isSameGroup()) {
            cost++;
        }
    }
    return cost;
}

float evalMove(Wire* wires, Cell* cells, int idx){
    int size = cells[idx].wireIdx.size();
    float potential_gain=0;
    
    for (int cnt =0; cnt<size; cnt++) {
        int wireIdx= cells[idx].wireIdx[cnt];
        int total = wires[wireIdx].cellsOfWire.size();
        int cellsInSameGroup = 0;
        int currentGroup = cells[idx].group;
        for (int i = 0; i < total; i++) {
            if(wires[wireIdx].cellsOfWire[i]->group == currentGroup) {
                cellsInSameGroup++;
            }
        }
        potential_gain += -(cellsInSameGroup - (total+1)/2.0)*2.0/((float)(total-1));
    }
    //printf("group[%d] cells[%d] wire_num:%d potential_gain:%f\n",cells[idx].group,idx,size,potential_gain);
    return potential_gain;
}