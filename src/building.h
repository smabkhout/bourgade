#include "position.h"
#include "resource.h"
#include "color.h"
#include "player.h"

#ifndef MAX_BUILDINGS_PER_PLAYER
 #define MAX_BUILDINGS_PER_PLAYER 7
#endif

struct building_t{
  char* nom;
  int value[MAX_RESOURCES];
  int earns[MAX_RESOURCES];
  int costs[MAX_RESOURCES];
  int supplies[MAX_RESOURCES];
  enum color_t joueur; //propriétaire du building
};

struct building_t* make_building(char* nom, int *value, int *earns, int *costs, int *supplies, enum color_t joueur);

//void place_building (struct player_t player, struct position_t position, struct building_t building)
void place_building(struct player_t *player, struct position_t* position, struct building_t *building);