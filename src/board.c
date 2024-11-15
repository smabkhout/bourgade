#include <stdio.h>
#include "board.h"

struct position_t* get_pos_from_cell(struct cell_t c){
    return c.pos;
}

struct buiding_t* get_building_from_cell(struct cell_t c){
    return c.building;
}

struct worker_t* get_worker_from_cell(struct cell_t c){
    return c.worker;
}

struct mine_t* get_mine_from_cell(struct cell_t c){
    return c.mine;
}

struct board_t init_board(int num_players){ //pour initialiser le board, on place N/4 mines de ressources où N est le nombre
//de positions valides (N>=16). Il faut aussi qu'il y au moins une mine de chaque type sur le board (Field, Forest, River, Rock
//mine). De plus, le nombre d'emplacement valide parmi les voisins de chaque mine est >= 1. 

}

int main(int argc, char* argv[]){
    return 0;
}