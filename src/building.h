#ifndef __BUILDING_H__
#define __BUILDING_H__


#include "player.h"
#include "cell.h"

#ifndef MAX_BUILDINGS_PER_PLAYER
 #define MAX_BUILDINGS_PER_PLAYER 7
#endif

struct building_t{
  char nom[12];
  unsigned int value[NUM_RESOURCES];
  unsigned int earns[NUM_RESOURCES];
  unsigned int costs[NUM_RESOURCES];
  unsigned int supplies[NUM_RESOURCES];
  enum color_t joueur; //propriétaire du building
};

struct cell_t;

void free_building(struct building_t* batiment);

void place_building(struct player_t *player, struct cell_t* cell, struct building_t *building);

void activate_building(struct player_t *owner, struct player_t *user, struct building_t* building);

struct building_t** list_buildings_costing_less_than();

unsigned int **resource_vectors(struct player_t *player);

unsigned int building_in_resource_vectors(struct building_t *building, unsigned int **resource_vectors, struct player_t *player);

void free_resource_vectors(unsigned int **resource_vectors, struct player_t *player);

void free_affordable_buildings(struct building_t** batiments);

int length_of_affordable_buildings(struct building_t **affordable_buildings);

void copy_building(struct building_t* building1, struct building_t* building2);

//achievement 4
int reward_cathedral(struct building_t* cathedral, struct board_t* board, struct position_t* position);

int reward_castle(struct building_t* castle,struct board_t* board,struct position_t* position);

int reward_tower(struct building_t* tower, struct board_t* board,struct position_t* position);

int reward_farm(struct building_t* farm, struct board_t* board,struct position_t* position); //on invente un pouvoir pour
//le batiment farm

#endif