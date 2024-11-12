#include "resource.h"
#include "color.h"
#include "position.h"
#include "player.h"

#ifndef MAX_WORKERS_PER_PLAYER
    #define MAX_WORKERS_PER_PLAYER 6
#endif

struct worker_t {
    char* nom;
    int cout[MAX_RESOURCES]; //ressources nécessaires à payer l'employé 
    enum color_t joueur; //couleur représentant le joueur qui contrôle l'employé
};

struct worker_t* make_worker(char* name, int cost[MAX_RESOURCES], enum color_t couleur);


