#include "position.h"
#include "worker.h"
#include "building.h"
#include "mine.h"

struct cell_t {
    struct position_t* pos;
    struct building_t* building;
    struct worker_t* worker;
    struct mine_t* mine;
};

struct cell_t board[MAX_X*MAX_Y];

struct position_t* get_pos_from_cell(struct cell_t c);

struct buiding_t* get_building_from_cell(struct cell_t c);

struct worker_t* get_worker_from_cell(struct cell_t c);

struct mine_t* get_mine_from_cell(struct cell_t c);