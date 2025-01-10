#ifndef __MINE_H__
#define __MINE_H__

#include "position.h"
#include "resource.h"

struct cell_t;



struct mine_t {
    char name[12];
    enum resource_t r;
};

struct mine_t* make_mine(enum resource_t r, char nom[12]);



int max(int a, int b);

void construct_mines(struct mine_t* present_mines);

void free_mine(struct mine_t* mine);



#endif