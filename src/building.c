#include <stdio.h>
#include "position.h"
#include "resource.h"
#include "color.h"

#ifndef MAX_BUILDINGS_PER_PLAYER
 #define MAX_BUILDINGS_PER_PLAYER 7
#endif


struct batiment_t{
  char* nom;
  int value[10];
  int earns[10];
  int costs[10];
  int supplies[10];
  struct position_t *position;
  enum color_t joueur;
};



struct batiment_t make_batiment(char* nom, int *value, int *earns, int *costs, int *supplies, struct position_t *position, enum color_t joueur){
  struct batiment_t batiment;
  int i = 0;
  while (i<10){
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

int main(){
  return 0;
}
