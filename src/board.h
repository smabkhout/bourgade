#ifndef __BOARD_H__
#define __BOARD_H__

#include "cell.h"


struct board_t {
    struct cell_t** tab;
};

int occurrences(struct mine_t tab[MAX_POSITIONS/4]);

struct board_t* init_board();

void free_board(struct board_t* board);

void reset_workers_still_on_board(struct board_t*);

#endif