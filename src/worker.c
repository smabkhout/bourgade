#include <stdio.h>
#include <stdlib.h>
#include "worker.h"



struct worker_t* make_worker(char* name,unsigned int cost[NUM_RESOURCES], enum color_t couleur){
    struct worker_t *worker  = NULL;
    worker = (struct worker_t*)malloc(sizeof(struct worker_t));
    worker->nom = name;
    worker->joueur = couleur;
    for (int i = 0; i<NUM_RESOURCES; ++i){
        worker->costs[i] = cost[i];
    }
    worker->position = make_invalid_position();
    return worker;
}

void pay_worker(struct player_t *player, struct worker_t* worker)
{
    resource_sub(player->stockage,worker->costs,player->stockage); //on déduit de l'entrepot du joueur le salaire du travailleur
}

void place_worker(struct player_t *player, struct cell_t* cell,  struct worker_t *worker){
    cell->worker = worker;
    player->number_of_workers -= 1; //on décrémente le nombre de worker disponibles pour le player
    worker->joueur = player->color; //le worker est maintenant associé au joueur
}

void free_worker(struct worker_t* worker)
{
    free(worker);
}

void pay_workers_on_board(struct board_t* board, int num_players, struct player_t** players)
{
    for (int i = 0; i<MAX_POSITIONS; ++i)
        {
            if (board->tab[i]->worker != NULL)
            {
                //identifier son joueur (employer)
                int j = 0;
                while (j<num_players)
                {
                    if (players[j]->color == board->tab[i]->worker->joueur)
                    break;
                    ++j;
                }
                //vérifier si le joueur peut payer le worker, sinon il va etre éliminé
                if (resource_le_than(players[j]->stockage, board->tab[i]->worker->costs))
                {
                    players[j]->eliminated = 1;
                }
                else
                {
                    pay_worker(players[j], board->tab[i]->worker);
                }
            }
        }
}

void reset_workers_still_on_board(struct board_t* board)
{
    for (int i = 0; i<MAX_POSITIONS; ++i)
    {
        if (board->tab[i]->worker != NULL && board->tab[i]->building == NULL)
        {
            free_worker(board->tab[i]->worker);
            board->tab[i]->worker = NULL;
        }
    }
}
