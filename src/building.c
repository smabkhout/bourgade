#include <stdio.h>
#include <stdlib.h>
#include "building.h"


struct building_t* make_building(char* nom, int *value, int *earns, int *costs, int *supplies, enum color_t joueur){
  struct building_t* batiment = NULL;
  batiment = (struct building_t*)malloc(sizeof(struct building_t));
  int i = 0;
  while (i<MAX_RESOURCES){
    batiment->value[i] = value[i];
    batiment->earns[i] = earns[i];
    batiment->costs[i] = costs[i];
    batiment->supplies[i] = supplies[i];
    ++i;
  }
  int j=0;
  while (nom[j] != 0){
    batiment->nom[j] = nom[j];
    ++j;
  }
  batiment->joueur = joueur;
  batiment->position = make_invalid_position();
  return batiment;
}


//définir un tableau global avec tous les buildings
struct building_t list_buildings[MAX_BUILDINGS_PER_PLAYER];

void place_building(struct player_t *player, struct position_t* position, struct building_t *building){
  resource_sub(player->stockage, building->costs, player->stockage); //achat du batiment
  building->position = position;
}

int main(int argc, char* argv[])
{
  return 0;
}