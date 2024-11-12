#include "color.h"
#include "resource.h"

#ifndef MAX_PLAYERS
    #define MAX_PLAYERS 4
#endif

struct player_t{
    enum color_t color;
    int stockage[MAX_RESOURCES]; //entrepot representant les reserves du joueur
    int number_of_workers; //to follow the number of workers that are used by the player
};

struct player_t initialize_player(enum color_t couleur){
    struct player_t player; 
    player.color = couleur;
    for (int i = 0; i < MAX_RESOURCES; ++i)
    {
        player.stockage[i] = 0;
    }
    return player;
}

//gérer les ressources, employés et batiments de chacun des joueurs