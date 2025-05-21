#include <stdio.h>
#include <stdlib.h>
#include "pt_core.h"

#define INITIAL_ADD_CAP 1024

int main() {

  char *text = "\n";

  PieceTable *pt = pt_init(text, INITIAL_ADD_CAP);
  pt_insert_text(pt, "hello", 0);
  pt_insert_text(pt, "cool", 5);
  pt_insert_text(pt, "world!!", 9);

  pt_print(pt);

  pt_delete_text(pt, 1, 16);

  char* after = pt_get_content(pt);
  printf("after deleting: \n%s\n", after);
  pt_print(pt);
  printf("after deleting: \n%s\n", after);

  pt_cleanup(pt);
  free(after);

  return 0;
}
