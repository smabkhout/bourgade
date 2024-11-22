#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#include "position.h"
#include "board.h"
#include "player.h"

#define NUM_ROUNDS 4

int exists_a_player_with_free_workers(struct player_t** players,int taille){ //return 1 if there is a player with at least one worker available, and else 0
  int result = 0;
  for (int i = 0;i<taille; ++i)
  {
    if (players[i]->number_of_workers > 0)
      result = 1;
  }
  return result;
}

int exists_an_empty_cell(struct board_t* board) //return 1 if there is at least one free cell on the board and else 0
{
  for (int i = 0; i<MAX_X*MAX_Y; ++i)
  {
    if (is_free_cell(board->tab[i]))
      return 1;
  }
  return 0;
}

void game(int num_players)
{
  init_positions(num_players); // initialisation des positions

  struct board_t *board = NULL;
  board = init_board(); // initialisation du plateau

  struct player_t **players = NULL;
  players = (struct player_t **)malloc(sizeof(struct player_t *) * num_players);
  for (int j = 0; j < NUM_ROUNDS; ++j)
  {
    int random_color = rand() % MAX_COLORS;
    for (int i = 0; i < num_players; ++i)
    {
      players[i] = initialize_player(random_color%MAX_COLORS);
      ++random_color;
    } // fin de l'initialisation des joueurs


    while (exists_a_player_with_free_workers(players, num_players) && exists_an_empty_cell(board)) {
      int current_player = 0; //indice pour repérer le joueur actuel dans players
      if (players[current_player]->number_of_workers > 0)
      {
        struct cell_t* current_cell = NULL;
        int a_pos = rand()%MAX_X*MAX_Y;
        while (!is_free_cell(board->tab[a_pos]))
        {
          a_pos = rand()%MAX_X*MAX_Y;
        }
        current_cell = board->tab[a_pos]; //on a besoin de son indice/position sur le board
        place_worker(players[current_player],current_cell,make_worker(workers_names[rand()%6],workers_costs,players[current_player]->color));
        struct building_t ** affordable_buildings = NULL;
        affordable_buildings = list_buildings_costing_less_than(players[current_player]);
        if (length_of_affordable_buildings(affordable_buildings) > 0)
        {
          
        }
        else
        {
          unsigned int x = a_pos % MAX_X;
          unsigned int y = (a_pos - x) / MAX_X;
          struct position_t **neighbors = NULL;
          neighbors = (struct position_t **)malloc(sizeof(struct position_t *)*8);
          list_neighbors(POS(x,y), neighbors);
          int e = 0;
          while(e<8 && is_valid_position(neighbors[e])){
            //if is_mine => resource add
            //else if is_building => if wishes to activate => activate building
            unsigned int neighbor_x = PX(neighbors[e]);
            unsigned int neighbor_y = PY(neighbors[e]);
            int neighbor=neighbor_y*MAX_X + neighbor_x;
            if (board->tab[neighbor]->mine != NULL)
            {
              int resource = board->tab[neighbor]->mine->r;
              ++players[current_player]->stockage[resource];
            }
            else if (board->tab[neighbor]->building != NULL)
            {
              int choice = rand()%2; //choice of the player whether to activate or not (random for now) 
              if (choice) //player wishes to activate     //if he wishes so and can't afford => eliminate player
              {
                int owner = board->tab[neighbor]->building->joueur;
                //activate_building(players[owner], players[current_player], board->tab[neighbor]->building);
              }
            }
          }
        }
      }
    }
  }
}

int main(void)
{
}

// penser a free tous les mallocs