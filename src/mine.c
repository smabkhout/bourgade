#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "mine.h"
#include "position.h"
#include "cell.h"

void place_mine(struct cell_t* cell, struct mine_t m) {
    if (cell->mine == NULL) {
        cell->mine = malloc(sizeof(struct mine_t));
    }
    cell->mine->r = m.r;
    int i = 0;
    while (m.name[i] != 0) {
        cell->mine->name[i] = m.name[i];
        ++i;
    }
    cell->mine->name[i] = 0;
}


void construct_mines(){ //modifie le tableau present_mines pour avoir MAX_POSITIONS/4 mines aléatoires
    int i=0;
    while (i<MAX_POSITIONS/4){
        int a=rand() % 4;
        present_mines[i]=list_mines[a];
        ++i;
    }
}

void free_mine(struct mine_t* mine)
{
    free(mine);
}
