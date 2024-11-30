#include <stdio.h>
#include <stdlib.h>

#include "optimal_pos.h"

struct position_t *choose_optimal_pos(struct board_t *board)
{
    if (!board)
    {
        return NULL; // Vérification de sécurité
    }
    int max_resources = 0; // Nombre maximum de ressources autour d'une position
    struct position_t *optimal_pos = NULL;
    // Parcourir chaque position du plateau
    for (int y = 0; y < MAX_Y; ++y)
    {
        for (int x = 0; x < MAX_X; ++x)
        {
            if (x >= 0 && x < MAX_X && y >=0 && y < MAX_Y)
            {
                int current_index = y * MAX_X + x;
                if (board->tab[current_index] != NULL)
                {
                    struct cell_t *cell = board->tab[current_index];
                    // Vérifier si la case est libre
                    if (cell->mine == NULL && cell->worker == NULL && cell->building == NULL)
                    {
                        int resource_count = 0;
                        // Vérifier les voisins de la position courante
                        struct position_t **neighbors = malloc(sizeof(struct position_t *) * 8);
                        for (int i = 0; i< 8; ++i)
                        {
                            neighbors[i] = make_invalid_position();
                        }
                        list_neighbors(POS(x, y), neighbors);
                        for (int i = 0; i < 8; ++i)
                        {
                            if (neighbors[i] != NULL && is_valid_position(neighbors[i]))
                            {
                                int neighbor_index = PY(neighbors[i]) * MAX_X + PX(neighbors[i]);
                                if (board->tab[neighbor_index]->mine != NULL)
                                {
                                    ++resource_count; // Augmenter le compte si un voisin contient une mine
                                }
                            }
                        }
                        free(neighbors);
                        // Si cette position a plus de ressources autour, elle devient optimale
                        if (resource_count > max_resources)
                        {
                            max_resources = resource_count;
                            optimal_pos = POS(x, y);
                        }
                    }
                }
            }
        }
    }
    return optimal_pos; // Retourne la position optimale, ou NULL si aucune n'est valide
}
