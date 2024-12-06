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

int cost_of_mine_placement(struct board_t *board)
{
    int longueur = 0;
    int indices_composantes[MAX_POSITIONS / 2] = {-1};
    // contient les indices des positions
    // on a MAX_POS/4 mines, au pire on a
    // MAX_POS/4 amas différents et MAX_POS/4 0 entre chaque donc MAX_POS/2 au total
    for (int i = 0; i < MAX_POSITIONS; ++i)
    {

        if (board->tab[i]->mine)
        {
            unsigned int x = i % MAX_X;
            unsigned int y = (i - x) / MAX_X;
            /*
            struct position_t **neighbors = (struct position_t **)malloc(sizeof(struct position_t *) * 8);
            for (int j = 0; j < 8; ++j)
            {
                neighbors[i] = make_invalid_position();
            }
            list_neighbors(POS(x, y), neighbors);

            for (int k = 0; k < 8; ++k)
            {
                parcours_composante_connexe(neighbors[i], indices_composantes, longueur, board);
            }
            free(neighbors);*/
            parcours_composante_connexe(POS(x,y),indices_composantes,longueur,board);
        }
    }
    int length_of_group=0;
    int cost = 0;
    int i = 0;
    while (i < MAX_POSITIONS/2)
    {
        if (indices_composantes[i] != -1)
        {
            ++length_of_group;
        }
        else 
        {
            cost += length_of_group*length_of_group;
            length_of_group = 0;
        }
        ++i;
    }
    return cost;
}
