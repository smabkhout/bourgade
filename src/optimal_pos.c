#include <stdio.h>
#include <stdlib.h>

#include "optimal_pos.h"

struct position_t* choose_optimal_pos(struct board_t* board){
    int i=0;
    int histogramme_nb_ressources[MAX_POSITIONS+1]={0};
    while (i<MAX_POSITIONS){
        if (board->tab[i]->mine != NULL){
            unsigned int x = i % MAX_X;
            unsigned int y = (i - x) / MAX_X;
            struct position_t **neighbors = NULL;
            neighbors = (struct position_t **)malloc(sizeof(struct position_t *)*8);
            list_neighbors(POS(x,y), neighbors);
            for (int i = 0; i<8; ++i){
                int a = PY(neighbors[i])*MAX_X + PX(neighbors[i]);
                ++histogramme_nb_ressources[a];
            }
            for (int i = 0; i<8; ++i){
                free(neighbors[i]);
            }
            free(neighbors);
        }
        ++i;
    }
    int max=0;
    int pos_optimale=0;
    for (int i = 0; i<MAX_POSITIONS; ++i){
        if (histogramme_nb_ressources[i]>max){
            max=histogramme_nb_ressources[i];
            pos_optimale=i;
        }
    }
    unsigned int x = pos_optimale % MAX_X;
    unsigned int y = (pos_optimale - x) / MAX_X;
    return POS(x,y);
}
