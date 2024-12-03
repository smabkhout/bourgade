#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "building.h"



void free_building(struct building_t* batiment){
  free(batiment);
}


//définir un tableau global avec tous les buildings
struct building_t list_buildings[MAX_BUILDINGS_PER_PLAYER] = {
 {.nom = "Farm", .costs = {0,0,0,0,0,1}, .earns={0,0,0,0,0,3}, .supplies={0,1,0,0,0,0}, .value={0,0,0,1,0,0}, .joueur=0},
 {.nom = "Samwill", .costs = {0,0,0,0,0,1}, .earns={0,0,0,0,0,4}, .supplies={0,0,0,3,0,0}, .value={0,0,0,2,0,0}, .joueur=0},
 {.nom = "Pontoon", .costs = {0,0,0,0,0,1}, .earns={0,0,0,0,0,5}, .supplies={0,0,2,0,0,0}, .value={0,0,0,3,0,0}, .joueur=0},
 {.nom = "Quarry", .costs = {0,0,0,0,0,2}, .earns={0,0,0,0,0,5}, .supplies={0,0,0,2,0,0}, .value={0,0,0,3,0,0}, .joueur=0},
 {.nom = "Market", .costs = {0,0,0,0,0,3}, .earns={0,0,0,0,0,3}, .supplies={1,1,1,0,1,0}, .value={0,0,0,0,0,6}, .joueur=0},
 {.nom = "Bakery", .costs = {0,0,0,0,0,1}, .earns={0,0,0,0,0,2}, .supplies={0,3,0,0,0,0}, .value={0,0,0,1,1,0}, .joueur=0},
 {.nom = "Factory", .costs = {0,0,0,1,1,1}, .earns={0,1,0,0,0,2}, .supplies={0,3,0,1,1,0}, .value={0,1,1,1,1,0}, .joueur=0}
};


void place_building(struct player_t *player, struct cell_t* cell, struct building_t *building){ //acheter un batiment
  resource_sub(player->stockage, building->value, player->stockage); //achat du batiment
  resource_add(player->stockage, building->earns, player->stockage); //recompense immédiate à l'achat
  cell->building = building; //on place le batiment au bon endroit
  building->joueur=player->color; //on colore le batiment de la couleur de son propriétaire
}

void activate_building(struct player_t *owner, struct player_t *user, struct building_t* building){ //activer la production d'un batiment
  resource_sub(user->stockage, building->costs, user->stockage); //user paye le cout d'activation
  resource_add(user->stockage, building->supplies, user->stockage); //user récupère les supplies
  resource_add(owner->stockage, building->costs, owner->stockage); //owner récupère le cout d'activation
}

struct building_t** list_buildings_costing_less_than(struct player_t* player){
  struct building_t **affordable_buildings = NULL;
  affordable_buildings = (struct building_t **)malloc(sizeof(struct building_t *)*7);
  for (int i = 0; i < 7; ++i)
  {
    affordable_buildings[i] = NULL;
  }
  int i=0;
  int j=0;
  while (i<7){
    int is_affordable=1;
    for (int res= 0; res<NUM_RESOURCES; ++res){
      is_affordable = is_affordable*(list_buildings[i].value[res] <= player->stockage[res]);
    }
    if (is_affordable){
      affordable_buildings[j]= &list_buildings[i];
      ++j;
    }
    ++i;
  }
  return affordable_buildings;
}


void free_affordable_buildings(struct building_t** batiments){
  free(batiments);
}



int length_of_affordable_buildings(struct building_t **affordable_buildings){
  int count=0;
  while (affordable_buildings[count] != NULL){
    ++count;
  }
  return count;
}

void copy_building(struct building_t* b2, struct building_t* b1){
  b2->joueur = b1->joueur;
  strcpy(b2->nom, b1->nom);
  for (int i = 0; i<NUM_RESOURCES; ++i)
  {
    b2->costs[i] = b1->costs[i];
    b2->earns[i] = b1->earns[i];
    b2->value[i] = b1->value[i];
    b2->supplies[i] = b1->supplies[i];
  }
}