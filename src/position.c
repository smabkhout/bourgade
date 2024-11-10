#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "position.h"
#include "worker.h"
#include "building.h"

struct position_t{
  unsigned int x;
  unsigned int y;
  struct building_t *building;
  struct worker_t * worker;
};

struct position_t positions[MAX_X][MAX_Y] = {{{0,0}}};

void init_positions(unsigned int seed){
    srand(time(NULL));
}

struct position_t invalid_pos;

struct position_t* make_invalid_position(){
  invalid_pos.x = MAX_X;
  invalid_pos.y = MAX_Y;
  return &invalid_pos;
}

struct position_t* make_position(unsigned int x, unsigned int y){
  if (x<MAX_X && y<MAX_Y){
    positions[x][y].x = x;
    positions[x][y].y = y;
    return &positions[x][y];
  }
  return make_invalid_position();
}

unsigned int position_x(const struct position_t* p){
  return p->x;
}

unsigned int position_y(const struct position_t* p){
  return p->y;
}

void mirror(char* str)
{
    int l = strlen(str);
    int m = l / 2; 
    int i = 0;
    char tmp;
    while (i < m) {
        tmp = str[i];
        str[i] = str[l-i-1];
        str[l-i-1] = tmp;
        i = i + 1;
    }
}

void itoa(int val, char* dst) //int to string
{
    char digits[] = "0123456789";
    int i = 0;
    int neg = val < 0;

    if (neg) // Sinon le modulo est negatif
        val = - val;

    do {
        dst[i] = digits[val % 10];
        val = val / 10;
        i = i + 1;
    } while (val);

    if (neg) { // cas particulier pour le signe
        dst[i] = '-';
        i = i + 1;
    }

    dst[i] = 0;
    mirror(dst);
}

void position_to_string(const struct position_t* p, char* buf) {
    char x_string[12];
    char y_string[12];

    itoa(p->x, x_string);
    itoa(p->y, y_string);

    strcpy(buf, "L'abscisse est : ");
    strcat(buf, x_string);
    strcat(buf, ", L'ordonnee est : ");
    strcat(buf, y_string);
}


int is_valid_position(const struct position_t* p){
  return ((p->x < MAX_X)&&(p->y < MAX_Y));
}

//on check tous les voisins
// de la position pointée par p et on place celles valides dans le tableau ns. Pour celles qui ne sont 
//pas valides, on utilise make_invalid_position
void list_neighbors(const struct position_t* p, struct position_t** ns) {
    int dx[] = {-1, -1, -1, 0, 0, 1, 1, 1};  // Déplacements en x
    int dy[] = {-1, 0, 1, -1, 1, -1, 0, 1};  // Déplacements en y

    unsigned int count = 0;
    for (int i = 0; i < 8; i++) {
        int nx = p->x + dx[i];
        int ny = p->y + dy[i];

        if (nx >= 0 && nx < MAX_X && ny >= 0 && ny < MAX_Y) {
            ns[count++] = make_position(nx, ny);
        }
    }
    // Remplir le reste avec des positions invalides si moins de MAX_NEIGHBORS voisins
    for (; count < MAX_NEIGHBORS; count++) {
        ns[count] = &invalid_pos;
    }
}
