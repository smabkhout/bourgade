#include <stdio.h>
#include "mine.h"


void place_mine(struct cell_t* cell, struct mine_t* m){
    cell->mine->r = m->r;
    int i = 0;
    while(m->name[i]!=0)
    {
        cell->mine->name[i] = m->name[i];
        ++i;
    }
}

//struct mine_t* construct_mine(char* name, enum resource_t r){
//}

int main(){
    return 0;
}

