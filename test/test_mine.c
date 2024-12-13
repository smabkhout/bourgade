#include "mine.h"
#include "cell.h"
#include "position.h"
#include "board.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


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

void test_place_mine() {
    struct cell_t* cell = init_cell();
    struct mine_t mine = {"GoldMine", 100};
    place_mine(cell, mine);
    assert(cell->mine != NULL);
    assert(cell->mine->r == 100);
    assert(strcmp(cell->mine->name, "GoldMine") == 0);
    free(cell->mine);
    free(cell);
}

void test_construct_mines() {
    struct mine_t* present_mines = NULL;
    present_mines=(struct mine_t*)malloc(sizeof(struct mine_t)*MAX_POSITIONS/4);
    construct_mines(present_mines);
    for (int i = 0; i < MAX_POSITIONS / 4; ++i) {
        printf("Mine %d: %s\n", i, present_mines[i].name);
    }
    free_mine(present_mines);
}


void test_cost_of_mine_placement() {
    struct board_t *board = (struct board_t *)malloc(sizeof(struct board_t));
    board->tab = (struct cell_t**)malloc(sizeof(struct cell_t*)*MAX_POSITIONS);
    board->present_mines = (struct mine_t*)malloc(sizeof(struct mine_t));
    for (int i = 0; i < MAX_POSITIONS; ++i) {
        board->tab[i] = init_cell();
    }
    //test 1 : pas de mine
    assert(cost_of_mine_placement(board) == 0);
    //test 2 : une seule mine
    board->tab[0]->mine = make_mine(3,"Mineeeeeeeee"); // Place une mine à la position 0
    assert(cost_of_mine_placement(board) == 1);
    //test 3 : deux mines adjacentes
    board->tab[1]->mine = make_mine(4,"Mineeeeeeeee");
    print_board(board);
    assert(cost_of_mine_placement(board) == 4);
    //test 4 : deux mines non adjacentes (deux groupes)
    free_mine(board->tab[1]->mine);
    board->tab[1]->mine = NULL;

    board->tab[10]->mine = make_mine(4,"Mineeeeeeeee");
    assert(cost_of_mine_placement(board) == 2);
    //test 5 : trois mines formant un groupe et une mine seule
    board->tab[1]->mine = make_mine(4,"Mineeeeeeeee");
    board->tab[2]->mine = make_mine(1,"Mineeeeeeeee");
    print_board(board);
    assert(cost_of_mine_placement(board) == 10); 
    // Nettoyage
    free_board(board);
    printf("Tous les tests sont passés avec succès !\n");
}

int main() {
    init_positions(0);
    test_place_mine();
    test_construct_mines();
    test_cost_of_mine_placement();
    printf("All mine tests passed!\n");
    return 0;
}
