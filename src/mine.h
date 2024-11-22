#ifndef __MINE_H__
#define __MINE_H__

#include "resource.h"
#include "position.h"

struct cell_t;

struct mine_t {
    char* name;
    enum resource_t r;
};

extern struct mine_t list_mines[4];

void place_mine(struct cell_t* cell, struct mine_t m);

extern struct mine_t present_mines[MAX_POSITIONS/4];

void construct_mines();

void free_mine(struct mine_t* mine);

#endif