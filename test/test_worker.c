#include "worker.h"
#include "player.h"
#include "cell.h"
#include "resource.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

void test_make_worker() {
    unsigned int costs[NUM_RESOURCES] = {1, 2, 0, 0, 0, 0};
    enum color_t couleur = BLUE;
    char* name = "Worker1";
    struct worker_t* worker = make_worker(name, costs, couleur);
    assert(worker != NULL); 
    assert(worker->nom != NULL);  
    assert(worker->joueur == couleur); 
    for (int i = 0; i < NUM_RESOURCES; ++i) {
        assert(worker->costs[i] == costs[i]); 
    }
    assert(is_valid_position(worker->position) == 0);
    free(worker);
}

void test_pay_worker() {
    enum color_t couleur = RED;
    struct player_t* player = initialize_player(couleur);
    unsigned int costs[NUM_RESOURCES] = {1, 2, 0, 0, 0, 0};
    char* name = "Worker2";
    struct worker_t* worker = make_worker(name, costs, couleur);
    player->stockage[0] = 10; 
    player->stockage[1] = 5;  
    pay_worker(player, worker);
    assert(player->stockage[0] == 9);  // 10 - 1 = 9
    assert(player->stockage[1] == 3);  // 5 - 2 = 3
    free(player);
    free(worker);
}

void test_place_worker() {
    enum color_t couleur = GREEN;
    struct player_t* player = initialize_player(couleur);
    struct cell_t* cell = init_cell();
    unsigned int costs[NUM_RESOURCES] = {0, 0, 0, 0, 0, 0};
    char* name = "Worker3";
    struct worker_t* worker = make_worker(name, costs, couleur);
    assert(cell->worker == NULL);
    place_worker(player, cell, worker);
    assert(cell->worker == worker);
    assert(player->number_of_workers == MAX_WORKERS_PER_PLAYER - 1); 
    free(player);
    free(cell);
    free(worker);
}

int main() {
    test_make_worker();
    test_pay_worker();
    test_place_worker();
    printf("All worker tests passed!\n");
    return 0;
}
