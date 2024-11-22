#include "building.h"
#include "resource.h"
#include "color.h"
#include "position.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void test_place_building() {
    struct player_t player = { .stockage = {1, 1, 1, 1, 1, 1}, .color = BLUE };
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
        building->value[i]= value[i];
        building->earns[i]= earns[i];
        building->supplies[i]=supplies[i];
    }
    building->position = make_invalid_position();
    char name[10] = "Farm";
    int j = 0;
    puts("A");
    while(j<10)
    {
        building->nom[j] = name[j];
        ++j;
    }
    puts("B");
    place_building(&player, position, building);
    puts("C");
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
    char nom[10] = "Farm";
    int j = 0;
    while(nom[j]!=0)
    {
        building->nom[j] = nom[j];
    }
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
    puts("1");
    test_place_building();
    puts("2");
    puts("3");
    test_activate_building();
    puts("4");
    test_list_buildings_costing_less_than();
    puts("5");
    printf("All building tests passed!\n");
    return 0;
}
