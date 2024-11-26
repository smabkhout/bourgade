#include "optimal_pos.h"
#include "board.h"
#include "position.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

void test_choose_optimal_pos() {
    struct board_t* board = init_board();
    struct position_t* optimal_pos = choose_optimal_pos(board);
    unsigned int x = PX(optimal_pos);
    unsigned int y = PY(optimal_pos);
    free_board(board);
}

int main() {
    test_choose_optimal_pos();
    printf("All optimal position tests passed!\n");
    return 0;
}
