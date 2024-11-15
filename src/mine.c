#include <stdio.h>
#include "mine.h"
#include <stdlib.h>
#include <time.h>


void place_mine(struct cell_t cell, struct mine_t m){
    cell.mine->r = m.r;
    int i = 0;
    while(m.name[i]!=0)
    {
        cell.mine->name[i] = m.name[i];
        ++i;
    }
}

void construct_mines(){
    int i=0;
    while (i<MAX_POSITIONS/4){
        int a=rand() % 4;
        present_mines[i]=list_mines[a];
        ++i;
    }
}


int main(){
    return 0;
}

