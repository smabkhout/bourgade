#ifndef __MINE_H__
#define __MINE_H__

#include "resource.h"
#include "position.h"

struct cell_t;

struct mine_t {
    char name[12];
    enum resource_t r;
};

extern struct mine_t list_mines[4];

struct mine_t* make_mine(enum resource_t r, char nom[12]);

void place_mine(struct cell_t* cell, struct mine_t m);

int max(int a, int b);

struct mine_t* construct_mines();

void free_mine(struct mine_t* mine);

#endif