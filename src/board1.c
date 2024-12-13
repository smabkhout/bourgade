#include <stdio.h>
#include <stdlib.h>
#include "board.h"

int occurrences(struct mine_t *tab)
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

struct board_t *init_board(int seed)
{ // pour initialiser le board, on place N/4 mines de ressources où N est le nombre
    // de positions valides (N>=16). Il faut aussi qu'il y au moins une mine de chaque type sur le board (Field, Forest, River, Rock
    // mine). De plus, le nombre d'emplacement valide parmi les voisins de chaque mine est >= 1.
    (void ) seed;
    struct board_t *board;
    board = (struct board_t *)malloc(sizeof(struct board_t));
    board->tab = (struct cell_t **)malloc(sizeof(struct cell_t *) * MAX_POSITIONS);
    board->present_mines = (struct mine_t *)malloc(sizeof(struct mine_t) * MAX_POSITIONS / 4);
    int nb_of_mines = 0;
    for (int i = 0; i < MAX_X * MAX_Y; i++)
    {
        board->tab[i] = init_cell();
    }
    construct_mines(board->present_mines);   // construct mines s'occupe d'avoir au moins une mine de chaque type
    int temp_invalid_pos[MAX_POSITIONS / 2]; // à chaque fois qu'on place une mine, on résérve une position voisine à ne pas
                                             // utiliser lors du placement des autres mines afin de garantir qu'on toujours au moins
                                             // une position voisine valide.
    int count_invalid_pos = 0;
    for (int i = 0; i < MAX_POSITIONS / 2; ++i)
    {
        temp_invalid_pos[i] = -1;
    }

    do
    {
        int i = rand() % (MAX_X * MAX_Y);
        int is_free_position = 1;
        int is_free_neighbor = 8;
        unsigned int x = i % MAX_X;
        unsigned int y = (i - x) / MAX_X;
        for (int k = 0; k < MAX_POSITIONS / 2; k++)
        {
            if (temp_invalid_pos[k] == i || !is_free_cell(board->tab[i]) || !is_valid_position(POS(x, y)))
                is_free_position = 0;
        } // verifier si la position est libre

        if (is_free_position)
        {
            temp_invalid_pos[count_invalid_pos] = i;
            count_invalid_pos++;
            struct position_t **neighbors = NULL;
            neighbors = (struct position_t **)malloc(sizeof(struct position_t *) * 8);
            for (int j = 0; j < 8; j++)
            {
                neighbors[j] = make_invalid_position();
            }
            list_neighbors(POS(x, y), neighbors);
            for (int j = 0; j < 8; ++j)
            {
                if (is_valid_position(neighbors[j]))
                {
                    int a = PY(neighbors[j]) * MAX_X + PX(neighbors[j]);
                    for (int k = 0; k < MAX_POSITIONS / 2; k++)
                    {
                        if (temp_invalid_pos[k] == a)
                            is_free_neighbor--; // vérifie si au moins un voisin de la position i est libre
                    }
                    if (is_free_neighbor)
                    {
                        temp_invalid_pos[count_invalid_pos] = a;
                        count_invalid_pos++;
                        break;
                    }
                }
            }
            if (is_free_position && is_free_neighbor)
            {
                place_mine(board->tab[i], board->present_mines[nb_of_mines]);
                nb_of_mines += 1;
            }
            free(neighbors);
        }
    } while (nb_of_mines < MAX_POSITIONS / 4);
    // nb_of_mines vaut (N/4)-1 à la fin du prgramme mais c'est normal car initialisée à 0
    //  deuxième condition : chaque mine a au moins 1 position valide dans ses voisins sinon => on refait une génération
    return board;
}

struct board_t* init_best_board(int seed){
    srand(seed);
    struct board_t *board;
    board = (struct board_t *)malloc(sizeof(struct board_t));
    board->tab = (struct cell_t **)malloc(sizeof(struct cell_t *) * MAX_POSITIONS);
    board->present_mines = (struct mine_t *)malloc(sizeof(struct mine_t) * MAX_POSITIONS / 4);
    construct_mines(board->present_mines);
    for (int i = 0; i < MAX_X * MAX_Y; i++)
    {
        board->tab[i] = init_cell();
    }
    int* repartition = best_mine_placement();
    for (int i = 0 ; i< MAX_POSITIONS/4;++i)
    {
        place_mine(board->tab[repartition[i]],board->present_mines[i]);
    }
    free(repartition);
    return board;
}

void free_board(struct board_t *board)
{
    for (int i = 0; i < MAX_POSITIONS; ++i)
    {
        if (board->tab[i]->mine != NULL)
        {
            free_mine(board->tab[i]->mine);
        }
        if (board->tab[i]->worker != NULL)
        { // free workers
            free_worker(board->tab[i]->worker);
        }
        /*
        if (board->tab[i]->building != NULL)
        { // free buildings
            free_building(board->tab[i]->building);
        }*/
        free_cell(board->tab[i]);
    }
    free(board->tab);
    if (board->present_mines != NULL)
        free_mine(board->present_mines);
    free(board);
}