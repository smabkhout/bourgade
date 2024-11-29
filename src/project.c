#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#include "position.h"
#include "board.h"
#include "player.h"
#include "optimal_pos.h"
#include "building.h"

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



void print_board(struct board_t *board) {
    for (int y = 0; y < MAX_Y; ++y) {
        // Ligne horizontale supérieure avec bordure gauche et droite
        printf("|"); // Bordure gauche
        for (int x = 0; x < MAX_X; ++x) {
            printf("--------");
            if (x < MAX_X - 1) {
                printf("|"); // Ajouter les séparateurs verticaux sauf après le dernier
            }
        }
        printf("|"); // Bordure droite
        printf("\n");

        // Deux lignes vides avec bordures gauche et droite
        for (int line = 0; line < 2; ++line) {
            printf("|"); // Bordure gauche
            for (int x = 0; x < MAX_X; ++x) {
                printf("        ");
                if (x < MAX_X - 1) {
                    printf("|"); // Ajouter les séparateurs verticaux sauf après le dernier
                }
            }
            printf("|"); // Bordure droite
            printf("\n");
        }
    }

    // Dernière ligne horizontale avec bordures gauche et droite
    printf("|"); // Bordure gauche
    for (int x = 0; x < MAX_X; ++x) {
        printf("--------");
        if (x < MAX_X - 1) {
            printf("|"); // Ajouter les séparateurs verticaux sauf après le dernier
        }
    }
    printf("|"); // Bordure droite
    printf("\n");
}


void game(int num_players)
{
  struct building_t* present_buildings = NULL;
  present_buildings = (struct building_t*)malloc(sizeof(struct building_t)*MAX_POSITIONS/3);

  init_positions(num_players); // initialisation des positions

  struct board_t *board = NULL;
  board = init_board(); // initialisation du plateau

  struct player_t **players = NULL;
  players = (struct player_t **)malloc(sizeof(struct player_t *) * num_players);
  int NB_OF_WORKERS = rand()%(MAX_WORKERS_PER_PLAYER) + MAX_POSITIONS/3/num_players;
  int random_color = rand() % MAX_COLORS;
  for (int i = 0; i < num_players; ++i)
  {
    players[i] = initialize_player(random_color%MAX_COLORS);
    players[i]->number_of_workers = NB_OF_WORKERS;
    ++random_color;
  } // fin de l'initialisation des joueurs

  for (int j = 0; j < NUM_ROUNDS; ++j)
  {
    int current_player = 0; //indice pour repérer le joueur actuel dans players
    int nb_batiments_construits = 0;
    while (exists_a_player_with_free_workers(players, num_players) && exists_an_empty_cell(board)) {
      if (players[current_player]->number_of_workers > 0)
      {
        struct cell_t* current_cell = NULL;
        struct position_t* a_pos = choose_optimal_pos(board);
        int a_pos_index = PY(a_pos)*MAX_X+PX(a_pos);
        current_cell = board->tab[a_pos_index]; //on a besoin de son indice/position sur le board
        place_worker(players[current_player],current_cell,make_worker(workers_names[rand()%6],workers_costs,players[current_player]->color));
        struct building_t ** affordable_buildings = NULL;
        affordable_buildings = list_buildings_costing_less_than(players[current_player]);
        int build_choice = rand()%2;
        if (length_of_affordable_buildings(affordable_buildings) > 0 && build_choice)
        {
          int building_choice = rand()%length_of_affordable_buildings(affordable_buildings); //le batiment à construire est choisi aleatoirement pour l'instant
          struct building_t* a_building = affordable_buildings[building_choice]; //on séléctionne ce batiment à construire
          present_buildings[nb_batiments_construits]= *a_building;
          ++nb_batiments_construits;
          place_building(players[current_player], a_pos, a_building); //stocker les buildings dans un tableau present buildings
        }
        else
        {
          struct position_t **neighbors = NULL;
          neighbors = (struct position_t **)malloc(sizeof(struct position_t *)*8);
          list_neighbors(a_pos, neighbors);
          int e = 0;
          while(e<8 && is_valid_position(neighbors[e])){
            //if is_mine => resource add
            //else if is_building => if wishes to activate => activate building
            unsigned int neighbor_x = PX(neighbors[e]);
            unsigned int neighbor_y = PY(neighbors[e]);
            int neighbor=neighbor_y*MAX_X + neighbor_x; 
            if (board->tab[neighbor]->mine != NULL) //le joueur récupère les resources sur les cases voisines
            {
              int resource = board->tab[neighbor]->mine->r;
              ++players[current_player]->stockage[resource];
            }
            else if (board->tab[neighbor]->building != NULL)
            {
              int activation_choice = rand()%2; //choice of the player whether to activate or not (random for now) 
              if (activation_choice) //player wishes to activate     //if he wishes so and can't afford => eliminate player
              {
                int owner = board->tab[neighbor]->building->joueur;
                activate_building(players[owner], players[current_player], board->tab[neighbor]->building);
              }
            }
          }
          free(neighbors);
        }
        free_affordable_buildings(affordable_buildings);
      }
      ++current_player;
    }
    reset_workers_still_on_board(board);
    for (int i = 0; i < num_players; ++i)
    {
      players[i]->number_of_workers = NB_OF_WORKERS;
    }
    //fin de la manche
    //free la mémoire
  }
  for (int i = 0; i < num_players; ++i)
  {
    free_player(players[i]);
  }
  free(present_buildings);
  free(players);
  free_board(board);
}

int main(void)
{
  //game(4);
  print_board(NULL);
  return 0;
}

// penser a free tous les mallocs