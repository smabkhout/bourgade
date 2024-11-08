#include "position.h"
#include "resource.h"

#ifndef MAX_BUILDINGS_PER_PLAYER
 #define MAX_BUILDINGS_PER_PLAYER 7
#endif


struct batiment_t{
  char* nom;
  enum resource_t *value;
  enum resource_t *earns;
  enum resource_t *costs;
  enum resource_t *supplies;
  struct position_t *position;
};

const char* nom_batiment(struct batiment_t batiment){
  return batiment.nom;
}

enum resource_t* batiment_value(struct batiment_t batiment){
  return batiment.value;
}

enum resource_t* batiment_costs(struct batiment_t batiment){
  return batiment.costs;
}

enum resource_t* batiment_supplies(struct batiment_t batiment){
  return batiment.supplies;
}

enum resource_t* batiment_earns(struct batiment_t batiment){
  return batiment.earns;
}

struct position_t* batiment_position(struct batiment_t batiment){
  return batiment.position;
}
