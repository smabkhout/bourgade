#ifndef __BOARD_H__
#define __BOARD_H__

#include "worker.h"
#include "building.h"
#include "mine.h"

struct cell_t {
    struct building_t* building;
    struct worker_t* worker;
    struct mine_t* mine;
};

struct board_t {
    struct cell_t tab[MAX_X*MAX_Y];
};

struct building_t* get_building_from_cell(struct cell_t c);

struct worker_t* get_worker_from_cell(struct cell_t c);

struct mine_t* get_mine_from_cell(struct cell_t c);

int occurrences(struct mine_t tab[MAX_POSITIONS/4]);

struct cell_t init_cell();

int is_free_cell(struct cell_t c);

int is_free_neighbor(struct cell_t cell, int indice);

struct board_t init_board(int num_players);

#endif