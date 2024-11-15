#include "resource.h"
#include "board.h"
#include "position.h"

struct mine_t {
    char* name;
    enum resource_t r;
};

struct mine_t list_mines[4] = {
 {.name = "Field", .r=CORN},
 {.name = "Forest", .r=WOOD},
 {.name = "River", .r=FISH},
 {.name = "Rock Mine", .r=STONE},
};

struct mine_t present_mines[MAX_POSITIONS/4];

void construct_mines();

