#include "building.h"
#include "resource.h"
#include "color.h"
#include "position.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

unsigned int tab_init[6] = {1,1,1,1,1,1};
unsigned int tab_final[6] = {1,1,1,0,1,4};

void test_place_building() {
    struct player_t *player = NULL;
    player = (struct player_t*)malloc(sizeof(struct player_t));
    player->color = BLUE;
    player->number_of_workers = 7;
    for (int i = 0;i<6;++i)
    {
        player->stockage[i] = tab_init[i];
    }
    struct position_t* position = make_position(1, 1);
    unsigned int value[NUM_RESOURCES] = {0, 0, 0, 1, 0, 0};
    unsigned int earns[NUM_RESOURCES] = {0, 0, 0, 0, 0, 3};
    unsigned int costs[NUM_RESOURCES] = {0, 0, 0, 0, 0, 1};
    unsigned int supplies[NUM_RESOURCES] = {0, 1, 0, 0, 0, 0};
    struct building_t* building = NULL;
    building = (struct building_t*)malloc(sizeof(struct building_t));
    for (int i = 0; i< NUM_RESOURCES; ++i)
    {
        building->costs[i] = costs[i];
        building->value[i] = value[i];
        building->earns[i] = earns[i];
        building->supplies[i] = supplies[i];
    }
    place_building(player, position, building);
    assert(building->position == position);
    assert(building->joueur == BLUE);
    for (int i = 0; i < NUM_RESOURCES; i++) {
        assert(player->stockage[i] == tab_final[i]);
    }
    free(building);
    free(player);
}

unsigned int tab_initial_user[6] = {5,5,5,5,5,5};
unsigned int tab_initial_owner[6] = {1,1,1,1,1,1};

unsigned int tab_final_owner[6] = {1,1,1,1,1,2};

void test_activate_building() {
    struct player_t *owner = NULL;
    owner = (struct player_t*)malloc(sizeof(struct player_t));
    struct player_t *user = NULL;
    user = (struct player_t*)malloc(sizeof(struct player_t));
    for (int i = 0; i< NUM_RESOURCES; ++i)
    {
        owner->stockage[i] = tab_initial_owner[i];
        user->stockage[i] = tab_initial_user[i];
    }
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
    activate_building(owner, user, building);
    for (int i = 0; i < NUM_RESOURCES; i++) {
        assert(user->stockage[i] == 5 - costs[i] + supplies[i]);
        assert(owner->stockage[i] == tab_final_owner[i]);
    }
    free_building(building);
    free_player(owner);
    free_player(user);
}




void test_list_buildings_costing_less_than() {
    struct player_t *player = NULL;
    player=(struct player_t*)malloc(sizeof(struct player_t));
    for (int i = 0; i < NUM_RESOURCES; ++i) {
        player->stockage[i] = i * 10;
    }
    struct building_t** affordable_buildings = list_buildings_costing_less_than(player);
    printf("Testing list_buildings_costing_less_than:\n");
    for (int i = 0; i < 7; ++i) {
        if (affordable_buildings[i] != NULL) {
            printf("Affordable building found: %s\n", affordable_buildings[i]->nom);
            int is_affordable = 1;
            for (int res = 0; res < NUM_RESOURCES; ++res) {
                if (affordable_buildings[i]->value[res] > player->stockage[res]) {
                    is_affordable = 0;
                    printf("Error: Building '%s' costs more than available resources!\n", affordable_buildings[i]->nom);
                }
            }
            if (is_affordable) {
                printf("Building '%s' is affordable. Test passed.\n", affordable_buildings[i]->nom);
            }
        }
    }
    free_affordable_buildings(affordable_buildings);
    free_player(player);
    printf("All tests completed.\n");
}


int main() {
    test_place_building();
    test_activate_building();
    test_list_buildings_costing_less_than();
    printf("All building tests passed!\n");
    return 0;
}
