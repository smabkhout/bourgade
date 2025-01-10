#ifndef __BOARD_H__
#define __BOARD_H__

#include "building.h"

struct board_t {
    struct cell_t** tab;
    struct mine_t* present_mines;
    struct building_t** present_buildings;
    int *indexes_present_buildings;
};

int occurrences(struct mine_t* tab);

struct board_t* init_board(int seed);

struct board_t* init_best_board(int seed);

void free_board(struct board_t* board);

void pay_workers_on_board(struct board_t* board, int num_players, struct player_t** players);

void reset_workers_still_on_board(struct board_t*);

void parcours_composante_connexe(struct position_t* pos_initial, int* indices_composante, int *longueur, struct board_t* board, int debut_composante);

int cost_of_mine_placement(struct board_t* board);

int* best_mine_placement();

//achievement 4
unsigned int reward_cathedral(struct board_t* board, struct position_t* position);

void parcours_composante_connexe_building(struct position_t *pos_initial, int *indices_composantes_connexes, int *longueur, struct board_t *board, int debut_composante, enum color_t joueur, int pouvoir);

unsigned int reward_castle(struct board_t* board,struct position_t* position);

unsigned int reward_tower(struct board_t* board,struct position_t* position);

unsigned int reward_farm(struct board_t* board,struct position_t* position); 
//on invente un pouvoir pourle batiment farm

void place_mine(struct cell_t* cell, struct mine_t m);

struct worker_t *make_worker(char* name, unsigned int cost[NUM_RESOURCES], enum color_t couleur);

void place_worker(struct player_t *player, struct cell_t* cell, struct worker_t *worker);

#endif