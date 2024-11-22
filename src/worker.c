#include <stdio.h>
#include <stdlib.h>
#include "worker.h"
#include "cell.h"

char* workers_names[MAX_WORKERS_PER_PLAYER] = {
    "Franck",
    "John",
    "Francis",
    "Walter",
    "Hanck",
    "Georges"
};

unsigned int workers_costs[NUM_RESOURCES] = {0,1,1,1,1,0};

struct worker_t* make_worker(char* name,unsigned int cost[NUM_RESOURCES], enum color_t couleur){
    struct worker_t *worker  = NULL;
    worker = (struct worker_t*)malloc(sizeof(struct worker_t));
    worker->nom = name;
    worker->joueur = couleur;
    for (int i = 0; i<NUM_RESOURCES; ++i){
        worker->costs[i] = cost[i];
    }
    worker->position = make_invalid_position();
    return worker;
}

void pay_worker(struct player_t *player, struct worker_t* worker)
{
    resource_sub(player->stockage,worker->costs,player->stockage); //on déduit de l'entrepot du joueur le salaire du travailleur
}

void place_worker(struct player_t *player, struct cell_t* cell,  struct worker_t *worker){
    cell->worker = worker;
    player->number_of_workers -= 1; //on décrémente le nombre de worker disponibles pour le player
}

void free_worker(struct worker_t* worker)
{
    free(worker);
}
