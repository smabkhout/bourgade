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

void parcours_composante_connexe(struct position_t *pos_initial, int *indices_composantes_connexes, int longueur, struct board_t *board)
{
    // question : on prend composantes connexes aussi en diagonales ou uniquement dans les directions cardinales
    printf("La position courante est (%d,%d)\n", PY(pos_initial) + 1, PX(pos_initial) + 1);
    int index_pos_initial = PY(pos_initial) * MAX_X + PX(pos_initial);
    for (int j = 0; j < longueur; ++j)
    {
        if (indices_composantes_connexes[j] == index_pos_initial)
        {
            return; // si notre position existe déjà dans une composante connexe, on sort de la fonction
        }
    }
    indices_composantes_connexes[longueur] = index_pos_initial;
    for (int i = 0; i < MAX_POSITIONS/2; ++i)
    {
        printf("%d__", indices_composantes_connexes[i]);
    }
    printf("\n");
    ++longueur; // on stock cette position et on incrémente l'indice actuel (longueur) de notre tableau indices_composantes_connexes
    struct position_t **neighbors = malloc(sizeof(struct position_t *) * 8);
    for (int i = 0; i < 8; ++i)
    {
        neighbors[i] = make_invalid_position();
    }
    list_neighbors(pos_initial, neighbors);
    for (int i = 0; i < 8; ++i)
    {
        int neighbor_index = PY(neighbors[i]) * MAX_X + PX(neighbors[i]);
        printf("Le voisin courant est (%d,%d)\n", PY(neighbors[i]) + 1, PX(neighbors[i]) + 1);
        printf("%d\n", i);
        int appartient_autre_composante = 0;
        for (int j = 0; j < longueur; ++j)
        {
            if (indices_composantes_connexes[j] == neighbor_index)
            {
                appartient_autre_composante = 1;
                printf("La position (%d,%d) appartient à une composante\n", PY(neighbors[i]) + 1, PX(neighbors[i]) + 1);
                break;
            }
        }
        if (!appartient_autre_composante)
        {
            if (neighbors[i] != NULL && is_valid_position(neighbors[i]) && (board->tab[neighbor_index]->mine))
            {
                printf("Appel récursif de la position (%d,%d) à la position (%d,%d)\n", PY(neighbors[i]) + 1, PX(neighbors[i]) + 1, PY(pos_initial) + 1, PX(pos_initial) + 1);
                parcours_composante_connexe(neighbors[i], indices_composantes_connexes, longueur, board);
            }
        }
        printf("%d\n", i);
    }
    int est_pos_finale_composante_connexe = 1;
    for (int i = 0; i < 8; ++i)
    {
        int neighbor_index = PY(neighbors[i]) * MAX_X + PX(neighbors[i]);
        int appartient_autre_composante = 0;
        for (int j = 0; j < longueur; ++j)
        {
            if (indices_composantes_connexes[j] == neighbor_index)
            {
                appartient_autre_composante = 1;
                break;
            }
        }
        if (is_valid_position(neighbors[i]))
        {
            if (board->tab[neighbor_index]->mine)
            {
                if (!appartient_autre_composante)
                {
                    est_pos_finale_composante_connexe = 0; // si l'un des voisins est une mine qui n'est dans aucune des composantes alors on n'a pas encore finit notre composante connexe
                    break;                                 // il se peut qu'on est deja sur à cette etape qu'on est dans la derniere case puisqu'on a sortit de l'appel recursif
                                                           // tester sans tous ces boucles là
                }
            }
        }
    }
    if (est_pos_finale_composante_connexe)
    {
        printf("La position (%d,%d) est la dernière de cette composante\n", PY(pos_initial) + 1, PX(pos_initial) + 1);
        ++longueur; //la variable longueur ne se modifie pas après le return, il faut une autre solution
                    //solution possible est de stocker la variable longueur et passer en paramètre un pointeur
                    //vers cette variable afin dde le modifier à chaque fois qu'on le veut
        free(neighbors);
        return; // si notre case est la derniere de la composante, il suffit d'incrémenter longueur vers l'indice suivant et s'arreter
    }
    free(neighbors);
    // penser à mettre une condition d'arret pour le dernier element d'une composante;
    // on parcoure tous les voisins, si tous ces voisins sont deja dans indices composantes connexes/invalides/pas des mines on s'arrete
    // on met un -1 apres la fin de chaque composante;
}

int cost_of_mine_placement(struct board_t *board)
{
    int longueur = 0;
    int indices_composantes[MAX_POSITIONS / 2];
    for (int i = 0; i < MAX_POSITIONS/2; ++i)
    {
        indices_composantes[i]=-1;
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
            parcours_composante_connexe(POS(x, y), indices_composantes, longueur, board);
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
    for (int i = 0; i < MAX_POSITIONS/2; ++i)
    {
        printf("%d__", indices_composantes[i]);
    }
    printf("\n");
    return cost;
}