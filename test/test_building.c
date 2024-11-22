#include "building.h"
#include "resource.h"
#include "color.h"
#include "position.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void test_make_building() {
    int value[NUM_RESOURCES] = {0, 0, 0, 1, 0, 0};
    int earns[NUM_RESOURCES] = {0, 0, 0, 0, 0, 3};
    int costs[NUM_RESOURCES] = {0, 0, 0, 0, 0, 1};
    int supplies[NUM_RESOURCES] = {0, 1, 0, 0, 0, 0};
    char nom[] = "Farm";
    struct player_t player = { .stockage = {1, 1, 1, 1, 1, 1}, .color = RED };
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
    int value[NUM_RESOURCES] = {0, 0, 0, 1, 0, 0};
    int earns[NUM_RESOURCES] = {0, 0, 0, 0, 0, 3};
    int costs[NUM_RESOURCES] = {0, 0, 0, 0, 0, 1};
    int supplies[NUM_RESOURCES] = {0, 1, 0, 0, 0, 0};
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
    int value[NUM_RESOURCES] = {0, 0, 0, 1, 0, 0};
    int earns[NUM_RESOURCES] = {0, 0, 0, 0, 0, 3};
    int costs[NUM_RESOURCES] = {0, 0, 0, 0, 0, 1};
    int supplies[NUM_RESOURCES] = {0, 1, 0, 0, 0, 0};
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
    struct player_t player = { .stockage = {1, 1, 1, 1, 1, 1}, .color = BLUE };
    struct building_t** affordable_buildings = list_buildings_costing_less_than(&player);
    int count = length_of_affordable_buildings(affordable_buildings);
    assert(count == 7);
    for (int i = 0; i < count; ++i) {
        struct building_t* building = affordable_buildings[i];
        unsigned int new_stockage[NUM_RESOURCES] = {1};
        resource_sub(player.stockage, building->value, new_stockage);
        int is_affordable = 1;
        for (int j = 0; j < NUM_RESOURCES; ++j) {
            if (new_stockage[j] < 0) {
                is_affordable = 0;
                break;
            }
        }
        assert(is_affordable == 1);
    }
    for (int i = 0; i < count; ++i) {
        free(affordable_buildings[i]);
    }
    free(affordable_buildings);
}

int main() {
    test_make_building();
    test_place_building();
    test_activate_building();
    test_list_buildings_costing_less_than();
    printf("All building tests passed!\n");
    return 0;
}
