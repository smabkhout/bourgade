#ifndef __MINE_H__
#define __MINE_H__

#include "resource.h"
#include "position.h"
#include "cell.h"

struct cell_t;

struct mine_t {
    char name[12];
    enum resource_t r;
};

struct mine_t* make_mine(enum resource_t r, char nom[12]);

void place_mine(struct cell_t* cell, struct mine_t m);

int max(int a, int b);

void construct_mines(struct mine_t* present_mines);

void free_mine(struct mine_t* mine);

#endif