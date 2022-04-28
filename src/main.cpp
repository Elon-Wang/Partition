#include <bits/stdc++.h>
#include <vector>
// #include "Wire.h"
#include "Circuit.h"

using namespace std;

int main(int argc, char* argv[]) {

    if (argc != 2 && argc != 4 ) {
        printf("Usage: %s <benchmark-name> [--detail <1-3>]\n"
            "Example: %s cm150a --detail 2\n", argv[0], argv[0]);
        return 0;
    }
    char prefix[15] ="../benchmarks/";
	char postfix[10]   =".txt";
	char fileName[50];
	strcpy(fileName,prefix);
	strcat(fileName,argv[1]);
	strcat(fileName,postfix);
    // char fileName[50] = "../benchmarks/cm150a.txt";

    int detail_level = 1;
    if (argc ==4 && strcmp(argv[2], "--detail") == 0) {
        sscanf(argv[3],"%d",&detail_level);  
        assert(detail_level >=1 && detail_level <= 3);
    }

    Circuit target;
    // if  (!target.readFile(fileName,detail_level)) {
    if  (!target.readFile(fileName)) {
        printf("Cannot open file: %s\n",fileName); 
        return 0;
    }
    target.construct();
    for (int cnt=0; cnt<6; cnt++ ) {
        target.partition();
    }
    
    if(detail_level >= 2) {
        target.putCells();
        target.draw(detail_level);
    }

    return 0;
}
