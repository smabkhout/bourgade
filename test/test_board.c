#include "board.h"
#include "cell.h"
#include "mine.h"
#include "position.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>


void test_init_board(int seed) {
    struct board_t* board = init_board(seed);
    for (int i = 0; i < MAX_X * MAX_Y; ++i) {
        assert(board->tab[i] != NULL);
        assert(board->tab[i]->worker == NULL); 
        assert(board->tab[i]->building == NULL);
    }
    int mine_count = 0;
    for (int i = 0; i < MAX_X * MAX_Y; ++i) {
        if (board->tab[i]->mine != NULL) {
            printf("%s\n",board->tab[i]->mine->name);
            ++mine_count;
        }
    }
    assert(mine_count == (MAX_POSITIONS / 4));
    int occurrences_result = occurrences(board->present_mines);
    assert(occurrences_result != 0); 
    free_board(board);       
}

void test_find_free_cell(int seed) {
    struct board_t* board = init_board(seed);
    struct cell_t* free_cellule = init_cell();
    assert(free_cellule != NULL);
    assert(is_free_cell(free_cellule) == 1);
    free_cell(free_cellule);
    free_board(board);      
}

void test_free_board(int seed) {
    struct board_t* board = init_board(seed);
    free_board(board);
}

int main() {
    int seed = 4;
    test_init_board(seed);
    test_find_free_cell(seed);
    test_free_board(seed);
    printf("All board tests passed!\n");
    return 0;
}
