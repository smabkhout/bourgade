#include <stdio.h>
#include "board.h"

struct position_t* get_pos_from_cell(struct cell_t c){
    return c.pos;
}

struct buiding_t* get_building_from_cell(struct cell_t c){
    return c.building;
}

struct worker_t* get_worker_from_cell(struct cell_t c){
    return c.worker;
}

struct mine_t* get_mine_from_cell(struct cell_t c){
    return c.mine;
}

int main(int argc, char* argv[]){
    return 0;
}