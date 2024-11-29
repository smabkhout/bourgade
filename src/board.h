#ifndef __BOARD_H__
#define __BOARD_H__

#include "cell.h"


struct board_t {
    struct cell_t** tab;
    struct mine_t* present_mines;
};

int occurrences(struct mine_t* tab);

struct board_t* init_board();

void free_board(struct board_t* board);

void pay_workers_on_board(struct board_t* board, int num_players, struct player_t** players);

void reset_workers_still_on_board(struct board_t*);

#endif