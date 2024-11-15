#ifndef __WORKER_H__
#define __WORKER_H__

#include "resource.h"
#include "color.h"
#include "position.h"
#include "player.h"

#ifndef MAX_WORKERS_PER_PLAYER
    #define MAX_WORKERS_PER_PLAYER 6
#endif

struct worker_t {
    char* nom;
    int costs[NUM_RESOURCES]; //ressources nécessaires à payer l'employé 
    enum color_t joueur; //couleur représentant le joueur qui contrôle l'employé
    struct position_t * position;
};

struct worker_t *make_worker(char* name, int cost[MAX_RESOURCES], enum color_t couleur);

void pay_worker(struct player_t *player, struct worker_t* worker);

void place_worker(struct player_t *player, struct position_t *position, struct worker_t *worker);

#endif