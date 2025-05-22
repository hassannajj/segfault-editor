#include <stdio.h>
#include <stdlib.h>
#include "pt_core.h"

#define INITIAL_ADD_CAP 1024

int main() {

  char *text = "";

  PieceTable *pt = pt_init(text, INITIAL_ADD_CAP);

  pt_append_text(pt, "hello");
  pt_append_text(pt, "cool");
  pt_append_text(pt, "world!!");

  pt_print(pt);

  pt_delete_text(pt, 0, 16);

  char* after = pt_get_content(pt);
  printf("after deleting: \n%s\n", after);
  pt_print(pt);
  printf("after deleting: \n%s\n", after);

  pt_cleanup(pt);
  free(after);

  return 0;
}
