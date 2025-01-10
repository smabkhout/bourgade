#include <stdio.h>
#include <stdlib.h>
#include "board.h"

int occurrences(struct mine_t *tab)
{ // fonction qui vérifie si on a au moins une mine de chaque ressource
    int occ1, occ2, occ3, occ4 = 0;
    for (int i = 0; i < MAX_POSITIONS / 4; ++i)
    {
        if (tab[i].r == CORN)
            ++occ1;
        else if (tab[i].r == WOOD)
            ++occ2;
        else if (tab[i].r == STONE)
            ++occ3;
        else if (tab[i].r == FISH)
            ++occ4;
    }
    return (occ1 * occ2 * occ3 * occ4); // retourne 0 si une occurrence est nulle et un entier non nul sinon
}

struct board_t *init_board(int seed)
{ // pour initialiser le board, on place N/4 mines de ressources où N est le nombre
    // de positions valides (N>=16). Il faut aussi qu'il y au moins une mine de chaque type sur le board (Field, Forest, River, Rock
    // mine). De plus, le nombre d'emplacement valide parmi les voisins de chaque mine est >= 1.
    (void ) seed;
    struct board_t *board;
    board = (struct board_t *)malloc(sizeof(struct board_t));
    board->tab = (struct cell_t **)malloc(sizeof(struct cell_t *) * MAX_POSITIONS);
    board->present_mines = (struct mine_t *)malloc(sizeof(struct mine_t) * MAX_POSITIONS / 4);
    int nb_of_mines = 0;
    for (int i = 0; i < MAX_X * MAX_Y; i++)
    {
        board->tab[i] = init_cell();
    }
    construct_mines(board->present_mines);   // construct mines s'occupe d'avoir au moins une mine de chaque type
    int temp_invalid_pos[MAX_POSITIONS / 2]; // à chaque fois qu'on place une mine, on résérve une position voisine à ne pas
                                             // utiliser lors du placement des autres mines afin de garantir qu'on toujours au moins
                                             // une position voisine valide.
    int count_invalid_pos = 0;
    for (int i = 0; i < MAX_POSITIONS / 2; ++i)
    {
        temp_invalid_pos[i] = -1;
    }

    do
    {
        int i = rand() % (MAX_X * MAX_Y);
        int is_free_position = 1;
        int is_free_neighbor = 8;
        unsigned int x = i % MAX_X;
        unsigned int y = (i - x) / MAX_X;
        for (int k = 0; k < MAX_POSITIONS / 2; k++)
        {
            if (temp_invalid_pos[k] == i || !is_free_cell(board->tab[i]) || !is_valid_position(POS(x, y)))
                is_free_position = 0;
        } // verifier si la position est libre

        if (is_free_position)
        {
            temp_invalid_pos[count_invalid_pos] = i;
            count_invalid_pos++;
            struct position_t **neighbors = NULL;
            neighbors = (struct position_t **)malloc(sizeof(struct position_t *) * 8);
            for (int j = 0; j < 8; j++)
            {
                neighbors[j] = make_invalid_position();
            }
            list_neighbors(POS(x, y), neighbors);
            for (int j = 0; j < 8; ++j)
            {
                if (is_valid_position(neighbors[j]))
                {
                    int a = PY(neighbors[j]) * MAX_X + PX(neighbors[j]);
                    for (int k = 0; k < MAX_POSITIONS / 2; k++)
                    {
                        if (temp_invalid_pos[k] == a)
                            is_free_neighbor--; // vérifie si au moins un voisin de la position i est libre
                    }
                    if (is_free_neighbor)
                    {
                        temp_invalid_pos[count_invalid_pos] = a;
                        count_invalid_pos++;
                        break;
                    }
                }
            }
            if (is_free_position && is_free_neighbor)
            {
                place_mine(board->tab[i], board->present_mines[nb_of_mines]);
                nb_of_mines += 1;
            }
            free(neighbors);
        }
    } while (nb_of_mines < MAX_POSITIONS / 4);
    // nb_of_mines vaut (N/4)-1 à la fin du prgramme mais c'est normal car initialisée à 0
    //  deuxième condition : chaque mine a au moins 1 position valide dans ses voisins sinon => on refait une génération
    return board;
}

struct board_t* init_best_board(int seed){
    srand(seed);
    struct board_t *board;
    board = (struct board_t *)malloc(sizeof(struct board_t));
    board->tab = (struct cell_t **)malloc(sizeof(struct cell_t *) * MAX_POSITIONS);
    board->present_mines = (struct mine_t *)malloc(sizeof(struct mine_t) * MAX_POSITIONS / 4);
    construct_mines(board->present_mines);
    for (int i = 0; i < MAX_X * MAX_Y; i++)
    {
        board->tab[i] = init_cell();
    }
    int* repartition = best_mine_placement();
    for (int i = 0 ; i< MAX_POSITIONS/4;++i)
    {
        place_mine(board->tab[repartition[i]],board->present_mines[i]);
    }
    free(repartition);
    return board;
}

void free_board(struct board_t *board)
{
    for (int i = 0; i < MAX_POSITIONS; ++i)
    {
        if (board->tab[i]->mine != NULL)
        {
            free_mine(board->tab[i]->mine);
        }
        if (board->tab[i]->worker != NULL)
        { // free workers
            free_worker(board->tab[i]->worker);
        }
        /*
        if (board->tab[i]->building != NULL)
        { // free buildings
            free_building(board->tab[i]->building);
        }*/
        free_cell(board->tab[i]);
    }
    free(board->tab);
    if (board->present_mines != NULL)
        free_mine(board->present_mines);
    free(board);
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
                    free_worker(board->tab[i]->worker);//on supprime son worker et donc ses batiments
                    board->tab[i]->worker = NULL;
                    
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


void parcours_composante_connexe(struct position_t *pos_initial, int *indices_composantes_connexes, int *longueur, struct board_t *board, int debut_composante)
{
    // question : on prend composantes connexes aussi en diagonales ou uniquement dans les directions cardinales
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
        int appartient_autre_composante = 0;
        for (int j = 0; j < *longueur; ++j)
        {
            if (indices_composantes_connexes[j] == neighbor_index)
            {
                appartient_autre_composante = 1;
                break;
            }
        }
        //printf("%d : is valid position, %d : is mine, %d : neighbor index\n",is_valid_position(neighbors[i]),(board->tab[neighbor_index]->mine!=NULL), neighbor_index);
        if (!appartient_autre_composante && neighbors[i] && is_valid_position(neighbors[i]) && (board->tab[neighbor_index]->mine))
        {
            indices_voisins_a_parcourir[nb_voisins_a_parcourir] = i;
            ++nb_voisins_a_parcourir;
        }
    }
    for (int j = 0; j < nb_voisins_a_parcourir; ++j) // appeler la fonction recursivement sur ceux qui ne sont pas déjà dans le tableau
    {
        int neighbor_index = PY(neighbors[indices_voisins_a_parcourir[j]]) * MAX_X + PX(neighbors[indices_voisins_a_parcourir[j]]);
        if (neighbors[indices_voisins_a_parcourir[j]] && is_valid_position(neighbors[indices_voisins_a_parcourir[j]]) && (board->tab[neighbor_index]->mine))
        {
            parcours_composante_connexe(neighbors[indices_voisins_a_parcourir[j]], indices_composantes_connexes, longueur, board, 0);
        }
    }

    if (debut_composante)
    {
        ++*longueur;
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
    // MAX_POS/4 amas différents et MAX_POS/4 -1 entre chaque donc MAX_POS/2 au total
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
    free(longueur);
    return cost;
}

int *best_mine_placement()
{
    int *tab = (int *)malloc(sizeof(int) * MAX_POSITIONS / 4); // renvoyer un tableau contenant les indices des mines sur le plateau
    int *repartitions = (int *)malloc(sizeof(int) * (MAX_POSITIONS / 4) * 100);
    int *costs = (int *)malloc(sizeof(int) * 100);
    struct board_t* board;
    for (int j = 0; j < 100; ++j) // sur 100 itérations, on garde la meilleure répartition
    {
        int nb_of_mines = 0;
        board = init_board(j);
        costs[j] = cost_of_mine_placement(board);
        for (int i = 0; i < MAX_POSITIONS; ++i)
        {
            if (board->tab[i]->mine)
            {
                repartitions[j * (MAX_POSITIONS / 4) + nb_of_mines] = i;
                nb_of_mines++;
            }
        }
        free_board(board);
    }

    int indice_min = 0;
    int min = costs[0];
    for (int i = 1; i< 100;++i)
    {
        if (costs[i] < min){
            min = costs[i];
            indice_min = i;
        }
    }

    for (int k = 0 ; k < MAX_POSITIONS/4 ; ++k)
    {
        tab[k] = repartitions[indice_min * (MAX_POSITIONS/4) + k];
    }
    free(repartitions);
    free(costs);
    return tab;
}

// achivement 4 (pouvoirs de certains batiments)
unsigned int reward_cathedral(struct board_t *board, struct position_t *position)
{
  struct position_t **neighbors = NULL;
  neighbors = (struct position_t **)malloc(sizeof(struct position_t *) * 8);
  list_neighbors(position, neighbors);
  int count = 0;
  for (int i = 0; i < 8; ++i)
  {
    if (is_valid_position(neighbors[i]))
    {
      int indice = PY(neighbors[i]) * MAX_X + PX(neighbors[i]);
      if ((board->tab[indice]->worker) && (board->tab[indice]->building == NULL))
      {
        ++count;
      }
    }
  }
  free(neighbors);
  return count;
}

void parcours_composante_connexe_building(struct position_t *pos_initial, int *indices_composantes_connexes, int *longueur, struct board_t *board, int debut_composante, enum color_t joueur, int pouvoir)
{
  // int pouvoir permet de savoir si on a besoin de la fonction pour les parcours lors de la verification des pouvoirs (=1) ou bien lors de la verification des objectifs (=0)
  //  question : on prend composantes connexes aussi en diagonales ou uniquement dans les directions cardinales
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
    int appartient_autre_composante = 0;
    for (int j = 0; j < *longueur; ++j)
    {
      if (indices_composantes_connexes[j] == neighbor_index)
      {
        appartient_autre_composante = 1;
        break;
      }
    }
    // printf("%d : is valid position, %d : is mine, %d : neighbor index\n",is_valid_position(neighbors[i]),(board->tab[neighbor_index]->building !=NULL), neighbor_index);
    if (pouvoir)
    {
      if (!appartient_autre_composante && neighbors[i] && is_valid_position(neighbors[i]) && (board->tab[neighbor_index]->building))
      {
        indices_voisins_a_parcourir[nb_voisins_a_parcourir] = i;
        ++nb_voisins_a_parcourir;
      }
    }
    else
    {
      if (!appartient_autre_composante && neighbors[i] && is_valid_position(neighbors[i]) && (board->tab[neighbor_index]->building) && (board->tab[neighbor_index]->building->joueur == joueur)) // dans une composante connexe, il faut que les batiments appartiennent au meme joueur
      {
        indices_voisins_a_parcourir[nb_voisins_a_parcourir] = i;
        ++nb_voisins_a_parcourir;
      }
    }
  }
  for (int j = 0; j < nb_voisins_a_parcourir; ++j) // appeler la fonction recursivement sur ceux qui ne sont pas déjà dans le tableau
  {
    int neighbor_index = PY(neighbors[indices_voisins_a_parcourir[j]]) * MAX_X + PX(neighbors[indices_voisins_a_parcourir[j]]);
    if (neighbors[indices_voisins_a_parcourir[j]] && is_valid_position(neighbors[indices_voisins_a_parcourir[j]]) && (board->tab[neighbor_index]->building))
    {
      parcours_composante_connexe_building(neighbors[indices_voisins_a_parcourir[j]], indices_composantes_connexes, longueur, board, 0, joueur, pouvoir);
    }
  }

  if (debut_composante)
  {
    ++*longueur;
    free(neighbors);
    return;
  }
  else
  {
    free(neighbors);
    return;
  }
}

unsigned int reward_castle(struct board_t *board, struct position_t *position)
{
  int *longueur = NULL;
  longueur = (int *)malloc(sizeof(int));
  *longueur = 0;
  int indices_composantes[MAX_POSITIONS / 2];
  int gold_power = 0;
  for (int i = 0; i < MAX_POSITIONS / 2; ++i)
  {
    indices_composantes[i] = -1;
  }
  parcours_composante_connexe_building(position, indices_composantes, longueur, board, 1, 0, 1); // ici on l'utilise avec un 1 car c'est pour verifier un pouvoir et le enum du joueur est 0 car on n'en a pas besoin
  for (int i = 0; i < MAX_POSITIONS / 2; ++i)
  {
    if (indices_composantes[i] != -1)
    {
      if (board->tab[indices_composantes[i]]->building)
      {
        ++gold_power;
      }
    }
    else
    {
      break;
    }
  }
  return gold_power;
}

unsigned int reward_tower(struct board_t *board, struct position_t *position)
{
  struct position_t **neighbors = NULL;
  neighbors = (struct position_t **)malloc(sizeof(struct position_t *) * 8);
  list_neighbors(position, neighbors);
  int count = 0;
  for (int i = 0; i < 8; ++i)
  {
    if (is_valid_position(neighbors[i]))
    {
      int indice = PY(neighbors[i]) * MAX_X + PX(neighbors[i]);
      if ((board->tab[indice]->worker == NULL) && (board->tab[indice]->building == NULL))
      {
        ++count;
      }
    }
  }
  free(neighbors);
  return count;
}

// on imagine que la ferme gagne 2 gold si la ferme est construite a cote d'un field (mine de ressources)
unsigned int reward_farm(struct board_t *board, struct position_t *position)
{
  struct position_t **neighbors = NULL;
  neighbors = (struct position_t **)malloc(sizeof(struct position_t *) * 8);
  list_neighbors(position, neighbors);
  for (int i = 0; i < 8; ++i)
  {
    if (is_valid_position(neighbors[i]))
    {
      int indice = PY(neighbors[i]) * MAX_X + PX(neighbors[i]);
      if (board->tab[indice]->mine)
      {
        if (board->tab[indice]->mine->r == CORN)
        {
          free(neighbors);
          return 2;
        }
      }
    }
  }
  free(neighbors);
  return 0;
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

void place_worker(struct player_t *player, struct cell_t* cell,  struct worker_t *worker){
    cell->worker = worker;
    player->number_of_workers -= 1; //on décrémente le nombre de worker disponibles pour le player
    worker->joueur = player->color; //le worker est maintenant associé au joueur
}