#include <stdio.h>
#include "pt_core.h"

#define INITIAL_ADD_CAP 1024

int main() {

  char *text = "";

  PieceTable *pt = pt_init(text, INITIAL_ADD_CAP);

  pt_insert_text(pt, "Hey", 0);
  pt_insert_text(pt, "E", 1);
  pt_insert_text(pt, "L", 2);
  pt_insert_text(pt, "L", 3);
  pt_insert_text(pt, "O", 4);
  pt_insert_text(pt, " ", 5);
  pt_insert_text(pt, "\n", 5);
  pt_insert_text(pt, "l", 3);

  char *content = pt_get_content(pt);

  printf("\n\nCONTENT: %s\n", content);
  printf("pieces count: %d\n", pt->piece_count);


  pt_cleanup(pt, content);

  return 0;
}
