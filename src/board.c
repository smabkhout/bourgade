#include <stdio.h>
#include <stdlib.h>
#include "board.h"

struct building_t* get_building_from_cell(struct cell_t c){
    return c.building;
}

struct worker_t* get_worker_from_cell(struct cell_t c){
    return c.worker;
}

struct mine_t* get_mine_from_cell(struct cell_t c){
    return c.mine;
}

int occurrences(struct mine_t tab[MAX_POSITIONS/4]){ //fonction qui vérifie si on a au moins une mine de chaque ressource
    int occ1, occ2, occ3, occ4 = 0;
    for (int i = 0; i < MAX_POSITIONS/4; ++i)
    {
        if (tab[i].r == CORN)
            ++occ1;
        else if (tab[i].r == WOOD)
            ++occ2;
        else if (tab[i].r == STONE)
            ++occ3;
        else if (tab[i].r == FISH)
            ++occ4;
    }
    return (occ1*occ2*occ3*occ4); //retourne 0 si une occurrence est nulle et un entier non nul sinon
}

struct cell_t init_cell(){
    struct cell_t cell;
    cell.building = NULL;
    cell.mine = NULL;
    cell.worker = NULL;
    return cell;
}

int is_free_cell(struct cell_t c){ //retourne 1 si la cellule ne contient pas de buildings ou de worker ou de mine
    return (c.building == NULL)&&(c.mine==NULL)&&(c.worker==NULL);
}


int is_free_neighbor(struct cell_t cell, int indice){ //si au moins un voisin est libre => return 1 sinon 0
    return 1;
}

struct board_t init_board(int num_players){ //pour initialiser le board, on place N/4 mines de ressources où N est le nombre
//de positions valides (N>=16). Il faut aussi qu'il y au moins une mine de chaque type sur le board (Field, Forest, River, Rock
//mine). De plus, le nombre d'emplacement valide parmi les voisins de chaque mine est >= 1. 
    struct board_t board;
    int nb_of_mines = 0;
    for (int i = 0; i<MAX_X*MAX_Y; i++)
    {
        board.tab[i] = init_cell();
    }
    do {
        construct_mines();
    } while (!occurrences(present_mines)); //première condition : compter le nombre d'occurences de chaque mine dans le tableau present_mines => si une mine
    //a une occurence à 0 => on refait une génération
    do {

    int i = rand()%(MAX_X*MAX_Y);
    place_mine(&board.tab[i],present_mines[nb_of_mines]);
    nb_of_mines+=1;

    } while (1);
    //deuxième condition : chaque mine a au moins 1 position valide dans ses voisins sinon => on refait une génération 
    return board;  
}

void free_board(struct board_t* board) {
    for (int i = 0; i < MAX_X * MAX_Y; ++i) {
        if (board->tab[i].mine != NULL) { //free mines
            free(board->tab[i].mine);
            board->tab[i].mine = NULL; 
        }
        if (board->tab[i].worker != NULL) { //free workers
            free(board->tab[i].worker);
            board->tab[i].worker = NULL;
        }
        if (board->tab[i].building != NULL) { //free buildings
            free(board->tab[i].building);
            board->tab[i].building = NULL;
        }
    }
}


int main(int argc, char* argv[]){
    return 0;
}