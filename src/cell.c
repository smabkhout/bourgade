#include <stdio.h>
#include <stdlib.h>

#include "cell.h"

struct building_t *get_building_from_cell(struct cell_t c)
{
    return c.building;
}

struct worker_t *get_worker_from_cell(struct cell_t c)
{
    return c.worker;
}

struct mine_t *get_mine_from_cell(struct cell_t c)
{
    return c.mine;
}

struct cell_t* init_cell()
{
    struct cell_t* cell = NULL;
    cell = (struct cell_t*)malloc(sizeof(struct cell_t));
    cell->building = NULL;
    cell->mine = NULL;
    cell->worker = NULL;
    return cell;
}

int is_free_cell(struct cell_t c)
{ // retourne 1 si la cellule ne contient pas de buildings ou de worker ou de mine
    return (c.building == NULL) && (c.mine == NULL) && (c.worker == NULL);
}

int main()
{
    return 0;
}