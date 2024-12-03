#include "player.h"
#include "worker.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

void test_initialize_player() {
    enum color_t couleur = BLUE;
    struct player_t* player = initialize_player(couleur);
    assert(player != NULL);  
    assert(player->color == couleur); 
    assert(player->number_of_workers == 2); 
    for (int i = 0; i < NUM_RESOURCES; ++i) {
        assert(player->stockage[i] == 0);
    }
    free(player);
}

void test_player_resource_storage() {
    enum color_t couleur = RED;
    struct player_t* player = initialize_player(couleur);
    for (int i = 0; i < NUM_RESOURCES; ++i) {
        assert(player->stockage[i] == 0); 
    }
    player->stockage[0] = 10; 
    player->stockage[1] = 5;  
    assert(player->stockage[0] == 10);
    assert(player->stockage[1] == 5);
    free(player);
}

void test_player_number_of_workers() {
    enum color_t couleur = GREEN;
    struct player_t* player = initialize_player(couleur);
    assert(player->number_of_workers == MAX_WORKERS_PER_PLAYER-4);
    free(player);
}

int main() {
    test_initialize_player();
    test_player_resource_storage();
    test_player_number_of_workers();
    printf("All player tests passed!\n");
    return 0;
}
