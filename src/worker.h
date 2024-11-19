#ifndef __WORKER_H__
#define __WORKER_H__

#include "resource.h"
#include "color.h"
#include "position.h"
#include "player.h"

#ifndef MAX_WORKERS_PER_PLAYER
    #define MAX_WORKERS_PER_PLAYER 6
#endif

struct cell_t;

char* workers_names[MAX_WORKERS_PER_PLAYER] = {
    "Franck",
    "John",
    "Francis",
    "Walter",
    "Hanck",
    "Georges"
};

int workers_costs[NUM_RESOURCES] = {0,1,1,1,1,0};

struct worker_t {
    char* nom;
    unsigned int costs[NUM_RESOURCES]; //ressources nécessaires à payer l'employé 
    enum color_t joueur; //couleur représentant le joueur qui contrôle l'employé
    struct position_t * position;
};

struct worker_t *make_worker(char* name, int cost[NUM_RESOURCES], enum color_t couleur);

void pay_worker(struct player_t *player, struct worker_t* worker);

void place_worker(struct player_t *player, struct cell_t* cell, struct worker_t *worker);

#endif