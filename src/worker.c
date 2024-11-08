#include <stdio.h>
#include "resource.h"
#include "color.h"

#ifndef MAX_WORKERS_PER_PLAYER
    #define MAX_WORKERS_PER_PLAYER 6
#endif

struct employe_t {
    char* nom;
    enum resource_t* cout; //ressources nécessaires à payer l'employé
    enum color_t joueur; //couleur représentant le joueur qui contrôle l'employé
};

const char* name_employe(struct employe_t worker){
    return worker.nom;
}

enum resource_t* cost_employe(struct employe_t worker){
    return worker.cout;
}

enum color_t joueur_employe(struct employe_t worker){
    return worker.joueur;
}

struct employe_t make_employee(char* name, enum resource_t* cost, enum color_t couleur){
    struct employe_t worker;
    worker.nom = name;
    worker.cout = cost;
    worker.joueur = couleur;
    return worker;
}

int main()
{
    return 0;
}