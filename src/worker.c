#include <stdio.h>
#include <stdlib.h>
#include "worker.h"


void pay_worker(struct player_t *player, struct worker_t* worker)
{
    resource_sub(player->stockage,worker->costs,player->stockage); //on déduit de l'entrepot du joueur le salaire du travailleur
}



void free_worker(struct worker_t* worker)
{
    free(worker);
}


