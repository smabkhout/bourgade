#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "position.h"
#include <string.h>

struct position_t{
  int x;
  int y;
};


void init_positions(unsigned int seed){
  srand(seed);
}


struct position_t* make_position(unsigned int x, unsigned int y){
  struct position_t *pos = NULL;
  pos = (struct position_t*)malloc(sizeof(struct position_t));
  pos->x = x;
  pos->y = y;
  return pos;
}

void free_position(struct position_t * pos){
  free(pos);
}

struct position_t* make_invalid_position(){
  struct position_t * pos = NULL;
  pos = (struct position_t*)malloc(sizeof(struct position_t));
  pos->x = MAX_X+1;
  pos->y = MAX_Y+1;
  return pos;
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

void position_to_string(const struct position_t* p, char* buf){
  int x = p->x;
  char* x_string;
  itoa(x,x_string);
  int y = p->y;
  char* y_string;
  itoa(y,y_string);
  char abscisse[] = "L'abscisse est :";
  char ordonnee[] = "L'ordonnee est :";
  buf = strcat(abscisse,strcat(x_string,strcat(ordonnee,y_string)));
}


int is_valid_position(const struct position_t* p){
  return ((p->x < MAX_X+1)&&(p->y < MAX_Y+1));
}

void list_neighbors(const struct position_t* p, struct position_t** ns){ //on check tous les voisins
// de la position pointée par p et on place celles valides dans le tableau ns. Pour celles qui ne sont 
//pas valides, on utilise make_invalid_position
  int i = p->x;
  int j = p->y;
  int k = 0;
  int n = 0;
  struct position_t * p1 = NULL;
  p1 = (struct position_t *)malloc(sizeof(struct position_t));
  p1->x = i-1;
  p1->y = j-1;
  struct position_t * p2= NULL;
  p2 = (struct position_t *)malloc(sizeof(struct position_t));
  p2->x = i-1;
  p2->y = j;
  struct position_t * p3= NULL;
  p3 = (struct position_t *)malloc(sizeof(struct position_t));
  p3->x = i-1;
  p3->y = j+1;
  struct position_t * p4= NULL;
  p4 = (struct position_t *)malloc(sizeof(struct position_t));
  p4->x = i;
  p4->y = j+1;
  struct position_t * p5= NULL;
  p5 = (struct position_t *)malloc(sizeof(struct position_t));
  p5->x = i+1;
  p5->y = j+1;
  struct position_t * p6= NULL;
  p6 = (struct position_t *)malloc(sizeof(struct position_t));
  p6->x = i+1;
  p6->y = j;
  struct position_t * p7= NULL;
  p7 = (struct position_t *)malloc(sizeof(struct position_t));
  p7->x = i+1;
  p7->y = j-1;
  struct position_t * p8= NULL;
  p8 = (struct position_t *)malloc(sizeof(struct position_t));
  p8->x = i;
  p8->y = j-1;
  if (is_valid_position(p1)){
    ns[n] = p1;
    ++n;
  }
  if (is_valid_position(p2)){
    ns[n] = p2;
    ++n;
  }
  if (is_valid_position(p3)){
    ns[n]= p3;
    ++n;
  }
  if (is_valid_position(p4)){
    ns[n] = p4;
    ++n;
  }
  if (is_valid_position(p5)){
    ns[n] = p5;
    ++n;
  }
  if (is_valid_position(p6)){
    ns[n] = p6;
    ++n;
  }
  if (is_valid_position(p7)){
    ns[n] = p7;
    ++n;
  }
  if (is_valid_position(p8)){
    ns[n] = p8;
    ++n;
  }
  while(n<8){
    ns[n] = make_invalid_position();
    ++n;
  }
}

int main(int argc, char* argv[]){
    return 0;
}
