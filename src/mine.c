#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "mine.h"
#include "position.h"
#include "cell.h"
#include <assert.h>

struct mine_t list_mines[4] = {
 {.name = "Field", .r=CORN},
 {.name = "Forest", .r=WOOD},
 {.name = "River", .r=FISH},
 {.name = "Rock Mine", .r=STONE},
};

struct mine_t* make_mine(enum resource_t r, char nom[12]){
    struct mine_t* mine = NULL;
    mine = (struct mine_t*)malloc(sizeof(struct mine_t));
    mine->r = r;
    int i = 0;
    while(i<12){
        mine->name[i] = nom[i];
        ++i;
    }
    return mine;
}

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

int max(int a, int b)
{
    if (a>b)
        return a;
    else
        return b;
}

void construct_mines(struct mine_t* present_mines){ //modifie le tableau present_mines pour avoir MAX_POSITIONS/4 mines aléatoires
    int j=4;
    for (int i =0; i< max(4, MAX_POSITIONS/4); i++)
    {
        present_mines[i] = list_mines[i];
         //on s'assure d'avoir au moins une mine de chaque type
        //en remplissant à la main les 4 premières 
    }
    while (j<MAX_POSITIONS/4){
        int a=rand()%4;
        present_mines[j]=list_mines[a];
        ++j;
    }
}

void free_mine(struct mine_t* mine)
{
    free(mine);
}
