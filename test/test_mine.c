#include "mine.h"
#include "cell.h"
#include "position.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void test_place_mine() {
    struct cell_t* cell = init_cell();
    struct mine_t mine = {"GoldMine", 100};
    place_mine(cell, mine);
    assert(cell->mine != NULL);
    assert(cell->mine->r == 100);
    assert(strcmp(cell->mine->name, "GoldMine") == 0);
    free(cell->mine);
    free(cell);
}

void test_construct_mines() {
    struct mine_t* present_mines = NULL;
    present_mines = (struct mine_t*)malloc(sizeof(struct mine_t)*MAX_POSITIONS/4);
    present_mines = construct_mines();
    for (int i = 0; i < MAX_POSITIONS / 4; ++i) {
        printf("Mine %d: %s\n", i, present_mines[i].name);
    }
}


int main() {
    test_place_mine();
    test_construct_mines();
    printf("All mine tests passed!\n");
    return 0;
}
