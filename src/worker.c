#include <stdio.h>
#include <stdlib.h>
#include "worker.h"

struct worker_t* make_worker(char* name, int cost[MAX_RESOURCES], enum color_t couleur){
    struct worker_t* worker  = NULL;
    worker = (struct worker_t*)malloc(sizeof(struct worker_t));
    worker->nom = name;
    worker->joueur = couleur;
    for (int i = 0; i<MAX_RESOURCES; ++i){
        worker->cout[i] = cost[i];
    }
    return worker;
}

//void place_worker(struct player_t player, struct position_t position, struct worker_t worker)

int main(){
    return 0;
}