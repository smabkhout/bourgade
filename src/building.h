#ifndef __BUILDING_H__
#define __BUILDING_H__


#include "position.h"
#include "player.h"

#ifndef MAX_BUILDINGS_PER_PLAYER
 #define MAX_BUILDINGS_PER_PLAYER 7
#endif

struct building_t{
  char* nom;
  unsigned int value[NUM_RESOURCES];
  unsigned int earns[NUM_RESOURCES];
  unsigned int costs[NUM_RESOURCES];
  unsigned int supplies[NUM_RESOURCES];
  enum color_t joueur; //propriétaire du building
  struct position_t *position;
};

struct building_t* make_building(char* nom, int *value, int *earns, int *costs, int *supplies, enum color_t joueur);

void place_building(struct player_t *player, struct position_t* position, struct building_t *building);

struct building_t** list_buildings_costing_less_than();

int length_of_affordable_buildings(struct building_t **affordable_buildings);

#endif