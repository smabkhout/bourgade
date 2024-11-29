#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>


#include "position.h"
#include "board.h"
#include "player.h"
#include "optimal_pos.h"
#include "building.h"

#define NUM_ROUNDS 4
#define CELL_WIDTH 8


int exists_a_player_with_free_workers(struct player_t **players, int taille)
{ // return 1 if there is a player with at least one worker available, and else 0
  int result = 0;
  for (int i = 0; i < taille; ++i)
  {
    if (players[i]->number_of_workers > 0)
      result = 1;
  }
  return result;
}

int exists_an_empty_cell(struct board_t *board) // return 1 if there is at least one free cell on the board and else 0
{
  for (int i = 0; i < MAX_X * MAX_Y; ++i)
  {
    if (is_free_cell(board->tab[i]))
      return 1;
  }
  return 0;
}

int exists_a_player(struct player_t **players, int num_players)
{
  int a = 0;
  for (int i = 0; i < num_players; ++i)
  {
    a += players[i]->eliminated;
  }
  a = (a == num_players - 1) || (a == num_players); // si il ne reste qu'un seul joueur ou aucun joueur
  return a;
}

// Fonction pour formater le contenu d'une cellule
void print_cell_content(const char *content, int width) {
    int len = strlen(content);
    int spaces_to_pad = width - len;
    
    // Si le contenu est trop long, tronquer et ajouter "..."
    if (len > width) {
        content = "....";
        printf("%s", content);
        return;
    }
    int left_padding = spaces_to_pad / 2;
    int right_padding = spaces_to_pad - left_padding;
    for (int i = 0; i < left_padding; ++i) {
        printf(" ");
    }
    printf("%s", content);
    for (int i = 0; i < right_padding; ++i) {
        printf(" ");
    }
}

void print_board(struct board_t *board) {
    if (!board) {
        puts("board null print");
        return;
    }

    for (int y = 0; y < MAX_Y; ++y) {
        // Ligne supérieure
        for (int x = 0; x < MAX_X; ++x) {
            printf("+--------");
        }
        printf("+\n");

        // Première ligne de contenu (affichage des mines ou "opt")
        for (int x = 0; x < MAX_X; ++x) {
            printf("|");
            int index = y * MAX_X + x;
            struct cell_t *cell = board->tab[index];
            if (cell->mine) {
                // Afficher les 2 premières lettres du nom de la mine
                char mine_name[9]; // Assurer que le nom tient dans la case
                if (strlen(cell->mine->name) > 8) {
                    strncpy(mine_name, cell->mine->name, 5);
                    mine_name[5] = '.';
                    mine_name[6] = '.';
                    mine_name[7] = '.';
                    mine_name[8] = '\0';
                } else {
                    strcpy(mine_name, cell->mine->name);
                }
                print_cell_content(mine_name, 8);
            } else {
                print_cell_content("", 8); // Case vide
            }
        }
        printf("|\n");

        // Deuxième ligne de contenu (worker ou bâtiment)
        for (int x = 0; x < MAX_X; ++x) {
            printf("|");
            struct cell_t *cell = board->tab[y * MAX_X + x];
            if (cell->worker) {
                // Affichage du worker avec sa couleur
                printf("   %sW%d%s   ",
                       color_start(cell->worker->joueur),
                       cell->worker->joueur,
                       color_stop());
            } else if (cell->building) {
                // Affichage du bâtiment avec sa couleur
                printf("   %sB%d%s   ",
                       color_start(cell->building->joueur),
                       cell->building->joueur,
                       color_stop());
            } else {
                print_cell_content("", 8); // Case vide
            }
        }
        printf("|\n");

        // Troisième ligne de contenu (vide ou autre)
        for (int x = 0; x < MAX_X; ++x) {
            printf("|");
            print_cell_content("", 8); // Ligne vide
        }
        printf("|\n");
    }

    // Ligne inférieure
    for (int x = 0; x < MAX_X; ++x) {
        printf("+--------");
    }
    printf("+\n");
}




void display_winner(int num_players, struct player_t **players)
{
  int remaining_players[num_players];
  int count = 0;
  for (int i =  0; i<num_players; ++i)
  {
    if (players[i]->eliminated == 0)
    {
      remaining_players[count] = i;
      ++count;
    }
  }
  if (count == 0)
  {
    printf("Personne n'a gagné (-_-)\n");
  }
  else
  {
    if (count == 1)
    {
      printf("Félicitations au joueur");
      printf("%s %s %s\n",color_start(players[count]->color), color_to_string(players[count]->color), color_stop());
    }
    else
    {
      int gagnants[count];
      for (int i = 0; i<count; ++i)
      {
        gagnants[i] = 0;
      }
      int gagnant = 0;
      int nb_de_gagnants = 1;
      for (int i = 1; i<count; ++i)
      {
        if (players[remaining_players[i]]->stockage[5] > players[remaining_players[gagnant]]->stockage[5]) //si il y a un seul gagnant
        {
          gagnant = i;
          nb_de_gagnants = 1;
          for (int i = 0; i<count; ++i)
          {
            gagnants[i] = i;
          }
        }
        if (players[remaining_players[i]]->stockage[5] == players[remaining_players[gagnant]]->stockage[5]) // si on a plusieurs gagnants
        {
          gagnants[nb_de_gagnants] = i;
          ++nb_de_gagnants;
        }
      }
      if (nb_de_gagnants == 1)
      {
        printf("Félicitations au joueur");
        printf("%s %s %s\n",color_start(players[gagnant]->color), color_to_string(players[gagnant]->color), color_stop());
      }
      else
      {
        printf("Félicitations au %d gagnants !!\n", nb_de_gagnants);
        for (int i = 0; i<nb_de_gagnants; ++i)
        {
          printf("Joueur %s %s %s\n",color_start(players[gagnants[i]]->color), color_to_string(players[gagnants[i]]->color), color_stop());
        }
      }
    }
  }
}

void game(int num_players,int seed)
{
  struct building_t *present_buildings = NULL;
  present_buildings = (struct building_t *)malloc(sizeof(struct building_t) * MAX_POSITIONS / 3);

  init_positions(seed); // initialisation des positions

  struct board_t *board = NULL;
  board = init_board(); // initialisation du plateau

  print_board(board);

  struct player_t **players = NULL;
  players = (struct player_t **)malloc(sizeof(struct player_t *) * num_players);
  int NB_OF_WORKERS = rand() % (MAX_WORKERS_PER_PLAYER) + MAX_POSITIONS / 3 / num_players;
  int random_color = rand() % MAX_COLORS;
  for (int i = 0; i < num_players; ++i)
  {
    players[i] = initialize_player(random_color % MAX_COLORS);
    players[i]->number_of_workers = NB_OF_WORKERS;
    ++random_color;
  } // fin de l'initialisation des joueurs

  int nb_batiments_construits = 0;
  for (int j = 0; j < NUM_ROUNDS; ++j)
  {
    int current_player = 0; // indice pour repérer le joueur actuel dans players
    while (exists_a_player_with_free_workers(players, num_players) && exists_an_empty_cell(board) && exists_a_player(players, num_players))
    {
      if (players[current_player]->number_of_workers > 0 && players[current_player]->eliminated == 0)
      {
        struct cell_t *current_cell = NULL;
        struct position_t *a_pos = choose_optimal_pos(board);
        int a_pos_index = PY(a_pos) * MAX_X + PX(a_pos);
        current_cell = board->tab[a_pos_index]; // on a besoin de son indice/position sur le board
        place_worker(players[current_player], current_cell, make_worker(workers_names[rand() % 6], workers_costs, players[current_player]->color));
        struct building_t **affordable_buildings = NULL;
        affordable_buildings = list_buildings_costing_less_than(players[current_player]);
        int build_choice = rand() % 2;
        if (length_of_affordable_buildings(affordable_buildings) > 0 && build_choice)
        {
          int building_choice = rand() % length_of_affordable_buildings(affordable_buildings); // le batiment à construire est choisi aleatoirement pour l'instant
          struct building_t *a_building = affordable_buildings[building_choice];               // on séléctionne ce batiment à construire
          present_buildings[nb_batiments_construits] = *a_building;
          ++nb_batiments_construits;
          place_building(players[current_player], a_pos, a_building); // stocker les buildings dans un tableau present buildings
        }
        else
        {
          struct position_t **neighbors = NULL;
          neighbors = (struct position_t **)malloc(sizeof(struct position_t *) * 8);
          list_neighbors(a_pos, neighbors);
          int e = 0;
          while (e < 8 && is_valid_position(neighbors[e]))
          {
            // if is_mine => resource add
            // else if is_building => if wishes to activate => activate building
            unsigned int neighbor_x = PX(neighbors[e]);
            unsigned int neighbor_y = PY(neighbors[e]);
            int neighbor = neighbor_y * MAX_X + neighbor_x;
            if (board->tab[neighbor]->mine != NULL) // le joueur récupère les resources sur les cases voisines
            {
              int resource = board->tab[neighbor]->mine->r;
              ++players[current_player]->stockage[resource];
            }
            else if (board->tab[neighbor]->building != NULL)
            {
              int activation_choice = rand() % 2; // choice of the player whether to activate or not (random for now)
              if (activation_choice)              // player wishes to activate     //if he wishes so and can't afford => eliminate player
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
    // payer les couts d'entretien et eliminer les joueurs qui ne peuvent pas le faire
    pay_workers_on_board(board, num_players, players);
    // tous les joueurs restants récupèrents leurs employés
    reset_workers_still_on_board(board);
    for (int i = 0; i < num_players; ++i)
    {
      players[i]->number_of_workers = NB_OF_WORKERS;
    }
    //fin de la manche
  //annoncer le/les gagnant(s) si existe
  //fin du jeu, free la mémoire
  }
  display_winner(num_players, players);
  for (int i = 0; i < num_players; ++i)
  {
    free_player(players[i]);
  }
  free(present_buildings);
  free(players);
  free_board(board);
}

int main(int argc, char *argv[])
{
  int seed = 156;
  int num_players = 4;
  int init_param = 2;
  int opt;
  while ((opt = getopt(argc, argv, "s:p:c")) != -1)
  {
    switch (opt)
    {
    case 's':
      seed = atoi(optarg);
      break;
    case 'c':
      init_param = atoi(optarg);
      break;
    case 'p':
      num_players = atoi(optarg);
      break;
    default:
      num_players = 4;
      seed = 156;
      init_param = 2;
      break;
    }
  }
  game(num_players,seed);
  return 0;
}