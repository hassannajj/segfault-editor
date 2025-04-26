#include <stdio.h>
#include <stdlib.h>
#include "pt_core.h"

#define INITIAL_ADD_CAP 1024

int main() {

  char *text = "World";

  PieceTable *pt = pt_init(text, INITIAL_ADD_CAP);

  pt_insert_text(pt, "Hello ", 0);
  pt_print(pt);
  int i = 3;
  printf("index %d: %c", i, pt_get_char_at(pt, i));

  char *content = pt_get_content(pt);

  printf("\n\nCONTENT: %s\n", content);
  printf("pieces count: %d\n", pt->piece_count);

  pt_cleanup(pt);
  free(content);

  return 0;
}
