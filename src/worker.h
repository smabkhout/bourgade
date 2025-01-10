#ifndef __WORKER_H__
#define __WORKER_H__

#include "player.h"

#ifndef MAX_WORKERS_PER_PLAYER
    #define MAX_WORKERS_PER_PLAYER 6
#endif

struct cell_t;

struct board_t; 

struct worker_t {
    char* nom;
    unsigned int costs[NUM_RESOURCES]; //ressources nécessaires à payer l'employé 
    enum color_t joueur; //couleur représentant le joueur qui contrôle l'employé
    struct position_t * position;
};



void pay_worker(struct player_t *player, struct worker_t* worker);


void free_worker(struct worker_t* worker);


#endif