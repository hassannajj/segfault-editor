#include <stdio.h>
#include "pt_core.h"

int main() {
    /*TODO: split this function in pt_core.h (public) and pt_core.c (public / private) , basically all the public functions are used in this main func */

  char *text = "";

  PieceTable *pt = pt_init(text);

  pt_insert_text(pt, "Hey", 0);
  pt_insert_text(pt, "E", 1);
  pt_insert_text(pt, "L", 2);
  pt_insert_text(pt, "L", 3);
  pt_insert_text(pt, "O", 4);
  pt_insert_text(pt, " ", 5);
  pt_insert_text(pt, "l", 3);

  char *content = pt_get_content(pt);

  printf("\n\nCONTENT: %s\n", content);
  printf("pieces count: %d\n", pt->pieces_count);

  pt_print(pt);
  pt_cleanup(pt, content);

  return 0;
}
