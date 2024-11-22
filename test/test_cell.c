#include "cell.h"
#include "building.h"
#include "worker.h"
#include "mine.h"
#include "position.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

void test_init_cell() {
    struct cell_t* cell = init_cell();
    assert(cell != NULL);
    assert(cell->building == NULL);
    assert(cell->mine == NULL);
    assert(cell->worker == NULL);
    free(cell);
}

void test_is_free_cell() {
    // Cas 1 : Cellule vide
    struct cell_t* cell1 = init_cell();
    assert(is_free_cell(cell1) == 1); 
    // Cas 2 : Cellule avec un bâtiment
    struct building_t* building = make_building("Farm", (int[]){0, 0, 0, 1, 0, 0}, (int[]){0, 0, 0, 0, 0, 3}, (int[]){0, 0, 0, 0, 0, 1}, (int[]){0, 1, 0, 0, 0, 0}, BLUE);
    struct cell_t* cell2 = init_cell();
    cell2->building = building;
    assert(is_free_cell(cell2) == 0); 
    // Cas 3 : Cellule avec un travailleur
    struct worker_t* worker = create_worker();
    struct cell_t* cell3 = init_cell();
    cell3->worker = worker;
    assert(is_free_cell(cell3) == 0);  
    // Cas 4 : Cellule avec une mine
    struct mine_t* mine = create_mine();
    struct cell_t* cell4 = init_cell();
    cell4->mine = mine;
    assert(is_free_cell(cell4) == 0); 
    free(building);
    free(worker);
    free(mine);
    free(cell1);
    free(cell2);
    free(cell3);
    free(cell4);
}

void test_get_building_from_cell() {
    struct building_t* building = make_building("Farm", (int[]){0, 0, 0, 1, 0, 0}, (int[]){0, 0, 0, 0, 0, 3}, (int[]){0, 0, 0, 0, 0, 1}, (int[]){0, 1, 0, 0, 0, 0}, BLUE);
    struct cell_t* cell = init_cell();
    cell->building = building;
    assert(get_building_from_cell(*cell) == building);
    free(building);
    free(cell);
}

void test_get_worker_from_cell() {
    struct worker_t* worker = create_worker();  
    struct cell_t* cell = init_cell();
    cell->worker = worker;
    assert(get_worker_from_cell(*cell) == worker);
    free(worker);
    free(cell);
}

void test_get_mine_from_cell() {
    struct mine_t* mine = create_mine(); 
    struct cell_t* cell = init_cell();
    cell->mine = mine;
    assert(get_mine_from_cell(*cell) == mine);
    free(mine);
    free(cell);
}

int main() {
    test_init_cell();
    test_is_free_cell();
    test_get_building_from_cell();
    test_get_worker_from_cell();
    test_get_mine_from_cell();
    printf("All cell tests passed!\n");
    return 0;
}
