 #ifndef __PLAYER_H__
 #define __PLAYER_H__

#include "color.h"
#include "resource.h"

#ifndef MAX_PLAYERS
    #define MAX_PLAYERS 4
#endif

struct player_t{
    enum color_t color;
    unsigned int stockage[NUM_RESOURCES]; //entrepot representant les reserves du joueur
    int number_of_workers; //to follow the number of workers that are used by the player, initialisé à MAX_WORKERS_PAR_PLAYER
    int eliminated; // 0 ou 1 si le joueur est éliminé ou non
};

struct player_t* initialize_player(enum color_t couleur);

void free_player(struct player_t* player);


#endif