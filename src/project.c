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

// definition des variables globales propres à ce fichier
static int affichage = 0;

static char *workers_names[MAX_WORKERS_PER_PLAYER] = {
    "Franck",
    "John",
    "Francis",
    "Walter",
    "Hanck",
    "Georges"};

static unsigned int workers_costs[NUM_RESOURCES] = {0, 1, 1, 1, 1, 0};

int exists_a_player_with_free_workers(struct player_t **players, int taille)
{ // return 1 if there is a player with at least one worker available, and else 0
  int result = 0;
  for (int i = 0; i < taille; ++i)
  {
    if (players[i]->number_of_workers > 0 && players[i]->eliminated == 0) // il faut que les joueurs qui vérifient
                                                                          // la condition ne soient pas éliminés
      result = 1;
  }
  return result;
}

int exists_an_empty_cell(struct board_t *board) // return 1 if there is at least one free cell on the board and else 0
{
  for (int i = 0; i < MAX_X * MAX_Y; ++i)
  {
    unsigned int x = i % MAX_X;
    unsigned int y = (i - x) / MAX_X;
    if (is_free_cell(board->tab[i]) && is_valid_position(POS(x, y)))
      return 1;
  }
  return 0;
}

int exists_a_player(struct player_t **players, int num_players) // return the number of players who are not eliminated
{
  int a = 0;
  for (int i = 0; i < num_players; ++i)
  {
    if (players[i]->eliminated)
      ++a;
  }
  return num_players - a;
}

// Fonction pour formater le contenu d'une cellule
void print_cell_content(const char *content, int width)
{
  int len = strlen(content);
  int spaces_to_pad = width - len;

  // Si le contenu est trop long, tronquer et ajouter "..."
  if (len > width)
  {
    content = "....";
    printf("%s", content);
    return;
  }
  int left_padding = spaces_to_pad / 2;
  int right_padding = spaces_to_pad - left_padding;
  for (int i = 0; i < left_padding; ++i)
  {
    printf(" ");
  }
  printf("%s", content);
  for (int i = 0; i < right_padding; ++i)
  {
    printf(" ");
  }
}

void print_board(struct board_t *board)
{
  if (!board)
  {
    puts("board null print");
    return;
  }

  for (int y = 0; y < MAX_Y; ++y)
  {
    // Ligne supérieure
    for (int x = 0; x < MAX_X; ++x)
    {
      printf("+--------");
    }
    printf("+\n");

    // Première ligne de contenu (affichage des mines)
    for (int x = 0; x < MAX_X; ++x)
    {
      printf("|");
      int index = y * MAX_X + x;
      struct cell_t *cell = board->tab[index];

      if (!is_valid_position(POS(x, y)))
      {
        // printf("%sxxxxxxxx%s",color_start(ORANGE),color_stop());
        printf("  %sxxxx%s  ", color_start(RED), color_stop());
      }
      else
      {

        if (cell->mine)
        {
          // Afficher les 2 premières lettres du nom de la mine
          char mine_name[9]; // Assurer que le nom tient dans la case
          if (strlen(cell->mine->name) > 8)
          {
            strncpy(mine_name, cell->mine->name, 5);
            mine_name[5] = '.';
            mine_name[6] = '.';
            mine_name[7] = '.';
            mine_name[8] = '\0';
          }
          else
          {
            strcpy(mine_name, cell->mine->name);
          }
          print_cell_content(mine_name, 8);
        }
        else
        {
          print_cell_content("", 8); // Case vide
        }
      }
    }
    printf("|\n");

    // Deuxième ligne de contenu (worker ou bâtiment)
    for (int x = 0; x < MAX_X; ++x)
    {
      printf("|");
      struct cell_t *cell = board->tab[y * MAX_X + x];

      if (!is_valid_position(POS(x, y)))
      {
        // printf("%sxxxxxxxx%s",color_start(ORANGE),color_stop());
        printf(" %sx%s%sxxxx%s%sx%s ", color_start(RED), color_stop(), color_start(ORANGE), color_stop(), color_start(RED), color_stop());
      }
      else
      {
        if (cell->worker && !cell->building)
        {
          // Affichage du worker avec sa couleur
          printf("   %sW%s    ",
                 color_start(cell->worker->joueur),
                 color_stop());
        }
        else if (cell->building && cell->worker)
        {
          // Affichage du bâtiment avec sa couleur
          printf("   %sB%s    ",
                 color_start(cell->building->joueur),
                 color_stop());
        }
        else
        {
          print_cell_content("", 8); // Case vide
        }
      }
    }
    printf("|\n");

    // Troisième ligne de contenu (vide ou autre)
    for (int x = 0; x < MAX_X; ++x)
    {
      printf("|");
      if (!is_valid_position(POS(x, y)))
      {
        // printf("%sxxxxxxxx%s", color_start(ORANGE), color_stop());
        printf(" %sx%s%sx%s%sxx%s%sx%s%sx%s ", color_start(RED), color_stop(), color_start(ORANGE), color_stop(), color_start(YELLOW), color_stop(), color_start(ORANGE), color_stop(), color_start(RED), color_stop());
      }
      else
      {
        print_cell_content("", 8); // Ligne vide
      }
    }
    printf("|\n");
  }

  // Ligne inférieure
  for (int x = 0; x < MAX_X; ++x)
  {
    printf("+--------");
  }
  printf("+\n");
  printf("\n");
}

void display_winner(int num_players, struct player_t **players)
{
  int remaining_players[num_players];
  int count = 0;
  for (int i = 0; i < num_players; ++i)
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
      printf("%s %s %s\n", color_start(players[count]->color), color_to_string(players[count]->color), color_stop());
    }
    else
    {
      int gagnants[count];
      for (int i = 0; i < count; ++i)
      {
        gagnants[i] = 0;
      }
      int gagnant = 0;
      int nb_de_gagnants = 1;
      for (int i = 1; i < count; ++i)
      {
        if (players[remaining_players[i]]->stockage[5] > players[remaining_players[gagnant]]->stockage[5]) // si il y a un seul gagnant
        {
          gagnant = i;
          nb_de_gagnants = 1;
          for (int i = 0; i < count; ++i)
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
        printf("%s %s %s\n", color_start(players[gagnant]->color), color_to_string(players[gagnant]->color), color_stop());
      }
      else
      {
        printf("Félicitations au %d gagnants !!\n", nb_de_gagnants);
        for (int i = 0; i < nb_de_gagnants; ++i)
        {
          printf("Joueur %s %s %s\n", color_start(players[gagnants[i]]->color), color_to_string(players[gagnants[i]]->color), color_stop());
        }
      }
    }
  }
}

void display_remaining_players(int num_players, struct player_t **players)
{
  puts("Les joueurs restants sont :");
  for (int i = 0; i < num_players; ++i)
  {
    if (players[i]->eliminated == 0)
    {
      printf("Joueur %s %s %s avec %d workers\n", color_start(players[i]->color), color_to_string(players[i]->color), color_stop(), players[i]->number_of_workers);
    }
  }
  printf("\n");
}

void game(int num_players, int init_param, int seed)
{
  init_positions(init_param); // initialisation des positions
  int count_valide = 0;
  for (int i = 0; i < MAX_POSITIONS; ++i)
  {
    unsigned int x = i % MAX_X;
    unsigned int y = (i - x) / MAX_X;
    if (is_valid_position(POS(x, y)))
      ++count_valide;
  }
  printf("Nombre de positions valides est : %d\n", count_valide);

  struct board_t *board = NULL;
  board = init_best_board(seed); // initialisation du plateau sans les buildings
  int cost = cost_of_mine_placement(board);
  printf("Le cout de cette répartition de mines est : %d\n", cost);
  board->present_buildings = (struct building_t **)malloc(sizeof(struct building_t *) * MAX_BUILDINGS_PER_PLAYER * num_players);
  for (int i = 0; i < MAX_BUILDINGS_PER_PLAYER * num_players; ++i)
  {
    board->present_buildings[i] = (struct building_t *)malloc(sizeof(struct building_t));
    for (int j = 0; j < 10; ++j)
    {
      board->present_buildings[i]->nom[j] = '-';
    }
  } // initialisation des batiments du plateau

  print_board(board);

  struct player_t **players = NULL;
  players = (struct player_t **)malloc(sizeof(struct player_t *) * num_players);
  int NB_OF_WORKERS = rand() % (MAX_WORKERS_PER_PLAYER) + MAX_POSITIONS / 3 / num_players;
  int random_color = rand() % MAX_COLORS;
  for (int i = 0; i < num_players; ++i)
  {
    players[i] = initialize_player(random_color % MAX_COLORS);
    ++random_color;
  } // fin de l'initialisation des joueurs

  int nb_batiments_construits = 0;
  for (int j = 0; j < NUM_ROUNDS; ++j)
  {
    printf("Début de la manche N°%d.\n", j + 1);
    int current_player = 0; // indice pour repérer le joueur actuel dans players
    while (exists_a_player_with_free_workers(players, num_players) && exists_an_empty_cell(board) && exists_a_player(players, num_players))
    {
      if (players[current_player] != NULL)
      {
        if (players[current_player]->number_of_workers > 0 && players[current_player]->eliminated == 0)
        {
          struct cell_t *current_cell;
          struct position_t *a_pos = choose_optimal_pos(board);
          int a_pos_index = 0;
          a_pos_index = PY(a_pos) * MAX_X + PX(a_pos);
          current_cell = board->tab[a_pos_index]; // on a besoin de son indice/position sur le board
          place_worker(players[current_player], current_cell, make_worker(workers_names[rand() % 6], workers_costs, players[current_player]->color));
          printf("Le joueur %s place un worker à la position (%d,%d)\n", color_to_string(players[current_player]->color), PY(a_pos) + 1, PX(a_pos) + 1); // on inverse l'affichage par rapport à ce qu'on a fait pour faire comme une matrice
          struct building_t **affordable_buildings = NULL;
          affordable_buildings = list_buildings_costing_less_than(players[current_player]);
          int build_choice = rand() % 2;
          if (length_of_affordable_buildings(affordable_buildings) > 0 && build_choice)
          {
            int building_choice = rand() % length_of_affordable_buildings(affordable_buildings);                      // le batiment à construire est choisi aleatoirement pour l'instant
            struct building_t *a_building = affordable_buildings[building_choice];                                    // on séléctionne ce batiment à construire
            copy_building(board->present_buildings[nb_batiments_construits], a_building);                             // le contenu de a_building n'est pas copié et par suite apres free_affordable_buildings les batiments deja construits disparaissent
            place_building(players[current_player], current_cell, board->present_buildings[nb_batiments_construits]); // stocker les buildings dans un tableau present buildings
            ++nb_batiments_construits;
            printf("Le joueur %s construit un batiment %s à la position (%d,%d)\n", color_to_string(players[current_player]->color), a_building->nom, PY(a_pos) + 1, PX(a_pos) + 1);
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
                  int owner = (board->tab[neighbor]->building->joueur) % num_players;
                  activate_building(players[owner], players[current_player], board->tab[neighbor]->building);
                  printf("Le joueur %s, qui est sur la case (%d,%d), active le batiment %s qui appartient à %s sur la case (%d,%d).\n", color_to_string(players[current_player]->color), PY(a_pos) + 1, PX(a_pos) + 1, board->tab[neighbor]->building->nom, color_to_string(players[owner]->color), neighbor_y + 1, neighbor_x + 1);
                }
              }
              ++e;
            }
            free(neighbors);
          }
          free_affordable_buildings(affordable_buildings);
          // current_player = (current_player + 1) % num_players;
          if (affichage)
          {
            print_board(board);
          }
        }
        else
        {
          // puts("1");
          // current_player = (current_player + 1) % num_players; //le probleme lors de l'execution de ./project -c 3 si cette ligne n'etait pas
          // autre probleme lors de l'execution de ./project -c 3 pour MAX_X=7 MAX_Y=7
        }
      }

      current_player = (current_player + 1) % num_players;

      if (affichage)
      {
        display_remaining_players(num_players, players);
        print_board(board);
      }
    }

    // printf("Player with free workers %d, empty cell %d, exists a player %d.\n", exists_a_player_with_free_workers(players, num_players), exists_an_empty_cell(board), exists_a_player(players, num_players));

    // payer les couts d'entretien et eliminer les joueurs qui ne peuvent pas le faire
    pay_workers_on_board(board, num_players, players);
    // tous les joueurs restants récupèrents leurs employés

    // printf("Player with free workers %d, empty cell %d, exists a player %d.\n", exists_a_player_with_free_workers(players, num_players), exists_an_empty_cell(board), exists_a_player(players, num_players));

    // dans ce test personne n'a pu payer ses couts d'entretien vers la fin de la manche N°2
    reset_workers_still_on_board(board);
    for (int i = 0; i < num_players; ++i)
    {
      players[i]->number_of_workers = NB_OF_WORKERS;
    }

    if ((exists_a_player(players, num_players) == 0) || (exists_a_player(players, num_players) == 1)) // tous les joueurs sont éliminés, ou alors il n'en reste qu'un
    {
      break;
    }
    // fin de la manche
    // annoncer le/les gagnant(s) si existe
    // fin du jeu, free la mémoire
  }
  print_board(board);
  display_winner(num_players, players);
  for (int i = 0; i < num_players; ++i)
  {
    free_player(players[i]);
  }
  free(players);
  for (int i = 0; i < MAX_BUILDINGS_PER_PLAYER * num_players; ++i)
  {
    free(board->present_buildings[i]);
  }
  free(board->present_buildings);
  free_board(board); // Les batiments sont free() deux fois, une avec free_affordable buildings et l'autre ici, (meme avec la condition batiment != NULL)
}

int main(int argc, char *argv[])
{
  int seed = 156;
  int num_players = 4;
  unsigned int init_param = 0;
  int opt;
  while ((opt = getopt(argc, argv, "s:p:c:v")) != -1)
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
    case 'v':
      affichage = 1;
      break;
    default:
      num_players = 4;
      seed = 156;
      init_param = 0;
      break;
    }
  }
  game(num_players, init_param, seed);
  return 0;
}