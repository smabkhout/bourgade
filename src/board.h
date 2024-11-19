#ifndef __BOARD_H__
#define __BOARD_H__

#include "cell.h"


struct board_t {
    struct cell_t** tab;
};

int occurrences(struct mine_t tab[MAX_POSITIONS/4]);

struct board_t* init_board();

void free_board(struct board_t* board);

struct cell_t* find_free_cell(struct board_t* board);

#endif