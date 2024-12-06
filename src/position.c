#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "position.h"

struct position_t
{
    unsigned int x;
    unsigned int y;
    int init; // booléen qui dit si la position a déjà été initialisée ou pas
};

int type_plateau = 14;

struct position_t positions[MAX_X * MAX_Y + 1];

struct position_t *make_invalid_position()
{
    positions[MAX_X * MAX_Y].x = MAX_X;
    positions[MAX_X * MAX_Y].y = MAX_Y;
    positions[MAX_X * MAX_Y].init = 1;
    return &positions[MAX_X * MAX_Y];
}

struct position_t *make_position(unsigned int x, unsigned int y)
{
    if (x >= MAX_X || y >= MAX_Y)
    {
        return make_invalid_position();
    }

    if (positions[y * MAX_X + x].init == 1)
    {
        return &positions[y * MAX_X + x];
    }

    positions[y * MAX_X + x].x = x;
    positions[y * MAX_X + x].y = y;
    positions[y * MAX_X + x].init = 1;
    return &positions[y * MAX_X + x];
}

void init_positions(unsigned int seed)
{
    switch (seed)
    {
    case 1: // voisins distance 2
        type_plateau = 1;
        break;
    case 2: // géométrie torique
        type_plateau = 2;
        break;
    case 3: // plateau infernal
        type_plateau = 3;
        break;
    default: // jeu de base
        type_plateau = 0;
        break;
    }
    if (type_plateau == 3)
    {
        for (int i = 0; i < MAX_X; ++i)
        {
            for (int j = 0; j < MAX_Y; ++j)
            {
                if ((i % 2 == 1) && (j % 2 == 1))
                {
                    positions[j * MAX_X + i] = *make_invalid_position();
                    puts("1");
                }
                else
                {
                    positions[j * MAX_X + i] = *make_position(i, j);
                    puts("2");
                }
            }
        }
    }
    else
    {
        for (int i = 0; i < MAX_X; ++i)
        {
            for (int j = 0; j < MAX_Y; ++j)
            {
                positions[j * MAX_X + i] = *make_position(i, j);
            }
        }
    }
}

unsigned int position_x(const struct position_t *p)
{
    return p->x;
}

unsigned int position_y(const struct position_t *p)
{
    return p->y;
}

void mirror(char *str)
{
    int l = strlen(str);
    int m = l / 2;
    int i = 0;
    char tmp;
    while (i < m)
    {
        tmp = str[i];
        str[i] = str[l - i - 1];
        str[l - i - 1] = tmp;
        i = i + 1;
    }
}

void itoa(int val, char *dst) // int to string
{
    char digits[] = "0123456789";
    int i = 0;
    int neg = val < 0;

    if (neg) // Sinon le modulo est negatif
        val = -val;

    do
    {
        dst[i] = digits[val % 10];
        val = val / 10;
        i = i + 1;
    } while (val);

    if (neg)
    { // cas particulier pour le signe
        dst[i] = '-';
        i = i + 1;
    }

    dst[i] = 0;
    mirror(dst);
}

void position_to_string(const struct position_t *p, char *buf)
{
    char x_string[12];
    char y_string[12];

    itoa(p->x, x_string);
    itoa(p->y, y_string);

    strcpy(buf, "L'abscisse est : ");
    strcat(buf, x_string);
    strcat(buf, ", L'ordonnee est : ");
    strcat(buf, y_string);
}

int is_valid_position(const struct position_t *p)
{
    if (type_plateau == 0 || type_plateau == 1 || type_plateau == 2)
        return (p && (p->x < MAX_X) && (p->y < MAX_Y));
    else // plateau infernal
    {
        return  (p&&(p->x != MAX_X)&&(p->y != MAX_Y));
    }
}

// on check tous les voisins
//  de la position pointée par p et on place celles valides dans le tableau ns. Pour celles qui ne sont
// pas valides, on utilise make_invalid_position
void list_neighbors(const struct position_t *p, struct position_t **ns)
{
    if (type_plateau == 0 || type_plateau == 3) // jeu de base ou plateau de jeu infernal
    {
        int dx[] = {-1, -1, -1, 0, 0, 1, 1, 1}; // Déplacements en x
        int dy[] = {-1, 0, 1, -1, 1, -1, 0, 1}; // Déplacements en y

        unsigned int count = 0;
        for (int i = 0; i < 8; i++)
        {
            int nx = p->x + dx[i];
            int ny = p->y + dy[i];

            if (nx >= 0 && nx < MAX_X && ny >= 0 && ny < MAX_Y)
            {
                ns[count++] = make_position(nx, ny);
            }
        }
        // Remplir le reste avec des positions invalides si moins de MAX_NEIGHBORS voisins
        for (; count < MAX_NEIGHBORS; count++)
        {
            ns[count] = make_invalid_position();
        }
    }
    else if (type_plateau == 1) // voisins distance 2 directions cardinales
    {
        int dx[] = {-1, -2, 1, 2, 0, 0, 0, 0}; // Déplacements en x
        int dy[] = {0, 0, 0, 0, -1, -2, 1, 2}; // Déplacements en y

        unsigned int count = 0;
        for (int i = 0; i < 8; i++)
        {
            int nx = p->x + dx[i];
            int ny = p->y + dy[i];

            if (nx >= 0 && nx < MAX_X && ny >= 0 && ny < MAX_Y)
            {
                ns[count++] = make_position(nx, ny);
            }
        }
        // Remplir le reste avec des positions invalides si moins de MAX_NEIGHBORS voisins
        for (; count < MAX_NEIGHBORS; count++)
        {
            ns[count] = make_invalid_position();
        }
    }
    else if (type_plateau == 2) // géométrie torique
    {
        int dx[] = {-1, -1, -1, 0, 0, 1, 1, 1}; // Déplacements en x
        int dy[] = {-1, 0, 1, -1, 1, -1, 0, 1}; // Déplacements en y

        unsigned int count = 0;
        for (int i = 0; i < 8; i++)
        {
            int nx = p->x + dx[i];
            int ny = p->y + dy[i];

            if (nx >= 0 && nx < MAX_X && ny >= 0 && ny < MAX_Y)
            {
                ns[count++] = make_position(nx, ny);
            }
            else
            {
                if (nx >= MAX_X)
                    nx = nx % MAX_X;
                else if (nx < 0)
                    nx += MAX_X;
                if (ny >= MAX_Y)
                    ny = ny % MAX_Y;
                else if (ny < 0)
                    ny += MAX_Y;
            }
        }
        // Remplir le reste avec des positions invalides si moins de MAX_NEIGHBORS voisins
        for (; count < MAX_NEIGHBORS; count++)
        {
            ns[count] = make_invalid_position();
        }
    }
}
