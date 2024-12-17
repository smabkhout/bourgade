#ifndef __CELL_H__
#define __CELL_H__

#include "worker.h"
#include "mine.h"

struct building_t;

struct cell_t {
    struct building_t* building;
    struct worker_t* worker;
    struct mine_t* mine;
};

struct building_t* get_building_from_cell(struct cell_t c);

struct worker_t* get_worker_from_cell(struct cell_t c);

struct mine_t* get_mine_from_cell(struct cell_t c);

int is_free_cell(struct cell_t* c);

struct cell_t* init_cell();

void free_cell(struct cell_t* cell);

#endif