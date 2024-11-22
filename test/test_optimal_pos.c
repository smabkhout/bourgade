#include "optimal_pos.h"
#include "board.h"
#include "position.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

void test_choose_optimal_pos() {
    struct board_t* board = (struct board_t*)malloc(sizeof(struct board_t));
    for (int i = 0; i < MAX_POSITIONS; ++i) {
        board->tab[i] = (struct cell_t*)malloc(sizeof(struct cell_t));
        board->tab[i]->mine = NULL;
        board->tab[i]->worker = NULL;
        board->tab[i]->building = NULL;
    }
    struct mine_t mine = { .r = 1, .name = "Field" };
    board->tab[5]->mine = &mine;
    struct mine_t mine2 = { .r = 3, .name = "Forest" };
    board->tab[10]->mine = &mine2;
    struct mine_t mine3 = { .r = 2, .name = "River" };
    board->tab[20]->mine = &mine3;
    struct position_t* optimal_pos = choose_optimal_pos(board);
    unsigned int x = PX(optimal_pos);
    unsigned int y = PY(optimal_pos);
    assert(x == 5 % MAX_X); 
    assert(y == (5 - x) / MAX_X); 
    for (int i = 0; i < MAX_POSITIONS; ++i) {
        free(board->tab[i]);
    }
    free(board);
}

int main() {
    test_choose_optimal_pos();
    printf("All optimal position tests passed!\n");
    return 0;
}
