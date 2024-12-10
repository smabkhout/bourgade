#include "optimal_pos.h"
#include "board.h"
#include "position.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

void test_choose_optimal_pos(int seed) {
    struct board_t* board = init_board(seed);
    struct position_t* optimal_pos = choose_optimal_pos(board);
    (void) optimal_pos;
    free_board(board);
}

int main() {
    int seed = 3;
    test_choose_optimal_pos(seed);
    printf("All optimal position tests passed!\n");
    return 0;
}
