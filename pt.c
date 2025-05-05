#include <stdio.h>
#include <stdlib.h>
#include "pt_core.h"

#define INITIAL_ADD_CAP 1024

int main() {

  char *text = "Hello\nWorld";

  PieceTable *pt = pt_init(text, INITIAL_ADD_CAP);

  pt_insert_text(pt, "cool\n", 6);
  pt_print(pt);
  int i = 3;
  printf("index %d: %c", i, pt_get_char_at_i(pt, i));

  char *content = pt_get_content(pt);

  printf("\n\nCONTENT: \n%s\n", content);
  printf("pieces count: %d\n", pt->piece_count);

  int y = 1;
  int x = 5;
  printf("get char at Y:%d, X:%d -> %c\n", y, x, pt_get_char_at_YX(pt, y, x));

  pt_cleanup(pt);
  free(content);

  return 0;
}
