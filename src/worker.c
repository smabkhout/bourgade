#include <stdio.h>
#include <stdlib.h>
#include "worker.h"
#include "cell.h"

struct worker_t* make_worker(char* name, int cost[NUM_RESOURCES], enum color_t couleur){
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

int main(){
    return 0;
}