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
    struct mine_t* present_mines[MAX_POSITIONS / 4] = { NULL };
    construct_mines();
    for (int i = 0; i < MAX_POSITIONS / 4; ++i) {
        assert(present_mines[i] != NULL);
        printf("Mine %d: %s\n", i, present_mines[i]->name);
    }
}

void test_random_mine_selection() {
    struct mine_t* present_mines[MAX_POSITIONS / 4] = { NULL };
    construct_mines();
    for (int i = 0; i < MAX_POSITIONS / 4; ++i) {
        assert(present_mines[i] == &list_mines[0] || 
               present_mines[i] == &list_mines[1] || 
               present_mines[i] == &list_mines[2] || 
               present_mines[i] == &list_mines[3]);
    }
}

int main() {
    test_place_mine();
    test_construct_mines();
    test_random_mine_selection();

    printf("All mine tests passed!\n");
    return 0;
}
