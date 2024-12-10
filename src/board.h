#ifndef __BOARD_H__
#define __BOARD_H__

#include "cell.h"


struct board_t {
    struct cell_t** tab;
    struct mine_t* present_mines;
    struct building_t** present_buildings;
};

int occurrences(struct mine_t* tab);

struct board_t* init_board(int seed);

struct board_t* init_best_board(int seed);

void free_board(struct board_t* board);



#endif