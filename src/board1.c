#include <stdio.h>
#include <stdlib.h>
#include "board.h"
#include "position.h"



int occurrences(struct mine_t tab[MAX_POSITIONS / 4])
{ // fonction qui vérifie si on a au moins une mine de chaque ressource
    int occ1, occ2, occ3, occ4 = 0;
    for (int i = 0; i < MAX_POSITIONS / 4; ++i)
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
    return (occ1 * occ2 * occ3 * occ4); // retourne 0 si une occurrence est nulle et un entier non nul sinon
}


struct board_t* init_board()
{ // pour initialiser le board, on place N/4 mines de ressources où N est le nombre
    // de positions valides (N>=16). Il faut aussi qu'il y au moins une mine de chaque type sur le board (Field, Forest, River, Rock
    // mine). De plus, le nombre d'emplacement valide parmi les voisins de chaque mine est >= 1.
    struct board_t* board = NULL;
    board = (struct board_t*)malloc(sizeof(struct board_t));
    for (int i = 0; i < MAX_X*MAX_Y; ++i)
    {
        board->tab[i] = (struct cell_t *)malloc(sizeof(struct cell_t));
    }
    int nb_of_mines = 0;
    for (int i = 0; i < MAX_X * MAX_Y; i++)
    {
        board->tab[i] = init_cell();
    }
    do
    {
        construct_mines();
    } while (occurrences(present_mines) == 0); // première condition : compter le nombre d'occurences de chaque mine dans le tableau present_mines => si une mine
    // a une occurence à 0 => on refait une génération

    int temp_invalid_pos[MAX_POSITIONS / 2]; // à chaque fois qu'on place une mine, on résérve une position voisine à ne pas
                                             // utiliser lors du placement des autres mines afin de garantir qu'on toujours au moins
                                             // une position voisine valide.
    do
    {
        int i = rand() % (MAX_X * MAX_Y);
        int is_free_position = 1;
        int is_free_neighbor = 1;
        for (int k = 0; k < MAX_POSITIONS / 4; k++)
        {
            if (temp_invalid_pos[k] == i)
                is_free_position = 0;
        } // verifier si la position est libre

        unsigned int x = i % MAX_X;
        unsigned int y = (i - x) / MAX_X;
        struct position_t **neighbors = NULL;
        neighbors = (struct position_t **)malloc(sizeof(struct position_t *)*8);
        list_neighbors(POS(x,y), neighbors);
        for (int i = 0; i<8;++i){
            int a = PY(neighbors[i])*MAX_X + PX(neighbors[i]);
            for (int k = 0; k < MAX_POSITIONS / 4; k++)
            {
                if (temp_invalid_pos[k] == a)
                    is_free_neighbor = 0; // vérifie si au moins un voisin de la position i est libre
            }
        }
        if (is_free_position && is_free_neighbor){
            place_mine(board->tab[i], present_mines[nb_of_mines]);
            nb_of_mines += 1;
        }
        for (int i = 0; i<8; ++i){
            free(neighbors[i]);
        }
        free(neighbors);
    } while (nb_of_mines < MAX_POSITIONS/4);
    //nb_of_mines vaut (N/4)-1 à la fin du prgramme mais c'est normal car initialisée à 0
    // deuxième condition : chaque mine a au moins 1 position valide dans ses voisins sinon => on refait une génération

    return board;
}

void free_board(struct board_t *board)
{
    for (int i = 0; i < MAX_X * MAX_Y; ++i)
    {
        if (board->tab[i]->mine != NULL)
        { // free mines
            free(board->tab[i]->mine);
        }
        if (board->tab[i]->worker != NULL)
        { // free workers
            free(board->tab[i]->worker);
        }
        if (board->tab[i]->building != NULL)
        { // free buildings
            free(board->tab[i]->building);
        }
    }
}

struct cell_t* find_free_cell(struct board_t* board){
    int i = rand()%MAX_X*MAX_Y;
    while (!is_free_cell(board->tab[i]))
    {
        i = rand()%MAX_X*MAX_Y;
    }
    return board->tab[i]; //position libre aléatoire
}

int main()
{
    return 0;
}