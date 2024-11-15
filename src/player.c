#include <stdio.h>
#include <stdlib.h>
#include "player.h"
#include "worker.h"

struct player_t* initialize_player(enum color_t couleur){
    struct player_t* player = NULL; 
    player->color = couleur;
    for (int i = 0; i < MAX_RESOURCES; ++i)
    {
        player->stockage[i] = 0;
    }
    player->number_of_workers = MAX_WORKERS_PER_PLAYER;
    return player;
}


//gérer les ressources, employés et batiments de chacun des joueurs

int main()
{
    return 0;
}