#include <stdio.h>
#include <stdlib.h>
#include "pt_core.h"

#define INITIAL_ADD_CAP 1024

int main() {

  char *text = "a";

  PieceTable *pt = pt_init(text, INITIAL_ADD_CAP);
  pt_insert_text(pt, "b", 1);
  pt_insert_text(pt, "c", 2);

  pt_print(pt);

  char *content = pt_get_content(pt);

  printf("\n\nCONTENT: \n%s\n", content);
  printf("pieces count: %d\n", pt->piece_count);
  printf("num lines: %d\n", pt->num_lines);
  free(content);

  //int y = 1;
  //int x = 0;
  //printf("deleting char at Y:%d, X:%d -> %c\n", y, x, pt_get_char_at_YX(pt, y, x));
  
  int index = 1;
  int len = 2;
  printf("delete char at index %d\n", index);
  pt_delete_text(pt, index, len);
  char *content2 = pt_get_content(pt);
  pt_print(pt);
  printf("\n\nAfter deleting\nCONTENT: \n%s\n", content2);
  pt_cleanup(pt);
  free(content2);

  return 0;
}
