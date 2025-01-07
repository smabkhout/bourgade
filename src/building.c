#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "building.h"

void free_building(struct building_t *batiment)
{
  free(batiment);
}

// définir un tableau global avec tous les buildings
static struct building_t list_buildings[MAX_BUILDINGS_PER_PLAYER] = {
    {.nom = "Farm", .costs = {0, 0, 0, 0, 0, 1}, .earns = {0, 0, 0, 0, 0, 3}, .supplies = {0, 1, 0, 0, 0, 0}, .value = {0, 0, 0, 1, 0, 0}, .joueur = 0},
    {.nom = "Samwill", .costs = {0, 0, 0, 0, 0, 1}, .earns = {0, 0, 0, 0, 0, 4}, .supplies = {0, 0, 0, 3, 0, 0}, .value = {0, 0, 0, 2, 0, 0}, .joueur = 0},
    {.nom = "Pontoon", .costs = {0, 0, 0, 0, 0, 1}, .earns = {0, 0, 0, 0, 0, 5}, .supplies = {0, 0, 2, 0, 0, 0}, .value = {0, 0, 0, 3, 0, 0}, .joueur = 0},
    {.nom = "Quarry", .costs = {0, 0, 0, 0, 0, 2}, .earns = {0, 0, 0, 0, 0, 5}, .supplies = {0, 0, 0, 2, 0, 0}, .value = {0, 0, 0, 3, 0, 0}, .joueur = 0},
    {.nom = "Cathedral", .costs = {0, 0, 0, 0, 0, 3}, .earns = {0, 0, 0, 0, 0, 3}, .supplies = {1, 1, 1, 0, 1, 0}, .value = {0, 0, 0, 0, 0, 6}, .joueur = 0},
    {.nom = "Tower", .costs = {0, 0, 0, 0, 0, 1}, .earns = {0, 0, 0, 0, 0, 2}, .supplies = {0, 3, 0, 0, 0, 0}, .value = {0, 0, 0, 1, 1, 0}, .joueur = 0},
    {.nom = "Castle", .costs = {0, 0, 0, 1, 1, 1}, .earns = {0, 1, 0, 0, 0, 2}, .supplies = {0, 3, 0, 1, 1, 0}, .value = {0, 1, 1, 1, 1, 0}, .joueur = 0}};

void place_building(struct player_t *player, struct cell_t *cell, struct building_t *building)
{                                                                    // acheter un batiment
  resource_sub(player->stockage, building->value, player->stockage); // achat du batiment
  resource_add(player->stockage, building->earns, player->stockage); // recompense immédiate à l'achat
  cell->building = building;                                         // on place le batiment au bon endroit
  ++player->nb_of_buildings_placed;                                  // on augmente le nombre de batiments construits par le joueur
  building->joueur = player->color;                                  // on colore le batiment de la couleur de son propriétaire
}

void activate_building(struct player_t *owner, struct player_t *user, struct building_t *building)
{                                                                   // activer la production d'un batiment
  resource_sub(user->stockage, building->costs, user->stockage);    // user paye le cout d'activation
  resource_add(user->stockage, building->supplies, user->stockage); // user récupère les supplies
  resource_add(owner->stockage, building->costs, owner->stockage);  // owner récupère le cout d'activation
}

struct building_t **list_buildings_costing_less_than(struct player_t *player)
{
  struct building_t **affordable_buildings = NULL;
  affordable_buildings = (struct building_t **)malloc(sizeof(struct building_t *) * 7);
  for (int i = 0; i < 7; ++i)
  {
    affordable_buildings[i] = NULL;
  }
  int i = 0;
  int j = 0;
  while (i < 7)
  {
    int is_affordable = 1;
    for (int res = 0; res < NUM_RESOURCES; ++res)
    {
      is_affordable = is_affordable * (list_buildings[i].value[res] <= player->stockage[res]);
    }
    if (is_affordable)
    {
      affordable_buildings[j] = &list_buildings[i];
      ++j;
    }
    ++i;
  }
  return affordable_buildings;
}

unsigned int **resource_vectors(struct player_t *player)
{
  unsigned int **resource_vectors = NULL;
  int count = 1;
  for (int i = 0; i < NUM_RESOURCES; ++i)
  {
    if (player->stockage[i] != 0)
      count *= player->stockage[i] + 1; // nombre de vecteurs resource inférieurs au vecteur stockage du joueur
  }
  if (count == 1)
  {
    return resource_vectors; // le joueur n'a pas de resources, on retourne un pointeur NULL
  }
  resource_vectors = (unsigned int **)malloc(sizeof(unsigned int *) * count);
  for (int i = 0; i < count; ++i)
  {
    resource_vectors[i] = (unsigned int *)malloc(sizeof(unsigned int) * NUM_RESOURCES);
  }
  int index_nb_of_vectors = 0;
  for (unsigned int j = 0; j < NUM_RESOURCES; ++j)
  {
    if (player->stockage[j])
    {
      if (index_nb_of_vectors == 0) // si on n'a pas encore commencer à remplir resource_vectors, on aura donc besoin juste de
                                    //  copier les resources du joueurs en ne changeant qu'une seule resource
      {
        for (unsigned int i = 0; i < (player->stockage[j] + 1); ++i)
        {
          for (unsigned int resource = 0; resource < NUM_RESOURCES; ++resource)
          {
            if (resource != j)
            {
              resource_vectors[index_nb_of_vectors][resource] = player->stockage[resource];
            }
            else
            {
              resource_vectors[index_nb_of_vectors][resource] = player->stockage[resource] - i;
            }
          }
          ++index_nb_of_vectors;
        }
      }
      else // on doit pour chacun des vecteurs déjà presents refaire la meme chose on remplacant à chaque fois uniquement une resource
      {
        unsigned int current_nb_of_vectors = index_nb_of_vectors;
        for (unsigned int vector = 0; vector < current_nb_of_vectors; ++vector)
        {
          for (unsigned int i = 1; i < (player->stockage[j] + 1); ++i)
          {
            for (unsigned int resource = 0; resource < NUM_RESOURCES; ++resource)
            {
              if (resource != j)
              {
                resource_vectors[index_nb_of_vectors][resource] = resource_vectors[vector][resource];
              }
              else
              {
                resource_vectors[index_nb_of_vectors][resource] = resource_vectors[vector][resource] - i;
              }
            }
            ++index_nb_of_vectors;
          }
        }
      }
    }
  }
  return resource_vectors;
}

void free_resource_vectors(unsigned int **resource_vectors, struct player_t *player)
{
  unsigned int count = 1;
  for (int i = 0; i < NUM_RESOURCES; ++i)
  {
    if (player->stockage[i] != 0)
      count *= player->stockage[i] + 1; // nombre de vecteurs resource inférieurs au vecteur stockage du joueur
  }
  if (count != 1) // afin de s'assurer que resource_vectors n'est pas un pointeur NULL;
  {
    for (unsigned int i = 0; i < count; ++i)
    {
      free(resource_vectors[i]);
    }
    free(resource_vectors);
  }
}

unsigned int building_in_resource_vectors(struct building_t *building, unsigned int **resource_vectors, struct player_t *player) // retourne 0 si n'appartient pas, sinon retourne son indice dans resource_vectors
{
  unsigned int count = 1;
  for (int i = 0; i < NUM_RESOURCES; ++i)
  {
    if (player->stockage[i] != 0)
      count *= player->stockage[i] + 1; // nombre de vecteurs resource inférieurs au vecteur stockage du joueur
  }
  for (unsigned int vector = 0; vector < count; ++vector)
  {
    int appartient = 1;
    for (int resource = 0; resource < NUM_RESOURCES; ++resource)
    {
      appartient *= (resource_vectors[vector][resource] == building->value[resource]); // si au moins une resource est differente alors ce n'est pas ce vecteur
    }
    if (appartient)
      return vector;
  }
  return 0;
}

void free_affordable_buildings(struct building_t **batiments)
{
  free(batiments);
}

int length_of_affordable_buildings(struct building_t **affordable_buildings)
{
  int count = 0;
  while (affordable_buildings[count] != NULL)
  {
    ++count;
  }
  return count;
}

void copy_building(struct building_t *b2, struct building_t *b1)
{
  b2->joueur = b1->joueur;
  strcpy(b2->nom, b1->nom);
  for (int i = 0; i < NUM_RESOURCES; ++i)
  {
    b2->costs[i] = b1->costs[i];
    b2->earns[i] = b1->earns[i];
    b2->value[i] = b1->value[i];
    b2->supplies[i] = b1->supplies[i];
  }
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
  parcours_composante_connexe_building(position, indices_composantes, longueur, board, 1, NULL, 1); //ici on l'utilise avec un 1 car c'est pour verifier un pouvoir et le enum du joueur est NULL car on n'en a pas besoin
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