#include "board.h"
#include "cell.h"
#include "mine.h"
#include "position.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>


void test_init_board() {
    struct board_t* board = init_board();
    for (int i = 0; i < MAX_X * MAX_Y; ++i) {
        assert(board->tab[i] != NULL);
        assert(board->tab[i]->worker == NULL); 
        assert(board->tab[i]->building == NULL);
    }
    int mine_count = 0;
    for (int i = 0; i < MAX_X * MAX_Y; ++i) {
        if (board->tab[i]->mine != NULL) {
            ++mine_count;
            printf("%s\n",board->tab[i]->mine->name);
        }
    }
    assert(mine_count == MAX_POSITIONS / 4);
    int occurrences_result = occurrences(board->present_mines);
    assert(occurrences_result != 0); 
    free_board(board);       
}

void test_find_free_cell() {
    struct board_t* board = init_board();
    struct cell_t* free_cellule = init_cell();
    assert(free_cellule != NULL);
    assert(is_free_cell(free_cellule) == 1);
    free_cell(free_cellule);
    free_board(board);      
}

void test_free_board() {
    struct board_t* board = init_board();
    free_board(board);
}

int main() {
    test_init_board();
    test_find_free_cell();
    test_free_board();
    printf("All board tests passed!\n");
    return 0;
}
