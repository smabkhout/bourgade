#include "position.h"
#include "resource.h"
#include "color.h"

#ifndef MAX_BUILDINGS_PER_PLAYER
 #define MAX_BUILDINGS_PER_PLAYER 7
#endif


struct building_t{
  char* nom;
  int value[MAX_RESOURCES];
  int earns[MAX_RESOURCES];
  int costs[MAX_RESOURCES];
  int supplies[MAX_RESOURCES];
  struct position_t *position;
  enum color_t joueur;
};


struct building_t make_building(char* nom, int *value, int *earns, int *costs, int *supplies, struct position_t *position, enum color_t joueur){
  struct building_t batiment;
  int i = 0;
  while (i<MAX_RESOURCES){
    batiment.value[i] = value[i];
    batiment.earns[i] = earns[i];
    batiment.costs[i] = costs[i];
    batiment.supplies[i] = supplies[i];
    ++i;
  }
  int j=0;
  while (nom[j] != 0){
    batiment.nom[j] = nom[j];
    ++j;
  }
  batiment.position = position;
  batiment.joueur = joueur;
  return batiment;
}

