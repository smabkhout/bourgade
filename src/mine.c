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
    printf("La position courante est (%d,%d)\n", PY(pos_initial) + 1, PX(pos_initial) + 1);
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
    printf("La position (%d,%d) est stocké\n", PY(pos_initial) + 1, PX(pos_initial) + 1);
    for (int i = 0; i < MAX_POSITIONS / 2; ++i)
    {
        printf("%d__", indices_composantes_connexes[i]);
    }
    printf("\n");
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
        if (is_valid_position(neighbors[i]))
        {
            printf("Le voisin courant est (%d,%d)\n", PY(neighbors[i]) + 1, PX(neighbors[i]) + 1);
            printf("%d\n", i);
        }
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
            printf("Le voisin (%d,%d) est stocké afin qu'il soit parcouru\n", PY(neighbors[i]) + 1, PX(neighbors[i]) + 1);
        }
    }
    for (int j = 0; j < nb_voisins_a_parcourir; ++j) // appeler la fonction recursivement sur ceux qui ne sont pas déjà dans le tableau
    {
        int neighbor_index = PY(neighbors[indices_voisins_a_parcourir[j]]) * MAX_X + PX(neighbors[indices_voisins_a_parcourir[j]]);
        if (neighbors[indices_voisins_a_parcourir[j]] && is_valid_position(neighbors[indices_voisins_a_parcourir[j]]) && (board->tab[neighbor_index]->mine))
        {
            printf("Appel récursif de la position (%d,%d) à la position (%d,%d)\n", PY(neighbors[indices_voisins_a_parcourir[j]]) + 1, PX(neighbors[indices_voisins_a_parcourir[j]]) + 1, PY(pos_initial) + 1, PX(pos_initial) + 1);
            parcours_composante_connexe(neighbors[indices_voisins_a_parcourir[j]], indices_composantes_connexes, longueur, board, 0);
        }
    }

    if (debut_composante)
    {
        ++*longueur;
        printf("C'est la fin du parcours de la composante connexe acuelle.\n");
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
    for (int i = 0; i < MAX_POSITIONS / 2; ++i)
    {
        printf("%d__", indices_composantes[i]);
    }
    printf("\n");
    free(longueur);
    return cost;
}