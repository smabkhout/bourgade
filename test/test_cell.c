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
    struct building_t* building  = NULL;
    building = (struct building_t*)malloc(sizeof(struct building_t));
    struct cell_t* cell2 = init_cell();
    cell2->building = building;
    assert(is_free_cell(cell2) == 0); 
    // Cas 3 : Cellule avec un travailleur
    struct worker_t* worker = NULL;
    worker = (struct worker_t*)malloc(sizeof(struct worker_t));
    struct cell_t* cell3 = init_cell();
    cell3->worker = worker;
    assert(is_free_cell(cell3) == 0);  
    // Cas 4 : Cellule avec une mine
    struct mine_t* mine = NULL;
    mine = (struct mine_t*)malloc(sizeof(struct mine_t));
    struct cell_t* cell4 = init_cell();
    cell4->mine = mine;
    assert(is_free_cell(cell4) == 0); 
    free_cell(cell1);
    free_cell(cell2);
    free_cell(cell3);
    free_cell(cell4);
    free_building(building);
    free_worker(worker);
    free_mine(mine);
}

void test_get_building_from_cell() {
    unsigned int value[NUM_RESOURCES] = {0, 0, 0, 1, 0, 0};
    unsigned int earns[NUM_RESOURCES] = {0, 0, 0, 0, 0, 3};
    unsigned int costs[NUM_RESOURCES] = {0, 0, 0, 0, 0, 1};
    unsigned int supplies[NUM_RESOURCES] = {0, 1, 0, 0, 0, 0};
    struct building_t* building = NULL;
    building = (struct building_t*)malloc(sizeof(struct building_t));
    for (int i = 0; i< NUM_RESOURCES; ++i)
    {
        building->costs[i] = costs[i];
        building->value[i]= value[i];
        building->earns[i]= earns[i];
        building->supplies[i]=supplies[i];
    }
    building->position = make_invalid_position();
    struct cell_t* cell = init_cell();
    cell->building = building;
    assert(get_building_from_cell(*cell) == building);
    free(building);
    free(cell);
}

void test_get_worker_from_cell() {
    char nom[10] = "John";
    struct worker_t* worker = make_worker(nom, (unsigned int[]){0,1,1,1,1,0}, BLUE);  
    struct cell_t* cell = init_cell();
    cell->worker = worker;
    assert(get_worker_from_cell(*cell) == worker);
    free(worker);
    free(cell);
}

void test_get_mine_from_cell() {
    char nom[12] = "Farm";
    struct mine_t* mine = make_mine(CORN, nom); 
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
