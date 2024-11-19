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
      current_cell = find_free_cell(board);
      place_worker(players[current_player],current_cell,make_worker(workers_names[rand()%6],workers_costs,players[current_player]->color));
      struct building_t ** affordable_buildings = NULL;
      affordable_buildings = list_buildings_costing_less_than(players[current_player]);
      if (length_of_affordable_buildings(affordable_buildings) > 0)
      {
        
      }
    }
  }

}

int main(void)
{
}

// penser a free tous les mallocs