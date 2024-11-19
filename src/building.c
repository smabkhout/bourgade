#include <stdio.h>
#include <stdlib.h>
#include "building.h"
#include "position.h"

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
struct building_t list_buildings[MAX_BUILDINGS_PER_PLAYER] = {
 {.nom = "Farm", .costs = {0,0,0,0,0,1}, .earns={0,0,0,0,0,3}, .supplies={0,1,0,0,0,0}, .value={0,0,0,1,0,0}, .joueur=0, .position = NULL},
 {.nom = "Samwill", .costs = {0,0,0,0,0,1}, .earns={0,0,0,0,0,4}, .supplies={0,0,0,3,0,0}, .value={0,0,0,2,0,0}, .joueur=0, .position = NULL},
 {.nom = "Pontoon", .costs = {0,0,0,0,0,1}, .earns={0,0,0,0,0,5}, .supplies={0,0,2,0,0,0}, .value={0,0,0,3,0,0}, .joueur=0, .position = NULL},
 {.nom = "Quarry", .costs = {0,0,0,0,0,2}, .earns={0,0,0,0,0,5}, .supplies={0,0,0,2,0,0}, .value={0,0,0,3,0,0}, .joueur=0, .position = NULL},
 {.nom = "Market", .costs = {0,0,0,0,0,3}, .earns={0,0,0,0,0,3}, .supplies={1,1,1,0,1,0}, .value={0,0,0,0,0,6}, .joueur=0, .position = NULL},
 {.nom = "Bakery", .costs = {0,0,0,0,0,1}, .earns={0,0,0,0,0,2}, .supplies={0,3,0,0,0,0}, .value={0,0,0,1,1,0}, .joueur=0, .position = NULL},
 {.nom = "Factory", .costs = {0,0,0,1,1,1}, .earns={0,1,0,0,0,2}, .supplies={0,3,0,1,1,0}, .value={0,1,1,1,1,0}, .joueur=0, .position = NULL}
};


void place_building(struct player_t *player, struct position_t* position, struct building_t *building){ //acheter un batiment
  resource_sub(player->stockage, building->value, player->stockage); //achat du batiment
  resource_add(player->stockage, building->earns, player->stockage); //recompense immédiate à l'achat
  building->position = position; //on place le batiment au bon endroit
  building->joueur=player->color; //on colore le batiment de la couleur de son propriétaire
}

void activate_building(struct player_t *owner, struct player_t *user, struct building_t* building){ //activer la production d'un batiment
  resource_sub(user->stockage, building->costs, user->stockage); //user paye le cout d'activation
  resource_add(user->stockage, building->supplies, user->stockage); //user récupère les supplies
  resource_add(owner->stockage, building->costs, owner->stockage); //owner récupère le cout d'activation
}



int main()
{
  return 0;
}