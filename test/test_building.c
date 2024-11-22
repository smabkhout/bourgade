#include "building.h"
#include "resource.h"
#include "color.h"
#include "position.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void test_make_building() {
    unsigned int value[NUM_RESOURCES] = {0, 0, 0, 1, 0, 0};
    unsigned int earns[NUM_RESOURCES] = {0, 0, 0, 0, 0, 3};
    unsigned int costs[NUM_RESOURCES] = {0, 0, 0, 0, 0, 1};
    unsigned int supplies[NUM_RESOURCES] = {0, 1, 0, 0, 0, 0};
    char nom[] = "Farm";
    struct building_t* building = make_building(nom, value, earns, costs, supplies, RED);
    assert(building != NULL);
    assert(strcmp(building->nom, "Farm") == 0);
    for (int i = 0; i < NUM_RESOURCES; ++i) {
        assert(building->value[i] == value[i]);
        assert(building->earns[i] == earns[i]);
        assert(building->costs[i] == costs[i]);
        assert(building->supplies[i] == supplies[i]);
    }
    assert(building->joueur == RED);
    assert(is_valid_position(building->position) == 0); 
    free(building);
}

void test_place_building() {
    struct player_t player = { .stockage = {1, 1, 1, 1, 1, 1}, .color = BLUE };
    struct position_t* position = make_position(1, 1);
    unsigned int value[NUM_RESOURCES] = {0, 0, 0, 1, 0, 0};
    unsigned int earns[NUM_RESOURCES] = {0, 0, 0, 0, 0, 3};
    unsigned int costs[NUM_RESOURCES] = {0, 0, 0, 0, 0, 1};
    unsigned int supplies[NUM_RESOURCES] = {0, 1, 0, 0, 0, 0};
    struct building_t* building = make_building("Farm", value, earns, costs, supplies, BLUE);
    place_building(&player, position, building);
    assert(building->position == position);
    assert(building->joueur == BLUE);
    for (int i = 0; i < NUM_RESOURCES; i++) {
        assert(player.stockage[i] == 1 - costs[i] + earns[i]);
    }
    free(building);
    free(position);
}

void test_activate_building() {
    struct player_t owner = { .stockage = {1, 1, 1, 1, 1, 1}, .color = RED };
    struct player_t user = { .stockage = {5, 5, 5, 5, 5, 5}, .color = BLUE };
    struct position_t* position = make_position(1, 1);
    unsigned int value[NUM_RESOURCES] = {0, 0, 0, 1, 0, 0};
    unsigned int earns[NUM_RESOURCES] = {0, 0, 0, 0, 0, 3};
    unsigned int costs[NUM_RESOURCES] = {0, 0, 0, 0, 0, 1};
    unsigned int supplies[NUM_RESOURCES] = {0, 1, 0, 0, 0, 0};
    struct building_t* building = make_building("Farm", value, earns, costs, supplies, RED);
    place_building(&owner, position, building);
    activate_building(&owner, &user, building);
    for (int i = 0; i < NUM_RESOURCES; i++) {
        assert(user.stockage[i] == 5 - costs[i] + supplies[i]);
        assert(owner.stockage[i] == 1 + costs[i]);
    }
    free(building);
    free(position);
}

void test_list_buildings_costing_less_than() {
    struct player_t player;
    for (int i = 0; i < NUM_RESOURCES; ++i) {
        player.stockage[i] = i * 10;
    }
    struct building_t** affordable_buildings = list_buildings_costing_less_than(&player);
    printf("Testing list_buildings_costing_less_than:\n");
    for (int i = 0; i < 7; ++i) {
        if (affordable_buildings[i] != NULL) {
            printf("Affordable building found: %s\n", affordable_buildings[i]->nom);
            int is_affordable = 1;
            for (int res = 0; res < NUM_RESOURCES; ++res) {
                if (affordable_buildings[i]->value[res] > player.stockage[res]) {
                    is_affordable = 0;
                    printf("Error: Building '%s' costs more than available resources!\n", affordable_buildings[i]->nom);
                }
            }
            if (is_affordable) {
                printf("Building '%s' is affordable. Test passed.\n", affordable_buildings[i]->nom);
            }
        }
    }
    for (int i = 0; i < 7; ++i) {
        if (affordable_buildings[i] != NULL) {
            free(affordable_buildings[i]); 
        }
    }
    free(affordable_buildings);
    printf("All tests completed.\n");
}


int main() {
    test_make_building();
    test_place_building();
    test_activate_building();
    test_list_buildings_costing_less_than();
    printf("All building tests passed!\n");
    return 0;
}
