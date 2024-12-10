#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>

#include "mine.h"

static struct mine_t list_mines[4] = {
    {.name = "Field", .r = CORN},
    {.name = "Forest", .r = WOOD},
    {.name = "River", .r = FISH},
    {.name = "Rock Mine", .r = STONE},
};

struct mine_t *make_mine(enum resource_t r, char nom[12])
{
    struct mine_t *mine = NULL;
    mine = (struct mine_t *)malloc(sizeof(struct mine_t));
    mine->r = r;
    int i = 0;
    while (i < 12)
    {
        mine->name[i] = nom[i];
        ++i;
    }
    return mine;
}

void place_mine(struct cell_t *cell, struct mine_t m)
{
    if (cell->mine == NULL)
    {
        cell->mine = malloc(sizeof(struct mine_t));
    }
    cell->mine->r = m.r;
    int i = 0;
    while (m.name[i] != 0)
    {
        cell->mine->name[i] = m.name[i];
        ++i;
    }
    cell->mine->name[i] = 0;
}

int max(int a, int b)
{
    if (a > b)
        return a;
    else
        return b;
}

void construct_mines(struct mine_t *present_mines)
{ // modifie le tableau present_mines pour avoir MAX_POSITIONS/4 mines aléatoires
    int j = 4;
    for (int i = 0; i < max(4, MAX_POSITIONS / 4); i++)
    {
        present_mines[i] = list_mines[i];
        // on s'assure d'avoir au moins une mine de chaque type
        // en remplissant à la main les 4 premières
    }
    while (j < MAX_POSITIONS / 4)
    {
        int a = rand() % 4;
        present_mines[j] = list_mines[a];
        ++j;
    }
}

void free_mine(struct mine_t *mine)
{
    free(mine);
}

void parcours_composante_connexe(struct position_t *pos_initial, int *indices_composantes_connexes, int *longueur, struct board_t *board, int debut_composante)
{
    // question : on prend composantes connexes aussi en diagonales ou uniquement dans les directions cardinales
    int index_pos_initial = PY(pos_initial) * MAX_X + PX(pos_initial);
    for (int j = 0; j < *longueur; ++j)
    {
        if (indices_composantes_connexes[j] == index_pos_initial)
        {
            return; // si notre position existe déjà dans une composante connexe, on sort de la fonction
        }
    }
    indices_composantes_connexes[*longueur] = index_pos_initial;
    ++*longueur; // on stock cette position et on incrémente l'indice actuel (longueur) de notre tableau indices_composantes_connexes
    struct position_t **neighbors = malloc(sizeof(struct position_t *) * 8);
    for (int i = 0; i < 8; ++i)
    {
        neighbors[i] = make_invalid_position();
    }
    list_neighbors(pos_initial, neighbors);
    int indices_voisins_a_parcourir[8] = {0};
    int nb_voisins_a_parcourir = 0;
    for (int i = 0; i < 8; ++i) // premierement stocker les mines voisines dans le tableau composante connexe
    {
        int neighbor_index = PY(neighbors[i]) * MAX_X + PX(neighbors[i]);
        int appartient_autre_composante = 0;
        for (int j = 0; j < *longueur; ++j)
        {
            if (indices_composantes_connexes[j] == neighbor_index)
            {
                appartient_autre_composante = 1;
                break;
            }
        }
        if (!appartient_autre_composante && neighbors[i] && is_valid_position(neighbors[i]) && (board->tab[neighbor_index]->mine))
        {
            indices_voisins_a_parcourir[nb_voisins_a_parcourir] = i;
            ++nb_voisins_a_parcourir;
        }
    }
    for (int j = 0; j < nb_voisins_a_parcourir; ++j) // appeler la fonction recursivement sur ceux qui ne sont pas déjà dans le tableau
    {
        int neighbor_index = PY(neighbors[indices_voisins_a_parcourir[j]]) * MAX_X + PX(neighbors[indices_voisins_a_parcourir[j]]);
        if (neighbors[indices_voisins_a_parcourir[j]] && is_valid_position(neighbors[indices_voisins_a_parcourir[j]]) && (board->tab[neighbor_index]->mine))
        {
            parcours_composante_connexe(neighbors[indices_voisins_a_parcourir[j]], indices_composantes_connexes, longueur, board, 0);
        }
    }

    if (debut_composante)
    {
        ++*longueur;
        free(neighbors);
        return;
    }
    else
    {
        free(neighbors);
        return;
    }
}

int cost_of_mine_placement(struct board_t *board)
{
    int *longueur = (int *)malloc(sizeof(int));
    *longueur = 0;
    int indices_composantes[MAX_POSITIONS / 2];
    int debut_composante = 1;
    for (int i = 0; i < MAX_POSITIONS / 2; ++i)
    {
        indices_composantes[i] = -1;
    }
    // contient les indices des positions
    // on a MAX_POS/4 mines, au pire on a
    // MAX_POS/4 amas différents et MAX_POS/4 0 entre chaque donc MAX_POS/2 au total
    for (int i = 0; i < MAX_POSITIONS; ++i)
    {

        if (board->tab[i]->mine)
        {
            unsigned int x = i % MAX_X;
            unsigned int y = (i - x) / MAX_X;
            parcours_composante_connexe(POS(x, y), indices_composantes, longueur, board, debut_composante);
        }
    }

    int length_of_group = 0;
    int cost = 0;
    int i = 0;
    while (i < MAX_POSITIONS / 2)
    {
        if (indices_composantes[i] != -1)
        {
            ++length_of_group;
        }
        else
        {
            cost += length_of_group * length_of_group;
            length_of_group = 0;
        }
        ++i;
    }
    free(longueur);
    return cost;
}

int *best_mine_placement()
{
    int *tab = (int *)malloc(sizeof(int) * MAX_POSITIONS / 4); // renvoyer un tableau contenant les indices des mines sur le plateau
    int *repartitions = (int *)malloc(sizeof(int) * (MAX_POSITIONS / 4) * 100);
    int *costs = (int *)malloc(sizeof(int) * 100);
    struct board_t* board;
    for (int j = 0; j < 100; ++j) // sur 100 itérations, on garde la meilleure répartition
    {
        int nb_of_mines = 0;
        board = init_board(j);
        costs[j] = cost_of_mine_placement(board);
        for (int i = 0; i < MAX_POSITIONS; ++i)
        {
            if (board->tab[i]->mine)
            {
                repartitions[j * (MAX_POSITIONS / 4) + nb_of_mines] = i;
                nb_of_mines++;
            }
        }
        free_board(board);
    }

    int indice_min = 0;
    int min = costs[0];
    for (int i = 1; i< 100;++i)
    {
        if (costs[i] < min){
            min = costs[i];
            indice_min = i;
        }
    }

    for (int k = 0 ; k < MAX_POSITIONS/4 ; ++k)
    {
        tab[k] = repartitions[indice_min * (MAX_POSITIONS/4) + k];
    }
    free(repartitions);
    free(costs);
    return tab;
}